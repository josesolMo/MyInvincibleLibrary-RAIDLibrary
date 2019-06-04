
#include "RAIDController.h"


/**
 * Representa un controlador para el RAIDLibrary, manejara lo necesario para
 * cumplir el uso correcto del RAID 5.
 *
 * @since 04/06/19
 */


///Constructores


/**
 * Constructor de RAIDController.
 * @param _raidLibrary
 */
RAIDController::RAIDController() {
    disk1 = new Disk();
    disk2 = new Disk();
    disk3 = new Disk();
    disk4 = new Disk();

    actualSplit1 = "";
    actualSplit2 = "";
    actualSplit3 = "";
    actualParity = "";

}


///Metodos


/**
 * Divide la imagen en [n cantidad] para ser ingresadas en los discos.
 * @param image
 */
void RAIDController::splitImage(Image *image) {

    //

}


///Getters & Setters


/**
 * Getter de disk1 de RAIDController.
 * @return disk
 */
Disk *RAIDController::getDisk1() {
    return disk1;
}

/**
 * Setter de disk1 de RAIDController.
 * @param _disk1
 */
void RAIDController::setDisk1(Disk *_disk1) {
    disk1 = _disk1;
}

/**
 * Getter de disk2 de RAIDController.
 * @return disk
 */
Disk *RAIDController::getDisk2() {
    return disk2;
}

/**
 * Setter de disk2 de RAIDController.
 * @param _disk2
 */
void RAIDController::setDisk2(Disk *_disk2) {
    disk2 = _disk2;
}

/**
 * Getter de disk3 de RAIDController.
 * @return disk
 */
Disk *RAIDController::getDisk3() {
    return disk3;
}

/**
 * Setter de disk3 de RAIDController.
 * @param _disk3
 */
void RAIDController::setDisk3(Disk *_disk3) {
    disk3 = _disk3;
}

/**
 * Getter de disk4 de RAIDController.
 * @return disk
 */
Disk *RAIDController::getDisk4() {
    return disk4;
}

/**
 * Setter de disk4 de RAIDController.
 * @param _disk4
 */
void RAIDController::setDisk4(Disk *_disk4) {
    disk4 = _disk4;
}

/**
 * Getter de actualSplit1 de RAIDController.
 * @return string
 */
string RAIDController::getActualSplit1() {
    return actualSplit1;
}

/**
 * Setter de actualSplit1 de RAIDController.
 * @param _actualSplit1
 */
void RAIDController::setActualSplit1(string _actualSplit1) {
    actualSplit1 = _actualSplit1;
}

/**
 * Getter de actualSplit2 de RAIDController.
 * @return string
 */
string RAIDController::getActualSplit2() {
    return actualSplit2;
}

/**
 * Setter de actualSplit2 de RAIDController.
 * @param _actualSplit2
 */
void RAIDController::setActualSplit2(string _actualSplit2) {
    actualSplit2 = _actualSplit2;
}

/**
 * Getter de actualSplit3 de RAIDController.
 * @return string
 */
string RAIDController::getActualSplit3() {
    return actualSplit3;
}

/**
 * Setter de actualSplit3 de RAIDController.
 * @param _actualSplit3
 */
void RAIDController::setActualSplit3(string _actualSplit3) {
    actualSplit3 = _actualSplit3;
}

/**
 * Getter de actualParity de RAIDController.
 * @return string
 */
string RAIDController::getActualParity() {
    return actualParity;
}

/**
 * Setter de actualParity de RAIDController.
 * @param _actualParity
 */
void RAIDController::setActualParity(string _actualParity) {
    actualParity = _actualParity;
}
