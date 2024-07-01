#include "Cola.h"
#include <stdexcept>
#include <iostream>


Cola::Cola() : frente(nullptr), final(nullptr) {}

Cola::~Cola() {
    while (!estaVacia()) {
        desencolar();
    }
}

void Cola::encolar(Pasajero p) {
    Nodo* nuevoNodo = new Nodo{p, nullptr};
    if (final) {
        final->siguiente = nuevoNodo;
    } else {
        frente = nuevoNodo;
    }
    final = nuevoNodo;
}

void Cola::desencolar() {
    if (frente) {
        Nodo* temp = frente;
        frente = frente->siguiente;
        if (!frente) {
            final = nullptr;
        }
        delete temp;
    }
}

bool Cola::estaVacia() const {
    return frente == nullptr;
}

Pasajero Cola::obtenerFrente() const {
    if (frente) {
        return frente->pasajero;
    }
    throw runtime_error("La cola esta vacia");
}

void Cola::graficar() {
    FILE* archivo = fopen("Cola.dot", "w");
    if (archivo != nullptr) {
        fprintf(archivo, "digraph G {\n");
        fprintf(archivo, "node [shape=record];\n");
        fprintf(archivo, "rankdir = LR;\n");
        fprintf(archivo, "label = \"Cola de Pasajeros\";\n");

        Nodo* temp = frente;
        int i = 0;
        while (temp != nullptr) {
            fprintf(archivo, "node%d [label = \"Vuelo: %s | Asiento: %d\"];\n", i, temp->pasajero.vuelo.c_str(), temp->pasajero.asiento);
            if (temp->siguiente != nullptr) {
                fprintf(archivo, "node%d -> node%d;\n", i, i + 1);
            }
            i++;
            temp = temp->siguiente;
        }

        fprintf(archivo, "}\n");
        fclose(archivo);

        system("dot -Tpng Cola.dot -o Cola.png");
        system("start Cola.png");
    } else {
        std::cout << "Error al abrir el archivo" << std::endl;
    }
}
