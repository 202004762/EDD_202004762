#ifndef LISTACIRCULARDOBLE_H
#define LISTACIRCULARDOBLE_H
#include "Avion.h"


struct Nodo {
    Avion avion;
    Nodo* siguiente;
    Nodo* anterior;

    Nodo(Avion _avion) : avion(_avion), siguiente(nullptr), anterior(nullptr) {}
};

class ListaCircularDoble {
    Nodo* cabeza;

public:
    ListaCircularDoble() : cabeza(nullptr) {}

    void insertar(Avion avion);
    void mostrar();
    bool buscar(string numero_de_registro);
    void eliminar(string numero_de_registro);
    bool obtenerAvionPorNumeroRegistro(string numero_de_registro, Avion& avion); // Nuevo método
    void generarArchivoDOT(const std::string& archivo) const;
};


#endif
