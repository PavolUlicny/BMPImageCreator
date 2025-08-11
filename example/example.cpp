#include "bmp_image_creator.h"

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
