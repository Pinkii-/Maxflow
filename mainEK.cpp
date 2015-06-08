#include "edmonds.hpp"
using namespace std;

int personas, viajes;

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
        if (pEnV) pViaje[i] = 1.0 / pEnV;
        else pViaje[i] = 0;
    }

    for (int i = 0; i < personas; ++i) {
        for (int j = 0; j < viajes; ++j) {
            if (m[i][j] != 0) ret[i] += pViaje[j]; 
        }
    }
    return ret; 
}

int main(){
    cin >> personas >> viajes;
    MI entrada(personas,VI(viajes));
    for(int i = 0; i < personas; ++i)
        for(int j = 0; j < viajes; ++j)
            cin >> entrada[i][j];

    bool posibleA = true;
    bool posibleB = true;

    VF probabilidades = calcularProbabilidades(entrada);


    Network grafo(personas+viajes+4); 
    int S = 0;// S'
    int s = 1;// S
    int t = grafo.size()-1;// T'

    // Declarando S'
       // *conexion con S
      int sumDif = 0;
      for (int i = 0; i < personas; ++i) if (int(probabilidades[i]) < probabilidades[i]) ++sumDif;
      if (sumDif) grafo[S].push_back(make_pair(s,sumDif));
     // *aristas a las personas
    for (int i = 0; i < personas; ++i) if(int(probabilidades[i]) > 0) grafo[S].push_back(make_pair(i+2,int(probabilidades[i])));
    // Declarando S
     // *aristas a personas
    for (int i = 0; i < personas; ++i) if (int(probabilidades[i]) != probabilidades[i]) grafo[s].push_back(make_pair(i+2,1));
     // *arista a T'
    int sumLower = 0;
    for (int i = 0; i < personas; ++i) sumLower += int(probabilidades[i]);
    if (sumLower) grafo[s].push_back(make_pair(t,sumLower+sumDif-viajes));
    
    // Declarando todos los viajes
    for (int i = 0; i < viajes; ++i) grafo[i+personas+2].push_back(make_pair(t,1));
    
    int resultado = sumDif + sumLower;
 
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

    MI f;

    if (posibleA) {
        int ed =  edmonds(grafo,f,0,grafo.size()-1);
        if (resultado == ed) {
            cout << "A" << endl;
            return 0;
        }
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
    if (posibleB) {
        int ed =  edmonds(grafo,f,0,grafo.size()-1);
        if (resultado == ed) {
            cout << "B" << endl;
            return 0;
        }
    }
    for (int i = 0; i < viajes; ++i) {
        for (int j = 0; j < personas; ++j) {
            if (entrada[j][i] == 1) grafo[j+2].push_back(make_pair(2+personas+i,1));
        }
    } 
    int ed = edmonds(grafo,f,0,grafo.size()-1);

    if (ed == resultado) cout << "C" << endl;
    else cout << "D"<< endl;

    return 0;
}
