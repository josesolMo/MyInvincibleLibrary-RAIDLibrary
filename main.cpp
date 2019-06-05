
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



/*
int FileExists( char *filename)
{
    ifstream inFile (filename);
    if (inFile.fail())
        return 0;
    else
        return 1;
    cout << "Function fileexists encounted a line of code that should ";
    cout << "have been impossible to reach.\n \n";
}
*/





/**
 * Main del Programa.
 */
int main(int argc, char **argv) {
    cout << "MyInvincibleLibrary -> RAIDLibrary" << endl;

    raidController = new RAIDController();




    ////////////////////////////////////////////////////////////////////
    ///                                                              ///
    ///                       File input test                        ///
    ///                                                              ///
    ////////////////////////////////////////////////////////////////////


    /*char data[1000];

    ifstream image;

    string directory = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/";

    string filename = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/color.bmp";



   ifstream imagex( filename.c_str(), ios::binary | ios::in); //color.bmp   CMakeLists.txt

    if (!imagex) {
        cout << "Did not open" << endl;
    } else {
        cout << "Success" << endl;
    }


    // get size of file
    imagex.seekg(0,ifstream::end);
    long size = imagex.tellg();
    imagex.seekg(0);

    cout << "Length: " << size << endl;


    imagex.getline(data,4);
    //imagex.getline(data,2);

    for (int n = 0; n < 6; n++) {
        cout << data[n] << "";
    }

    image.close();*/

    ////////////////////////////////////////////////////////////////////

   /* char filename[50];
    ifstream image;

    cin.getline(filename,50);

    image.open(filename);

    if (!image.is_open()) {
        cout << "Could not open" << endl;

        exit(EXIT_FAILURE);
    } else {
        cout << "Success" << endl;

        char word[50];
        image >> word;

        while(image.good()) {
            cout << word << " ";
            image >> word;
        }

    }*/

    ////////////////////////////////////////////////////////////////////


       /* std::ifstream fileStream("/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/color.bmp");
        double trainsample;
        if(!fileStream.good()){
            std::cerr << "Could not open file." << std::endl;
            return 1;
        }
        while(fileStream >> trainsample){
            std::cout << trainsample << std::endl;
        }
        if(fileStream.fail())
            std::cerr << "Input file stream error bit is set, possible read error on file." << std::endl;

*/

    ////////////////////////////////////////////////////////////////////


    unsigned char image_data[54];
    string fileName = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/landscape.bmp";

    int bytevalue;
    int index = 0;

    FILE *file;
    file = fopen(fileName.c_str(), "rb");

    if (file != NULL) {
        while (bytevalue != EOF) {
            if (index == 54) {
                break;
            } else {
                bytevalue = fgetc(file);
                image_data[index] = bytevalue;

                index++;
            }
        }
    } else {
        for (int i = 0; i < 29; i++) {
            image_data[i]='\0';
        }
    }

    fclose(file);




    int image_width = image_data[20-1] * 256 + image_data[19-1];
    int image_height = image_data[24-1] * 256 + image_data[23-1];
    int image_bits = image_data[29-1];

    //cout << image_width << "," << image_height << "," << image_bits ;

    printf("\nImage width: %d \n", image_width);
    printf("Image height: %d \n", image_height);

    printf("%d bit image\n", image_bits);

}
