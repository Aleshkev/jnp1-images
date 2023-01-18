Pliki z rozwiązaniem:

- [functional.h](functional.h)
- [images.h](images.h)
- [images.cc](images.cc)

Notatki 🌟

- Funkcje w argumentach przyjmują `const T &`, ale wszystkie funkcje są
  przekazywane po wartości (bo inaczej trochę przestaje być ładnie funkcyjnie
  jak trzeba stworzyć specjalny obiekt żeby przekazać funkcję).
- Kompilacja jest dziwna, jakoś pliki `.c` prof. Peczarskiego nie
  robią `#include` odpowiedniego pliku `.h`, tylko same załączają wszystkie
  nagłówki. Więc nie może być implementacji w plikach `.h`. Oprócz implementacji
  template'ów, które tam muszą być.
