#ifndef OKNO_H
#define OKNO_H

#include <QWidget>
#include <QPoint>
#include <QLine>
#include <QPainter>
#include <QPen>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Okno; }
QT_END_NAMESPACE

class Okno : public QWidget
{
    Q_OBJECT

public:
    Okno(QWidget *parent = nullptr);
    ~Okno();

private slots:
    void on_clearButton_clicked();
    void on_closeButton_clicked();
    void on_generateButton_clicked();

    void refreshPointsCountText();
    void generatePoints();
    void clearPoints();

protected:
    void paintEvent(QPaintEvent *event);
    void drawPoints(QPainter *painter) const;
    void drawLines(QPainter *painter) const;

    void mousePressEvent(QMouseEvent *event);

private:
    Ui::Okno *ui;
    size_t startPointsCount{20};
    std::vector<QPoint> points;
    bool isVecCleared {false};

};



#endif // OKNO_H
