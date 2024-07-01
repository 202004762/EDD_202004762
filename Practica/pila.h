#ifndef PILA_H
#define PILA_H
#include <iostream>
#include <cstdio>


class NodoPila {
public:
    int equipaje;
    NodoPila* siguiente;

    NodoPila(int equipaje, NodoPila* siguiente = nullptr)
        : equipaje(equipaje), siguiente(siguiente) {}

    int getEquipaje() const { return equipaje; }
    NodoPila* getSiguiente() const { return siguiente; }
};

class Pila {
private:
    NodoPila* tope;

public:
    Pila() : tope(nullptr) {}
    ~Pila() {
        while (!vacia()) {
            pop();
        }
    }

    void push(int equipaje) {
        tope = new NodoPila(equipaje, tope);
    }

    bool vacia() const {
        return tope == nullptr;
    }

    int pop() {
        if (vacia()) {
            throw std::runtime_error("La pila está vacía");
        }
        NodoPila* temp = tope;
        int equipaje = temp->getEquipaje();
        tope = tope->getSiguiente();
        delete temp;
        return equipaje;
    }

    void mostrar() const {
        NodoPila* temp = tope;
        while (temp != nullptr) {
            std::cout << temp->getEquipaje() << " ";
            temp = temp->getSiguiente();
        }
        std::cout << std::endl;
    }

    void graficar() {
        if (vacia()) {
            std::cout << "La pila está vacía" << std::endl;
            return;
        }
        FILE* archivo = fopen("Pila.dot", "w");
        if (archivo != nullptr) {
            fprintf(archivo, "digraph G {\n");
            fprintf(archivo, "node [shape=record];\n");
            fprintf(archivo, "rankdir=TB;\n");
            fprintf(archivo, "label=\"Pila de Equipaje\";\n");
            NodoPila* temp = tope;
            while (temp != nullptr) {
                fprintf(archivo, "node%p [label=\"Equipaje: %d\"];\n", (void*)temp, temp->getEquipaje());
                if (temp->getSiguiente() != nullptr) {
                    fprintf(archivo, "node%p -> node%p;\n", (void*)temp, (void*)temp->getSiguiente());
                }
                temp = temp->getSiguiente();
            }
            fprintf(archivo, "}");
            fclose(archivo);
            system("dot -Tpng Pila.dot -o Pila.png");
            system("start Pila.png");
        } else {
            std::cout << "Error al abrir el archivo" << std::endl;
        }
    }
};

#endif // PILA_H
