cmake_minimum_required(VERSION 3.28)
project(Proyecto)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(Proyecto main.cpp
        include/json.hpp
        Avion.h
        Piloto.h
        ArbolB.h
        ArbolB.cpp
        ListaCircularDoble.h
        ListaCircularDoble.cpp
        ArbolBBusqueda.h
        ArbolBBusqueda.cpp
        TablaHash.h
        TablaHash.cpp
        Grafo.h
        Grafo.cpp
        MatrizDispersa.h
        MatrizDispersa.cpp
        ListaAviones.h
        ListaAviones.cpp
        ListaPilotos.h
        ListaPilotos.cpp
        ListaPilotos.cpp
)
