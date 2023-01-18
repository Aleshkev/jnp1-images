// Biblioteka implementująca obrazy funkcyjne.

#ifndef IMAGES_H
#define IMAGES_H

#include "color.h"
#include "coordinate.h"
#include <cassert>
#include <cmath>
#include <functional>

// Typ zmiennoprzecinkowy kompatybilny z std::is_floating_point obsługujący
// liczby z zakresu [0, 1]. Ten typ może zostać zdefiniowany jako alias na
// jeden z typów fundamentalnych.
using Fraction = double;

// Szablon aliasu na std::function o następującej definicji:
//     template<typename T>
//     using Base_image = std::function<T(const Point)>;
// Base_image jest nazywany dalej obrazem bazowym.
template <typename T> using Base_image = std::function<T(const Point)>;

// Region: równoważny Base_image<bool>. Region (przez duże R) jest
// nazywany dalej regionem lub wycinanką. Regiony służą do składania obrazów
// właściwych (zob. poniżej), np. z użyciem funkcji cond opisanej niżej.
using Region = Base_image<bool>;

// Image: równoważny Base_image<Color>. Image jest nazywany dalej obrazem
// właściwym. Obraz właściwy jest kanonicznym przykładem obrazu funkcyjnego.
using Image = Base_image<Color>;

// Blend: równoważny Base_image<Fraction>. Blend jest nazywany dalej
// mieszanką.  Mieszanka służy do mieszania kolorów w proporcjach zadanych
// parametrem typu Fraction.
using Blend = Base_image<Fraction>;

// Base_image<T> constant(T t) – tworzy stały obraz bazowy, a więc zwracający:
//
// - wartość true w przypadku regionu dla t == true,
// - kolor „cynober” w przypadku obrazu właściwego  dla t == Colors::
//   Vermilion lub
// - wartość 0,42 w przypadku mieszanki dla t == 0.42.
//
template <typename T> Base_image<T> constant(const T &t) {
  return [=](const Point point) { return t; };
}

// Obraca obraz bazowy o kąt phi (wyrażony w radianach). Kierunek obrotu można
// określić na podstawie pliku rotate.bmp.
template <typename T> Base_image<T> rotate(Base_image<T> image, double phi) {
  return [=](const Point p) {
    auto q = to_polar(p);
    return image(from_polar({q.first, q.second - phi, true}));
  };
}

// Przesuwa obraz bazowy o wektor v. Kierunek przesunięcia można określić na
// podstawie pliku translate.bmp.
template <typename T> Base_image<T> translate(Base_image<T> image, Vector v) {
  return [=](const Point p) {
    return image({p.first - v.first, p.second - v.second});
  };
}

// Skaluje  obraz  bazowy o współczynnik s. Dla s == 2 otrzymuje się obraz
// bazowy dwukrotnie powiększony.
template <typename T> Base_image<T> scale(Base_image<T> image, double s) {
  return [=](const Point p) { return image({p.first / 2, p.second / 2}); };
}

// Tworzy obraz bazowy koła o środku q, promieniu r oraz zwracanych wartościach
// inner w kole i na obwodzie oraz outer poza nim.
template <typename T>
Base_image<T> circle(Point q, double r, T inner, T outer) {
  return [=](const Point p) { return distance(p, q) <= r ? inner : outer; };
}

// Tworzy obraz bazowy szachownicy o szerokości kratki d oraz zwracanych
// wartościach zadanych przez pozostałe dwa argumenty. Położenie this_way i
// that_way należy wydedukować na podstawie plików example.cc oraz
// checker.bmp.
template <typename T> Base_image<T> checker(double d, T this_way, T that_way) {
  return [=](const Point p) {
    return static_cast<size_t>(floor(p.first / d) + floor(p.second / d)) % 2 ==
                   0
               ? this_way
               : that_way;
  };
}
// Tworzy obraz bazowy „szachownicy biegunowej”. W implementacji należy
// odwołać się do zdefiniowanej wcześniej funkcji checker i przekazać jej
// argument d. Znaczenie parametru n oraz położenie this_way i that_way należy
// wydedukować na podstawie plików example.cc oraz polar_checker.bmp. Można
// założyć, że użytkownik będzie wywoływać funkcję z argumentem n o parzystej
// wartości.
template <typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
  assert(n % 2 == 0);
  auto arc = M_PI / n * 2; // Kąt jaki zajmuje jedno pole szachownicy.
  return [=](const Point p) {
    auto polar = to_polar(p);
    auto x = polar.first;
    // Dodajemy 2 * n * arc, żeby na pewno dostać dodatni kąt.
    auto y = floor((polar.second + 2 * n * arc) / arc) * d;
    return checker(d, this_way, that_way)({x, y});
  };
}
// Obraz bazowy koncentrycznych naprzemiennych pasów szerokości d o środku
// w punkcie q. Położenie this_way i that_way należy wydedukować na podstawie
// plików example.cc oraz rings.bmp.
template <typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way) {
  return [=](const Point p) {
    return static_cast<size_t>(std::floor(distance(p, q) / d)) % 2 == 0
               ? this_way
               : that_way;
  };
}

// Obraz bazowy w postaci wycentrowanego paska this_way o szerokości d
// otoczonego przez that_way.
template <typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
  return [=](const Point p) {
    return std::abs(p.first) <= d / 2 ? this_way : that_way;
  };
}

// Korzysta z regionu (wycinanki), który wycina i klei dwa obrazy właściwe,
// tworząc w ten sposób nowy. Dla punktów p, takich że region(p) == true, brany
// jest obraz this_way, zaś dla pozostałych – that_way.
Image cond(Region region, Image this_way, Image that_way);

// Korzysta z mieszanki, która wyznacza proporcje mieszania kolorów dwóch
// obrazów właściwych, tworząc w ten sposób nowy. Dla każdego punktu p argument
// blend zwraca parametr mieszania oznaczony jako w, który informuje, ile w
// finalnym obrazie dla punktu p będzie koloru z obrazu this_way, a ile z
// obrazu that_way (por. argument o nazwie w metody weighted_mean struktury
// Color w pliku color.h oraz poniższe funkcje darken i lighten).
Image lerp(Blend blend, Image this_way, Image that_way);

// Ściemnia obraz. Dla szczególnego przypadku blend tożsamościowo równego:
// - 1 oznacza stworzenie czarnego obrazu,
// - 0 oznacza kopię image.
Image darken(Image image, Blend blend);

// Rozjaśnia obraz. Dla szczególnego przypadku blend tożsamościowo równego:
// - 1 oznacza stworzenie białego obrazu,
// - 0 oznacza kopię image.
Image lighten(Image image, Blend blend);

#endif
