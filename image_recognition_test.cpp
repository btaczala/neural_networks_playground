
#include <fmt/format.h>
#include <gtest/gtest.h>

#include <Eigen/Dense>

#include "mnist_reader.hpp"
#include "network_runtime.hpp"

const std::string kMnistDir = MNIST_DIR;

std::vector<double> convert(const Eigen::MatrixXd& mat) {
    return std::vector<double>{mat.data(),
                               mat.data() + mat.rows() * mat.cols()};
}

TEST(DigitRecognition, allTrainSet) {
    MNistReaderImageSet images{fs::path(kMnistDir) / "train-images-idx3-ubyte"};
    MNistReaderLabelSet labels{fs::path(kMnistDir) / "train-labels-idx1-ubyte"};
    ASSERT_TRUE(images.valid());

    NetworkRuntime network{{784, 5, 10}};
    const auto sigmoid = [](double f) -> double {
        return static_cast<double>(1) / (1.0f + std::exp(-f));
    };

    int hit = 0;
    for (int i = 0; i < images.count(); ++i) {
        const auto img = images.readImage(i);
        const auto label = labels.readLabel(i);
        std::vector<double> input;
        std::transform(img.data.begin(), img.data.end(),
                       std::back_inserter(input),
                       [](const Pixel& p) { return 0.0f; });
        const auto prediction = convert(network.predict(input, sigmoid));
        auto index = std::distance(
            prediction.begin(),
            std::max_element(prediction.begin(), prediction.end()));

        EXPECT_TRUE(index >= 0 && index <= 9);

        if (index == label) {
            hit++;
        }
    }

    const auto accuracy =
        100.0f * static_cast<double>(hit) / static_cast<double>(images.count());

    EXPECT_GT(accuracy, 0);

    std::cout << fmt::format("Accuracy: {:.2f}%\n", accuracy);
}
