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

bool checkCollision(const QPoint& point1, const QPoint& point2, const int minimumDistance) {
    return ((pow(point1.x() - point2.x(), 2) < minimumDistance) && (pow(point1.y() - point2.y(), 2) < minimumDistance));
}

} // namespace

Okno::Okno(QWidget *parent) : QWidget(parent), ui(new Ui::Okno) {
  ui->setupUi(this);
  refreshPointsCountText();
  generatePoints();
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
    generatePoints();
    refreshPointsCountText();
    isVecCleared = false;
    update();
  }
}

// Points-----------------------------------------
void Okno::refreshPointsCountText() {
  ui->points_count_label->setText(QString::number(points.size(), 10));
}

void Okno::generatePoints() {
  while (points.size() < startPointsCount) {
    if (points.size() == 0) {
      int x = QRandomGenerator::global()->bounded(30, 770);
      int y = QRandomGenerator::global()->bounded(50, 530);
      points.push_back(QPoint(x, y));
    } else {
      int x = QRandomGenerator::global()->bounded(30, 770);
      int y = QRandomGenerator::global()->bounded(60, 530);
      bool collision{false};
      for (auto &p : points) {
        if ((pow(p.x() - x, 2) < 300) && (pow(p.y() - y, 2) < 300)) {
          collision = true;
        }
      }
      if (!collision)
        points.push_back(QPoint(x, y));
    }
  }
  refreshPointsCountText();
}

void Okno::clearPoints() { points.clear(); }

// Mouse---------------------------------------
void Okno::mousePressEvent(QMouseEvent *event) {
  QPoint mousePoint{event->pos()};
  bool mouseCollision{false};
  std::vector<QPoint>::iterator itPointToDelete;

  if (event->button() == Qt::LeftButton) {
    for (std::vector<QPoint>::iterator it = points.begin(); it != points.end();
         ++it) {
      int distance = 400;
      if (checkCollision(mousePoint, *it, distance)) {
        mouseCollision = true;
      }
    }
    if (!mouseCollision)
      points.push_back(mousePoint);
    update();
    refreshPointsCountText();
  }

  if (event->button() == Qt::RightButton) {

    for (std::vector<QPoint>::iterator it = points.begin(); it != points.end();
         ++it) {
      int distance = 400;
      if (checkCollision(mousePoint, *it, distance)) {
        mouseCollision = true;
        itPointToDelete = it;
      }
    }
    if (mouseCollision)
      points.erase(itPointToDelete);
    update();
    refreshPointsCountText();
  }
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
           [&](QPoint point) { painter->drawEllipse(point, 10, 10); });
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
