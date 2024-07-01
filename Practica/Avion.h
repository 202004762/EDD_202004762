#ifndef AVION_H
#define AVION_H
#include <string>

using namespace std;


struct Avion {
    string vuelo;
    string numero_de_registro;
    string modelo;
    string fabricante;
    int ano_fabricacion;
    int capacidad;
    int peso_max_despegue;
    string aerolinea;
    string estado;
};

#endif
