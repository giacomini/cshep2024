#include <SFML/Graphics.hpp>
#include <complex>

using Complex = std::complex<double>;

int mandelbrot(Complex const& c)
{
  int i = 0;
  auto z = c;
  for (; i != 256 && norm(z) < 4.; ++i) {
    z = z * z + c;
  }
  return i;
}

auto to_color(int k)
{
  return k < 256 ? sf::Color{static_cast<sf::Uint8>(10 * k), 0, 0}
                 : sf::Color::Black;
}

int main()
{
  int const display_width{800};
  int const display_height{800};

  Complex const top_left{-2.2, 1.5};
  Complex const lower_right{0.8, -1.5};
  auto const diff = lower_right - top_left;

  auto const delta_x = diff.real() / display_width;
  auto const delta_y = diff.imag() / display_height;

  sf::Image image;
  image.create(display_width, display_height);

  for (int row = 0; row != display_height; ++row) {
    for (int column = 0; column != display_width; ++column) {
      auto k = mandelbrot(top_left + Complex{delta_x * column, delta_y * row});
      image.setPixel(column, row, to_color(k));
    }
  }

  image.saveToFile("mandelbrot.png");
}
