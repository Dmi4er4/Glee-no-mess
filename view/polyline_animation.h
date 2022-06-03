#ifndef VIEW_POLYLINE_ANIMATION_H_
#define VIEW_POLYLINE_ANIMATION_H_

#include <QApplication>
#include <QDebug>
#include <QJsonArray>
#include <QPoint>

#include <algorithm>
#include <numeric>
#include <vector>

class PolylineAnimation {
 public:
  PolylineAnimation(const QString& filename);

  void Do(int millis, double max_progress = 1);
  void Do(int millis, int index_in_queue);
  QPoint Position() const;
  bool Finished() const;

  auto GetTimePassed() { return static_cast<int>(progress_ * time_ms_); }

 private:
  std::vector<QPoint> waypoints_;
  std::vector<int> stops_;
  double length_;
  double time_ms_;
  std::vector<double> segments_;
  std::vector<double> prefix_sums_;
  double progress_;
};

#endif  // VIEW_POLYLINE_ANIMATION_H_
