#ifndef PASAJERO_H
#define PASAJERO_H
#include <string>
#include <iostream>

using namespace std;


struct Pasajero {
    string nombre;
    string nacionalidad;
    string numero_de_pasaporte;
    string vuelo;
    int asiento;
    string destino;
    string origen;
    int equipaje_facturado;

    bool operator<(const Pasajero& other) const {
        if (vuelo == other.vuelo) {
            return asiento < other.asiento;
        }
        return vuelo < other.vuelo;
    }

    void mostrar() const {
        cout << "Nombre: " << nombre << endl;
        //cout << "Nacionalidad: " << nacionalidad << endl;
        cout << "Numero de pasaporte: " << numero_de_pasaporte << endl;
        //cout << "Vuelo: " << vuelo << endl;
        //cout << "Asiento: " << asiento << endl;
        //cout << "Destino: " << destino << endl;
        //cout << "Origen: " << origen << endl;
        cout << "Equipaje facturado: " << equipaje_facturado << endl;
    }
};

#endif
