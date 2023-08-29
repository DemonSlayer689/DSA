// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include <cmath>

// #include <omp.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <omp.h>

// ...




// Define image dimensions
const int IMAGE_WIDTH = 800;
const int IMAGE_HEIGHT = 600;

// Generate random image data
std::vector<std::vector<int>> generateImage() {
    std::vector<std::vector<int>> image(IMAGE_HEIGHT, std::vector<int>(IMAGE_WIDTH));
    for (int i = 0; i < IMAGE_HEIGHT; ++i) {
        for (int j = 0; j < IMAGE_WIDTH; ++j) {
            image[i][j] = std::rand() % 256; // Random pixel values between 0 and 255
        }
    }
    return image;
}

// Apply blur to a region of the image
void applyBlur(std::vector<std::vector<int>>& image, int startY, int endY, int blurRadius) {
    int imageWidth = image[0].size();
    int imageHeight = image.size();
    
    for (int y = startY; y < endY; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            int sum = 0;
            int count = 0;
            for (int offsetY = -blurRadius; offsetY <= blurRadius; ++offsetY) {
                for (int offsetX = -blurRadius; offsetX <= blurRadius; ++offsetX) {
                    int ny = std::min(imageHeight - 1, std::max(0, y + offsetY));
                    int nx = std::min(imageWidth - 1, std::max(0, x + offsetX));
                    sum += image[ny][nx];
                    ++count;
                }
            }
            image[y][x] = sum / count;
        }
    }
}
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    std::vector<std::vector<int>> image = generateImage();
    const int numThreads = 4;
    const int blurRadius = 3;

    omp_set_num_threads(numThreads); // Set number of threads

    auto start_time = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for
    for (int i = 0; i < numThreads; ++i) {
        int startY = i * IMAGE_HEIGHT / numThreads;
        int endY = (i + 1) * IMAGE_HEIGHT / numThreads;
        applyBlur(image, startY, endY, blurRadius);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    double execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000000.0;

    std::cout << "Image processing took " << execution_time << " seconds.\n";

    return 0;
}

// int main() {
//     std::srand(static_cast<unsigned>(std::time(nullptr)));
    
//     std::vector<std::vector<int>> image = generateImage();
//     const int numThreads = 4;
//     const int blurRadius = 3;
    
//     omp_set_num_threads(numThreads); // Set number of threads
    
//     double start_time = omp_get_wtime();
    
//     #pragma omp parallel for
//     for (int i = 0; i < numThreads; ++i) {
//         int startY = i * IMAGE_HEIGHT / numThreads;
//         int endY = (i + 1) * IMAGE_HEIGHT / numThreads;
//         applyBlur(image, startY, endY, blurRadius);
//     }
    
//     double end_time = omp_get_wtime();
//     double execution_time = end_time - start_time;

//     std::cout << "Image processing took " << execution_time << " seconds.\n";

//     return 0;
// }
