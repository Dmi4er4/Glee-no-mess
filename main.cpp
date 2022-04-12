#include <QApplication>

#include "controller.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Controller::Instance();
  return QApplication::exec();
}
