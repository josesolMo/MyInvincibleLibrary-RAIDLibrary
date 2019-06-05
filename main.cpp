
#include <iostream>

#include "RAIDController.h"

#include <fstream>
#include <iterator>
#include <vector>

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
int main() {
    cout << "MyInvincibleLibrary -> RAIDLibrary" << endl;

    int image[256][3]; // first number here is 1024 pixels in my image, 3 is for RGB values
    FILE *streamIn;
    streamIn = fopen("color.bmp", "r");
    if (streamIn == (FILE *)0){
        printf("File opening error ocurred. Exiting program.\n");
        exit(0);
    }

    int byte;
    int count = 0;
    for(int i=0;i<54;i++) byte = getc(streamIn);  // strip out BMP header

    for(int i=0;i<1024;i++){    // foreach pixel
        image[i][2] = getc(streamIn);  // use BMP 24bit with no alpha channel
        image[i][1] = getc(streamIn);  // BMP uses BGR but we want RGB, grab byte-by-byte
        image[i][0] = getc(streamIn);  // reverse-order array indexing fixes RGB issue...
        printf("pixel %d : [%d,%d,%d]\n",i+1,image[i][0],image[i][1],image[i][2]);
    }

    fclose(streamIn);


}
