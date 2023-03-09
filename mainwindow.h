#pragma once

#include <QWidget>

class InteractivePointsTable;
class UserController;


class MainWindow : public QWidget {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void clearButtonClicked() const;
  void exitButtonClicked() const;
  void generateButtonClicked() const;

private:
  InteractivePointsTable *interactivePointsTable;
  UserController *userController;
};
