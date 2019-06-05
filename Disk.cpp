
#include "Disk.h"


/**
 * Representa un disco fisico que almacena datos, sera utilizado para
 * ejecutar una simulacion de un RAID 5,
 *
 * @since 04/06/19
 */


///Constructores


/**
 * Constructor de Disk.
 */
Disk::Disk(int _id) {
    bitList = new List();
    id = _id;
}




///Metodos


//-


///Getters & Setters


/**
 * Getter de bitList de Disk.
 * @return lista
 */
List* Disk::getBitList() {
    return bitList;
}

/**
 * Setter de bitList de Disk.
 * @param _bitList
 */
void Disk::setBitList(List* _bitList) {
    bitList = _bitList;
}

/**
 * Getter de id de Disk.
 * @return int
 */
int Disk::getId() {
    return 0;
}

/**
 * Setter de id de Disk.
 * @param _id
 */
void Disk::setId(int _id) {
    id = _id;
}

