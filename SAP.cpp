#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef vector<int> VI;
typedef vector<VI> MI;
typedef vector<bool> VB;

void updateResidual(MI& rG, const MI& G, const MI& F){
	for(int i = 0; i < rG.size(); ++i){
		for(int j = 0; j < rG[i].size(); ++j){
			if(G[i][j] > 0){
				rG[i][j] = G[i][j] - F[i][j];
				rG[j][i] = F[i][j];
			}
		}
	}
}

int augment(const MI& rG, MI& F, const VI& path){
	int u = path.size() - 1;
	int min = 10e6;

	while(u != 0){
		int v = path[u];
		if(rG[u][v] > 0){
			min = (min > rG[u][v] - F[u][v]?rG[u][v] - F[u][v]:min);
		} else {
			min = (min > F[v][u]?F[v][u]:min);
		}
		u = v;
	}

	u = path.size() - 1;
	while(u != 0){
		int v = path[u];
		if(rG[u][v] > 0){
			F[u][v] += min;
		} else {
			F[v][u] -= min;
		}
	}

	return min;
}

bool bfs(const MI& rG, int s, int t, VI& path){
	queue<int> Q;
	path = VI(rG.size(),-1);
	VB visited(rG.size(),false);

	visited[0] = true;
	Q.push(0);

	while(!Q.empty()){
		int u = Q.front();
		Q.pop();

		for(int v = 0; v < rG.size(); ++v){
			if(!visited[v] && rG[u][v] > 0){
				visited[v] = true;
				path[v] = u;
				Q.push(v);
				if(v == rG.size() - 1) return true;
			}
		}
	}

	return false;

}

int edmons(const MI& G, int s, int t){
	MI F = MI(G.size(),VI(G.size(),0));
	MI rG = G;

	VI path(G.size());

	int maxflow = 0;

	while(bfs(rG, s, t, path)){
		maxflow += augment(rG,F,path);
		updateResidual(rG,G,F);
	}

}

int main(){
	int n;
	
	cin >> n;

	MI G = MI(n,VI(n));

	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			cin >> G[i][j];

	cout << "MAXFLOW = " << edmons(G,0,n-1) << endl;

}