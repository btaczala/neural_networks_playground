#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>
#include <random>
#include <range/v3/all.hpp>

inline Eigen::MatrixXd randomMatrix(std::size_t rows, std::size_t cols,
                                    std::size_t numberOfInputs) {
    Eigen::MatrixXd toRet{rows, cols};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist{-3, 3};
    std::generate(toRet.data(), toRet.data() + rows * cols,
                  [&dist, &gen, &numberOfInputs]() {
                      return dist(gen) /
                             std::sqrt(static_cast<double>(numberOfInputs));
                  });

    return toRet;
}

struct NetworkRuntime {
    NetworkRuntime(const std::vector<std::size_t>& layerSizes) {
        for (std::size_t i = 1; i < layerSizes.size(); ++i) {
            weights.emplace_back(randomMatrix(
                layerSizes.at(i), layerSizes.at(i - 1), layerSizes.at(i - 1)));
            Eigen::MatrixXd bias{layerSizes.at(i), 1};
            std::fill_n(bias.data(), layerSizes.at(i), 0);
            biases.emplace_back(bias);
        }
    }

    template <typename ActivationFunction>
    Eigen::MatrixXd predict(const std::vector<double>& input,
                              ActivationFunction&& fn) {
        Eigen::MatrixXd inputVector{input.size(), 1};
        std::copy(input.begin(), input.end(), inputVector.data());

        const Eigen::IOFormat HeavyFmt(Eigen::FullPrecision, 0, ", ", ";\n",
                                       "[", "]", "[", "]");

        assert(weights.size() == biases.size());
        for (const auto& matrices : ranges::views::zip(weights, biases)) {
            const auto w = matrices.first;
            const auto b = matrices.second;
            inputVector = Eigen::MatrixXd{w * inputVector + b}.unaryExpr(
                std::forward<ActivationFunction>(fn));
        }

        return inputVector;
    }

    Eigen::Vector2d input;
    std::vector<Eigen::MatrixXd> weights;
    std::vector<Eigen::MatrixXd> biases;
};

