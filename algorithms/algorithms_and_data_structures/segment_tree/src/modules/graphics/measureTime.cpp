#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "main.cpp"

using namespace std;

int main() {
    vector<int> sizes = {10, 100, 1000, 10000, 100000, 1000000};

    ofstream out("results.txt");

    mt19937 rng(123);

    for (int n : sizes) {

        SegmentTree st(n);
        uniform_int_distribution<int> di(0, n - 1);
        uniform_int_distribution<int> dv(1, 1000);

        int l = di(rng);
        int r = di(rng);
        if (l > r) swap(l, r);
        int v = dv(rng);
        int idx = di(rng);

        auto t1 = chrono::high_resolution_clock::now();
        st.update(0, 0, n - 1, l, r, v);
        auto t2 = chrono::high_resolution_clock::now();
        long long update_time =
            chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();

        t1 = chrono::high_resolution_clock::now();
        volatile long long x = st.getValue(0, 0, n - 1, idx);
        t2 = chrono::high_resolution_clock::now();
        long long get_time =
            chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();

        out << n << " " << update_time << " " << get_time << "\n";
    }

    out.close();

    return 0;
}