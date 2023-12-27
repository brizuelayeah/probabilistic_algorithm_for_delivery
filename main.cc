#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iomanip> 
#include <numeric>
#include <chrono>

using namespace std;


// Definicion de una estructura para representar una linea
struct Linea{
public:
    int N;
    int M;
    int C;
    int A;
    int B;
};

// Definición de una estructura para representar una carretera 
struct Carretera {
    int start;
    int end;
    int time;
    double probabilityStartToEnd;
    double probabilityEndToStart;
};


int getRandomNonZeroIndex(vector<double> values,  size_t size) {
    
    double* cumulativeProbabilities = new double[size];
    double cumulativeSum = 0.0;

    for (size_t i = 0; i < size; ++i) {
        cumulativeSum += values[i];
        cumulativeProbabilities[i] = cumulativeSum;
    }

    double randomValue = static_cast<double>(rand()) / RAND_MAX;

    for (size_t i = 0; i < size; ++i) {
        if (randomValue <= cumulativeProbabilities[i]) {
            delete[] cumulativeProbabilities;
            return static_cast<int>(i + 1);
        }
    }
    delete[] cumulativeProbabilities;
    return -1;
}




double probabilistic_algorithm(vector<vector<double>> mapa_probabilidades, vector<vector<double>> mapa_tiempos, 
                                int n_vertices, int casa, int almacen_empresa) {
        int interseccion_actual = almacen_empresa;
        bool entregado = false;  
        vector<int> camino;
        int carreteras_recorridas = 0;
        camino.insert(camino.begin() + 0, interseccion_actual);
        while (!entregado) {       
            vector<double> destinos_interseccion;
            for (int j = 0; j < n_vertices; j++) {
                destinos_interseccion.insert(destinos_interseccion.begin() + j, mapa_probabilidades[interseccion_actual - 1][j]);
            }
            interseccion_actual = getRandomNonZeroIndex(destinos_interseccion, n_vertices);
            carreteras_recorridas++;
            if (interseccion_actual == casa) {
                entregado = true;
            }
            camino.insert(camino.begin() + carreteras_recorridas, interseccion_actual);
        }
        double tiempo_camino = 0;
        for (int j = 0; j < carreteras_recorridas; j++) {
            tiempo_camino += mapa_tiempos[camino[j + 1] - 1][camino[j] - 1];
        }
        return tiempo_camino;
}

// Función que imprime por pantalla una matriz
void imprimirMatriz(const vector<vector<double>>& matriz){
    for(int i = 0; i < matriz.size(); i++){
        for(int j = 0; j < matriz.size(); j++){
            cout << std::setw(4) << matriz[i][j] << " ";
        }
        cout << endl;
    }
}

