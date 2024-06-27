// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() {

    //allocating memory for kernels
    double** sobelXKernel = new double*[3]{
        new double[3] {-1.0, 0.0, 1.0},
        new double[3] {-2.0, 0.0, 2.0},
        new double[3] {-1.0, 0.0, 1.0},
    };
    double** sobelYKernel = new double*[3]{
        new double[3] {-1.0, -2.0, -1.0},
        new double[3] {0.0, 0.0, 0.0},
        new double[3] {1.0, 2.0, 1.0},
    };

    //defining Convolution objects
    sobelX = Convolution(sobelXKernel, 3, 3, 1, true);
    sobelY = Convolution(sobelYKernel, 3, 3, 1, true);

    //removing kernels
    for (int i = 0; i < 3; ++i) {
        delete[] sobelXKernel[i];
        delete[] sobelYKernel[i];
    }
    delete[] sobelXKernel;
    delete[] sobelYKernel;

}

// Destructor
EdgeDetector::~EdgeDetector() = default;

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {

    ImageMatrix edgesX = sobelX.convolve(input_image);
    ImageMatrix edgesY = sobelY.convolve(input_image);
    ImageMatrix edges = ImageMatrix(input_image.get_height(), input_image.get_width());

    std::vector<std::pair<int, int>> edgePixels;

    double value;//gradient value of a pixel.
    double average(0.0);//average gradient value of all pixels used for thresholding
    for (int i = 0; i < input_image.get_height(); ++i) {

        for (int j = 0; j < input_image.get_width(); ++j) {

            value = std::sqrt(std::pow(edgesX.get_data(i, j), 2) + std::pow(edgesY.get_data(i, j), 2));
            edges.set_data(i, j, value);
            average += value;
        }
    }

    average = average / (input_image.get_height() * input_image. get_width());

    //finding edge pixels.
    for (int i = 0; i < input_image.get_height(); ++i) {

        for (int j = 0; j < input_image.get_width(); ++j) {

            if(edges.get_data(i, j) > average){

                edgePixels.emplace_back(i,j);
            }
        }
    }

    return edgePixels;
}

