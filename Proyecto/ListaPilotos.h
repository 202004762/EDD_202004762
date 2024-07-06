#ifndef LISTAPILOTOS_H
#define LISTAPILOTOS_H

#include "Piloto.h"
#include <iostream>

struct NodoPiloto {
    Piloto piloto;
    NodoPiloto* siguiente;
    NodoPiloto(const Piloto& piloto) : piloto(piloto), siguiente(nullptr) {}
};

class ListaPilotos {
public:
    NodoPiloto* cabeza;
    NodoPiloto* cola;

    ListaPilotos() : cabeza(nullptr), cola(nullptr) {}

    void insertar(const Piloto& piloto);
    void mostrar();
    ~ListaPilotos();
};

#endif // LISTAPILOTOS_H
