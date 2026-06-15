#include <iostream>
#include <vector>

struct Node{
    long long d;   
};

class SegmentTree{
    std::vector<Node> tree;
public:

    SegmentTree(long long n){
        tree.resize(4*n, {0});
    }

    void push(long long node){
        if (tree[node].d != 0) {
            tree[node*2+1].d += tree[node].d;
            tree[node*2+2].d += tree[node].d;
            tree[node].d = 0;
        }
    }

    void update(long long node, long long l, long long r, long long ql, long long qr, long long v){
        if (qr < l || r < ql) return;

        if (ql <= l && r <= qr){
            tree[node].d += v;
            return;
        }

        push(node);

        long long mid = (l + r) / 2;
        update(node*2+1, l, mid, ql, qr, v);
        update(node*2+2, mid+1, r, ql, qr, v);
    }

    long long getValue(long long node, long long l, long long r, long long index){
        if (l == r){
            return tree[node].d;
        }

        push(node);

        long long mid = (l + r) / 2;
        if (index <= mid)
            return getValue(node*2+1, l, mid, index);
        else
            return getValue(node*2+2, mid+1, r, index);
    }
};

int main(){
    long long n, m;
    std::cin >> n >> m;

    SegmentTree st(n);
    std::vector<long long> ans;

    for (int j = 0; j < m; j++){
        int type;
        std::cin >> type;

        if (type == 1){
            long long l, r, v;
            std::cin >> l >> r >> v;
            st.update(0, 0, n-1, l, r-1, v);
        } else {
            long long i;
            std::cin >> i;
            ans.push_back(st.getValue(0, 0, n-1, i));
        }
    }

    for (auto x : ans) 
        std::cout << x << "\n";
    
    return 0;
}
