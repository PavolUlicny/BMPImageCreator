#include "bmp_image_creator.h"

#include <fstream>
#include <algorithm>

// Constructor
BMPImageCreator::BMPImageCreator(int32_t width1, int32_t height1) {
    if (width1 <= 0 || height1 <= 0) {
        width = 10;
        height = 5;
    }
    else {
        width = width1;
        height = height1;
    }

    padding_size = (4 - (width * 3) % 4) % 4;
    row_size = width * 3 + padding_size;
    pixel_data_size = row_size * height;
    file_size = file_header_size + bitmap_info_header_size + pixel_data_size;

    file_header[0] = 'B';
    file_header[1] = 'M';
    file_header[10] = static_cast<unsigned char>(pixel_info_offset);
    file_header[11] = static_cast<unsigned char>(pixel_info_offset >> 8);
    file_header[12] = static_cast<unsigned char>(pixel_info_offset >> 16);
    file_header[13] = static_cast<unsigned char>(pixel_info_offset >> 24);

    file_header[2] = static_cast<unsigned char>(file_size);
    file_header[3] = static_cast<unsigned char>(file_size >> 8);
    file_header[4] = static_cast<unsigned char>(file_size >> 16);
    file_header[5] = static_cast<unsigned char>(file_size >> 24);

    bitmap_info_header[0] = static_cast<unsigned char>(bitmap_info_header_size);

    bitmap_info_header[4] = static_cast<unsigned char>(width);
    bitmap_info_header[5] = static_cast<unsigned char>(width >> 8);
    bitmap_info_header[6] = static_cast<unsigned char>(width >> 16);
    bitmap_info_header[7] = static_cast<unsigned char>(width >> 24);

    bitmap_info_header[8] = static_cast<unsigned char>(height);
    bitmap_info_header[9] = static_cast<unsigned char>(height >> 8);
    bitmap_info_header[10] = static_cast<unsigned char>(height >> 16);
    bitmap_info_header[11] = static_cast<unsigned char>(height >> 24);

    bitmap_info_header[12] = static_cast<unsigned char>(color_planes);

    bitmap_info_header[14] = static_cast<unsigned char>(bits_per_pixel);

    bitmap_info_header[16] = static_cast<unsigned char>(compression);

    bitmap_info_header[20] = static_cast<unsigned char>(pixel_data_size);
    bitmap_info_header[21] = static_cast<unsigned char>(pixel_data_size >> 8);
    bitmap_info_header[22] = static_cast<unsigned char>(pixel_data_size >> 16);
    bitmap_info_header[23] = static_cast<unsigned char>(pixel_data_size >> 24);

    bitmap_info_header[24] = static_cast<unsigned char>(resolution);
    bitmap_info_header[25] = static_cast<unsigned char>(resolution >> 8);
    bitmap_info_header[26] = static_cast<unsigned char>(resolution >> 16);
    bitmap_info_header[27] = static_cast<unsigned char>(resolution >> 24);

    bitmap_info_header[28] = static_cast<unsigned char>(resolution);
    bitmap_info_header[29] = static_cast<unsigned char>(resolution >> 8);
    bitmap_info_header[30] = static_cast<unsigned char>(resolution >> 16);
    bitmap_info_header[31] = static_cast<unsigned char>(resolution >> 24);

    bitmap_info_header[32] = static_cast<unsigned char>(colors_used);

    bitmap_info_header[36] = static_cast<unsigned char>(important_colors);

    pixels = std::vector<std::vector<std::array<unsigned char, 3>>>(
        height, std::vector<std::array<unsigned char, 3>>(width, { 255, 255, 255 }));

    font_chars.resize(char_quantity, std::vector<std::vector<bool>>(char_height, std::vector<bool>(char_width)));
    cropped_chars.resize(char_quantity, std::vector<std::vector<bool>>(0, std::vector<bool>(0)));
}

