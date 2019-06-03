//
// Created by andreyzartty on 03/06/19.
//

#include "Galeria.h"

string *Galeria::getNombre()  {
    return Nombre;
}

void Galeria::setNombre(string *nombre) {
    Nombre = nombre;
}

string *Galeria::getCliente() {
    return Cliente;
}

void Galeria::setCliente(string *cliente) {
    Cliente = cliente;
}
