
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <utility>
#include <climits>
using namespace std;
 
typedef vector<int> VI;
typedef vector<VI> MI;
typedef vector<bool> VB;
typedef vector<float> VF;
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

VF calcularProbabilidades(MI m) {
    int personas = m.size();
    int viajes = m[0].size();
    VF ret(personas,0);
    VF pViaje(viajes,0);
    for (int i = 0; i < viajes; ++i) {
        int pEnV = 0;
        for (int j = 0; j < personas; ++j) {
            if (m[j][i] != 0) ++pEnV;
        }
        pViaje[i] = 1.0 / pEnV;
    }

    for (int i = 0; i < personas; ++i) {
        for (int j = 0; j < viajes; ++j) {
            if (m[i][j] != 0) ret[i] += pViaje[j]; 
        }
    }
    for (int i = 0; i < personas; ++i) cout << ret[i] << endl;
    return ret; 
}



int main()
{
    int personas, viajes;
    cin >> personas >> viajes;
    MI entrada(personas,VI(viajes));
    for(int i = 0; i < personas; ++i)
        for(int j = 0; j < viajes; ++j)
            cin >> entrada[i][j];


    bool posibleA = true;
    bool posibleB = true;

    VF probabilidades = calcularProbabilidades(entrada);


    Network grafo(personas+viajes+4); 
    // pos 0 S'
    // pos 1 S
    // pos n-2 T
    // pos n-1 T'

    // Declarando S'
     // *conexion con S
    int sum = 0;
    for (int i = 0; i < personas; ++i) if (int(probabilidades[i]) < probabilidades[i]) ++sum;
    grafo[0].push_back(make_pair(1,sum));
     // *aristas a las personas
    for (int i = 0; i < personas; ++i) if(int(probabilidades[i]) > 0) grafo[0].push_back(make_pair(i+2,int(probabilidades[i])));
    // Declarando S
     // *aristas a personas
    for (int i = 0; i < personas; ++i) if (int(probabilidades[i]) != probabilidades[i]) grafo[1].push_back(make_pair(i+2,1));
     // *arista a T'
    sum = 0;
    for (int i = 0; i < personas; ++i) sum += int(probabilidades[i]);
    grafo[1].push_back(make_pair(grafo.size()-1,sum));
    
    // Declarando T
    grafo[grafo.size()-2].push_back(make_pair(grafo.size()-1, viajes)); // A T' con el número de viajes
    grafo[grafo.size()-2].push_back(make_pair(1,INT_MAX)); // ponemos el inifinito para que siga siendo circulacion
 
    // CASO A
    // Creamos el grafo para ver si se puede cumplir todas las restricciones.
    // Metemos solamente las aristas que pertenecen a las personas con preferencia positiva
    // y las aristas de todas las personas a las que van a un viaje en el cual no existe ninguna preferencia positiva y les da igual
    for (int i = 0; i < viajes; ++i) {
        bool algun3 = false;
        for (int j = 0; j < personas; ++j) {
            if (entrada[j][i] == 3) {
                if (!algun3) algun3 = true;
                else { posibleA = false; break;} // nos lo ahorramos ya que vamos a tener que reiniciar todo para la opcion B
                grafo[j+2].push_back(make_pair(2+personas+i,1));
            }
        }
        if (!posibleA) break;
        if (!algun3) {
            for (int j = 0; j < personas; ++j) {
                if (entrada[j][i] == 2) {
                    grafo[j+2].push_back(make_pair(2+personas+i,1));
                }
            }
        }
    }
    
        printNetwork(grafo);

    if (posibleA) {
        cout << "La opcion A es posible" << endl;
        // Llamar al fulkerson para que lo resuelva
        int ed =  edmons(grafo,0,grafo.size()-1);
        if (viajes == ed) {
            cout << "La opcion A ha sido satisfecha" << endl;
            return 0;
        }
        else cout << "ola k ase AAA " << ed << endl;
    }

    // CASO B
    // Creamos el grafo para ver si se puede cumplir las restricciones negativas.
    // Metemos las aristas que pertenecen a las personas con preferencia positivia y sin preferencia.

    for (int i = 0; i < personas; ++i) grafo[i+2].clear();
    for (int i = 0; i < viajes; ++i) {
        bool viajeVacio = true;
        for (int j = 0; j < personas; ++j) {
            if (entrada[j][i] >= 2) {
                grafo[j+2].push_back(make_pair(2+personas+i,1));
                viajeVacio = false;
            }
        }
        if (viajeVacio) posibleB = false;
    } 

        //printNetwork(grafo);

    if (posibleB) {
        cout << "La opcion B es posible" << endl;

        int ed =  edmons(grafo,0,grafo.size()-1);
        if (viajes == ed) {
            cout << "La opcion B ha sido satisfecha" << endl;
            return 0;
        }
        else cout << "ola k ase " << ed << endl;

    }

    for (int i = 0; i < viajes; ++i) {
        for (int j = 0; j < personas; ++j) {
            if (entrada[j][i] == 1) grafo[j+2].push_back(make_pair(2+personas+i,1));
        }
    } 

    if (viajes == edmons(grafo,0,grafo.size()-1)) {
        cout << "La opcion C ha sido satisfecha" << endl;
        return 0;
    }
    else cout << "La opcion D es la unica posible" << endl;

    //printNetwork(grafo);

    return 0;
}
