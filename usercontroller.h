#pragma once

#include <QWidget>

class MainWindow;

class UserController : public QWidget {
  Q_OBJECT

public:
  explicit UserController(QWidget *parent = nullptr);

public slots:

private slots:

signals:
    void clearButtonClicked();
    void generateButtonClicked();
    void exitButtonClicked();
    void freeFallButtonClicked();
};