// Set default pixel RGB for whole image
void BMPImageCreator::setDefaultPixelRGB(int r, int g, int b) {
    r = std::clamp(r, 0, 255);
    g = std::clamp(g, 0, 255);
    b = std::clamp(b, 0, 255);

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            pixels[y][x] = { static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b) };
        }
    }
}

// Set single pixel at (x,y)
void BMPImageCreator::setPixel(int32_t x, int32_t y, int r, int g, int b) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    r = std::clamp(r, 0, 255);
    g = std::clamp(g, 0, 255);
    b = std::clamp(b, 0, 255);
    pixels[y][x] = { static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b) };
}

// Draw rectangle
void BMPImageCreator::drawRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, int r, int g, int b, bool fill) {
    if (x1 < x) std::swap(x, x1);
    if (y1 < y) std::swap(y, y1);

    if (fill) {
        for (int32_t i = y; i <= y1; ++i) {
            for (int32_t j = x; j <= x1; ++j) {
                setPixel(j, i, r, g, b);
            }
        }
    }
    else {
        for (int32_t i = x; i <= x1; ++i) {
            setPixel(i, y, r, g, b);
            setPixel(i, y1, r, g, b);
        }
        for (int32_t j = y; j <= y1; ++j) {
            setPixel(x, j, r, g, b);
            setPixel(x1, j, r, g, b);
        }
    }
}

