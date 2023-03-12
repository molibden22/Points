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
constexpr int clickCollisionDistance = pointRadius + penWidth + 20;
constexpr int generatorCollisionDistance = 30;
constexpr int pointsBorderDistance = pointRadius + penWidth;
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

void correctPointCordsNearBorder(QPointF &point,
                                 InteractivePointsTable *table) {
  double tableWidth = table->frameSize().width();
  double tableHeight = table->frameSize().height();
  point.setX(std::max(static_cast<double>((pointsBorderDistance / tableWidth)), point.x()));
  point.setX(std::min(static_cast<double>((tableWidth - pointsBorderDistance) / tableWidth), point.x()));
  point.setY(std::max(static_cast<double>(pointsBorderDistance / tableHeight), point.y()));
  point.setY(std::min(static_cast<double>((tableHeight - pointsBorderDistance) / tableHeight),point.y()));
}

} // namespace

InteractivePointsTable::InteractivePointsTable(QWidget *parent)
    : QFrame{parent} {
  setStyleSheet("background-color: rgb(10, 10, 10)");
  QRect rectangle(0, 0, 770, 500);
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

void InteractivePointsTable::generateRandomPoints() {
  while (points.size() < startPointsCount) {

    QPointF candidatePointF{QRandomGenerator::global()->generateDouble(),
                            QRandomGenerator::global()->generateDouble()};
    correctPointCordsNearBorder(candidatePointF, this);
    if (auto found = findCollision(candidatePointF, generatorCollisionDistance);
        found == points.end()) {
      points.push_back(candidatePointF);
    }
  }
  update();
  emit pointsCountChanged();
}

void InteractivePointsTable::mousePressEvent(QMouseEvent *event) {
  const auto &mouseClickPoint = transformToQPointF(event->pos());

  if (auto found = findCollision(mouseClickPoint, clickCollisionDistance);
      found != points.end()) {

    if (event->button() == Qt::LeftButton) {
      currentlySelectedPoint = &(*found);
      //qDebug() << "Mouse point" << mouseClickPoint << event->pos();

    } else if (event->button() == Qt::RightButton) {
      points.erase(found);
    }
    update();
    emit pointsCountChanged();
  } else {
    if (event->button() == Qt::LeftButton) {
      QPointF mousePoint{mouseClickPoint};
      correctPointCordsNearBorder(mousePoint, this);
      points.push_back(mousePoint);
      // qDebug() << " point" << p;
      update();
      emit pointsCountChanged();
    }
  }
}

void InteractivePointsTable::mouseReleaseEvent(QMouseEvent *event) {
  currentlySelectedPoint = nullptr;
}

void InteractivePointsTable::mouseMoveEvent(QMouseEvent *event) {
  const auto &mouseClickPoint = transformToQPointF(event->pos());

  if (event->buttons() & Qt::LeftButton && currentlySelectedPoint) {
    QPointF temp{mouseClickPoint};
    correctPointCordsNearBorder(temp, this);
    *currentlySelectedPoint = temp;
     qDebug() << "print mouse move" << currentlySelectedPoint;
    update();
    emit pointsCountChanged();
  }
}

void InteractivePointsTable::paintEvent(QPaintEvent *e) {
  QPainter painter(this);
  drawLines(painter);
  drawPoints(painter);
}

std::vector<QPointF>::const_iterator
InteractivePointsTable::findCollision(const QPointF &point1,
                                      const int distance) const {
  return std::find_if(points.begin(), points.end(),
                      [point1, distance, this](auto const &point2) {
                        return checkCollision(transformToQPoint(point1),
                                              transformToQPoint(point2),
                                              distance);
                      });
}

std::vector<QPointF>::iterator
InteractivePointsTable::findCollision(const QPointF &point1,
                                      const int distance) {
  return std::find_if(points.begin(), points.end(),
                      [point1, distance, this](auto const &point2) {
                        return checkCollision(transformToQPoint(point1),
                                              transformToQPoint(point2),
                                              distance);
                      });
}

void InteractivePointsTable::drawPoints(QPainter &painter) {
  configurePainterForPoints(painter);
  for (auto &pointF : points) {
    painter.drawEllipse(transformToQPoint(pointF), pointRadius, pointRadius);
  }
}

void InteractivePointsTable::drawLines(QPainter &painter) const {
  if (points.size() > 1) {
    configurePainterForLines(painter);
    for (auto it = points.begin(); it != points.end() - 1; ++it) {
      painter.drawLine(transformToQPoint(*it), transformToQPoint(*(it + 1)));
    }
  }
}

QPoint InteractivePointsTable::transformToQPoint(QPointF pointF) const {
  QPoint temp{static_cast<int>(pointF.x() * frameSize().width()),
              static_cast<int>(pointF.y() * frameSize().height())};
  return temp;
}

QPointF InteractivePointsTable::transformToQPointF(QPoint point) const {
  QPointF temp{(static_cast<double>(point.x()) / frameSize().width()),
               (static_cast<double>(point.y()) / frameSize().height())};
  return temp;
}
