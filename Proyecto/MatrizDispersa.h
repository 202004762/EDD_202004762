#ifndef MATRIZ_DISPERSA_H
#define MATRIZ_DISPERSA_H
#include <string>
#include <iostream>
#include <fstream>


class NodoMatriz {
public:
    std::string pilotoID;
    std::string vuelo;
    std::string ciudadDestino;
    NodoMatriz* siguienteFila;
    NodoMatriz* siguienteColumna;

    NodoMatriz(const std::string& id, const std::string& vuelo, const std::string& ciudad)
        : pilotoID(id), vuelo(vuelo), ciudadDestino(ciudad), siguienteFila(nullptr), siguienteColumna(nullptr) {}
};

class MatrizDispersa {
private:
    NodoMatriz* filaHeader;
    NodoMatriz* columnaHeader;

    NodoMatriz* obtenerFilaHeader(const std::string& vuelo);
    NodoMatriz* obtenerColumnaHeader(const std::string& ciudadDestino);

public:
    MatrizDispersa();
    ~MatrizDispersa();
    void insertar(const std::string& pilotoID, const std::string& vuelo, const std::string& ciudadDestino);
    void eliminarPiloto(const std::string& pilotoID);
    void mostrar();
    void graficarMatriz();
};

#endif // MATRIZ_DISPERSA_H
