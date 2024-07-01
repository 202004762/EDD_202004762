#ifndef COLA_H
#define COLA_H
#include "Pasajero.h"


struct Nodo {
    Pasajero pasajero;
    Nodo* siguiente;
};

struct Cola {
    Nodo* frente;
    Nodo* final;

    Cola();
    ~Cola();
    void encolar(Pasajero p);
    void desencolar();
    bool estaVacia() const;
    Pasajero obtenerFrente() const;
    void graficar();
};

#endif
