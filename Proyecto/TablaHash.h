#ifndef TABLAHASH_H
#define TABLAHASH_H
#include "Piloto.h"
#include <iostream>
#include <fstream>


class TablaHash {
    struct NodoPiloto {
        Piloto piloto;
        NodoPiloto* siguiente;

        NodoPiloto(const Piloto& _piloto) : piloto(_piloto), siguiente(nullptr) {}
    };

    NodoPiloto** tabla;
    int M;

    int funcionHash(const std::string& llave);

public:
    TablaHash(int tamanoInicial = 18);
    ~TablaHash();

    void insertar(const Piloto& piloto);
    Piloto* buscar(const std::string& numero_de_id);
    void mostrar();
    void imprimirValorAscii(const std::string& numero_de_id);
    void generarArchivoDOT() const;
    void eliminar(const std::string& numero_de_id);

};

#endif
