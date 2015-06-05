#include "utils.hpp"


using namespace std;

int bfs2(Network G, MI F, int s, int t, VI& path) {
    path = VI(G.size(),-1);
    path[s] = -2;
    VI m(G.size());
    m[s] = INT_MAX;
    queue<int> Q;
    Q.push(s);
    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        for (auto it = G[u].begin(); it != G[u].end(); ++it) {
            int v = (*it).first;
            if ((*it).second - F[u][v] > 0 && path[v] == -1) {
                path[v] = u;
                m[v] = min(m[u], (*it).second - F[u][v]);
                if (v == t) return m[t];
                Q.push(v);
            }
        }
    }
    return 0;
}

int edmons2(const Network& G, MI& F, int s, int t) {
    int maxflow = 0;
    F = MI(G.size(), VI(G.size(),0));
    while(true) {
        VI path(G.size());
        int m = bfs2(G,F,s,t,path);
        if (m == 0) break;
        maxflow += m;
        int v = t;
        while (v != s) {
            int u = path[v];
            F[u][v] += m;
            F[v][u] -= m;
            v = u;
        }
    }
    return maxflow;
}