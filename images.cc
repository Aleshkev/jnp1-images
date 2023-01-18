#include "images.h"
#include <iostream>

Image cond(Region region, Image this_way, Image that_way) {
  return [=](const Point p) { return region(p) ? this_way(p) : that_way(p); };
}

Image lerp(Blend blend, Image this_way, Image that_way) {
  return [=](const Point p) {
    return this_way(p).weighted_mean(that_way(p), blend(p));
  };
}

Image darken(Image image, Blend blend) {
  return lerp(blend, image, [](const Point) { return Colors::black; });
}

Image lighten(Image image, Blend blend) {
  return lerp(blend, image, [](const Point) { return Colors::white; });
}
