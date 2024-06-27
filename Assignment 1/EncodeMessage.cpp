#include "EncodeMessage.h"
#include <bitset>
#include <iostream>



// Default Constructor
EncodeMessage::EncodeMessage() = default;

// Destructors
EncodeMessage::~EncodeMessage() = default;


std::string string_to_binary_string(const std::string &input) {

    std::string output;

    for (char c : input) {
        output = std::bitset<7>(c).to_string() + output;
    }

    return output;
}

int fibonacci(int n) {
    int a = 0, b = 1, c, i;
    if (n == 0)
        return a;
    for (i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

bool isPrime(int n) {
    // Corner case
    if (n <= 1)
        return false;

    // Check from 2 to n-1
    for (int i = 2; i <= n / 2; i++)
        if (n % i == 0)
            return false;

    return true;
}

std::string manipulate_string(std::string input) {

    int shift;
    int c;

    for (int i = 0; i < input.length(); ++i) {

        if (isPrime(i)) {


            c = int(static_cast<unsigned char>(input[i]));
            c += fibonacci(i);
            if (c > 126) c = 126;
            if (c < 33) c += 33;

            input[i] = char(c);
        }
    }

    shift = input.length() / 2;

    input = input.substr(input.length() - shift,shift) + input.substr(0, input.length() -  shift);

    return input;
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {



    ImageMatrix output(img);
    std::string binary_message = string_to_binary_string(manipulate_string(message));

    int index(binary_message.length());
    int new_data;
    for (std::pair<int, int> edge : positions) {

        if (--index < 0) break;

        new_data = static_cast<int>(output.get_data(edge.first, edge.second));

        if (binary_message[index] == '0') {
            if (new_data % 2 == 1) {
                new_data -= 1;
            }
        }
        else {
            if (new_data % 2 == 0) {
                new_data += 1;
            }
        }

        output.set_data(edge.first, edge.second, new_data);
    }

    return output;
}
