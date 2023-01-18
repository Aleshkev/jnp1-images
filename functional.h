// pomocnicza biblioteka  implementująca  pewne konstrukcje znane  z języków
// funkcyjnych.

#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <functional>

// Realizuje złożenie funkcji w następujący sposób:
//
//     compose()(x) == identyczność(x)
//     compose(f)(x) == f(x)
//     compose(f, g)(x) == g(f(x))
//     itd.
//
template<typename _T = nullptr_t> auto compose() {
  return [=](auto x) { return x; };
}
template <typename F> auto compose(F f) {
  return f;
}
template <typename F, typename G, typename... H>
auto compose(F f, G g, H &&...h) {
  return compose([=](auto x) { return g(f(x)); }, h...);
}

// Realizuje „pointwise lifting” w następujący sposób:
//  lift(h)
//    a -> (p -> a)
//    h
//    Uwaga: h zwraca tutaj a.
//  lift(h, f1)
//    (a -> b) -> (p -> a) -> (p -> b)
//       h           f1
//  lift(h, f1, f2)
//    (a -> b -> g) -> (p -> a) -> (p -> b) -> (p -> g)
//          h             f1          f2
//  ...
//  lift(h, f1, ..., fn)
//    (a1 -> ... -> an -> d) -> (p -> a1) -> ... -> (p -> an) -> (p -> d)
//              h                  f1                  fn
//  ...
//
//  Celem  wyjaśnienia   bardziej  szczegółowo  zostanie   opisany  przypadek
//  lift(h, f1, f2)
//    (a -> b -> g) -> (p -> a) -> (p -> b) -> (p -> g)
//          h             f1          f2
//  - Funkcja h przyjmuje argumenty a oraz b i zwraca wartość g.
//  - Funkcja f1 przyjmuje argument p i zwraca wartość a, którą lift umiesz-
//    cza jako pierwszy argument faktyczny funkcji h.
//  - Funkcja f2  przyjmuje argument p i zwraca wartość b, którą lift umiesz-
//    cza jako drugi argument faktyczny funkcji h.
//  - Funkcja lift przyjmuje argumenty h, f1 oraz f2 i zwraca funkcję,  która
//    przyjmuje argument p i zwraca wartość g.
//  Nazwa „pointwise lifting” powinna w tym momencie wydawać się już bardziej
//  naturalna.

template <typename H> auto lift(H h) {
  return [=](auto) { return h(); };
}

template <typename H, typename F, typename... G> auto lift(H h, F f, G &&...g) {
  return [=](auto p) { return lift(std::bind_front(h, f(p)), g...)(p); };
}

#endif