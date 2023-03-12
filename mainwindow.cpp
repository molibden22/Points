#include "mainwindow.h"
#include "interactivepointstable.h"
#include "usercontroller.h"
#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      interactivePointsTable(new InteractivePointsTable(this)),
      userController(new UserController(this))
{
  QRect rectangle(400, 300, 800, 600);
  setGeometry(rectangle);
  // setStyleSheet("background-color: rgb(100, 100, 100)");
  QPalette paleta(QColor(230, 230, 230), QColor(60, 60, 60),
                  QColor(160, 160, 160), QColor(40, 40, 40), QColor(80, 80, 80),
                  QColor(30, 30, 30), QColor(120, 120, 120));
  QApplication::setPalette(paleta);

  auto verticalLayout = new QVBoxLayout(this);
  auto label = new QLabel(this);
  verticalLayout->addWidget(label);
  label->setText("Points count: 0");
  label->setFont(QFont("Segoe", 12, 25, 0));
  verticalLayout->addWidget(interactivePointsTable, 1);
  verticalLayout->addWidget(userController);

  interactivePointsTable->generateRandomPoints();

  connect(userController, &UserController::clearButtonClicked, this, &MainWindow::clearButtonClicked);
  connect(userController, &UserController::generateButtonClicked, this, &MainWindow::generateButtonClicked);
  connect(userController, &UserController::exitButtonClicked, this, &MainWindow::exitButtonClicked);
  connect(interactivePointsTable, &InteractivePointsTable::pointsCountChanged, label,
          [this, label](){ label->setText("Points count: " + QString::number(interactivePointsTable->pointsCount()));});
}

MainWindow::~MainWindow() {}
void MainWindow::clearButtonClicked() const { interactivePointsTable->clearPoints(); }
void MainWindow::generateButtonClicked() const { interactivePointsTable->generateRandomPoints(); }
void MainWindow::exitButtonClicked() const { QApplication::exit(); }

