# BMPImageCreator

* A simple C++ library to generate BMP images with basic drawing primitives and bitmap-font text rendering.

* If you get the fatal error "font file not found", check [Debugging](#debugging).

---

## Features

* Draw pixels, lines, rectangles (filled or outlined), and circles (filled or outlined).
* Load and render a cropped 8×8 monochrome font from a [`.fnt` file](src/font.fnt) with scaling and word wrap.
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

src/
  ├─ [bmp_image_creator.cpp](src/bmp_image_creator.cpp)
  ├─ [bmp_image_creator.h](src/bmp_image_creator.h)
  └─ [font.fnt](src/font.fnt)
example/
  ├─ [example.cpp](example/example.cpp)
  └─ [output_image.bmp](example/output_image.bmp)
legacy/
  └─ [bmp_image_creator_legacy.cpp](legacy/bmp_image_creator_legacy.cpp)

---

## How to run

* You can use the included [example_program](example/example_program.exe) executable in the example folder to run example.cpp.

* If you want to create a new executable from [example.cpp](example/example.cpp), what i recommend (if you're compiling with g++) is opening an integrated terminal in the [example folder](example/) and running the powershell command below in the terminal. The executable "program" will appear and create the image in the example folder.

```powershell
g++ -I example example.cpp ../src/bmp_image_creator.cpp -o program
```

---

## Debugging 

* If you the program can't find the font (throws an error), change the font path in the [header file](src/bmp_image_creator.h) to the [font file](src/font.fnt) location (based on where you're compiling from).

---

## Usage

```cpp
#include "../src/bmp_image_creator.h"

int main() {

    // Example usage of BMPImageCreator
    BMPImageCreator bmp(100, 100);

    // Set default pixel color to red
    bmp.setDefaultPixelRGB(255, 0, 0); 

    // Draw a filled green rectangle
    bmp.drawRectangle(10, 10, 90, 90, 0, 255, 0, true); 

    // Draw a blue line
    bmp.drawLine(10, 10, 90, 90, 0, 0, 255); 

    // Draw a yellow circle outline
    bmp.drawCircle(50, 50, 30, 255, 255, 0, false);  

    // Black wrapped text
    bmp.drawText(10, 10, "Hello,\nBMP world!", 0, 0, 0, 1, true);

    // Save the BMP image to a file
    bmp.saveFile("output_image"); 

    return 0;
}
```
## Font

* [Font](src/font.fnt) from darkrose (https://opengameart.org/content/8x8-ascii-bitmap-font-with-c-source) (modified, converted to .fnt (bit images of each character) and cropped)
---

## License

* Everyone is free to **use**, **modify**, and **distribute** this code for any purpose, with or without attribution.
