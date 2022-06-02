#ifndef POLYLINE_ANIMATION_H
#define POLYLINE_ANIMATION_H

#include <QApplication>
#include <QDebug>
#include <QJsonArray>
#include <QPoint>

#include <numeric>
#include <vector>

class PolylineAnimation {
 public:
  PolylineAnimation(const QString& filename,
                    QWidget* host,
                    double seconds = 1);

  void Do(int millis, double max_progress = 1);
  void Do(int millis, int index_in_queue);
  QPoint Position() const;
  bool Finished() const;

 private:
  std::vector<QPoint> waypoints_;
  std::vector<int> stops_;
  double length_;
  double time_ms_;
  std::vector<double> segments_;
  std::vector<double> prefix_sums_;
  double progress_;
  QWidget* host_;
};

#endif  // POLYLINE_ANIMATION_H
