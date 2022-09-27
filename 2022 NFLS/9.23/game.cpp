#include <bits/stdc++.h>
using namespace std;
#define For(Ti, Ta, Tb) for (int Ti = (Ta); Ti <= (Tb); ++Ti)
#define Dec(Ti, Ta, Tb) for (int Ti = (Ta); Ti >= (Tb); --Ti)
#define Debug(...) fprintf(stderr, __VA_ARGS__)
using ll = long long;
const int N = 2e5 + 5;
int n, m;
char tp;
double pr[N][2][2];
struct SegTree {
    struct Node {
        int l, r;
        double pr[2][2], e[2][2];
    } t[N * 4];
    Node Merge(const Node &ls, const Node &rs) {
        Node res{};
        res.l = ls.l, res.r = rs.r;
        For(i, 0, 1) For(j, 0, 1) {
            For(k, 0, 1) For(l, 0, 1) {
                double P = ls.pr[i][k] * rs.pr[l][j] * pr[rs.l][k][l];
                res.pr[i][j] += P;
                res.e[i][j] += P * (ls.e[i][k] + rs.e[l][j]);
            }
            res.e[i][j] /= res.pr[i][j];
        }
        return res;
    }
    void Build(int p, int l, int r) {
        t[p].l = l, t[p].r = r;
        if (l == r) {
            t[p].pr[0][0] = t[p].pr[1][1] = t[p].e[1][1] = 1;
            return;
        }
        int mid = (l + r) / 2;
        Build(p * 2, l, mid), Build(p * 2 + 1, mid + 1, r);
        t[p] = Merge(t[p * 2], t[p * 2 + 1]);
    }
    Node Query(int p, int l, int r) {
        if (l <= t[p].l && t[p].r <= r)
            return t[p];
        int mid = (t[p].l + t[p].r) / 2;
        if (r <= mid)
            return Query(p * 2, l, r);
        if (l > mid)
            return Query(p * 2 + 1, l, r);
        return Merge(Query(p * 2, l, r), Query(p * 2 + 1, l, r));
    }
} seg;
double Prob(map<int, int>::iterator it1, map<int, int>::iterator it2) {
    return seg.Query(1, it1->first, it2->first).e[it1->second][it2->second];
}
int main() {
    ios::sync_with_stdio(false), cin.tie(nullptr);
    freopen("game.in", "r", stdin);
    freopen("game.out", "w", stdout);
    cin >> n >> m >> ws >> tp >> pr[1][1][1];
    pr[1][1][0] = 1 - pr[1][1][1];
    For(i, 2, n) {
        cin >> pr[i][1][1] >> pr[i][0][1];
        pr[i][1][0] = 1 - pr[i][1][1], pr[i][0][0] = 1 - pr[i][0][1];
    }
    pr[0][0][1] = pr[n + 1][0][1] = pr[n + 1][1][1] = 1;
    seg.Build(1, 0, n + 1);
    map<int, int> pos{ { 0, 1 }, { n + 1, 1 } };
    double ans = seg.Query(1, 0, n + 1).e[1][1];
    cout << fixed << setprecision(6);
    For(_, 1, m) {
        char op[5];
        int i;
        cin >> op >> i;
        if (op[0] == 'a') {
            int c;
            cin >> c;
            auto it = pos.emplace(i, c).first;
            ans += Prob(prev(it), it) + Prob(it, next(it)) - Prob(prev(it), next(it)) - c;
        } else {
            auto it = pos.find(i);
            ans += Prob(prev(it), next(it)) - Prob(prev(it), it) - Prob(it, next(it)) + it->second;
            pos.erase(it);
        }
        cout << ans - 2 << '\n';
    }
#ifdef zyz
    Debug("Elapsed time: %dms\n", int(clock()));
#endif
    return 0;
}
