#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {

    double value = 1.0 / 9.0;//blur value

    //memory allocation for kernel
    double** kernel = new double*[3];
    for (int i = 0; i < 3; ++i) {

        kernel[i] = new double[3];
        for (int j = 0; j < 3; ++j) {

            kernel[i][j] = value;
        }
    }

    this->convolution = Convolution(kernel, 3, 3, 1, true);

    //deleting kernel
    for (int i = 0; i < 3; ++i) {

        delete[] kernel[i];
    }
    delete[] kernel;
}

ImageSharpening::~ImageSharpening() = default;

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {

    ImageMatrix blurredImage = convolution.convolve(input_image);

    ImageMatrix sharpenedImage = input_image + ((input_image - blurredImage) * k);
    sharpenedImage.clip();

    return sharpenedImage;
}
