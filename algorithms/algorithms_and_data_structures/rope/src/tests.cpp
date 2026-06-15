#include <gtest/gtest.h>
#include "../main.cpp"

TEST(RopeConstruction, BuildFromEmptyString) {
    Node* rope = buildRope("");
    EXPECT_EQ(rope, nullptr);
}

TEST(RopeConstruction, BuildFromShortString) {
    Node* rope = buildRope("Hello");
    std::string result;
    collect(rope, result);
    EXPECT_EQ(result, "Hello");
}

TEST(RopeConstruction, BuildFromLongString) {
    std::string longString(500, 'X');
    Node* rope = buildRope(longString);
    std::string result;
    collect(rope, result);
    EXPECT_EQ(result, longString);
}

TEST(RopeConcat, ConcatTwoNonEmptyRopes) {
    Node* rope1 = buildRope("Hello");
    Node* rope2 = buildRope("World");
    Node* result = concat(rope1, rope2);
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "HelloWorld");
}

TEST(RopeConcat, ConcatWithEmptyLeft) {
    Node* rope1 = buildRope("");
    Node* rope2 = buildRope("World");
    Node* result = concat(rope1, rope2);
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "World");
}

TEST(RopeConcat, ConcatWithEmptyRight) {
    Node* rope1 = buildRope("Hello");
    Node* rope2 = buildRope("");
    Node* result = concat(rope1, rope2);
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "Hello");
}

TEST(RopeSplit, SplitAtBeginning) {
    Node* rope = buildRope("Hello World");
    auto [left, right] = split(rope, 0);
    
    std::string leftStr, rightStr;
    collect(left, leftStr);
    collect(right, rightStr);
    
    EXPECT_EQ(leftStr, "");
    EXPECT_EQ(rightStr, "Hello World");
}

TEST(RopeSplit, SplitAtEnd) {
    Node* rope = buildRope("Hello World");
    auto [left, right] = split(rope, 11);
    
    std::string leftStr, rightStr;
    collect(left, leftStr);
    collect(right, rightStr);
    
    EXPECT_EQ(leftStr, "Hello World");
    EXPECT_EQ(rightStr, "");
}

TEST(RopeSplit, SplitInMiddle) {
    Node* rope = buildRope("Hello World");
    auto [left, right] = split(rope, 5);
    
    std::string leftStr, rightStr;
    collect(left, leftStr);
    collect(right, rightStr);
    
    EXPECT_EQ(leftStr, "Hello");
    EXPECT_EQ(rightStr, " World");
}

TEST(RopeSplit, SplitSingleCharacter) {
    Node* rope = buildRope("A");
    auto [left, right] = split(rope, 1);
    
    std::string leftStr, rightStr;
    collect(left, leftStr);
    collect(right, rightStr);
    
    EXPECT_EQ(leftStr, "A");
    EXPECT_EQ(rightStr, "");
}

TEST(RopeInsert, InsertAtBeginning) {
    Node* rope = buildRope("World");
    Node* result = insert(rope, 0, "Hello ");
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "Hello World");
}

TEST(RopeInsert, InsertAtEnd) {
    Node* rope = buildRope("Hello");
    Node* result = insert(rope, 5, " World");
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "Hello World");
}

TEST(RopeInsert, InsertInMiddle) {
    Node* rope = buildRope("Hello World");
    Node* result = insert(rope, 5, " Beautiful");
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "Hello Beautiful World");
}

TEST(RopeInsert, InsertEmptyString) {
    Node* rope = buildRope("Hello World");
    Node* result = insert(rope, 5, "");
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "Hello World");
}

TEST(RopeDelete, DeleteFromBeginning) {
    Node* rope = buildRope("Hello World");
    Node* result = deleteSubstr(rope, 0, 6);
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "World");
}

TEST(RopeDelete, DeleteFromEnd) {
    Node* rope = buildRope("Hello World");
    Node* result = deleteSubstr(rope, 5, 6);
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "Hello");
}

TEST(RopeDelete, DeleteFromMiddle) {
    Node* rope = buildRope("Hello World");
    Node* result = deleteSubstr(rope, 5, 1);
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "HelloWorld");
}

TEST(RopeDelete, DeleteZeroCharacters) {
    Node* rope = buildRope("Hello World");
    Node* result = deleteSubstr(rope, 5, 0);
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "Hello World");
}

TEST(RopeDelete, DeleteEntireString) {
    Node* rope = buildRope("Hello World");
    Node* result = deleteSubstr(rope, 0, 11);
    
    std::string strResult;
    collect(result, strResult);
    EXPECT_EQ(strResult, "");
}

TEST(RopeEdgeCases, VeryLongString) {
    std::string longStr(10000, 'A');
    Node* rope = buildRope(longStr);
    std::string result;
    collect(rope, result);
    EXPECT_EQ(result, longStr);
}

TEST(RopeEdgeCases, SeveralOperations) {
    Node* rope = buildRope("Start");
    rope = insert(rope, 5, " Middle");
    rope = insert(rope, 0, "Prefix ");
    rope = deleteSubstr(rope, 6, 7);
    
    std::string result;
    collect(rope, result);
    EXPECT_EQ(result, "PrefixMiddle");
}

TEST(RopeTraversal, CollectEmptyRope) {
    Node* rope = buildRope("");
    std::string result;
    collect(rope, result);
    EXPECT_EQ(result, "");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}