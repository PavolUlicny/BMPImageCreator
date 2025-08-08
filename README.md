# BMPImageCreator

<<<<<<< HEAD
A simple C++ library to generate BMP images with basic drawing primitives and bitmap-font text rendering.
=======
> A minimal **C++** library for creating BMP images with drawing primitives and bitmap-font text rendering.
>>>>>>> 37d6aaed0c262da1ee53861c7232b40cedd936d6

---

## 📋 Table of Contents

<<<<<<< HEAD
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
    bmp.drawRectangle(10, 10, 90, 90, 0, 255, 0, true);          // filled green box
    bmp.drawLine(10, 10, 90, 90, 0, 0, 255);                      // blue diagonal
    bmp.drawCircle(50, 50, 30, 255, 255, 0, false);               // yellow circle outline
    bmp.drawText(10, 10, "Hello,\nBMP world!", 0, 0, 0, 1, true); // wrapped text
    bmp.saveFile("output_image");                              // writes "output_image.bmp"
    return 0;
}
```
---

## Font

Font from darkrose (https://opengameart.org/content/8x8-ascii-bitmap-font-with-c-source) (modified, converted to `.fnt` (bit images of each char) and cropped)

---

## License

Everyone is free to **use**, **modify**, and **distribute** this code for any purpose, with or without attribution.
=======
1. Features
2. API Reference
3. Example
4. License

---

## 🌟 Features

* **Drawing primitives**: lines, rectangles (filled & outlined), circles (filled & outlined)
* **Bitmap font**: load, crop, scale, wrap, and render 8×8 monochrome fonts
* **Safe rendering**: automatic clipping of out-of-bounds pixels
* **Pure C++17**: no external dependencies

---

## 📚 API Reference

| Function                                                                                              | Description                                                        |
| ----------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ |
| **Constructor**<br>`BMPImageCreator(int32_t width, int32_t height)`                                   | Allocate a canvas and initialize BMP headers (24-bit color).       |
| **`void setDefaultPixelRGB(int r, int g, int b)`**                                                    | Fill the canvas with a solid RGB color. Values clamped \[0–255].   |
| **`void setPixel(int x, int y, int r, int g, int b)`**                                                | Set a single pixel; ignores out-of-bounds and clamps RGB.          |
| **`void drawRectangle(int x0, int y0, int x1, int y1, int r, int g, int b, bool fill)`**              | Draw a rectangle—filled or border only. Coordinates auto-swapped.  |
| **`void drawLine(int x0, int y0, int x1, int y1, int r, int g, int b)`**                              | Bresenham’s line algorithm for efficient line drawing.             |
| **`void drawCircle(int cx, int cy, int radius, int r, int g, int b, bool fill)`**                     | Midpoint Circle Algorithm; filled or outline.                      |
| **`bool loadFont(const std::string &filename)`**                                                      | Load & crop a 1-bit 8×8 `.fnt` file (128 glyphs). Returns success. |
| **`void drawText(int x, int y, const std::string &text, int r, int g, int b, int scale, bool wrap)`** | Render ASCII text with word-wrap and scaling.                      |
| **`void saveFile(const std::string &filename)`**                                                      | Write the canvas to `<filename>.bmp` (BGR, bottom-up rows).        |

---

## 💡 Example

```cpp
#include "BMPImageCreator.h"

int main() {
    // Create a 200×100 image
    BMPImageCreator bmp(200, 100);

    // Light gray background
    bmp.setDefaultPixelRGB(240, 240, 240);

    // Draw primitives
    bmp.drawRectangle(10, 10, 190, 90, 0, 128, 255, false);
    bmp.drawLine(0, 0, 199, 99, 255, 0, 0);
    bmp.drawCircle(100, 50, 30, 0, 255, 0, true);

    // Render scaled, wrapped text
    bmp.drawText(20, 20, "Hello, BMPImageCreator!", 0, 0, 0, 2, true);

    // Save to BMP
    bmp.saveFile("example_output");
    return 0;
}
```

Result: `example_output.bmp` shown below.

---

## 📄 License

**MIT License**. Everyone is free to use, modify, and distribute this code for any purpose, with or without attribution.
>>>>>>> 37d6aaed0c262da1ee53861c7232b40cedd936d6
