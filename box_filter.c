#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>

// Function to apply a 2D box filter to an image
void boxFilter(unsigned char *image, int width, int height, int channels, int ksize) {
    unsigned char *result = malloc(width * height * channels);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int c = 0; c < channels; ++c) {
                int sum = 0;
                int count = 0;

                for (int m = -ksize / 2; m <= ksize / 2; ++m) {
                    for (int n = -ksize / 2; n <= ksize / 2; ++n) {
                        int x = j + n;
                        int y = i + m;

                        if (x >= 0 && x < width && y >= 0 && y < height) {
                            sum += image[(y * width + x) * channels + c];
                            ++count;
                        }
                    }
                }

                result[(i * width + j) * channels + c] = sum / count;
            }
        }
    }

    // Copy the result back to the original image
    for (int i = 0; i < width * height * channels; ++i) {
        image[i] = result[i];
    }

    free(result);
}

int main() {
    // Load image using stb_image
    int width, height, channels;
    unsigned char *image = stbi_load("nala.jpg", &width, &height, &channels, 0);

    if (image == NULL) {
        fprintf(stderr, "Error loading image.\n");
        return 1;
    }

    // Apply box filter
    int ksize = 3;
    boxFilter(image, width, height, channels, ksize);

    // Save the filtered image using stb_image_write
    if (stbi_write_png("output.png", width, height, channels, image, width * channels) == 0) {
        fprintf(stderr, "Error writing image.\n");
        return 1;
    }

    // Free the image data
    stbi_image_free(image);

    return 0;
}
