#include "utils.hpp"

using namespace std;
 
void push(const Network& C, MI& F, VI& excess, int u, int v) {
	int cuv = 0;
	for(auto it = C[u].begin(); it != C[u].end(); ++it)
		if((*it).first == v) {
			cuv = (*it).second;
			break;
		}
  int send = min(excess[u], (cuv - F[u][v]));
  F[u][v] += send;
  F[v][u] -= send;
  excess[u] -= send;
  excess[v] += send;
}

void relabel(const Network& C, const MI& F, VI& height, int u) {
  int min_height = INT_MAX;
  for (int v = 0; v < C.size(); v++) {
  	int cuv = 0;
  	for(auto it = C[u].begin(); it != C[u].end(); ++it)
		if((*it).first == v) {
			cuv = (*it).second;
			break;
		}
    if (cuv - F[u][v] > 0) {
      min_height = min(min_height, height[v]);
      height[u] = min_height + 1;
    }
  }
}
 
void discharge(const Network& C, MI& F, VI& excess, VI& height, VI& seen, int u) {
  while (excess[u] > 0) {
    if (seen[u] < C.size()) {
      int v = seen[u];
      int cuv = 0;
      for(auto it = C[u].begin(); it != C[u].end(); ++it)
		if((*it).first == v) {
			cuv = (*it).second;
			break;
		}
      if ((cuv - F[u][v] > 0) && (height[u] > height[v])){
    	push(C, F, excess, u, v);
      }
      else
    seen[u] += 1;
    } else {
      relabel(C, F, height, u);
      seen[u] = 0;
    }
  }
}
 
void moveToFront(int i, VI& A) {
  int temp = A[i];
  int n;
  for (n = i; n > 0; n--){
    A[n] = A[n-1];
  }
  A[0] = temp;
}
 
int pushRelabel(const Network& C, MI& F, int source, int sink) {
  VI excess, height, list, seen;
 
  excess = VI(C.size());
  height = VI(C.size());
  seen = VI(C.size());
  
  for(int i = 1; i < C.size()-1; ++i)
  	list.push_back(i);
 
  height[source] = C.size();
  excess[source] = INT_MAX;

  for (int i = 0; i < C.size(); i++)
    push(C, F, excess, source, i);
 
  int p = 0;
  while (p < C.size() - 2) {
    int u = list[p];
    int old_height = height[u];
    discharge(C, F, excess, height, seen, u);
    if (height[u] > old_height) {
      moveToFront(p,list);
      p=0;
    }
    else
      p += 1;
  }
  int maxflow = 0;
  for (int i = 0; i < C.size(); i++)
    maxflow += F[source][i];
 
  return maxflow;
}
 
void printNetwork(const Network& n) {
    for (int i = 0; i < n.size(); ++i) {
        for (auto it = n[i].begin(); it != n[i].end(); ++it){
            cout << i << " => (" << (*it).first << "," << (*it).second << ") ";
        }
        cout << endl;
    }
}

void printMatrix(const MI& n) {
    for (int i = 0; i < n.size(); ++i) {
        for (int j = 0; j < n[i].size(); ++j){
            cout << i << " => (" << j << "," << n[i][j] << ") ";
        }
        cout << endl;
    }
}
 
int main() {
	MI flow(6,VI(6));
	Network capacities(6);
  /*int **flow, **capacities, i;
  flow = (int **) calloc(NODES, sizeof(int*));
  capacities = (int **) calloc(NODES, sizeof(int*));
  for (i = 0; i < NODES; i++) {
    flow[i] = (int *) calloc(NODES, sizeof(int));
    capacities[i] = (int *) calloc(NODES, sizeof(int));
  }*/
 
  //Sample graph
  /*capacities[0][1] = 2;
  capacities[0][2] = 9;
  capacities[1][2] = 1;
  capacities[1][3] = 0;
  capacities[1][4] = 0;
  capacities[2][4] = 7;
  capacities[3][5] = 7;
  capacities[4][5] = 4;
*/
  capacities[0].push_back(make_pair(1,2));
  capacities[0].push_back(make_pair(2,9));
  capacities[1].push_back(make_pair(2,1));
  //capacities[1].push_back(make_pair(3,0));
  //capacities[1].push_back(make_pair(4,0));
  capacities[2].push_back(make_pair(4,7));
  capacities[3].push_back(make_pair(5,7));
  capacities[4].push_back(make_pair(5,4));
 
  //printf("Capacity:\n");
  cout << "Capacity:" << endl;
  printNetwork(capacities);
 
  printf("Max Flow:\n%d\n", pushRelabel(capacities, flow, 0, 5));
 
  printf("Flows:\n");
  printMatrix(flow);
 
 }