# BMPImageCreator

A simple C++ library to generate BMP images with basic drawing primitives and bitmap-font text rendering.

---

## Features

* Draw pixels, lines, rectangles (filled or outlined), and circles (filled or outlined).
* Load and render a cropped 8×8 monochrome font from a `.fnt` file with scaling and word wrap.
* Automatic clipping of out-of-bounds pixels.
* Pure C++17: no external dependencies beyond the standard library.

---

## API Reference

| Function                                                                                   | Description                                                          |
| ------------------------------------------------------------------------------------------ | -------------------------------------------------------------------- |
| `BMPImageCreator(int32_t width, int32_t height)`                                           | Allocate canvas and initialize BMP headers (24-bit color).           |
| `void setDefaultPixelRGB(int r, int g, int b)`                                             | Fill entire canvas with a solid RGB color (clamped 0–255).           |
| `void setPixel(int x, int y, int r, int g, int b)`                                         | Set a single pixel at (x,y); ignores out-of-bounds and clamps color. |
| `void drawRectangle(int x0,int y0,int x1,int y1,int r,int g,int b,bool fill)`              | Draw a filled or outlined rectangle; swaps coords internally.        |
| `void drawLine(int x0,int y0,int x1,int y1,int r,int g,int b)`                             | Draw a line using Bresenham’s algorithm.                             |
| `void drawCircle(int cx,int cy,int radius,int r,int g,int b,bool fill)`                    | Draw a circle using the Midpoint algorithm (filled or outline).      |
| `bool loadFont(const std::string &filename)`                                               | Load and crop a bitpacked 8×8 `.fnt` font with 128 glyphs.           |
| `void drawText(int x,int y,const std::string &text,int r,int g,int b,int scale,bool wrap)` | Render ASCII text with scaling and word-wrap.                        |
| `void saveFile(const std::string &filename) const`                                         | Write the canvas to `<filename>.bmp` (BGR, bottom-up rows).          |

---

## Project Structure

```
core/
  ├─ bmp_image_creator.cpp
  ├─ bmp_image_creator.h
  └─ font.fnt
example/
  ├─ example.cpp
  └─ output_image.bmp
legacy/
  └─ bmp_image_creator_legacy.cpp
```

---

## Usage

```cpp
#include "core/BMPImageCreator.h"

int main() {
    BMPImageCreator bmp(100, 100);
    bmp.setDefaultPixelRGB(255, 0, 0);                            // red background
    bmp.drawRectangle(10, 10, 90, 90, 0, 255, 0, true);           // filled green box
    bmp.drawLine(10, 10, 90, 90, 0, 0, 255);                      // blue diagonal
    bmp.drawCircle(50, 50, 30, 255, 255, 0, false);               // yellow circle outline
    bmp.drawText(10, 10, "Hello,\nBMP world!", 0, 0, 0, 1, true); // wrapped text
    bmp.saveFile("output_image");                                 // writes "output_image.bmp"
    return 0;
}
```
## Font

* Font from darkrose (https://opengameart.org/content/8x8-ascii-bitmap-font-with-c-source) (modified, converted to .fnt (bit images of each character) and cropped)
---

## License

* Everyone is free to **use**, **modify**, and **distribute** this code for any purpose, with or without attribution.
