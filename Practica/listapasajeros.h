#ifndef LISTAPASAJEROS_H
#define LISTAPASAJEROS_H
#include "Pasajero.h"


struct NodoPasajero {
    Pasajero pasajero;
    NodoPasajero* siguiente;
    NodoPasajero* anterior;
};

class ListaPasajeros {
private:
    NodoPasajero* cabeza;
    NodoPasajero* cola;

public:
    ListaPasajeros();
    ~ListaPasajeros();
    void insertarOrdenado(const Pasajero& pasajero);
    void mostrar();
    bool contienePasajero(const string& numeroPasaporte) const;
    void mostrarPasajero(const string& numeroPasaporte) const;
    void graficar();
};

#endif
