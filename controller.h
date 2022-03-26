#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <memory>

class View;
class Model;

class Controller {
public:
  Controller();

  void keyPressEvent(QKeyEvent* event);

private:
  void ConnectSignals();

  std::unique_ptr<Model> model_;
  std::unique_ptr<View> view_;
};

#endif  // CONTROLLER_H_
