#include "okno.h"
#include "./ui_okno.h"

Okno::Okno(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Okno)
{
    ui->setupUi(this);
    refreshPointsCountText();
    generatePoints();
    ui->displayFrame->setWindowOpacity(0.2); //
}

Okno::~Okno()
{
    delete ui;
}

//Buttons----------------------------------------
void Okno::on_clearButton_clicked()
{
    clearPoints();
    refreshPointsCountText();
    isVecCleared = true;
    update();
}

void Okno::on_closeButton_clicked()
{
    QApplication::exit();
}

void Okno::on_generateButton_clicked()
{
    if(isVecCleared || points.size() < startPointsCount)
    {
    generatePoints();
    refreshPointsCountText();
    isVecCleared = false;
    update();
    }
}

//Points-----------------------------------------
void Okno::refreshPointsCountText()
{
   ui->points_count_label->setText(QString::number(points.size(),10));
}

void Okno::generatePoints() // dodac resizing
{
    while (points.size() < startPointsCount)
    {
        if (points.size() == 0)
        {
            int x = QRandomGenerator::global()->bounded(30,770);
            int y = QRandomGenerator::global()->bounded(50,530);
            points.push_back(QPoint(x,y));
        }
        else
        {
            int x = QRandomGenerator::global()->bounded(30,770);
            int y = QRandomGenerator::global()->bounded(60,530);
            bool collision{false};
            for (auto &p : points)
            {
                if ( (pow(p.x()-x, 2) < 300) && (pow(p.y()-y, 2) < 300) )
                {
                    collision = true;
                }
            }
            if (!collision) points.push_back(QPoint(x,y));
        }
    }
    refreshPointsCountText();
}

void Okno::clearPoints()
{
    points.clear();
}

//Mouse draw and delete-------------------------------------
void Okno::mousePressEvent(QMouseEvent *event)
{
    QPoint mousePoint {event->pos()};
    bool mouseCollision {false};
    std::vector<QPoint>::iterator itPointToDelete;

    for (std::vector<QPoint>::iterator it = points.begin() ; it != points.end(); ++it)
    {
        int distance = 400;
        if ( (pow((*it).x() - mousePoint.x(), 2) < distance) && (pow((*it).y() - mousePoint.y(), 2) < distance) )
        {
            mouseCollision = true;
            itPointToDelete = it;
        }
    }
    if (!mouseCollision) points.push_back(mousePoint);
    else points.erase(itPointToDelete);

    update();
    refreshPointsCountText();
}

//Draw-------------------------------------------
void Okno::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    drawLines(&painter);
    drawPoints(&painter);
}

void Okno::drawPoints(QPainter *painter) const
{
    QPen point_pen(Qt::white);
    point_pen.setWidth(1);
    painter->setPen(point_pen);
    QBrush brush(Qt::gray);
    painter->setBrush(brush);

    for_each (points.begin(), points.end(), [&](QPoint point){painter->drawEllipse(point, 10, 10);});
}

void Okno::drawLines(QPainter *painter) const
{
    if (points.size() > 1)
    {
        QPen line_pen(Qt::green);
        line_pen.setWidth(1);
        painter->setPen(line_pen);
        for (std::vector<QPoint>::const_iterator it = points.begin() ; it != points.end()-1; ++it)
        {
            painter->drawLine(*it, *(it+1));
        }
    }
}


