#include "ImageMatrix.h"
#include <cmath>


// Default constructor
ImageMatrix::ImageMatrix() : height(0), width(0), data(nullptr) {}


// Parameterized constructor for creating a blank image of given size
ImageMatrix::ImageMatrix(int imgHeight, int imgWidth) : height(imgHeight), width(imgWidth), data(new double*[imgHeight]) {

    // Allocate memory for the matrix and fill it with zeros.
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
        for (int j = 0; j < width; j++) {
            data[i][j] = 0;
        }
    }
}

// Parameterized constructor for loading image from file. PROVIDED FOR YOUR CONVENIENCE
ImageMatrix::ImageMatrix(const std::string &filepath) {
    // Create an ImageLoader object and load the image
    ImageLoader imageLoader(filepath);

    // Get the dimensions of the loaded image
    height = imageLoader.getHeight();
    width = imageLoader.getWidth();

    // Allocate memory for the matrix
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
    }

    // Copy data from imageLoader to data
    double** imageData = imageLoader.getImageData();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            data[i][j] = imageData[i][j];
        }
    }
}



// Destructor
ImageMatrix::~ImageMatrix() {

    // Deallocate memory
    if(data != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
}

// Parameterized constructor - direct initialization with 2D matrix
ImageMatrix::ImageMatrix(const double** inputMatrix, int imgHeight, int imgWidth) : height(imgHeight), width(imgWidth),
        data(new double*[imgHeight]){

    // Allocate memory for the matrix and copy data from imageLoader to data
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
        for (int j = 0; j < width; j++) {
            data[i][j] = inputMatrix[i][j];
        }
    }
}

// Copy assignment operator
ImageMatrix& ImageMatrix::operator=(const ImageMatrix &other) {

    // self-assignment check
    if (this == &other) {
        return *this;
    }

    // Deallocate memory to data
    if(data != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }

    this->height = other.height;
    this->width = other.height;


    // Allocate memory for the matrix and copy data from imageLoader to data
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
        for (int j = 0; j < width; j++) {
            data[i][j] = other.get_data(i, j);
        }
    }

    return *this;
}

// Copy constructor
ImageMatrix::ImageMatrix(const ImageMatrix &other) : ImageMatrix() {
    *this = other;
}


// Overloaded operators

// Overloaded operator + to add two matrices
ImageMatrix ImageMatrix::operator+(const ImageMatrix &other) const {

    if (this->data == nullptr or other.get_data() == nullptr
        or this->height != other.get_height() or this->width != other.get_width()) {
        return {};
    }

    double pixel;
    ImageMatrix returnImage(this->height, this->width);

    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            pixel = this->data[i][j] + other.get_data(i, j);
            returnImage.set_data(i, j, pixel);
        }

    }

    return returnImage;
}

// Overloaded operator - to subtract two matrices
ImageMatrix ImageMatrix::operator-(const ImageMatrix &other) const {

    if (this->data == nullptr or other.get_data() == nullptr
        or this->height != other.get_height() or this->width != other.get_width()) {
        return {};
    }

    double pixel;
    ImageMatrix returnImage(this->height, this->width);

    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            pixel = this->data[i][j] - other.get_data(i, j);
            returnImage.set_data(i, j, pixel);
        }

    }

    return returnImage;
}

// Overloaded operator * to multiply a matrix with a scalar
ImageMatrix ImageMatrix::operator*(const double &scalar) const {

    if (this->data == nullptr) {
        return {};
    }

    double pixel;
    ImageMatrix returnImage(this->width, this->height);

    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            pixel = static_cast<double>(this->data[i][j] * scalar);
            returnImage.set_data(i, j, pixel);
        }

    }

    return returnImage;
}

//clips image pixels in range of 0-255
void ImageMatrix::clip() {
    for (int i = 0; i < height; ++i) {

        for (int j = 0; j < width; ++j) {

            if(data[i][j] > 255.0) {

                data[i][j] = 255.0;
                continue;
            }
            if(data[i][j] < 0.0) {

                data[i][j] = 0.0;
            }
        }
    }
}

// Getter function to access the data in the matrix
double** ImageMatrix::get_data() const {return  data;}

// Getter function to access the data at the index (i, j)
double ImageMatrix::get_data(int i, int j) const {return data[i][j];}

// Getter function to access height value of data matrix
int ImageMatrix::get_height() const {return this->height;}

// Getter function to access width value of data matrix
int ImageMatrix::get_width() const {return this->width;}

//Setter function used for changing the value at the index (i, j) of data matrix
void ImageMatrix::set_data(int i, int j, double value) {data[i][j] = value;}

