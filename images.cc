#include "images.h"

#include <utility>

const Point Detail::as_polar(const Point &p) {
  return p.is_polar ? p : to_polar(p);
}

const Point Detail::as_cartesian(const Point &p) {
  return p.is_polar ? from_polar(p) : p;
}

Image cond(const Region &region, const Image &this_way, const Image &that_way) {
  return [=](const Point p) { return region(p) ? this_way(p) : that_way(p); };
}

Image lerp(Blend blend, Image this_way, Image that_way) {
  using namespace std::placeholders;
  return lift(std::bind(&Color::weighted_mean, _1, _2, _3),
              std::move(this_way),
              that_way,
              blend);
}

Image darken(Image image, Blend blend) {
  return lerp(std::move(blend), std::move(image), constant(Colors::black));
}

Image lighten(Image image, Blend blend) {
  return lerp(std::move(blend), std::move(image), constant(Colors::white));
}
