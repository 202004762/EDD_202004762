#include "Grafo.h"


Grafo::Grafo() : numCiudades(0) {
    std::memset(matriz, 0, sizeof(matriz));
}

int Grafo::obtenerIndiceCiudad(const std::string& ciudad) const {
    for (int i = 0; i < numCiudades; ++i) {
        if (ciudades[i] == ciudad) {
            return i;
        }
    }
    return -1;
}

void Grafo::cargarDesdeArchivo(const std::string& nombreArchivo) {
    std::ifstream inputFile(nombreArchivo);
    if (!inputFile.is_open()) {
        std::cerr << "Error abriendo el archivo de rutas." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(inputFile, linea)) {
        std::istringstream iss(linea);
        std::string origen, destino;
        int distancia;

        std::getline(iss, origen, '/');
        std::getline(iss, destino, '/');
        iss >> distancia;

        int indiceOrigen = obtenerIndiceCiudad(origen);
        if (indiceOrigen == -1) {
            indiceOrigen = numCiudades;
            ciudades[numCiudades++] = origen;
        }

        int indiceDestino = obtenerIndiceCiudad(destino);
        if (indiceDestino == -1) {
            indiceDestino = numCiudades;
            ciudades[numCiudades++] = destino;
        }

        matriz[indiceOrigen][indiceDestino] = distancia;
        matriz[indiceDestino][indiceOrigen] = distancia; // Considerando rutas bidireccionales
    }

    inputFile.close();
}

void Grafo::mostrarRutas() const {
    for (int i = 0; i < numCiudades; ++i) {
        std::cout << "Desde " << ciudades[i] << ":" << std::endl;
        for (int j = 0; j < numCiudades; ++j) {
            if (matriz[i][j] != 0) {
                std::cout << "  Hacia " << ciudades[j] << " con distancia " << matriz[i][j] << " km" << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

void Grafo::dijkstra(const std::string& origen, const std::string& destino) const {
    int distancias[MAX_CIUDADES];
    bool visitados[MAX_CIUDADES] = { false };
    int previos[MAX_CIUDADES];
    for (int i = 0; i < numCiudades; ++i) {
        distancias[i] = INT_MAX;
        previos[i] = -1;
    }

    int indiceOrigen = obtenerIndiceCiudad(origen);
    if (indiceOrigen == -1) {
        std::cerr << "Ciudad de origen no encontrada." << std::endl;
        return;
    }

    int indiceDestino = obtenerIndiceCiudad(destino);
    if (indiceDestino == -1) {
        std::cerr << "Ciudad de destino no encontrada." << std::endl;
        return;
    }

    distancias[indiceOrigen] = 0;

    for (int i = 0; i < numCiudades; ++i) {
        int minDistancia = INT_MAX;
        int u = -1;

        for (int j = 0; j < numCiudades; ++j) {
            if (!visitados[j] && distancias[j] < minDistancia) {
                minDistancia = distancias[j];
                u = j;
            }
        }

        if (u == -1) break;

        visitados[u] = true;

        for (int v = 0; v < numCiudades; ++v) {
            if (matriz[u][v] && !visitados[v] && distancias[u] + matriz[u][v] < distancias[v]) {
                distancias[v] = distancias[u] + matriz[u][v];
                previos[v] = u;
            }
        }
    }

    if (distancias[indiceDestino] == INT_MAX) {
        std::cerr << "No hay ruta disponible entre " << origen << " y " << destino << "." << std::endl;
        return;
    }

    std::cout << "Ruta mas corta desde " << origen << " a " << destino << " con distancia " << distancias[indiceDestino] << " km:" << std::endl;

    int camino[MAX_CIUDADES];
    int indice = 0;
    for (int v = indiceDestino; v != -1; v = previos[v]) {
        camino[indice++] = v;
    }

    for (int i = indice - 1; i >= 0; --i) {
        std::cout << ciudades[camino[i]];
        if (i > 0) std::cout << " -> ";
    }
    std::cout << std::endl;
}

void Grafo::generarArchivoDOT() const {
    std::ofstream archivo("Grafo.dot");
    archivo << "graph G {\n";
    archivo << "node [shape=circle];\n";

    // Escribir nodos
    for (int i = 0; i < numCiudades; ++i) {
        archivo << "    \"" << ciudades[i] << "\";\n";
    }

    // Escribir aristas
    for (int i = 0; i < numCiudades; ++i) {
        for (int j = i + 1; j < numCiudades; ++j) {
            if (matriz[i][j] != 0) {
                archivo << "    \"" << ciudades[i] << "\" -- \"" << ciudades[j] << "\" [label=\"" << matriz[i][j] << "\"];\n";
            }
        }
    }

    archivo << "}\n";
    archivo.close();

    // Generar imagen PNG y abrir en visor predeterminado
    system("dot -Tpng Grafo.dot -o Grafo.png");
    system("start Grafo.png"); // Para Windows, abrirá la imagen en el visor predeterminado
}