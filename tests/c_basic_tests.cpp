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

class CBasicHash: public ::testing::Test {
public:

    CBasicHash()
            : table(nullptr) {}

    void SetUp() override {
        table = ht_create(200);
        ASSERT_TRUE(table != nullptr);
        ASSERT_EQ(table->size, 200);
    }

    void TearDown() override {
        ht_free(table);
        table = nullptr;
    }

public:

    hash_table *table;
};

TEST_F(CBasicHash, Create) {
    ASSERT_TRUE(table->table != nullptr);
}

TEST_F(CBasicHash, GetPut) {
    ht_put(table, "key1", "value1");
    ht_put(table, "key2", "value2");
    ht_put(table, "key3", "value3");
    ASSERT_STREQ(ht_get(table, "key1"), "value1");
    ASSERT_STREQ(ht_get(table, "key2"), "value2");
    ASSERT_STREQ(ht_get(table, "key3"), "value3");
    ASSERT_TRUE(ht_get(table, "key4") == nullptr);
}

TEST_F(CBasicMemories, ReadFileContent) {
    ASSERT_EQ(size, 644);
    ASSERT_TRUE(buffer != nullptr);
}

TEST_F(CBasicMemories, Statistic) {
    struct cb_statistic statistic;

    cb_statistic_content(buffer, size, &statistic);
    EXPECT_EQ(statistic.symbols, 26);
    EXPECT_EQ(statistic.whitespaces, 118);
    EXPECT_EQ(statistic.digits, 0);
    EXPECT_EQ(statistic.alphabets, 500);
    EXPECT_EQ(statistic.lines, 13);
}

