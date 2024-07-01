#ifndef LISTADOBLE_H
#define LISTADOBLE_H
#include "Avion.h"

using namespace std;


struct NodoAvion {
    Avion avion;
    NodoAvion* siguiente;
    NodoAvion* anterior;
};

class Listadoble {
private:
    NodoAvion* inicio;
    string nombreLista;

public:
    Listadoble(string nombre);
    ~Listadoble();
    bool estaVacia() const;
    void insertarAlFinal(Avion avion);
    void mostrar() const;
    void moverAvion(Listadoble& otraLista, string numero_de_registro);
    void eliminar(string numero_de_registro);
    bool contieneAvion(const string& numero_de_registro) const;
    void graficar(const std::string &nombreArchivo);
};

#endif
