#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <utility>
using namespace std;

typedef vector<int> VI;
typedef vector<bool> VB;
typedef vector<list<pair<int,int> > > Network;

void printNetwork(Network n) {
    for (int i = 0; i < n.size(); ++i) {
        cout << endl << "El nodo " << i << " está conectado a: " << endl;
        for (auto it = n[i].begin(); it != n[i].end(); ++it){
            cout << "--> nodo " << (*it).first << " con capacidad " << (*it).second << endl;
        }
    }
}

void updateResidual(Network& rG, const Network& G, const Network& F){
	rG = Network(G.size());
	for(int u = 0; u < G.size(); ++u){
		for (auto it = G[u].begin(); it != G[u].end(); ++it){
			int v = (*it).first;
			int c = (*it).second;
			if(c > 0){
				int fuv = 0;
				for (auto itt = F[u].begin(); itt != F[u].end(); ++itt){
					if((*itt).first == v){
						fuv = (*itt).second;
						break;
					}
				}
				rG[u].push_back(make_pair(v,c-fuv));
				rG[v].push_back(make_pair(u,fuv));
			}
			
		}
	}
}

int augment(const Network& rG, Network& F, const VI& path){
	int u = path.size() - 1;
	int min = 10e6;

	while(u != 0){
		int v = path[u];
		int fuv = 0, fvu = 0, cuv = 0;

		for (auto it = rG[v].begin(); it != rG[v].end(); ++it){
			if((*it).first == u){
				cuv = (*it).second;
				break;
			}
		}
		if(cuv > 0){
			for (auto it = F[u].begin(); it != F[u].end(); ++it){
				if((*it).first == v){
					fuv = (*it).second;
					break;
				}
			}
			min = (min > cuv - fuv?cuv - fuv:min);
		} else {
			for (auto it = F[v].begin(); it != F[v].end(); ++it){
				if((*it).first == u){
					fvu = (*it).second;
					break;
				}
			}
			min = (min > fvu?fvu:min);
		}
		u = v;
	}

	u = path.size() - 1;
	while(u != 0){
		int v = path[u];
		int cuv = 0;
		for (auto it = rG[u].begin(); it != rG[u].end(); ++it){
			if((*it).first == v){
				cuv = (*it).second;
				break;
			}
		}
		if(cuv > 0){
			for (auto it = F[u].begin(); it != F[u].end(); ++it){
				if((*it).first == v){
					(*it).second += min;
					break;
				}
			}
		} else {
			for (auto it = F[v].begin(); it != F[v].end(); ++it){
				if((*it).first == u){
					(*it).second += min;
					break;
				}
			}
		}
		u = v;
	}

	return min;
}

bool bfs(const Network& rG, int s, int t, VI& path){
	queue<int> Q;
	path = VI(rG.size(),-1);
	VB visited(rG.size(),false);

	visited[0] = true;
	Q.push(0);

	while(!Q.empty()){
		int u = Q.front();
		Q.pop();

		for (auto it = rG[u].begin(); it != rG[u].end(); ++it){
			int v = (*it).first;
			if(!visited[v] && (*it).second > 0){
				visited[v] = true;
				path[v] = u;
				Q.push(v);
				if(v == rG.size() - 1) return true;
			}
        }
	}

	return false;

}

int edmons(const Network& G, int s, int t){
	Network F(G.size());

	for(int u = 0; u < F.size(); ++u)
		for(auto it = G[u].begin(); it != G[u].end(); ++it){
			F[u].push_back(make_pair((*it).first,0));
		}

	Network rG = G;

	VI path(G.size());

	int maxflow = 0;

	while(bfs(rG, s, t, path)){
		maxflow += augment(rG,F,path);
		updateResidual(rG,G,F);
	}

	return maxflow;
}

// int main(){
// 	int n;
	
// 	cin >> n;

// 	Network G(n);

// 	for(int i = 0; i < n; ++i)
// 		for(int j = 0; j < n; ++j){
// 			int c;
// 			cin >> c;
// 			if(c > 0)
// 				G[i].push_back(make_pair(j,c));
// 		}

// 	cout << "MAXFLOW = " << edmons(G,0,n-1) << endl;

// }