#ifndef ARBOLB_H
#define ARBOLB_H
#include <vector>
#include <fstream>
#include "Avion.h"

using namespace std;


struct NodoB {
    vector<Avion> claves;
    vector<NodoB*> hijos;
    bool hoja;

    NodoB(bool esHoja) : hoja(esHoja) {}
};

class ArbolB {
    int orden;
    NodoB* raiz;

public:
    ArbolB(int _orden) : orden(_orden), raiz(nullptr) {}

    void insertar(Avion k);
    void eliminar(string numero_de_registro);
    void recorrer();
    NodoB* buscar(string numero_de_registro);
    void exportarArbolDot(ArbolB& arbol_b);
    void generarDot(NodoB* nodo, ofstream& archivo);

    Avion* obtenerAvionPorNumeroRegistro(const std::string& numeroRegistro) {
        return buscarAvionPorNumero(raiz, numeroRegistro);
    }

private:
    void insertarNoLleno(NodoB* nodo, Avion k);
    void dividirHijo(NodoB* nodo, int i, NodoB* y);
    void recorrer(NodoB* nodo);
    NodoB* buscar(NodoB* nodo, string numero_de_registro);
    void eliminar(NodoB* nodo, string numero_de_registro);
    Avion encontrarSucesor(NodoB* nodo);
    void rellenar(NodoB* nodo, int indice);
    void moverClaveAnterior(NodoB* nodo, int indice);
    void moverClaveSiguiente(NodoB* nodo, int indice);
    void fusionar(NodoB* nodo, int indice);

    Avion* buscarAvionPorNumero(NodoB* nodo, const std::string& numeroRegistro) {
        if (nodo == nullptr) {
            return nullptr;
        }

        int i = 0;
        while (i < nodo->claves.size() && numeroRegistro > nodo->claves[i].numero_de_registro) {
            i++;
        }

        if (i < nodo->claves.size() && nodo->claves[i].numero_de_registro == numeroRegistro) {
            return &(nodo->claves[i]);
        }

        return (nodo->hoja) ? nullptr : buscarAvionPorNumero(nodo->hijos[i], numeroRegistro);
    }
};



#endif
