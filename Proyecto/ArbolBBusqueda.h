#ifndef ARBOLBBUSQUEDA_H
#define ARBOLBBUSQUEDA_H
#include "Piloto.h"


struct NodoBST {
    Piloto piloto;
    NodoBST* izquierda;
    NodoBST* derecha;

    NodoBST(Piloto _piloto) : piloto(_piloto), izquierda(nullptr), derecha(nullptr) {}
};

class ArbolBinarioBusqueda {
    NodoBST* raiz;

public:
    ArbolBinarioBusqueda() : raiz(nullptr) {}

    void insertar(Piloto piloto);
    void eliminar(const std::string& numero_de_id);
    void recorridoPreorden();
    void recorridoInorden();
    void recorridoPostorden();
    void generarDot(NodoBST* nodo, ofstream& archivo, bool isLeft);
    void graficarBST(ArbolBinarioBusqueda& arbol);


private:
    void insertar(NodoBST*& nodo, Piloto piloto);
    NodoBST* eliminarRecursivo(NodoBST* nodo, const std::string& numero_de_id);
    NodoBST* buscarMinimo(NodoBST* nodo);
    void recorridoPreorden(NodoBST* nodo);
    void recorridoInorden(NodoBST* nodo);
    void recorridoPostorden(NodoBST* nodo);
};

#endif //ARBOLBBUSQUEDA_H
