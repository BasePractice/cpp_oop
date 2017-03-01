#include <gtest/gtest.h>
#include <c_basic.h>
#include <environment.h>

class CBasicMemories : public ::testing::Test {
public:
    CBasicMemories()
            : buffer(nullptr), size(0) {}

    void SetUp() {
        std::string directory = TEST_DATA_DIRECTORY;
        int ret;

        ret = cb_file_content((directory + std::string("/c_basic_content.txt")).c_str(), &buffer, &size);
        ASSERT_EQ(ret, 1);
    }

    void TearDown() {
        cb_free_content(&buffer);
    }

    ~CBasicMemories() {}

    unsigned char *buffer;
    size_t size;
};

TEST_F(CBasicMemories, ReadFileContent) {
    ASSERT_EQ(size, 631);
    ASSERT_TRUE(buffer != nullptr);
}

TEST_F(CBasicMemories, Statistic) {
    struct cb_statistic statistic;

    cb_statistic_content(buffer, size, &statistic);
    EXPECT_EQ(statistic.symbols, 26);
    EXPECT_EQ(statistic.whitespaces, 105);
    EXPECT_EQ(statistic.digits, 0);
    EXPECT_EQ(statistic.alphabets, 500);
    EXPECT_EQ(statistic.lines, 13);
}

