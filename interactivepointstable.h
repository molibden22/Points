#pragma once
#include <QFrame>

class InteractivePointsTable : public QFrame {
  Q_OBJECT

public:
  explicit InteractivePointsTable(QWidget* parent = nullptr);

public:
  std::size_t pointsCount() const;
public slots:
  void clearPoints();
  void generateRandomPoints();

signals:
  void pointsCountChanged();

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

private:
  std::vector<QPoint>::const_iterator findCollision(const QPoint& point1, int distance) const;
  std::vector<QPoint>::iterator findCollision(const QPoint &point1, const int distance);
  void drawPoints(QPainter& painter) ;
  void drawLines(QPainter& painter) const;

private:
  std::vector<QPoint> points;
  std::vector<QPointF> pointsF;
  QPoint* currentySelectedPoint{nullptr};
};
