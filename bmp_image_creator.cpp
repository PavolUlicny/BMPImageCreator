// This code is a program that creates BMP images with specified dimensions, colors, shapes and text.
// You can draw outside the image area, and it will not throw an error because it only draws the pixels that fit within the image dimensions.
// Font from darkrose (https://opengameart.org/content/8x8-ascii-bitmap-font-with-c-source) (modified)

// Include necessary headers
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <algorithm>


// BMPImageCreator class to create BMP images
class BMPImageCreator {
private:

    // Constants for BMP file format
    static constexpr short file_header_size = 14;
    static constexpr short bitmap_info_header_size = 40;
    static constexpr short pixel_info_offset = file_header_size + bitmap_info_header_size;

    // BMP File Header (14 bytes)
    unsigned char file_header[14] = { 0 };

	//initialize file header values
    int32_t width;
    int32_t height;
    int32_t padding_size;
    int32_t row_size;
    int32_t pixel_data_size;
    int32_t file_size;

    // Initialize DIB header values
    static constexpr int32_t bits_per_pixel = 24;
    static constexpr int32_t color_planes = 1;
    static constexpr int32_t compression = 0;
    static constexpr int32_t resolution = 2835;
    static constexpr int32_t colors_used = 0;
    static constexpr int32_t important_colors = 0;

    // DIB Header (BITMAPINFOHEADER, 40 bytes)
    unsigned char bitmap_info_header[40] = { 0 };

	// 2D vector to hold pixel data (RGB)
    std::vector<std::vector<std::array<unsigned char, 3>>> pixels;

	// Variables for font loading
	bool font_loaded = false;

    const int char_width = 8;
    const int char_height = 8;
    const int char_quantity = 128;
    std::vector<std::vector<std::vector<bool>>> font_chars;
    std::vector<std::vector<bool>> cropped_char;
	std::vector<int> cropped_char_widths;

public:

	// Constructor to initialize BMPImageCreator with specified width and height
    BMPImageCreator(int32_t width1, int32_t height1) {

		// Check for valid width and height, set default values if invalid
        if (width1 <= 0 || height1 <= 0) {
            width = 10;
            height = 5;

        } else {

            width = width1;
            height = height1;
        }

		// Set values to variables for BMP file header and DIB header
        padding_size = (4 - (width * 3) % 4) % 4;
        row_size = width * 3 + padding_size;
        pixel_data_size = row_size * height;
        file_size = file_header_size + bitmap_info_header_size + pixel_data_size;

		// Set the file header values
        file_header[0] = 'B';
        file_header[1] = 'M';
        file_header[10] = static_cast<unsigned char>(pixel_info_offset);
        file_header[11] = static_cast<unsigned char>(pixel_info_offset >> 8);
        file_header[12] = static_cast<unsigned char>(pixel_info_offset >> 16);
        file_header[13] = static_cast<unsigned char>(pixel_info_offset >> 24);

		// Set file size in the file header
        file_header[2] = static_cast<unsigned char>(file_size);
        file_header[3] = static_cast<unsigned char>(file_size >> 8);
        file_header[4] = static_cast<unsigned char>(file_size >> 16);
        file_header[5] = static_cast<unsigned char>(file_size >> 24);

        // Set DIB header size
        bitmap_info_header[0] = static_cast<unsigned char>(bitmap_info_header_size);

        // Width
        bitmap_info_header[4] = static_cast<unsigned char>(width);
        bitmap_info_header[5] = static_cast<unsigned char>(width >> 8);
        bitmap_info_header[6] = static_cast<unsigned char>(width >> 16);
        bitmap_info_header[7] = static_cast<unsigned char>(width >> 24);

        // Height
        bitmap_info_header[8] = static_cast<unsigned char>(height);
        bitmap_info_header[9] = static_cast<unsigned char>(height >> 8);
        bitmap_info_header[10] = static_cast<unsigned char>(height >> 16);
        bitmap_info_header[11] = static_cast<unsigned char>(height >> 24);

        // Color planes
        bitmap_info_header[12] = static_cast<unsigned char>(color_planes);

        // Bits per pixel
        bitmap_info_header[14] = static_cast<unsigned char>(bits_per_pixel);

        // Compression
        bitmap_info_header[16] = static_cast<unsigned char>(compression);

        // Raw image size
        bitmap_info_header[20] = static_cast<unsigned char>(pixel_data_size);
        bitmap_info_header[21] = static_cast<unsigned char>(pixel_data_size >> 8);
        bitmap_info_header[22] = static_cast<unsigned char>(pixel_data_size >> 16);
        bitmap_info_header[23] = static_cast<unsigned char>(pixel_data_size >> 24);

        // Horizontal resolution
        bitmap_info_header[24] = static_cast<unsigned char>(resolution);
        bitmap_info_header[25] = static_cast<unsigned char>(resolution >> 8);
        bitmap_info_header[26] = static_cast<unsigned char>(resolution >> 16);
        bitmap_info_header[27] = static_cast<unsigned char>(resolution >> 24);

        // Vertical resolution
        bitmap_info_header[28] = static_cast<unsigned char>(resolution);
        bitmap_info_header[29] = static_cast<unsigned char>(resolution >> 8);
        bitmap_info_header[30] = static_cast<unsigned char>(resolution >> 16);
        bitmap_info_header[31] = static_cast<unsigned char>(resolution >> 24);

        // Colors used
        bitmap_info_header[32] = static_cast<unsigned char>(colors_used);

        // Important colors
        bitmap_info_header[36] = static_cast<unsigned char>(important_colors);

        // Create 2D pixel vector as RGB
        pixels = std::vector<std::vector<std::array<unsigned char, 3>>>(height, std::vector<std::array<unsigned char, 3>>(width, { 255,255,255 }));

        font_chars.resize(char_quantity, std::vector<std::vector<bool>>(char_height, std::vector<bool>(char_width)));

	}

