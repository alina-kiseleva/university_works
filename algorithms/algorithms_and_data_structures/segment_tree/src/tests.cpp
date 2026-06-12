#include <gtest/gtest.h>
#include "../main.cpp"

TEST(Test, SingleQuery) {
    SegmentTree st(5);
    st.update(0, 0, 4, 2, 2, 10);

    EXPECT_EQ(st.getValue(0, 0, 4, 2), 10);

    for (int i = 0; i < 5; ++i) {
        if (i == 2) continue;
        EXPECT_EQ(st.getValue(0, 0, 4, i), 0);
    }
}

TEST(Test, RangeUpdate) {
    SegmentTree st(6);

    st.update(0, 0, 5, 1, 4, 3);

    for (int i = 0; i < 5; ++i) {
        if (i >= 1 && i <= 4){
            EXPECT_EQ(st.getValue(0, 0, 5, i), 3);
        } else {
            EXPECT_EQ(st.getValue(0, 0, 5, i), 0);
        }
    }
}

TEST(Test, SeveralUpdates) {
    SegmentTree st(8);

    st.update(0, 0, 7, 0, 3, 5);
    st.update(0, 0, 7, 2, 6, 7);

    EXPECT_EQ(st.getValue(0, 0, 7, 0), 5);
    EXPECT_EQ(st.getValue(0, 0, 7, 1), 5);
    EXPECT_EQ(st.getValue(0, 0, 7, 2), 12);
    EXPECT_EQ(st.getValue(0, 0, 7, 3), 12);
    EXPECT_EQ(st.getValue(0, 0, 7, 4), 7);
    EXPECT_EQ(st.getValue(0, 0, 7, 5), 7);
    EXPECT_EQ(st.getValue(0, 0, 7, 6), 7);
    EXPECT_EQ(st.getValue(0, 0, 7, 7), 0);
}

TEST(Test, NoUpdates) {
    SegmentTree st(5);

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(st.getValue(0, 0, 4, i), 0);
    }
}
