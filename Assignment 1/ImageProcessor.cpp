#include "ImageProcessor.h"
#include <iostream>

ImageProcessor::ImageProcessor() : image_sharpening(), edge_detector(), decode_message(), encode_message() {}

ImageProcessor::~ImageProcessor() = default;

std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {

    ImageMatrix sharp_image = image_sharpening.sharpen(img, 2);
    std::vector<std::pair<int, int>> edges = edge_detector.detectEdges(sharp_image);

    return decode_message.decodeFromImage(sharp_image, edges);
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {

    ImageMatrix sharp_image = image_sharpening.sharpen(img, 2);
    std::vector<std::pair<int, int>> edges = edge_detector.detectEdges(sharp_image);

    return encode_message.encodeMessageToImage(img, message, edges);
}
