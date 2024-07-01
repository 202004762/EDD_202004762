cmake_minimum_required(VERSION 3.28)
project(Practica)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(Practica main.cpp
        include/json.hpp
        Pasajero.h
        cola.cpp
        Avion.h
        listadoble.h
        listadoble.cpp
        pila.h
        listapasajeros.h
        listapasajeros.cpp
)
