#include "mainwindow.h"
#include "interactivepointstable.h"
#include "usercontroller.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) : QWidget(parent), interactivePointsTable(new InteractivePointsTable(this)), userController(new UserController(this)) {
    QRect rect(400,300,800,600);
    setGeometry(rect);
    qDebug() << "Main Window:" << this->frameSize() << this->geometry();

  auto verticalLayout = new QVBoxLayout(this);
  auto label = new QLabel(this);
  verticalLayout->addWidget(label);
  label->setText("Points count: 0");
  verticalLayout->addWidget(interactivePointsTable, 1);
  verticalLayout->addWidget(userController);

  connect(userController, &UserController::clearButtonClicked, this, &MainWindow::clearButtonClicked);
  connect(userController, &UserController::generateButtonClicked, this, &MainWindow::generateButtonClicked);
  connect(userController, &UserController::exitButtonClicked, this, &MainWindow::exitButtonClicked);
  connect(interactivePointsTable, &InteractivePointsTable::pointsCountChanged, label, [this, label](){ label->setText("Points count: " + QString::number(interactivePointsTable->pointsCount()));});
  interactivePointsTable->generateRandomPoints();
}

MainWindow::~MainWindow() {}
void MainWindow::clearButtonClicked() const { interactivePointsTable->clearPoints(); }
void MainWindow::generateButtonClicked() const { interactivePointsTable->generateRandomPoints(); }
void MainWindow::exitButtonClicked() const { QApplication::exit(); }

