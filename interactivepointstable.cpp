#include "interactivepointstable.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QRandomGenerator>
#include <QThread>

namespace {

constexpr int pointRadius = 10;
constexpr int penWidth = 1;
constexpr int clickCollisionDistance =pointRadius+penWidth+ 20;
constexpr int generatorCollisionDistance = 30;
constexpr int pointsBorderDistance = pointRadius+penWidth;
constexpr int startPointsCount = 20;

void configurePainterForPoints(QPainter &painter) {
  QPen pen(Qt::white);
  pen.setWidth(penWidth);
  painter.setPen(pen);

  QBrush brush(Qt::gray);
  painter.setBrush(brush);
}

void configurePainterForLines(QPainter &painter) {
  QPen pen(Qt::green);
  pen.setWidth(penWidth);
  painter.setPen(pen);
}

bool checkCollision(const QPoint &point1, const QPoint &point2,
                    const int minimumDistance) {
  return (point1 - point2).manhattanLength() <= minimumDistance;
}

void correctPointCordsNearBorder(QPoint &point, InteractivePointsTable *table) {
    point.setX(std::max(pointsBorderDistance,point.x()));
    point.setX(std::min(table->frameSize().width() - pointsBorderDistance,point.x()));
    point.setY(std::max(pointsBorderDistance,point.y()));
    point.setY(std::min(table->frameSize().height() - pointsBorderDistance,point.y()));
}

} // namespace

InteractivePointsTable::InteractivePointsTable(QWidget *parent)
    : QFrame{parent} {
  setStyleSheet("background-color: rgb(10, 10, 10)");
  QRect rectangle(0,0,770,500);
  setGeometry(rectangle);
        qDebug() << this << this->frameSize() << this->geometry() << this->width();
}

std::size_t InteractivePointsTable::pointsCount() const {
  return points.size();
}

void InteractivePointsTable::clearPoints() {
  points.clear();
  update();
  emit pointsCountChanged();
}

void InteractivePointsTable::generateRandomPoints() // TODO: Add resizing
{
  while (points.size() < startPointsCount) {

    QPoint candidatePoint{ (QRandomGenerator::global()->bounded(1, 100) * this->frameSize().width())/100,
                           (QRandomGenerator::global()->bounded(1, 100) * this->frameSize().height())/100 };
    correctPointCordsNearBorder(candidatePoint, this);
    if (auto found = findCollision(candidatePoint, generatorCollisionDistance);
        found == points.end()) {
      points.push_back(candidatePoint);
    }
  }
  update();
  emit pointsCountChanged();
}

void InteractivePointsTable::mousePressEvent(QMouseEvent *event) {
  const auto &mouseClickPoint = event->pos();

  if (auto found = findCollision(mouseClickPoint, clickCollisionDistance);
      found != points.end()) {

    if (event->button() == Qt::LeftButton) {
      currentySelectedPoint = &(*found);
      // qDebug() << "InteractivePointsTable:" << this->frameSize() <<
      // this->geometry();
    } else if (event->button() == Qt::RightButton) {
      points.erase(found);
    }
    update();
    emit pointsCountChanged();
  } else {
    if (event->button() == Qt::LeftButton) {
      QPoint p{mouseClickPoint};
      correctPointCordsNearBorder(p, this);
      points.push_back(p);
      // qDebug() << " point" << p;
      update();
      emit pointsCountChanged();
    }
  }
}

void InteractivePointsTable::mouseReleaseEvent(QMouseEvent *event) {
  currentySelectedPoint = nullptr;
}

void InteractivePointsTable::mouseMoveEvent(QMouseEvent *event) {
  const auto &mouseClickPoint = event->pos();

  if (event->buttons() & Qt::LeftButton && currentySelectedPoint) {
    QPoint temp{mouseClickPoint};
    correctPointCordsNearBorder(temp, this);
    *currentySelectedPoint = temp;
                //qDebug() << "print mouse move" << currentySelectedPoint;
    update();
    emit pointsCountChanged();
  }
}

void InteractivePointsTable::paintEvent(QPaintEvent *e) {
  QPainter painter(this);
  drawLines(painter);
  drawPoints(painter);
}

std::vector<QPoint>::const_iterator
InteractivePointsTable::findCollision(const QPoint &point1,
                                      const int distance) const {
  return std::find_if(points.begin(), points.end(),
                      [point1, distance](auto const &point2) {
                        return checkCollision(point1, point2, distance);
                      });
}

std::vector<QPoint>::iterator
InteractivePointsTable::findCollision(const QPoint &point1,
                                      const int distance) {
  return std::find_if(points.begin(), points.end(),
                      [point1, distance](auto const &point2) {
                        return checkCollision(point1, point2, distance);
                      });
}

void InteractivePointsTable::drawPoints(QPainter &painter) {
  configurePainterForPoints(painter);

  for (auto &point : points) {
    painter.drawEllipse(point, pointRadius, pointRadius);
  }
}

void InteractivePointsTable::drawLines(QPainter &painter) const {
  if (points.size() > 1) {
    configurePainterForLines(painter);
    for (auto it = points.begin(); it != points.end() - 1; ++it) {
      painter.drawLine(*it, *(it + 1));
    }
  }
}
