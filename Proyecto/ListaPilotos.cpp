#include "ListaPilotos.h"

void ListaPilotos::insertar(const Piloto& piloto) {
    NodoPiloto* nuevoNodo = new NodoPiloto(piloto);
    if (cola) {
        cola->siguiente = nuevoNodo;
    } else {
        cabeza = nuevoNodo;
    }
    cola = nuevoNodo;
}

void ListaPilotos::mostrar() {
    NodoPiloto* actual = cabeza;
    while (actual) {
        std::cout << "ID: " << actual->piloto.numero_de_id
                  << ", Nombre: " << actual->piloto.nombre
                  << ", Nacionalidad: " << actual->piloto.nacionalidad
                  << ", Vuelo: " << actual->piloto.vuelo
                  << ", Horas de vuelo: " << actual->piloto.horas_de_vuelo
                  << ", Tipo de licencia: " << actual->piloto.tipo_de_licencia << std::endl;
        actual = actual->siguiente;
    }
}

ListaPilotos::~ListaPilotos() {
    NodoPiloto* actual = cabeza;
    while (actual) {
        NodoPiloto* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}
