
#include <gtest/gtest.h>

#include "mnist_reader.hpp"
#include "network_runtime.hpp"
const std::string kMnistDir = MNIST_DIR;

TEST(DigitRecognition, oneSample) {
    MNistReaderImageSet images{fs::path(kMnistDir) / "train-images-idx3-ubyte"};
    ASSERT_TRUE(images.valid());
}
