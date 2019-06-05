
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


//-

