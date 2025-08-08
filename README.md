BMPImageCreator

BMPImageCreator is a lightweight C++ library for generating BMP images with custom drawings and text.

Overview of Core Functions

Constructor: BMPImageCreator(int32_t width, int32_t height)

Initializes a blank canvas of given dimensions.

Computes BMP headers (file header & DIB header) with 24‑bit color.

Allocates and fills the internal pixel buffer (white by default).

void setDefaultPixelRGB(int r, int g, int b)

Fills the entire canvas with a solid RGB color.

Clamps each channel between 0 and 255.

void setPixel(int x, int y, int r, int g, int b)

Sets a single pixel at (x, y) to the specified RGB color.

Automatically ignores out‑of‑bounds coordinates.

Clamps RGB values to valid range.

void drawRectangle(int x0, int y0, int x1, int y1, int r, int g, int b, bool fill)

Draws a rectangle between (x0,y0) and (x1,y1).

If fill == true, fills the interior; otherwise draws only the border.

Coordinates may be passed in any order (function swaps as needed).

void drawLine(int x0, int y0, int x1, int y1, int r, int g, int b)

Implements Bresenham’s line algorithm.

Efficient integer-based line drawing between two points.

void drawCircle(int centerX, int centerY, int radius, int r, int g, int b, bool fill)

Uses the Midpoint Circle Algorithm.

If fill == true, draws horizontal scanlines; otherwise draws only the perimeter.

Font Loading: bool loadFont(const std::string& filename)

Reads a 1‑bit (8×8) .fnt file containing 128 glyphs.

Stores raw bitmaps in font_chars[char][x][y].

Automatically identifies and removes empty columns for each glyph, populating cropped_chars[char].

Returns true on complete success; leaves existing font data intact on failure.

void drawText(int startX, int startY, const std::string& text, int r, int g, int b, int scale, bool wrap)

Renders ASCII text using the loaded font.

Splits input into words, spaces, and newlines.

Measures word pixel width; applies line wrapping if wrap == true.

Draws each glyph scaled by scale at the appropriate position.

void saveFile(const std::string& filename)

Flattens the in‑memory pixel buffer into BMP pixel data (BGR, bottom‑up rows).

Writes the BMP and DIB headers followed by pixel data to <filename>.bmp.

Example Usage

#include "BMPImageCreator.h"
int main() {
    BMPImageCreator bmp(200, 100);
    bmp.setDefaultPixelRGB(240, 240, 240);
    bmp.drawRectangle(10, 10, 190, 90, 0, 128, 255, false);
    bmp.drawLine(0, 0, 199, 99, 255, 0, 0);
    bmp.drawCircle(100, 50, 30, 0, 255, 0, true);
    bmp.drawText(20, 20, "Hello, BMP!", 0, 0, 0, 2, true);
    bmp.saveFile("example_output");
    return 0;
}

License

This project is licensed under the MIT License. See LICENSE for details.
