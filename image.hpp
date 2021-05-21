#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <vector>

using Pixel = std::uint8_t;
struct Image {
    Image(std::size_t w = 28, std::size_t h = 28) : _w(w), _h(h) {
        data.resize(w * h);
    }
    Pixel pixel(std::size_t x, std::size_t y) const {
        return data.at(x + _h * y);
    }

    std::vector<Pixel> data;

   private:
    std::size_t _w;
    std::size_t _h;
};
using Images = std::vector<Image>;

inline bool operator==(const Image& lhs, const Image& rhs) {
    return lhs.data.size() != rhs.data.size()
               ? false
               : std::equal(lhs.data.begin(), lhs.data.end(), rhs.data.begin());
}
inline std::ostream& operator<<(std::ostream& os, const Image& im) {
    for (auto i = 0; i < 28; ++i) {
        for (auto j = 0; j < 28; ++j) {
            os << (im.pixel(j, i) == 0 ? "." : "@");
        }
        os << "\n";
    }
    return os;
}
