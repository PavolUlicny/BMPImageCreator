# BMP Image Creator

This project is a C++ program that creates 24-bit BMP images with support for drawing basic shapes and rendering custom bitmap text using an 8×8 pixel font.

It includes:
- Manual construction of BMP headers
- Drawing of pixels, lines, rectangles, and circles
- Bitmap font loading and scalable text rendering
- Saving images as `.bmp` files

> Font source (modified): [Darkrose 8x8 ASCII font](https://opengameart.org/content/8x8-ascii-bitmap-font-with-c-source)

---

## 🛠️ Class: `BMPImageCreator`

### Constructor
BMPImageCreator(int32_t width, int32_t height);
Initializes a new BMP image of specified dimensions.

If invalid dimensions are given, defaults to 10×5.

Prepares BMP headers and a white image.

void setDefaultPixelRGB(int r, int g, int b);
Fills the entire image with the specified RGB color.

Clamps values between 0–255.

void setPixel(int32_t x, int32_t y, int r, int g, int b);
Sets the color of a single pixel at (x, y).

Ignores coordinates outside the image bounds.

void drawRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, int r, int g, int b, bool fill);
Draws a rectangle from (x, y) to (x1, y1).

If fill is true, it fills the rectangle.

Otherwise, only the border is drawn.

void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int r, int g, int b);
Draws a straight line between two points using Bresenham’s algorithm.

void drawCircle(int32_t centerX, int32_t centerY, int32_t radius, int r, int g, int b, bool fill);
Draws a circle centered at (centerX, centerY) with a given radius.

If fill is true, it draws a filled circle using horizontal scanlines.

bool loadFont();
Loads a bitmap font from a file named font1.bmp.

Splits the image into 8×8 character tiles.

Converts each character into a 2D bool array (on/off pixels).

Handles missing or special characters.

Returns true if successful.

⚠️ The font image should be the one provided with the code.

void drawText(int startX, int startY, const std::string& text, int r, int g, int b, int scale, bool wrap);
Draws text starting at (startX, startY) using the loaded font.

Supports:

Custom RGB color

Scalable text size (scale)

Word wrapping if wrap is true

Cropped character width for better spacing

Newlines (\n) and spaces

void saveFile(const std::string& filename);
Saves the image as a .bmp file.

Converts RGB to BGR format and flips rows to match BMP specification.

Appends .bmp to the given filename.

🧪 Example Usage (in main())
cpp
Copy
Edit
BMPImageCreator bmp(100, 100);
bmp.setDefaultPixelRGB(255, 255, 255); // White background
bmp.drawRectangle(10, 10, 90, 90, 0, 255, 0, true); // Filled green square
bmp.drawLine(10, 10, 90, 90, 255, 0, 0); // Red diagonal line
bmp.drawCircle(50, 50, 30, 0, 0, 255, false); // Blue circle outline
bmp.drawText(5, 5, "Hello World!", 0, 0, 0, 1, true); // Black text
bmp.saveFile("output_image"); // Saves as output_image.bmp
