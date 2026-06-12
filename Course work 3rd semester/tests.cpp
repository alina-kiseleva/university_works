#include "avltree.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(AvlTreeTest, InsertSingleElement) {
    AvlTree tree;
    tree.insert("Pizza", 350);
    EXPECT_EQ(tree.searchByName("Pizza"), 350);
}

TEST(AvlTreeTest, SearchNonExistent) {
    AvlTree tree;
    EXPECT_EQ(tree.searchByName("Burger"), -1);
}

TEST(AvlTreeTest, LeftLeftRotation) {
    AvlTree tree;
    tree.insert("C", 300);
    tree.insert("B", 200);
    tree.insert("A", 100);
    
    EXPECT_EQ(tree.searchByName("A"), 100);
    EXPECT_EQ(tree.searchByName("B"), 200);
    EXPECT_EQ(tree.searchByName("C"), 300);
}

TEST(AvlTreeTest, RightRightRotation) {
    AvlTree tree;
    tree.insert("A", 100);
    tree.insert("B", 200);
    tree.insert("C", 300);
    
    EXPECT_EQ(tree.searchByName("A"), 100);
    EXPECT_EQ(tree.searchByName("B"), 200);
    EXPECT_EQ(tree.searchByName("C"), 300);
}

TEST(AvlTreeTest, LeftRightRotation) {
    AvlTree tree;
    tree.insert("C", 300);
    tree.insert("A", 100);
    tree.insert("B", 200);
    
    EXPECT_EQ(tree.searchByName("A"), 100);
    EXPECT_EQ(tree.searchByName("B"), 200);
    EXPECT_EQ(tree.searchByName("C"), 300);
}

TEST(AvlTreeTest, RightLeftRotation) {
    AvlTree tree;
    tree.insert("A", 100);
    tree.insert("C", 300);
    tree.insert("B", 200);
    
    EXPECT_EQ(tree.searchByName("A"), 100);
    EXPECT_EQ(tree.searchByName("B"), 200);
    EXPECT_EQ(tree.searchByName("C"), 300);
}

TEST(AvlTreeTest, DeleteNodeWithTwoChildren) {
    AvlTree tree;
    tree.insert("B", 200);
    tree.insert("A", 100);
    tree.insert("C", 300);
    
    tree.remove("B");
    EXPECT_EQ(tree.searchByName("A"), 100);
    EXPECT_EQ(tree.searchByName("C"), 300);
    EXPECT_EQ(tree.searchByName("B"), -1);
}

TEST(AvlTreeTest, DeleteNonExistent) {
    AvlTree tree;
    tree.insert("Pizza", 350);
    tree.remove("Burger");
    EXPECT_EQ(tree.searchByName("Pizza"), 350);
}

TEST(AvlTreeTest, UpdateExistingItem) {
    AvlTree tree;
    tree.insert("Pizza", 350);
    tree.insert("Pizza", 400);
    
    EXPECT_EQ(tree.searchByName("Pizza"), 400);
}

TEST(AvlTreeTest, MultipleInsertAndSearch) {
    AvlTree tree;
    std::vector<std::pair<std::string, int>> menu = {
        {"Pizza", 350},
        {"Burger", 250},
        {"Salad", 180},
        {"Pasta", 320},
        {"Soup", 150}
    };
    
    for (const auto& item : menu) {
        tree.insert(item.first, item.second);
    }
    
    for (const auto& item : menu) {
        EXPECT_EQ(tree.searchByName(item.first), item.second);
    }
}

TEST(AvlTreeTest, SequentialAddRemove) {
    AvlTree tree;
    
    for (int i = 0; i < 5; ++i) {
        tree.insert("Item" + std::to_string(i), i * 100);
    }

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(tree.searchByName("Item" + std::to_string(i)), i * 100);
    }

    for (int i = 0; i < 5; ++i) {
        tree.remove("Item" + std::to_string(i));
        EXPECT_EQ(tree.searchByName("Item" + std::to_string(i)), -1);
    }
}

TEST(AvlTreeTest, EmptyStringKey) {
    AvlTree tree;
    tree.insert("", 100);
    EXPECT_EQ(tree.searchByName(""), 100);
    tree.remove("");
    EXPECT_EQ(tree.searchByName(""), -1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}