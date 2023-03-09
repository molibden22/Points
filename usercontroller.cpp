#include "usercontroller.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QPushButton>


UserController::UserController(QWidget *parent)
    : QWidget{parent}
{
    auto horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(100);
    horizontalLayout->setMargin(5);

    auto clearButton = new QPushButton();
    horizontalLayout->addWidget(clearButton, Qt::AlignLeft);
    clearButton->setText("Clear");
    clearButton->setMinimumHeight(40);

    auto generateButton = new QPushButton();
    horizontalLayout->addWidget(generateButton, Qt::AlignLeft);
    generateButton->setText("Generate");
    generateButton->setMinimumHeight(40);

    auto exitButton = new QPushButton();
    horizontalLayout->addWidget(exitButton, Qt::AlignRight);
    exitButton->setText("Exit");
    exitButton->setMinimumHeight(40);
    exitButton->setMaximumWidth(100);

    connect(clearButton, &QPushButton::clicked, this, &UserController::clearButtonClicked);
    connect(generateButton, &QPushButton::clicked, this, &UserController::generateButtonClicked);
    connect(exitButton, &QPushButton::clicked, this, &UserController::exitButtonClicked);
}


