#include "mnist_reader.hpp"

#include <fmt/format.h>
#include <gtest/gtest.h>

#include <filesystem>

const std::string kMnistDir = MNIST_DIR;
namespace fs = std::filesystem;

TEST(mnist_reader, train_images_idx3_ubyte) {
    MNistReader reader{fs::path(kMnistDir) / "train-images-idx3-ubyte"};
    ASSERT_TRUE(reader.valid());
    EXPECT_EQ(reader.magicHeader(), 0x803);
    EXPECT_EQ(reader.count(), 60000);
}

TEST(mnist_reader, train_labels_idx3_ubyte) {
    MNistReader reader{fs::path(kMnistDir) / "train-labels-idx1-ubyte"};
    ASSERT_TRUE(reader.valid());
    EXPECT_EQ(reader.magicHeader(), 0x801);
    EXPECT_EQ(reader.count(), 60000);
}

TEST(mnist_reader_labels, fetchNLabel) {
    MNistReaderLabelSet labels{fs::path(kMnistDir) / "train-labels-idx1-ubyte"};

    const auto label = labels.readLabel(0);
    // EXPECT_TRUE(label >= 0 && label <= 9);
    EXPECT_EQ(label, 5);
}

struct MNistReaderImageSetTest : public ::testing::Test {
    MNistReaderImageSetTest()
        : images{fs::path(kMnistDir) / "train-images-idx3-ubyte"} {}

    void SetUp() override { ASSERT_TRUE(images.valid()); };
    MNistReaderImageSet images;
};

TEST_F(MNistReaderImageSetTest, image0) {
    const std::string data =
        R"(............................
............................
............................
............................
............................
............@@@@@@@@@@@@....
........@@@@@@@@@@@@@@@@....
.......@@@@@@@@@@@@@@@@.....
.......@@@@@@@@@@@..........
........@@@@@@@.@@..........
.........@@@@@..............
...........@@@@.............
...........@@@@.............
............@@@@@@..........
.............@@@@@@.........
..............@@@@@@........
...............@@@@@........
.................@@@@.......
..............@@@@@@@.......
............@@@@@@@@........
..........@@@@@@@@@.........
........@@@@@@@@@@..........
......@@@@@@@@@@............
....@@@@@@@@@@..............
....@@@@@@@@................
............................
............................
............................
)";

    std::stringstream ss;
    const auto im = images.readImage(0);
    ss << im;
    EXPECT_EQ(ss.str(), data);
}

TEST_F(MNistReaderImageSetTest, images_should_not_be_equal) {
    EXPECT_NE(images.readImage(0), Image{});
    EXPECT_NE(images.readImage(1), images.readImage(14));
}

TEST_F(MNistReaderImageSetTest, cols_rows) {
    EXPECT_EQ(images.magicHeader(), 0x803);
    EXPECT_EQ(images.count(), 60000);

    EXPECT_EQ(images.rows(), 28);
    EXPECT_EQ(images.columns(), 28);
}
