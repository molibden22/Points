#include "usercontroller.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QPushButton>

UserController::UserController(QWidget *parent) : QWidget{parent} {
  auto horizontalLayout = new QHBoxLayout(this);
  horizontalLayout->setSpacing(100);
  horizontalLayout->setMargin(5);

  auto clearButton = new QPushButton();
  horizontalLayout->addWidget(clearButton, Qt::AlignLeft);
  clearButton->setText("Clear");
  clearButton->setMinimumHeight(40);
  clearButton->setFont(QFont("Segoe", 12, 25, 0));

  auto generateButton = new QPushButton();
  horizontalLayout->addWidget(generateButton, Qt::AlignLeft);
  generateButton->setText("Generate");
  generateButton->setMinimumHeight(40);
  generateButton->setFont(QFont("Segoe", 12, 25, 0));

  auto exitButton = new QPushButton();
  horizontalLayout->addWidget(exitButton, Qt::AlignRight);
  exitButton->setText("Close");
  exitButton->setMinimumHeight(40);
  exitButton->setMaximumWidth(100);
  exitButton->setFont(QFont("Segoe", 12, 25, 0));

  connect(clearButton, &QPushButton::clicked, this,
          &UserController::clearButtonClicked);
  connect(generateButton, &QPushButton::clicked, this,
          &UserController::generateButtonClicked);
  connect(exitButton, &QPushButton::clicked, this,
          &UserController::exitButtonClicked);
}
