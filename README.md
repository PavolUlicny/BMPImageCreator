# BMPImageCreator

> A minimal **C++** library for creating BMP images with drawing primitives and bitmap-font text rendering.

---

## 📋 Table of Contents

1. [Features]
2. [API Reference]
3. [Example]
4. [License]

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

**MIT License**. See [LICENSE](LICENSE) for details.
