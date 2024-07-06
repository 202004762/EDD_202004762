#include "ListaAviones.h"

void ListaAviones::insertar(const Avion& avion) {
    NodoAvion* nuevoNodo = new NodoAvion(avion);
    if (cola) {
        cola->siguiente = nuevoNodo;
    } else {
        cabeza = nuevoNodo;
    }
    cola = nuevoNodo;
}

void ListaAviones::mostrar() {
    NodoAvion* actual = cabeza;
    while (actual) {
        std::cout << "Vuelo: " << actual->avion.vuelo
                  << ", Registro: " << actual->avion.numero_de_registro
                  << ", Modelo: " << actual->avion.modelo
                  << ", Capacidad: " << actual->avion.capacidad
                  << ", Aerolinea: " << actual->avion.aerolinea
                  << ", Ciudad destino: " << actual->avion.ciudad_destino
                  << ", Estado: " << actual->avion.estado << std::endl;
        actual = actual->siguiente;
    }
}

ListaAviones::~ListaAviones() {
    NodoAvion* actual = cabeza;
    while (actual) {
        NodoAvion* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}
