#pragma once

#include <Eigen/Dense>
#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

template <typename Scalar, std::size_t rows, std::size_t cols>
Eigen::Matrix<Scalar, rows, cols> randomMatrix(std::size_t numberOfInputs) {
    Eigen::Matrix<Scalar, rows, cols> toRet;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<Scalar> dist{-3, 3};
    std::generate(toRet.data(), toRet.data() + rows * cols,
                  [&dist, &gen, &numberOfInputs]() {
                      return dist(gen) /
                             std::sqrt(static_cast<Scalar>(numberOfInputs));
                  });

    return toRet;
}

template <typename Scalar, std::size_t rows, std::size_t cols>
Eigen::Matrix<Scalar, rows, cols> zeroMatrix() {
    Eigen::Matrix<Scalar, rows, cols> toRet;
    std::fill(toRet.data(), toRet.data() + rows * cols, 0);

    return toRet;
}

template <typename Scalar, std::size_t rows, std::size_t cols,
          typename ActivationFunction>
Eigen::Matrix<Scalar, rows, cols> activate(
    const Eigen::Matrix<Scalar, rows, cols>& input,
    ActivationFunction&& activate) {
    auto toRet = input;
    std::for_each(toRet.data(), toRet.data() + rows * cols, activate);

    return toRet;
}

template <typename Inputs, std::size_t InputsNumber, std::size_t HiddenNumber,
          std::size_t OutputsNumber>
struct NNetwork {
    NNetwork()
        : w1(randomMatrix<Inputs, HiddenNumber, InputsNumber>(HiddenNumber)),
          w2(randomMatrix<Inputs, OutputsNumber, HiddenNumber>(OutputsNumber)),
          b1(zeroMatrix<Inputs, HiddenNumber, 1>()),
          b2(zeroMatrix<Inputs, OutputsNumber, 1>()) {}

    constexpr static std::size_t NumberOfInputs = InputsNumber;

    Eigen::Matrix<Inputs, HiddenNumber, 1> hidden;
    Eigen::Matrix<Inputs, OutputsNumber, 1> outputs;

    Eigen::Matrix<Inputs, HiddenNumber, InputsNumber> w1;
    Eigen::Matrix<Inputs, HiddenNumber, 1> b1;
    Eigen::Matrix<Inputs, OutputsNumber, HiddenNumber> w2;
    Eigen::Matrix<Inputs, OutputsNumber, 1> b2;

    template <typename ActivationFunction>
    auto calculate(const std::array<Inputs, InputsNumber>& in,
                   ActivationFunction&& fn) {
        Eigen::Matrix<Inputs, InputsNumber, 1> inputs;
        std::copy(std::begin(in), std::end(in), inputs.data());

        Eigen::Matrix<Inputs, HiddenNumber, 1> a1 = (w1 * inputs) + b1;
        a1 = activate<Inputs, HiddenNumber, 1>(a1, fn);

        Eigen::Matrix<Inputs, OutputsNumber, 1> a2 = (w2 * a1) + b2;
        a2 = activate<Inputs, OutputsNumber, 1>(a2, fn);
        return a2;
    }
};

