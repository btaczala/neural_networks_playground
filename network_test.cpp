#include "network.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <gtest/gtest.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <ostream>
#include <range/v3/all.hpp>
#include <vector>

#include "network_runtime.hpp"

TEST(Matrix, idenetity) {
    const auto matrix = Eigen::Matrix<float, 10, 10>::Identity();

    EXPECT_EQ(matrix(0, 0), 1);
    EXPECT_EQ(matrix(1, 1), 1);
    EXPECT_EQ(matrix(2, 2), 1);
}

TEST(NNetwork, create) { NNetwork<float, 2, 3, 2> nn; }

TEST(NetworkRuntime, create) {
    const std::size_t inputLayerSize = 3;
    NetworkRuntime nn{{inputLayerSize, 5, 10}};
    const Eigen::IOFormat HeavyFmt(Eigen::FullPrecision, 0, ", ", ";\n", "[",
                                   "]", "[", "]");
    const auto sigmoid = [](double f) -> double {
        return static_cast<double>(1) / (1.0f + std::exp(-f));
    };

    const std::vector<double> input(inputLayerSize, 1);
    ASSERT_EQ(input.size(), inputLayerSize);
    const auto prediction = nn.calculate(input, sigmoid);
    EXPECT_GT(10, prediction.sum());
    EXPECT_GT(prediction.sum(), 0);
}

TEST(NNetwork, calculate) {
    NNetwork<float, 784, 5, 10> nn;
    Eigen::IOFormat HeavyFmt(Eigen::FullPrecision, 0, ", ", "\n", "[", "]", "",
                             "");

    auto sigmoid = [](float& f) { f = 1.0 / (1 + std::exp(-f)); };
    std::array<float, decltype(nn)::NumberOfInputs> inputs;
    std::fill(std::begin(inputs), std::end(inputs), 1);
    const auto res = nn.calculate(inputs, sigmoid);

    EXPECT_TRUE(res.sum() < 10);
}
