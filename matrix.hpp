#pragma once
#include <fmt/format.h>

template <typename T, std::size_t R, std::size_t C>
struct Matrix {
    Matrix() {
        for (auto& r : _data) std::fill_n(r.begin(), C, 0);
    }

    static Matrix<T, R, C> ident() {
        Matrix<T, R, C> mat;
        // static_assert(R == C, "Rows must be equal to cols");
        for (int i = 0; i < R; ++i) {
            for (int j = 0; j < C; ++j) {
                if (i == j) mat._data[j][i] = 1;
            }
        }
        return mat;
    }

    void addRow(std::size_t position, const std::array<T, R>& row) {
        _data.at(position) = row;
    }

    auto& operator[](std::size_t r) { return _data[r]; }
    const auto& operator[](std::size_t r) const { return _data[r]; }
    std::array<std::array<T, C>, R> _data;
};

template <typename T, std::size_t R, std::size_t C>
struct fmt::formatter<Matrix<T, R, C>> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template <typename FormatContext>
    auto format(const Matrix<T, R, C>& p, FormatContext& ctx) {
        std::string buff;
        for (const auto& t : p._data) {
            buff += fmt::format("{} \n", t);
        }
        return format_to(ctx.out(), "{}", buff);
    }
};