// Draw line using Bresenham's algorithm
void BMPImageCreator::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int r, int g, int b) {
    int32_t dx = abs(x1 - x0);
    int32_t dy = -abs(y1 - y0);
    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t sy = y0 < y1 ? 1 : -1;
    int32_t err = dx + dy;

    while (true) {
        setPixel(x0, y0, r, g, b);
        if (x0 == x1 && y0 == y1) break;
        int32_t e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// Draw circle (Midpoint Circle Algorithm)
void BMPImageCreator::drawCircle(int32_t centerX, int32_t centerY, int32_t radius, int r, int g, int b, bool fill) {
    if (radius <= 0) return;

    int32_t x = radius;
    int32_t y = 0;
    int32_t err = 0;

    while (x >= y) {
        if (fill) {
            for (int i = centerX - x; i <= centerX + x; ++i) {
                setPixel(i, centerY + y, r, g, b);
                setPixel(i, centerY - y, r, g, b);
            }
            for (int i = centerX - y; i <= centerX + y; ++i) {
                setPixel(i, centerY + x, r, g, b);
                setPixel(i, centerY - x, r, g, b);
            }
        }
        else {
            setPixel(centerX + x, centerY + y, r, g, b);
            setPixel(centerX + y, centerY + x, r, g, b);
            setPixel(centerX - y, centerY + x, r, g, b);
            setPixel(centerX - x, centerY + y, r, g, b);
            setPixel(centerX - x, centerY - y, r, g, b);
            setPixel(centerX - y, centerY - x, r, g, b);
            setPixel(centerX + y, centerY - x, r, g, b);
            setPixel(centerX + x, centerY - y, r, g, b);
        }
        y++;
        err += 2 * y + 1;
        if (2 * (err - x) + 1 > 0) {
            x--;
            err += 1 - 2 * x;
        }
    }
}

// Load font from .fnt file
bool BMPImageCreator::loadFont(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    for (auto& cols : cropped_chars) cols.clear();

    for (int c = 0; c < 128; ++c) {
        for (int y = 0; y < char_height; ++y) {
            uint8_t byte = file.get();
            for (int x = 0; x < char_width; ++x) {
                bool on = (byte >> (7 - x)) & 1;
                font_chars[c][x][y] = on;
            }
        }
    }

    std::vector<std::vector<int>> empty_columns(128);
    for (int i = 0; i < font_chars.size(); ++i) {
        if (i != 32) {
            for (int cx = 0; cx < char_width; ++cx) {
                bool empty = true;
                for (int cy = 0; cy < char_height; ++cy) {
                    if (font_chars[i][cx][cy]) {
                        empty = false;
                        break;
                    }
                }
                if (empty) {
                    empty_columns[i].push_back(cx);
                }
            }
        }
        else {
            for (int j = 0; j < 5; ++j) {
                empty_columns[i].push_back(j);
            }
        }
    }

    std::vector<bool> column_buffer;
    for (int i = 0; i < font_chars.size(); ++i) {
        for (int cx = 0; cx < char_width; ++cx) {
            if (std::find(empty_columns[i].begin(), empty_columns[i].end(), cx) == empty_columns[i].end()) {
                for (int cy = 0; cy < 8; ++cy) {
                    column_buffer.push_back(font_chars[i][cx][cy]);
                }
                cropped_chars[i].push_back(column_buffer);
                column_buffer.clear();
            }
        }
    }

    return true;
}

// Draw text with loaded font
void BMPImageCreator::drawText(int startX, int startY, const std::string& text, int r, int g, int b, int scale, bool wrap) {
    if (!font_loaded && !loadFont("font.fnt")) {
        return;
    }
    font_loaded = true;

    int current_x = startX;
    int current_y = startY;
    bool wrapped = false;

    std::vector<std::string> words;
    std::string cur;
    for (char ch : text) {
        if (ch == '\n') {
            if (!cur.empty()) {
                words.push_back(cur);
                cur.clear();
            }
            words.push_back("\n");
        }
        else if (ch == ' ') {
            if (!cur.empty()) {
                words.push_back(cur);
                cur.clear();
            }
            words.push_back(" ");
        }
        else {
            cur.push_back(ch);
        }
    }
    if (!cur.empty()) {
        words.push_back(cur);
    }

    for (auto& word : words) {
        if (word == "\n") {
            current_x = startX;
            current_y += (char_height + 1) * scale;
            continue;
        }

        int word_width = 0;
        for (char cc : word) {
            unsigned char c = static_cast<unsigned char>(cc);
            word_width += (cropped_chars[c].size() + 1) * scale;
        }

        wrapped = false;
        if (wrap && word_width > 0 && current_x + word_width > width && word_width <= width) {
            current_x = startX;
            current_y += (char_height + 1) * scale;
            wrapped = true;
        }

        for (char cc : word) {
            unsigned char c = static_cast<unsigned char>(cc);

            if (c >= cropped_chars.size() || wrapped && c == 32 && current_x == startX) {
                continue;
            }

            for (int cy = 0; cy < char_height; ++cy) {
                for (int cx = 0; cx < cropped_chars[c].size(); ++cx) {
                    if (!cropped_chars[c][cx][cy]) continue;
                    for (int dy = 0; dy < scale; ++dy) {
                        for (int dx = 0; dx < scale; ++dx) {
                            int px = current_x + cx * scale + dx;
                            int py = current_y + cy * scale + dy;
                            setPixel(px, py, r, g, b);
                        }
                    }
                }
            }

            current_x += (cropped_chars[c].size() + 1) * scale;
        }
    }
}

// Save image to file
void BMPImageCreator::saveFile(const std::string& filename) {
    std::vector<unsigned char> pixel_data(pixel_data_size, 0);
    for (size_t y = 0; y < height; ++y) {
        size_t row_start = y * row_size;
        const size_t src_y = height - 1 - y;
        for (size_t x = 0; x < width; ++x) {
            size_t i = row_start + x * 3;
            const auto& rgb = pixels[src_y][x];
            pixel_data[i + 0] = rgb[2];
            pixel_data[i + 1] = rgb[1];
            pixel_data[i + 2] = rgb[0];
        }
    }

    std::string filename1 = filename + ".bmp";

    std::ofstream file(filename1, std::ios::binary);
    if (!file) {
        return;
    }

    file.write(reinterpret_cast<char*>(file_header), sizeof(file_header));
    file.write(reinterpret_cast<char*>(bitmap_info_header), sizeof(bitmap_info_header));
    file.write(reinterpret_cast<char*>(pixel_data.data()), pixel_data_size);
    file.close();
}