
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
            if (m[j][i] != 0) ret[i] += pViaje[j]; 
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
    // pos n-1 T
    // pos n T'

    // Declarando S'
     // *conexion con S
    int sum = 0;
    for (int i = 0; i < personas; ++i) if (int(probabilidades[i]) < probabilidades[i]) ++sum;
    grafo[0].push_back(make_pair(1,sum));
     // *aristas a las personas
    for (int i = 0; i < personas; ++i) grafo[0].push_back(make_pair(i+2,int(probabilidades[i])));
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
        // if (viajes == edmons(grafo,0,grafo.size()-1)) {
        //     cout << "La opcion A ha sido satisfecha" << endl;
        //     return 0;
        // }
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

        printNetwork(grafo);

    if (posibleB) {
        cout << "La opcion B es posible" << endl;

        // if (viajes == edmons(grafo,0,grafo.size()-1)) {
        //     cout << "La opcion B ha sido satisfecha" << endl;
        //     return 0;
        // }
    }

    for (int i = 0; i < viajes; ++i) {
        for (int j = 0; j < personas; ++j) {
            if (entrada[j][i] == 1) grafo[j+2].push_back(make_pair(2+personas+i,1));
        }
    } 

    // if (viajes == edmons(grafo,0,grafo.size()-1)) {
    //     cout << "La opcion C ha sido satisfecha" << endl;
    //     return 0;
    // }
    // else cout << "La opcion D es la unica posible" << endl;

    printNetwork(grafo);

    return 0;
}
