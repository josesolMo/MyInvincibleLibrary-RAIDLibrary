
#include <iostream>

#include "RAIDController.h"

#include <fstream>
#include <cstdlib>
#include <vector>

#include <stdio.h>

using namespace std;


/**
 * Archivo Main de RAIDLibrary.
 *
 * @since 02/06/19
 */


///Instancia estática de RAIDController
static RAIDController* raidController;



/**
 * Main del Programa.
 */
int main(int argc, char **argv) {
    cout << "MyInvincibleLibrary -> RAIDLibrary" << endl;

    raidController = new RAIDController();


    Image* tImage = new Image("coachella");
    //tImage->testImage();
    tImage->toBinary();
    tImage->toBmp();







    ////////////////////////////////////////////////////////////////////
    ///                                                              ///
    ///                       xxxxxxxxxxxxxxxx                       ///
    ///                                                              ///
    ////////////////////////////////////////////////////////////////////

}
