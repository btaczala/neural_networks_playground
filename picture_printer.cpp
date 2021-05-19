#include <iostream>

#include "mnist_reader.hpp"

const std::string kMnistDir = MNIST_DIR;
int main(int argc, char* argv[]) {
    const std::size_t whichOne = (argc != 2 ? 0 : std::atoi(argv[1]));
    MNistReaderImageSet images{fs::path(kMnistDir) / "train-images-idx3-ubyte"};

    const auto image = images.readImage(whichOne);
    std::cout << image << std::flush;
}
