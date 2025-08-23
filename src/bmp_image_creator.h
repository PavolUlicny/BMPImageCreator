#ifndef BMP_IMAGE_CREATOR_H
#define BMP_IMAGE_CREATOR_H

#include <string>
#include <vector>
#include <array>
#include <cstdint>

class BMPImageCreator
{
private:
    // font path (change if you're using it outside the repo)
    std::string font_path = "../src/font.fnt";

    // Constants for BMP format
    static constexpr short file_header_size = 14;
    static constexpr short bitmap_info_header_size = 40;
    static constexpr short pixel_info_offset = file_header_size + bitmap_info_header_size;

    // BMP file header and DIB header
    unsigned char file_header[14] = {0};
    unsigned char bitmap_info_header[40] = {0};

    // Image dimensions and properties
    int32_t width;
    int32_t height;
    int32_t padding_size;
    int32_t row_size;
    int32_t pixel_data_size;
    int32_t file_size;

    // DIB header constants
    static constexpr int32_t bits_per_pixel = 24;
    static constexpr int32_t color_planes = 1;
    static constexpr int32_t compression = 0;
    static constexpr int32_t resolution = 2835;
    static constexpr int32_t colors_used = 0;
    static constexpr int32_t important_colors = 0;

    // Pixel data
    std::vector<std::vector<std::array<unsigned char, 3>>> pixels;

    // Font variables
    bool font_loaded = false;
    const int char_width = 8;
    const int char_height = 8;
    const int char_quantity = 128;
    std::vector<std::vector<std::vector<bool>>> font_chars;
    std::vector<std::vector<std::vector<bool>>> cropped_chars;
    std::vector<int> cropped_char_widths;

public:
    // Constructor
    BMPImageCreator(int32_t width, int32_t height);

    // Drawing functions
    void setDefaultPixelRGB(int r, int g, int b);
    void setPixel(int32_t x, int32_t y, int r, int g, int b);
    void drawRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, int r, int g, int b, bool fill);
    void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int r, int g, int b);
    void drawCircle(int32_t centerX, int32_t centerY, int32_t radius, int r, int g, int b, bool fill);
    void drawText(int startX, int startY, const std::string &text, int r, int g, int b, int scale, bool wrap);

    // Font loader
    bool loadFont(const std::string &filename);

    // File output
    void saveFile(const std::string &filename);
};

#endif // BMP_IMAGE_CREATOR_H
