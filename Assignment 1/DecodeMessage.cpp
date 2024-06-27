// DecodeMessage.cpp
#include "DecodeMessage.h"

// Default constructor
DecodeMessage::DecodeMessage() = default;

// Destructor
DecodeMessage::~DecodeMessage() = default;


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {

    std::string binary_message;

    std::string lsb;
    for (std::pair<int, int> index : edgePixels) {

            lsb = std::to_string(static_cast<int>(image.get_data(index.first, index.second)) & 1);
            binary_message += lsb;
    }

    while(binary_message.length() % 7 != 0) {

        binary_message = "0" + binary_message;
    }

    int first_loop_range = static_cast<int>(binary_message.length()) / 7;

    int decimal_value;
    std::string message;
    for (int i = 0; i < first_loop_range ; ++i) {
        decimal_value = 0;
        for (int j = 0; j < 7; ++j) {
            decimal_value += (binary_message[(i * 7 + j)] - '0') * (1 << (6-j)) ;
        }

        if (decimal_value > 126) decimal_value = 126;
        if (decimal_value < 33) decimal_value += 33;
        message += char(decimal_value);
    }
    return message;
}

