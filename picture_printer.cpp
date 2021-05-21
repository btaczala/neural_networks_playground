#include <fmt/format.h>
#include <lodepng.h>

#include <cxxopts.hpp>
#include <iostream>

#include "mnist_reader.hpp"

const std::string kMnistDir = MNIST_DIR;
int main(int argc, char* argv[]) {
    const MNistReaderImageSet images{fs::path(kMnistDir) /
                                     "train-images-idx3-ubyte"};

    cxxopts::Options opts{"MNist picture display",
                          "Displays images from MNist database"};

    // clang-format off
    opts.add_options()
        ("stdin", "Print image to stdin", cxxopts::value<bool>())
        ("f,file", "print image to a file",cxxopts::value<std::string>())
        ("i,index", "index of a image to print",cxxopts::value<int>()->default_value("0"))
        ("h,help", "Print usage");
    // clang-format on
    const auto res = opts.parse(argc, argv);
    if (res.count("h")) {
        std::cout << fmt::format("{}\n", opts.help());
        return EXIT_SUCCESS;
    }

    const auto image = images.readImage(res["i"].as<int>());

    if (res.count("stdin")) {
        std::vector<unsigned char> buffer;
        lodepng::encode(buffer, image.data.data(), image.w(), image.h(),
                        LCT_GREY, 8);
        for (unsigned char c : buffer) std::cout << c;
        std::cout << "\n";
    } else {
        if (res.count("f") == 0) {
            std::cerr << "Filename is missing. See help\n\n";
            std::cerr << fmt::format("{}\n", opts.help());
            return EXIT_FAILURE;
        }
        const std::string filename = res["f"].as<std::string>();
        std::cout << lodepng_error_text(lodepng::encode(
            filename, image.data.data(), image.w(), image.h(), LCT_GREY, 8));
    }
}
