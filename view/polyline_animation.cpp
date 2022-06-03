#include "polyline_animation.h"
#include "file_loader.h"
#include "view.h"

PolylineAnimation::PolylineAnimation(
    const QString& filename)
    : length_(0),
      progress_(0) {
  auto json = FileLoader::GetFile<QJsonDocument>(filename);
  time_ms_ = json["duration_millis"].toInt();
  auto parts = json["coord"].toArray();
  auto size = View::Instance().size();
  for (auto part : parts) {
    for (auto point : part.toArray()) {
      auto x = point[0].toDouble() * size.width();
      auto y = point[1].toDouble() * size.height();
      waypoints_.emplace_back(x, y);
    }
    stops_.push_back(waypoints_.size() - 1);
  }
  if (waypoints_.size() < 2) {
    qDebug() << "Failed to generate a polyline with "
             << waypoints_.size() << " waypoints_";
    qApp->exit(5);
  }
  segments_.reserve(waypoints_.size() - 1);
  for (int i = 1; i < waypoints_.size(); ++i) {
    auto vector = waypoints_[i] - waypoints_[i - 1];
    double r = std::sqrt(vector.x() * vector.x() +
        vector.y() * vector.y());
    length_ += r;
    segments_.push_back(r);
  }
  prefix_sums_.resize(waypoints_.size());
  std::partial_sum(segments_.begin(),
                   segments_.end(),
                   prefix_sums_.begin() + 1);
}

void PolylineAnimation::Do(int millis, double max_progress) {
  double delta = millis / time_ms_;
  progress_ = std::min(max_progress, progress_ + delta);
}

void PolylineAnimation::Do(int millis, int index_in_queue) {
  auto index_in_polyline =
      stops_[std::max(0UL, stops_.size() - 1 - index_in_queue)];
  Do(millis, prefix_sums_[index_in_polyline] / length_);
}

bool PolylineAnimation::Finished() const {
  return progress_ + 1e-12 > 1;
}

QPoint PolylineAnimation::Position() const {
  double current_dist = length_ * progress_;
  size_t i = std::upper_bound(prefix_sums_.begin(),
                              prefix_sums_.end(),
                              current_dist) - prefix_sums_.begin();
  if (i == prefix_sums_.size()) {
    --i;
  }
  current_dist -= prefix_sums_[i - 1];
  return waypoints_[i - 1] +
      (waypoints_[i] - waypoints_[i - 1])
      * (current_dist / segments_[i - 1]);
}
