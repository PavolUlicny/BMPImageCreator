# 🖼️ BMP Image Creator (C++)

This project is a lightweight C++ utility for generating 24-bit BMP image files from scratch.
It supports drawing basic shapes, rendering text using a custom 8×8 bitmap font, and saving the result as a `.bmp` file — all without external libraries.

> Font used: [Darkrose 8x8 ASCII font](https://opengameart.org/content/8x8-ascii-bitmap-font-with-c-source) (modified)

---

## Features

*  Create BMP images of any size
*  Draw pixels, rectangles, lines, and circles
*  Render text with optional wrapping and scaling
*  Save the final image as a valid BMP file
*  Pure C++ — no dependencies beyond the STL

---

## Class: `BMPImageCreator`

### Constructor

```cpp
BMPImageCreator(int32_t width, int32_t height);
```

Initializes the BMP image with the specified size. If width/height are invalid, defaults to 10×5 pixels.

---

### Drawing Functions

#### `void setDefaultPixelRGB(int r, int g, int b);`

Fills the entire image with a single background color (RGB). Values are clamped to 0–255.

---

#### `void setPixel(int32_t x, int32_t y, int r, int g, int b);`

Sets the color of a single pixel at position `(x, y)`.
Ignores coordinates outside the image boundary.

---

#### `void drawRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, int r, int g, int b, bool fill);`

Draws a rectangle between `(x, y)` and `(x1, y1)`.

* `fill = true` → filled rectangle
* `fill = false` → border only

---

#### `void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int r, int g, int b);`

Draws a straight line using **Bresenham’s algorithm**.

---

#### `void drawCircle(int32_t centerX, int32_t centerY, int radius, int r, int g, int b, bool fill);`

Draws a circle centered at `(centerX, centerY)`.

* `fill = true` → filled circle
* `fill = false` → outline only
* Uses the **Midpoint Circle Algorithm**

---

### Text Rendering

#### `bool loadFont();`

Loads and processes the bitmap font from `font1.bmp`.

* Splits the image into 8×8 tiles
* Converts each character into a `bool` matrix (on/off pixels)
* Handles placeholder/missing glyphs

---

#### `void drawText(int startX, int startY, const std::string& text, int r, int g, int b, int scale, bool wrap);`

Renders a text string at the specified position with optional **scaling** and **word wrapping**.

* Automatically crops extra spacing around characters
* Supports:

  * `\n` for line breaks
  * Word wrapping at image edge (if `wrap = true`)
  * Space optimization (cropped characters)
* Font is lazily loaded on first call

---

### Saving

#### `void saveFile(const std::string& filename);`

Saves the image as `filename.bmp`.
Handles:

* RGB → BGR conversion
* Bottom-up row order (BMP spec)
* Header writing

---

## Example Usage

```cpp
BMPImageCreator bmp(200, 150);

bmp.setDefaultPixelRGB(255, 255, 255); // White background
bmp.drawRectangle(20, 20, 180, 130, 0, 128, 0, true); // Filled green rectangle
bmp.drawLine(20, 20, 180, 130, 255, 0, 0); // Red diagonal line
bmp.drawCircle(100, 75, 40, 0, 0, 255, false); // Blue circle outline
bmp.drawText(10, 10, "Hello,\nBMP World!", 0, 0, 0, 1, true); // Black wrapped text

bmp.saveFile("output_image");
```

---

## Font Requirements

* Code only works with the bundled font image.
* If you wish to use another font, you will have to modify the code.

---

## Compilation

### Compile with g++

```bash
g++ -std=c++11 -o bmp_creator main.cpp
```

Then run:

```bash
./bmp_creator
```

---

## License

MIT License – you are free to use, modify, and distribute this code.
