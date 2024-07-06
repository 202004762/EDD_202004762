#ifndef LISTAAVIONES_H
#define LISTAAVIONES_H

#include "Avion.h"
#include <iostream>

struct NodoAvion {
    Avion avion;
    NodoAvion* siguiente;
    NodoAvion(const Avion& avion) : avion(avion), siguiente(nullptr) {}
};

class ListaAviones {
public:
    NodoAvion* cabeza;
    NodoAvion* cola;

    ListaAviones() : cabeza(nullptr), cola(nullptr) {}

    void insertar(const Avion& avion);
    void mostrar();
    ~ListaAviones();
};

#endif // LISTAAVIONES_H
