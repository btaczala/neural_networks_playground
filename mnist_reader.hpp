#pragma once
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "image.hpp"

namespace fs = std::filesystem;

inline Images readOne(const fs::path& pathToMnistImageFile,
                      std::size_t imageIndex) {
    return Images{};
}

inline std::uint32_t swapBytes(std::uint32_t value) {
    return (value << 24) | ((value << 8) & 0x00FF0000) |
           ((value >> 8) & 0X0000FF00) | (value >> 24);
}

struct MNistReader {
    static constexpr const std::size_t kMagicHeaderSize = sizeof(int);
    MNistReader(const fs::path& p) {
        if (!fs::exists(p)) {
            throw std::runtime_error("File does not exists");
        }
        std::ifstream file;
        _file.open(p.string(), std::ios::in | std::ios::binary);
        file.seekg(0, std::ios::beg);
    }

    bool valid() const noexcept { return !!_file; }

    template <typename T>
    T readFromPosition(std::size_t position) const {
        _file.seekg(position, std::ios::beg);
        T value;
        _file.read(reinterpret_cast<char*>(&value), sizeof(T));
        return value;
    }

    std::uint32_t magicHeader() const {
        return swapBytes(readFromPosition<std::uint32_t>(0));
    }

    std::uint32_t count() const {
        return swapBytes(readFromPosition<std::uint32_t>(4));
    }

   protected:
    mutable std::fstream _file;
};

struct MNistReaderImageSet : public MNistReader {
    std::uint32_t rows() const {
        return swapBytes(readFromPosition<std::uint32_t>(8));
    }
    std::uint32_t columns() const {
        return swapBytes(readFromPosition<std::uint32_t>(12));
    }

    Image readImage(std::size_t position) {
        constexpr const std::size_t imageOffset = 16;
        const std::size_t startPosition = imageOffset + 28 * 28 * position;
        _file.seekg(startPosition, std::ios::beg);
        Image img{};
        _file.read(reinterpret_cast<char*>(img.data.data()), 28 * 28);
        return img;
    }
};

struct MNistReaderLabelSet : public MNistReader {
    std::int8_t readLabel(std::size_t N) {
        const std::size_t position = 8 + N;
        return readFromPosition<std::int8_t>(position);
    }
};
