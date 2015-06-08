#include "utils.hpp"

using namespace std;

extern int personas, viajes; 

void prF(MI n) {
    cout << "Solucion preflowpush:" << endl;
    for (int i = 0; i < viajes; ++i) {
        cout << "viaje " << i << ": ";
        for (int j = 0; j < personas; ++j) {
            if (n[j+2][i+personas+2] != 0) cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;
}

 
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

void push(const Network& C, MI& F, VI& excess, int u, int v , int cuv) {
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
      push(C, F, excess, u, v, cuv);
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

int preflowpush(const Network& C, MI& F, int source, int sink) {
 
  VI seen(C.size());
  VI list;
  for(int i = 0; i < C.size(); ++i)
    if (i != source && i != sink) list.push_back(i);
 
  VI height(C.size(),0);
  height[source] = C.size();

  VI excess(C.size(),0);
  excess[source] = INT_MAX;

  F = MI(C.size(),VI(C.size(),0));
  for (auto it = C[source].begin(); it != C[source].end(); ++it)
    push(C, F, excess, source, (*it).first , (*it).second);
 
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
  for (int i = 0; i < C.size(); ++i)
    maxflow += F[source][i];
 
  return maxflow;
}

