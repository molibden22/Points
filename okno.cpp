#include "okno.h"
#include "./ui_okno.h"

namespace {

void configurePainterForPoints(QPainter &painter) {
  QPen point_pen(Qt::white);
  point_pen.setWidth(1);
  painter.setPen(point_pen);
  QBrush brush(Qt::gray);
  painter.setBrush(brush);
}

void configurePainterForLines(QPainter &painter) {
  QPen line_pen(Qt::green);
  line_pen.setWidth(1);
  painter.setPen(line_pen);
}

bool checkCollision(const QPoint &point1, const QPoint &point2,
                    const int minimumDistance) {
  return ((pow(point1.x() - point2.x(), 2) < minimumDistance) &&
          (pow(point1.y() - point2.y(), 2) < minimumDistance));
}

QPoint setRandomPoint() {
  int x = QRandomGenerator::global()->bounded(30, 770);
  int y = QRandomGenerator::global()->bounded(50, 530);
  return QPoint(x, y);
}

} // namespace

Okno::Okno(QWidget *parent) : QWidget(parent), ui(new Ui::Okno) {
  ui->setupUi(this);
  refreshPointsCountText();
  generateRandomPoints();
}

Okno::~Okno() { delete ui; }

// Buttons----------------------------------------
void Okno::on_clearButton_clicked() {
  clearPoints();
  refreshPointsCountText();
  isVecCleared = true;
  update();
}

void Okno::on_closeButton_clicked() { QApplication::exit(); }

void Okno::on_generateButton_clicked() {
  if (isVecCleared || points.size() < startPointsCount) {
    generateRandomPoints();
    refreshPointsCountText();
    isVecCleared = false;
    update();
  }
}

// Points-----------------------------------------
void Okno::refreshPointsCountText() {
  ui->points_count_label->setText(QString::number(points.size(), 10));
}

void Okno::generateRandomPoints() {
  while (points.size() < startPointsCount) {
    if (points.size() == 0) {
      points.push_back(setRandomPoint());
    } else {
      auto newPoint = setRandomPoint();
      bool collision = false;
      for (auto &point : points) {
        if (checkCollision(point, newPoint, 400))
          collision = true;
      }
      if (!collision)
        points.push_back(newPoint);
    }
  }
  refreshPointsCountText();
}

void Okno::clearPoints() { points.clear(); }

// Mouse---------------------------------------
void Okno::mousePressEvent(QMouseEvent *event) {
  QPoint mousePoint{event->pos()};

  if (event->button() == Qt::LeftButton) {
    if (auto found = findCollision(mousePoint, 400); found == points.end()) {
      points.push_back(mousePoint);
    }
    update();
    refreshPointsCountText();
  }

  if (event->button() == Qt::RightButton) {
    if (auto found = findCollision(mousePoint, 400); found != points.end()) {
      points.erase(found);
    }
    update();
    refreshPointsCountText();
  }
}

std::vector<QPoint>::const_iterator Okno::findCollision(const QPoint &point1,
                                                        int distance) const {
  return std::find_if(points.begin(), points.end(),
                      [point1, distance](auto const &point2) {
                        return checkCollision(point1, point2, distance);
                      });
}

// Draw-------------------------------------------
void Okno::paintEvent(QPaintEvent *e) {
  QPainter painter(this);
  drawLines(painter);
  drawPoints(painter);
}

void Okno::drawPoints(QPainter &painter) const {
  configurePainterForPoints(painter);
  for_each(points.begin(), points.end(),
           [&](QPoint point) { painter.drawEllipse(point, 10, 10); });
}

void Okno::drawLines(QPainter &painter) const {
  if (points.size() > 1) {
    configurePainterForLines(painter);
    for (std::vector<QPoint>::const_iterator it = points.begin();
         it != points.end() - 1; ++it) {
      painter.drawLine(*it, *(it + 1));
    }
  }
}