// Función principal
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    string nombreFichero = "generated_graph.txt";

    ifstream fichero;
    fichero.open(nombreFichero);

    if(!fichero.is_open()){
        cout << "Error al abrir el fichero" << endl;
        return 1;
    }

    Linea linea;

    fichero >> linea.N >> linea.M >> linea.C >> linea.A >> linea.B;
    Carretera carretera;
    carretera.start = linea.N;
    carretera.end = linea.M;
    carretera.time = linea.C;
    carretera.probabilityStartToEnd = linea.A;
    carretera.probabilityEndToStart = linea.B;

    int n_col_fich = 5; 
    int n_fil_fich = linea.M;
    int n_matriz = linea.N;
    
    vector<vector<double>> mapa_probabilidades(n_matriz, vector<double>(n_matriz, 0.0));
    vector<vector<double>> mapa_tiempos(n_matriz, vector<double>(n_matriz, 0.0));
 
    for(int i = 0; i < linea.M; i++){   
        double a, b, c, d, e;
        fichero >> a >> b >> c >> d >> e;
        size_t index_a = static_cast<size_t>(a);
        size_t index_b = static_cast<size_t>(b);
        mapa_tiempos[index_a-1][index_b-1] = c;
        mapa_tiempos[index_b-1][index_a-1] = c;
        mapa_probabilidades[index_a-1][index_b-1] = d;
        mapa_probabilidades[index_b-1][index_a-1] = e;
    }

    fichero.close();

    const int n = 100;
    double simulaciones_A[n] = {0};
    double simulaciones_B[n] = {0};
    double bootstraps_A[500] = {};
    double bootstraps_B[500] = {};
    double simulaciones_A_bootstrap[n] = {0};
    double simulaciones_B_bootstrap[n] = {0};

    auto start = chrono::high_resolution_clock::now();  

    for(int i = 0; i < n; i++) {
        simulaciones_A[i] = probabilistic_algorithm(mapa_probabilidades, mapa_tiempos, linea.N, linea.C, linea.A);
        simulaciones_B[i] = probabilistic_algorithm(mapa_probabilidades, mapa_tiempos, linea.N, linea.C, linea.B);
    }

    // Calcular media simulaciones almacen A
    int suma_simulaciones_A = 0;
    double media_simulaciones_A = 0.0;
    for (int i = 0; i < n; i++) {
        suma_simulaciones_A += simulaciones_A[i];
    }
    media_simulaciones_A = static_cast<double>(suma_simulaciones_A) / n;

    // Calcular media simulaciones almacen B
    int suma_simulaciones_B = 0;
    double media_simulaciones_B = 0.0;
    for (int i = 0; i < n; i++) {
        suma_simulaciones_B += simulaciones_B[i];
    }
    media_simulaciones_B = static_cast<double>(suma_simulaciones_B) / n;


  
    // Calcular bootstraps almacen A
    for (int i = 0; i < 500; i++) {
        int suma_simulaciones_A_bootstrap = 0;
        double media_simulaciones_A_bootstrap = 0.0;

        // Elegir aleatoriamente n valores con reemplazo
        for (int j = 0; j < n; j++) {
            int randomIndex = rand() % n;
            simulaciones_A_bootstrap[j] = simulaciones_A[randomIndex];
        }

        // Calcular media de esos valores
        for (int i = 0; i < n; i++) {
            suma_simulaciones_A_bootstrap += simulaciones_A_bootstrap[i];
        }
        media_simulaciones_A_bootstrap = static_cast<double>(suma_simulaciones_A_bootstrap) / n;

        // Cada bootstrap guardará una media
        bootstraps_A[i] = media_simulaciones_A_bootstrap;
    }

    // Calcular bootstraps almacen B
    for (int i = 0; i < 500; i++) {
        int suma_simulaciones_B_bootstrap = 0;
        double media_simulaciones_B_bootstrap = 0.0;

        // Elegir aleatoriamente n valores con reemplazo
        for (int j = 0; j < n; j++) {
            int randomIndex = rand() % n;
            simulaciones_B_bootstrap[j] = simulaciones_B[randomIndex];
        }

        // Calcular media de esos valores
        for (int i = 0; i < n; i++) {
            suma_simulaciones_B_bootstrap += simulaciones_B_bootstrap[i];
        }
        media_simulaciones_B_bootstrap = static_cast<double>(suma_simulaciones_B_bootstrap) / n;

        // Cada bootstrap guardará una media
        bootstraps_B[i] = media_simulaciones_B_bootstrap;
    }


    // Ordenar las medias de cada bootstrap
    sort(bootstraps_A, bootstraps_A + 500);
    sort(bootstraps_B, bootstraps_B + 500);


    // Intervalos [L, R]
    double R_A = 2 * media_simulaciones_A - ((bootstraps_A[24] + bootstraps_A[25]) / 2);
    double L_A = 2 * media_simulaciones_A - ((bootstraps_A[474] + bootstraps_A[475]) / 2);
    
    double R_B = 2 * media_simulaciones_B - ((bootstraps_B[24] + bootstraps_B[25]) / 2);
    double L_B = 2 * media_simulaciones_B - ((bootstraps_B[474] + bootstraps_B[475]) / 2);
    

    // Fin ejecución, se toma la medida del tiempo en microsegundos
    auto end = chrono::high_resolution_clock::now();

    auto tiempo_ejecucion = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << endl;
    cout << "L_A: " <<  L_A << endl;
    cout << "R_A: " << R_A << endl;
    cout << "-> Tiempo A: " << (R_A - L_A )<< endl;
    cout << "L_B: " << L_B << endl;
    cout << "R_B: " << R_B << endl;
    cout << "-> Tiempo B: " << R_B - L_B << endl;

    if(L_A < L_B && R_A < R_B) {
        cout << "Mejor empresa A" << endl;
    }
    else if(L_B < L_A && R_B < R_A) {
        cout << "Mejor empresa B" << endl;
    }
    else {
        cout << "No se sabe" << endl;
    }
    cout << "Tiempo de ejecucion del codigo para llevar a cabo la simulacion: " << tiempo_ejecucion << " microsegundos" << endl;
}