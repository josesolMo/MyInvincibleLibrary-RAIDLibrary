
#include "Galeria.h"

/**
 * Representa una galeria, es en esta donde se archivaran las fotos.
 *
 * @since 03/06/19
 */


///Constructores


Galeria::Galeria(string _nombre, string _cliente) {

    nombre = _nombre;
    cliente = _cliente;

}



///Metodos


void Galeria::insertarImagen() {

}


//Getters & Setters


/**
 * Getter del nombre de Galeria
 * @return string
 */
string Galeria::getNombre()  {
    return nombre;
}

/**
 * Setter del nombre de Galeria
 * @param nombre
 */
void Galeria::setNombre(string _nombre) {
    nombre = _nombre;
}

/**
 * Getter del cliente de Galeria
 * @return string
 */
string Galeria::getCliente() {
    return cliente;
}

/**
 * Setter del cliente de Galeria
 * @param cliente
 */
void Galeria::setCliente(string _cliente) {
    cliente = _cliente;
}
