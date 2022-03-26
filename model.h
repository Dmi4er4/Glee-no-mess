#ifndef MODEL_H_
#define MODEL_H_

#include <deque>
#include <QObject>
#include "visitor.h"

class View;
class Controller;

class Model : public QObject {
public:
  explicit Model(Controller* controller);
  void Paint();
  void Permit();
  void Reject();
  void ShiftQueue();
  void UpdateErrors();

  std::deque<Visitor*> queue_;
  Visitor* current_;
  int errors_{};

  static constexpr int kQueueLength = 3;

private:
  friend class Controller;

  Controller* controller_;
  View* view_{};
};

#endif  // MODEL_H_
