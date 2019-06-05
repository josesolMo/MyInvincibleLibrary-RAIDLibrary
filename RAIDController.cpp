
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
    disk0 = new Disk(0);
    disk1 = new Disk(1);
    disk2 = new Disk(2);
    disk3 = new Disk(3);

    actualImage = nullptr;

    actualSplit1 = "";
    actualSplit2 = "";
    actualSplit3 = "";
    actualParity = "";

    parityDiskIndex = 0;

}


///Metodos


/**
 * Divide la imagen en [n cantidad] para ser ingresadas en los discos.
 * @param image
 */
void RAIDController::splitImage(Image *image) {

    //

}

/**
 * Cambia el parityDiskIndex para saber cual disk será el que continua guardando el
 * byte de paridad. Va en orden ascendente hasta llegar al 3, ahí se devuelve al 0.
 */
void RAIDController::nextParityDiskIndex() {

    if (parityDiskIndex == 3) {
        parityDiskIndex = 0;
    } else if (parityDiskIndex >= 0) {
        parityDiskIndex++;
    } else {
        cout << "ParityDiskIndex out of bounds: " << parityDiskIndex << endl;
    }

}


///Getters & Setters


/**
 * Getter de disk0 de RAIDController.
 * @return disk
 */
Disk *RAIDController::getDisk0() {
    return disk0;
}

/**
 * Setter de disk0 de RAIDController.
 * @param _disk0
 */
void RAIDController::setDisk0(Disk *_disk0) {
    disk0 = _disk0;
}

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
 * Getter de actualImage de RAIDController.
 * @return imagen
 */
Image* RAIDController::getActualImage() {
    return actualImage;
}

/**
 * Setter de actualImage de RAIDController.
 * @param _actualImage
 */
void RAIDController::setActualImage(Image *_actualImage) {

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

/**
 * Getter de parityDiskIndex de RAIDController.
 * @return index
 */
int RAIDController::getParityDiskIndex() {
    return parityDiskIndex;
}

/**
 * Setter de parityDiskIndex de RAIDController.
 * @param _parityDiskIndex
 */
void RAIDController::setParityDiskIndex(int _parityDiskIndex) {
    parityDiskIndex = _parityDiskIndex;
}

