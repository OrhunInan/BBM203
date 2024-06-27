#include "Convolution.h"

// Default constructor 
Convolution::Convolution() : kernel_height(0), kernel_width(0), stride(0), padding(false), kernel(nullptr) {}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad) :kernel_height(kh),
        kernel_width(kw), stride(stride_val), padding(pad), kernel(new double*[kh]) {

    // Allocate memory for the matrix and copy data
    for (int i = 0; i < kernel_height; ++i) {
        this->kernel[i] = new double[kernel_width];
        for (int j = 0; j < kernel_width; ++j) {
            this->kernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    // Deallocate memory
    if (kernel != nullptr){
        for (int i = 0; i < kernel_height; ++i) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {

    // self-assignment check
    if (this == &other) {
        return *this;
    }

    // Deallocate old memory
    if (kernel != nullptr) {
        for (int i = 0; i < kernel_height; ++i) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }

    //primitive value assignments
    this->kernel_height = other.kernel_height;
    this->kernel_width = other.kernel_width;
    this->stride = other.stride;
    this->padding = other.padding;

    // Allocate memory for the matrix and copy data
    this->kernel = new double*[kernel_height];
    for (int i = 0; i < kernel_height; ++i) {
        kernel[i] = new double[kernel_width];
        for (int j = 0; j < kernel_width; ++j) {
            kernel[i][j] = other.kernel[i][j];
        }
    }

    return *this;
}

// Copy constructor
Convolution::Convolution(const Convolution &other) : Convolution() {
    *this = other;
}

// convolves input image with the custom kernel without padding.
ImageMatrix add_padding(const ImageMatrix &inputImage) {

    int height = inputImage.get_height();
    int width = inputImage.get_width();

    ImageMatrix paddedImage(inputImage.get_height() + 2, inputImage.get_width() + 2);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            paddedImage.set_data(i+1, j+1, inputImage.get_data(i, j));
        }
    }

    return paddedImage;
}

// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {

    ImageMatrix image;
    if(padding) image = add_padding(input_image);
    else image = input_image;

    int Height = ((input_image.get_height() - kernel_height + (2 * static_cast<int>(padding))) / stride) + 1;
    int width = ((input_image.get_width() - kernel_width + (2 * static_cast<int>(padding))) / stride) + 1;

    ImageMatrix output(Height, width);

    double sum;
    for (int i = 0; i < Height; ++i) {
        for (int j = 0; j < width; ++j) {
            sum = 0;
            for (int k = 0; k < kernel_height; ++k) {
                for (int l = 0; l < kernel_width; ++l) {
                    sum += kernel[k][l] * image.get_data(((i * stride)+k), ((j * stride)+l));
                }
            }
            output.set_data(i,j, sum);
        }
    }

    return output;
}