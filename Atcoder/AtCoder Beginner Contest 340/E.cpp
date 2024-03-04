#include <bits/stdc++.h>
using i64 = std::int64_t;

template <class Info, class Tag>
struct LazySegmentTree {
    /*
        Info: apply,operator +
        Tag: apply
        all operations obey [l,r)
    */
    const int n;
    std::vector<Info> info;
    std::vector<Tag> tag;
    LazySegmentTree(int n)
        : n(n), info(4 << std::__lg(n)), tag(4 << std::__lg(n)) {}
    LazySegmentTree(std::vector<Info> init) : LazySegmentTree(init.size()) {
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m, r);
            pull(p);
        };
        build(1, 0, n);
    }
    void pull(int p) { info[p] = info[2 * p] + info[2 * p + 1]; }
    void apply(int p, const Tag &v) {
        info[p].apply(v);
        tag[p].apply(v);
    }
    void push(int p) {
        apply(2 * p, tag[p]);
        apply(2 * p + 1, tag[p]);
        tag[p] = Tag();
    }
    void modify(int p, int l, int r, int x, const Info &v) {
        if (r - l == 1) {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        push(p);
        if (x < m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p);
    }
    void modify(int p, const Info &v) { modify(1, 0, n, p, v); }
    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) {
            return Info();
        }
        if (l >= x && r <= y) {
            return info[p];
        }
        int m = (l + r) / 2;
        push(p);
        return rangeQuery(2 * p, l, m, x, y) +
               rangeQuery(2 * p + 1, m, r, x, y);
    }
    Info rangeQuery(int l, int r) { return rangeQuery(1, 0, n, l, r); }
    void rangeApply(int p, int l, int r, int x, int y, const Tag &v) {
        if (l >= y || r <= x) {
            return;
        }
        if (l >= x && r <= y) {
            apply(p, v);
            return;
        }
        int m = (l + r) / 2;
        push(p);
        rangeApply(2 * p, l, m, x, y, v);
        rangeApply(2 * p + 1, m, r, x, y, v);
        pull(p);
    }
    void rangeApply(int l, int r, const Tag &v) {
        return rangeApply(1, 0, n, l, r, v);
    }
};

struct Tag {
    i64 x = 0;
    void apply(const Tag &a) { x = x + a.x; }
};

struct Info {
    i64 x = 0;
    int l, r;
    void apply(const Tag &a) { x += 1LL * (r - l) * a.x; }
};
Info operator+(const Info &a, const Info &b) { return {a.x + b.x, a.l, b.r}; }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N, M;
    std::cin >> N >> M;
    std::vector<Info> init(N);

    for (int i = 0; i < N; i++) {
        i64 x;
        std::cin >> x;
        init[i] = {x, i, i + 1};
    }

    LazySegmentTree<Info, Tag> sgtree(init);

    for (int i = 0; i < M; i++) {
        int b;
        std::cin >> b;

        auto cnt = sgtree.rangeQuery(b, b + 1).x;
        sgtree.rangeApply(b, b + 1, {-cnt});
        if (N - (b + 1) > cnt) {
            sgtree.rangeApply(b + 1, b + 1 + cnt, {1});
            continue;
        } else {
            sgtree.rangeApply(b + 1, N, {1});
        }
        cnt -= (N - (b + 1));
        auto left = cnt % N;
        auto full = cnt / N;

        sgtree.rangeApply(0, N, {full});
        if (left != 0) {
            sgtree.rangeApply(0, left, {1});
        }
    }

    for (int i = 0; i < N; i++) {
        std::cout << sgtree.rangeQuery(i, i + 1).x << " \n"[i == N - 1];
    }

    return 0;
}