#ifndef GRAFO_H
#define GRAFO_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>


const int MAX_CIUDADES = 100; // Máximo número de ciudades

class Grafo {
    int matriz[MAX_CIUDADES][MAX_CIUDADES];
    std::string ciudades[MAX_CIUDADES];
    int numCiudades;

    int obtenerIndiceCiudad(const std::string& ciudad) const;

public:
    Grafo();
    void cargarDesdeArchivo(const std::string& nombreArchivo);
    void mostrarRutas() const;
    void dijkstra(const std::string& origen, const std::string& destino) const;
    void generarArchivoDOT() const;
};

#endif // GRAFO_H
