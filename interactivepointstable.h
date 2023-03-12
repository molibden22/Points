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
  std::vector<QPointF>::const_iterator findCollision(const QPointF& point1, int distance) const;
  std::vector<QPointF>::iterator findCollision(const QPointF& point1, const int distance);
  void drawPoints(QPainter& painter) ;
  void drawLines(QPainter& painter) const;
  QPoint transformToQPoint(QPointF pointF) const;
  QPointF transformToQPointF(QPoint point) const;

private:
  std::vector<QPointF> points;
  QPointF* currentlySelectedPoint{nullptr};
};