	// Function to set the default pixel RGB color for the entire image
    void setDefaultPixelRGB(int r, int g, int b) {

        r = std::clamp(r, 0, 255);
        g = std::clamp(g, 0, 255);
        b = std::clamp(b, 0, 255);
        
		// Set the default pixel RGB color for the entire image
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                pixels[y][x] = { static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b) };
            }
        }
	}

	// Function to set a pixel at (x, y) with RGB values
    void setPixel(int32_t x, int32_t y, int r, int g, int b) {

		// Check if the pixel coordinates are within bounds
        if (x >= width || x < 0 || y >= height || y < 0) {
			return;
        }

        r = std::clamp(r, 0, 255);
        g = std::clamp(g, 0, 255);
        b = std::clamp(b, 0, 255);

		// Set the pixel at (x, y) with the specified RGB values
        pixels[y][x] = { static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b) };
	}

    void drawRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, int r, int g, int b, bool fill) {

		// Swap coordinates to ensure x1 >= x and y1 >= y
        if (x1 < x) {
            std::swap(x, x1);
        }
        if (y1 < y) {
            std::swap(y, y1);
        }

		// If fill is true, fill the rectangle with the specified color
        if (fill) {
            for (int32_t i = y; i <= y1; ++i) {
                for (int32_t j = x; j <= x1; ++j) {
                    setPixel(j, i, r, g, b);
                }
            }
        }

		// If fill is false, draw only the border of the rectangle
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

	// Function to draw a line using Bresenham's algorithm
    void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int r, int g, int b) {

		// Find the horizontal distance between the two points 
        int32_t horizontal_distance = abs(x1 - x0);

		// Find the vertical distance between the two points
        int32_t vertical_distance = -abs(y1 - y0);

		// If the line is a single point, set that pixel
        if (horizontal_distance == 0 && vertical_distance == 0) {
            setPixel(x0, y0, r, g, b);
            return;
		}

        short horizontal_direction;
        short vertical_direction;

		// Determine the direction of the line
        if (x0 < x1) {
			horizontal_direction = 1;
        } else {
			horizontal_direction = -1;
		}

		if (y0 < y1) {
            vertical_direction = 1;
		} else {
            vertical_direction = -1;
        }

        long error_value = horizontal_distance + vertical_distance;

		// Loop until the line is drawn
        while (true) {
            setPixel(x0, y0, r, g, b);

            if (x0 == x1 && y0 == y1) {
                break;
            }

            long double_error_value = 2 * error_value;

            if (double_error_value >= vertical_distance) {
                error_value += vertical_distance;
                x0 += horizontal_direction;
            }

            if (double_error_value <= horizontal_distance) {
                error_value += horizontal_distance;
                y0 += vertical_direction;
            }
        }
    }

	// Function to draw a circle using the Midpoint Circle Algorithm
    void drawCircle(int32_t centerX, int32_t centerY, int32_t radius, int r, int g, int b, bool fill) {

		// Check if the radius is valid
        if (radius <= 0) {
            return;
        }

        int32_t x = radius;
        int y = 0;
        int error = 0;

		// Start drawing the circle using the Midpoint Circle Algorithm
        while (x >= y) {

			// Draw symmetric points in all octants
            if (fill) {

                // Draw horizontal scanlines between symmetric points
                for (int i = centerX - x; i <= centerX + x; ++i) {
                    setPixel(i, centerY + y, r, g, b);
                    setPixel(i, centerY - y, r, g, b);
                }

                for (int i = centerX - y; i <= centerX + y; ++i) {
                    setPixel(i, centerY + x, r, g, b);
                    setPixel(i, centerY - x, r, g, b);
                }

            } else {

                // Only draw circle perimeter points
                setPixel(centerX + x, centerY + y, r, g, b);
                setPixel(centerX + y, centerY + x, r, g, b);
                setPixel(centerX - y, centerY + x, r, g, b);
                setPixel(centerX - x, centerY + y, r, g, b);
                setPixel(centerX - x, centerY - y, r, g, b);
                setPixel(centerX - y, centerY - x, r, g, b);
                setPixel(centerX + y, centerY - x, r, g, b);
                setPixel(centerX + x, centerY - y, r, g, b);
            }

			// Calculate the next point using the Midpoint Circle Algorithm
            y += 1;
            error += 2 * y + 1;

            if (2 * (error - x) + 1 > 0) {
                x -= 1;
                error += 1 - 2 * x;
            }
        }
    }

    bool loadFont() {

		// Load font image from BMP file
        std::ifstream file("font1.bmp", std::ios::binary);
        
		// Check if the file opened successfully
        if (!file) {
            return false;
        }

        std::vector<unsigned char> font_image_data;
        int32_t font_image_width = 128;
        int32_t font_image_height = 48;

        // Read file header (14 bytes) and DIB header (40 bytes)
        unsigned char file_header[14];
        unsigned char dib_header[40];

		// Read the file header and DIB header
        file.read(reinterpret_cast<char*>(file_header), 14);
        file.read(reinterpret_cast<char*>(dib_header), 40);

        // Extract width and height (little-endian)
        font_image_width = dib_header[4] | (dib_header[5] << 8) | (dib_header[6] << 16) | (dib_header[7] << 24);
        font_image_height = dib_header[8] | (dib_header[9] << 8) | (dib_header[10] << 16) | (dib_header[11] << 24);

		// Calculate the size of the pixel data
        int row_size = (font_image_width * 3 + 3) & ~3;
        int image_size = row_size * font_image_height;
        const int skip_index = 96;

		// Resize the font image data vector to hold the pixel data
        font_image_data.resize(image_size);

		// Read the pixel data from the file
        file.seekg(54);
        file.read(reinterpret_cast<char*>(font_image_data.data()), image_size);
        file.close();

		// Split the 1D vector into a 2D vector for easier access
        std::vector<std::vector<std::array<unsigned char, 3>>> font_pixels(font_image_height, std::vector<std::array<unsigned char, 3>>(font_image_width));
        for (int y = 0; y < font_image_height; ++y) {
            for (int x = 0; x < font_image_width; ++x) {
                int index = (font_image_height - 1 - y) * row_size + x * 3;
                if (index + 2 < static_cast<int>(font_image_data.size())) {
                    font_pixels[y][x] = { font_image_data[index + 2],  font_image_data[index + 1], font_image_data[index + 0] };
                }
            }
        }

		int chars_per_row = font_image_width / char_width;
        int real_index = 1;

        //for loop for loading the char images into a 3d vector
        for (int i = 0; i < char_quantity; ++i) {

            //switch case for missing chars
            switch (i) {

                //if the char is the empty space, skip it 
                case skip_index:
                    continue;
                    break;

                //copy over a placeholder char image for unprintable chars, because of incomplete font image 
                case 1:
                    font_chars[1] = font_chars[0];
                    real_index += 1;
                    continue;
                    break;

            }

            //find the position of the image in the font image 
            int charX = (i % chars_per_row) * char_width;
            int charY = (i / chars_per_row) * char_height;

            //load the images into the vector by scanning each 8x8 char image and converting it to bits (in the form of bools)
            for (int y = 0; y < char_height; ++y) {
                for (int x = 0; x < char_width; ++x) {
                    auto pixel = font_pixels[charY + y][charX + x];
                    bool on = (pixel[0] > 128 || pixel[1] > 128 || pixel[2] > 128);
                    font_chars[real_index][x][y] = on;
                }
            }
			++real_index;
        }
        return true;
    }

    void drawText(int startX, int startY, const std::string& text, int r, int g, int b, int scale, bool wrap) {

        //load font if its not loaded
        if (!font_loaded && !loadFont()) {
            return;
        }
        font_loaded = true;

        //initialize some variables
        int current_x = startX;
        int current_y = startY;

        bool wrapped = false;

        //split text into words, spaces and newlines
        std::vector<std::string> words;
        std::string cur;
        for (char ch : text) {
            if (ch == '\n') {
                if (!cur.empty()) { 
                    words.push_back(cur); 
                    cur.clear(); 
                }
                words.push_back("\n");
            
            } else if (ch == ' ') {
                if (!cur.empty()) { 
                    words.push_back(cur); 
                    cur.clear(); 
                }
                words.push_back(" ");
            } else {
                cur.push_back(ch);
            }
        }

        //if theres still a word in the word buffer, add it to the list of words
        if (!cur.empty()) {
            words.push_back(cur);
        }


        for (auto& word : words) {

            //if the word is a newline, go to a new line
            if (word == "\n") {
                current_x = startX; 
                current_y += (char_height + 1) * scale;
                continue;
            }

            //measure this word's pixel width
            int word_width = 0;
            for (char cc : word) {
                unsigned char c = static_cast<unsigned char>(cc);

                //find cropped width
                int char_width1 = char_width;
                cropped_char_widths.clear();
                if (c >= 0 && c < font_chars.size() && c != 32) {
                    for (int cx = 0; cx < char_width; ++cx) {
                        bool empty = true;
                        for (int cy = 0; cy < char_height; ++cy)
                            if (font_chars[c][cx][cy]) { 
                                empty = false; 
                                break; 
                            }
                        if (empty) {
                            cropped_char_widths.push_back(cx);
                        }
                    }
                    char_width1 = char_width - int(cropped_char_widths.size());
                } else if (c == 32) {
                    char_width1 = 3;
                }
                word_width += (char_width1 + 1) * scale;
            }

            if (word_width > width)
            //wrap if needed
            wrapped = false;
            if (wrap && word_width > 0 && current_x + word_width > width && word_width <= width) {
                current_x = startX;
                current_y += (char_height + 1) * scale;
                wrapped = true;
            }

            //now draw each character of the word
            for (char cc : word) {
                unsigned char c = static_cast<unsigned char>(cc);


                //if its an invalid or its a space on a new line, ignore it
                if (c >= font_chars.size() || c < 0 || wrapped && c == 32 && current_x == startX) {
                    continue;
                }

                //crop the char again
                int char_width2 = char_width;
                cropped_char_widths.clear();
                if (c != 32) {
                    for (int cx = 0; cx < char_width; ++cx) {
                        bool empty = true;
                        for (int cy = 0; cy < char_height; ++cy)
                            if (font_chars[c][cx][cy]) { 
                                empty = false; 
                                break; 
                            }
                        if (empty) {
                            cropped_char_widths.push_back(cx);
                        }
                    }
                    char_width2 = char_width - int(cropped_char_widths.size());
                } else {
                    // fixed narrow space
                    char_width2 = 3;
                    cropped_char_widths = { 0,1,2,3,4 };
                }

                // build cropped_char
                cropped_char.assign(char_width2, std::vector<bool>(char_height, false));
                int cx2 = 0;
                for (int cx = 0; cx < char_width; ++cx) {
                    if (std::find(cropped_char_widths.begin(), cropped_char_widths.end(), cx) == cropped_char_widths.end()) {
                        for (int cy = 0; cy < char_height; ++cy) {
                            cropped_char[cx2][cy] = font_chars[c][cx][cy];
                        }
                        ++cx2;
                    }
                }

                // draw scaled
                for (int cy = 0; cy < char_height; ++cy) {
                    for (int cx = 0; cx < char_width2; ++cx) {
                        if (!cropped_char[cx][cy]) continue;
                        for (int dy = 0; dy < scale; ++dy) {
                            for (int dx = 0; dx < scale; ++dx) {
                                int px = current_x + cx * scale + dx;
                                int py = current_y + cy * scale + dy;
                                setPixel(px, py, r, g, b);
                            }
                        }
                    }
                }

                // advance
                current_x += (char_width2 + 1) * scale;
            }
        }
    }

	// Function to save the BMP image to a file
    void saveFile(const std::string& filename) {

        // Flatten pixel data (convert RGB to BGR and reverse rows)
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

        // Write to file
        std::ofstream file(filename1, std::ios::binary);
        if (!file) {
            return;
        }

        file.write(reinterpret_cast<char*>(file_header), sizeof(file_header));
        file.write(reinterpret_cast<char*>(bitmap_info_header), sizeof(bitmap_info_header));
        file.write(reinterpret_cast<char*>(pixel_data.data()), pixel_data_size);

        file.close();

    }
};

int main() {

	// Example usage of BMPImageCreator
	// BMPImageCreator bmp(100, 100);
	// bmp.setDefaultPixelRGB(255, 0, 0);  Set default pixel color to red
	// bmp.drawRectangle(10, 10, 90, 90, 0, 255, 0, true);  Draw a filled green rectangle
	// bmp.drawLine(10, 10, 90, 90, 0, 0, 255);  Draw a blue line
	// bmp.drawCircle(50, 50, 30, 255, 255, 0, false);  Draw a yellow circle outline
    // bmp.drawText(10, 10, "Hello,\nBMP World!", 0, 0, 0, 1, true); Black wrapped text
	// bmp.saveFile("output_image");  Save the BMP image to a file

    return 0;
}