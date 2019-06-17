
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <cstring>
#include <json-c/json.h>
#include <sstream>

#include "RAIDController.h"

#define PORT 3550
#define BACKLOG 4
#define MAXDATASIZE 999999

using namespace std;


/**
 * Archivo Main de RAIDLibrary.
 *
 * @since 02/06/19
 */


///Instancia estática de RAIDController
static RAIDController* raidController;

///Instancia estática del contenedor del BinaryData
static string binaryData;



/**
 * Convierte un numero decimal a hexadecimal.
 * @param d
 * @return h
 */
string decimalToHex(int d) {

    stringstream ss;
    ss<< hex << d;
    string h ( ss.str() );

    if (h.length() < 2) {
        h = "0" + h;
    }

    return h;

}

/**
 * Convierte un numero hexadecimal a decimal.
 * @param h
 * @return d
 */
int hexToDecimal(string h) {

    unsigned int d;

    h = "0x" + h;

    stringstream ss;
    ss << std::hex << h;
    ss >> d;

    return d;
}




/**
 * Convierte un numero decimal a binario.
 * @param d
 * @return binary
 */
string decimalToBinary(int d) {

    int bin = 0;
    int i = 1;

    while(d > 0) {

        bin += (d % 2) * i;
        d /= 2;
        i *= 10;

    }

    ///Lo convierte a string
    string b = to_string(bin);

    int len = b.length();

    ///Para hacer que siempre tengan 8 digitos
    if (len != 8) {
        int z = 8 - len;
        while (z > 0) {
            b = "0" + b;
            z--;
        }
    }

    //cout << b << endl;

    return b;

}


string decimalData(string directory) {

    ///Contenedor del DecimalData
    string decimalData;

    ///Variables para el recorrido
    int byteValue;
    int index = 0;

    ///Para abrir la imagen
    FILE *file;
    file = fopen(directory.c_str(), "rb");

    if (file != NULL) {

        while (byteValue != EOF) {
            if (index >= 0) {

                byteValue = fgetc(file);

                //cout << byteValue << endl;

                decimalData += to_string(byteValue) + " ";

            }
            index++;
        }

        fclose(file);

    } else {

        printf("\nFile not found.");

        return "";

    }

    cout << "\ndecimalData: " << decimalData << endl;
    cout << "Length: " << decimalData.length() << endl;
    cout << "Index: " << index << endl;

    return decimalData;


}





/**
 * Hace una lectura de los bytes de la imagen .bmp y guarda cada uno, en hexadecimal.
 * @return bitString
 */
string BMPtoHexData(string directory) {

    ///Contenedor del HexData
    string hexData;

    ///Variables para el recorrido
    int byteValue;
    int index = 0;

    ///Para abrir la imagen
    FILE *file;
    file = fopen(directory.c_str(), "rb");


    if (file != NULL) {

        while (byteValue != EOF) {
            if (index >= 0) {

                byteValue = fgetc(file);

                //binaryData += std::hex << byteValue;

            }
            index++;
        }

        fclose(file);

    } else {

        printf("\nFile not found.");

        return "";

    }

    cout << "\nrawBinString: " << endl;
    cout << "Length: " << binaryData.length() << endl;

    return binaryData;

}







/**
 * Hace una lectura de los bytes de la imagen .bmp y guarda cada uno, en binario.
 * @return bitString
 */
string BMPtoBinaryData(string directory) {

    ///Contenedor del BinaryData
    string binaryData;

    ///Variables para el recorrido
    int byteValue;
    int index = 0;

    ///Para abrir la imagen
    FILE *file;
    file = fopen(directory.c_str(), "rb");


    if (file != NULL) {

        while (byteValue != EOF) {
            if (index >= 0) {

                byteValue = fgetc(file);

                binaryData += decimalToBinary(byteValue);

            }
            index++;
        }

        fclose(file);

    } else {

        printf("\nFile not found.");

        return "";

    }

    cout << "\nrawBinString: " << endl;
    cout << "Length: " << binaryData.length() << endl;

    return binaryData;

}


string raidWrite(string name, string hexData, string key) {

    ///Se crea un objeto con el nombre y el hexData de la nueva imagen
    Image* newImage = new Image(name, hexData);

    ///Se envia la imagen al raidController para que sea guardada en discos
    bool isWritten = raidController->write(newImage);





    ///Preparacion del JSON por enviar
    json_object *jobjAvailability = json_object_new_object();
    json_object *jstringAvailability = json_object_new_string( to_string( isWritten ).c_str() ); /// "1"
    json_object_object_add(jobjAvailability, key.c_str(), jstringAvailability);
    return json_object_to_json_string(jobjAvailability);


}


/**
 *
 * CAMBIAR POR "WRITE"
 *
 * @param name
 * @param binaryData
 * @return
 */
string checkAvailability(string name, string binaryData, string key) {

    ///Se crea un objeto con el nombre y el binaryData de la nueva imagen
    Image* newImage = new Image(name, binaryData);

    ///Se envia la imagen al raidController para que sea guardada en discos
    bool saved = raidController->saveImage(newImage);


    ///Cuando ya ha sido guardada
    bool isWritten = true;


    ///Preparacion del JSON por enviar
    json_object *jobjAvailability = json_object_new_object();
    json_object *jstringAvailability = json_object_new_string( to_string(isWritten ).c_str()); /// "1"
    json_object_object_add(jobjAvailability, key.c_str(), jstringAvailability);
    return json_object_to_json_string(jobjAvailability);

}


////////////////////////////////////test/////////////////////////////////////////////////



/**
 * Envia un JSON a la base de datos para verificar la disponibilidad de el nombre de la galeria entrante.
 * @param newGallery
 * @param newGalleryKey
 * @return JSON
 */
string sendNewGallery(string newGallery, string newGalleryKey) {
        json_object *jobjError = json_object_new_object();
        json_object *jstringError = json_object_new_string("1");
        json_object_object_add(jobjError, "NEWGALLERY", jstringError);
        return json_object_to_json_string(jobjError);
}



////////////////////////////////////test/////////////////////////////////////////////////





/**
 * Corre el servidor
 * @return int
 */
int runServer() {

    int fd, fd2;

    struct sockaddr_in server;

    struct sockaddr_in client;


    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("error en socket()\n");
        exit(-1);
    }

    server.sin_family = AF_INET;

    server.sin_port = htons(PORT);

    server.sin_addr.s_addr = INADDR_ANY;

    bzero(&(server.sin_zero), 8);

    if (bind(fd, (struct sockaddr *) &server,
             sizeof(struct sockaddr)) == -1) {
        printf("error en bind() \n");
        exit(-1);
    }

    if (listen(fd, BACKLOG) == -1) {
        printf("error en listen()\n");
        exit(-1);
    }

    printf("\nServidor 'RAIDLibrary' abierto.\n");

    while (true) {

        unsigned int address_size = sizeof(client);

        if ((fd2 = accept(fd, (struct sockaddr *) &client,
                          &address_size)) == -1) {
            printf("error en accept()\n");
            exit(-1);
        }

        printf("\nSe obtuvo una conexión de un cliente.\n");

        ssize_t r;

        char buff[MAXDATASIZE];

        for (;;) {
            r = read(fd2, buff, MAXDATASIZE);

            if (r == -1) {
                perror("read");
                return EXIT_FAILURE;
            }
            if (r == 0)
                break;
            printf("\nREAD: %s\n", buff);



            ///JSON Reads

/*

            ///KEY: NEWIMAGE
            ///Obtiene el name de la nueva imagen para verificar si puede ser guardada.
            struct json_object *tempNewImage;
            json_object *parsed_jsonNewImage = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewImage, "NEWIMAGE", &tempNewImage);

            ///KEY: BINARYDATA
            ///Obtiene el binary data de una imagen completa para que sea guardada.
            struct json_object *tempBinaryData;
            json_object *parsed_jsonBinaryData = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonBinaryData, "BINARYDATA", &tempBinaryData);
           */



            /*
            ///KEY: TEMPLATE
            ///Obtiene un request para
            struct json_object *tempZZ;
            cout<<"ZZ"<<endl;
            json_object *parsed_jsonZZ = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonZZ, "ZZ", &tempZZ);
             */



            ///JSON Writes


/*
            ///Obtendra un request para obtener la disponibilidad de la nueva imagen.
            ///Verifica que reciba los KEYS: NEWIMAGE & BINARYDATA
            if (json_object_get_string(tempNewImage) != nullptr && json_object_get_string(tempBinaryData) != nullptr) {
                ///JSON saliente del servidor
                string newImage = checkAvailability("NEWIMAGE", json_object_get_string(tempNewImage), json_object_get_string(tempBinaryData));
                ///Envio al cliente
                send(fd2, newImage.c_str(), MAXDATASIZE, 0);
            }
*/



            /*
            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: TEMPLATE
            if (json_object_get_string(tempZZ) != nullptr ) {
                ///JSON saliente del servidor
                string aTypeZZ = sendAType("ZZ",json_object_get_string(tempZZ));
                ///Envio al cliente
                send(fd2, aTypeZZ.c_str(), MAXDATASIZE, 0);
            }
             */










            ////////////////////////////////TESTKEYS/////////////////////////////////

            ///KEY: NEWGALLERY
            ///Obtiene el nombre de la nueva galeria para verificar si puede ser guardada.
            struct json_object *tempNewGallery;
            json_object *parsed_jsonNewGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewGallery, "NEWGALLERY", &tempNewGallery);

            ///KEY: NEWIMAGE
            ///Obtiene el nombre de la nueva imagen para verificar si puede ser guardada.
            struct json_object *tempNewImage;
            json_object *parsed_jsonNewImage = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewImage, "NEWIMAGE", &tempNewImage);

            ///KEY: GALLERY
            ///Obtiene la galeria donde se cambiara o se retornara su contenido
            struct json_object *tempGallery;
            json_object *parsed_jsonGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonGallery, "GALLERY", &tempGallery);

            ///KEY: BINARYDATA
            ///Obtiene un request para obtener el binaryData de la imagen por
            struct json_object *tempBinaryData;
            json_object *parsed_jsonBinaryData = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonBinaryData, "BINARYDATA", &tempBinaryData);

            ////////////////////////////////TESTKEYS/////////////////////////////////

            ///Obtendra un request para obtener la disponibilidad de la galeria.
            ///Verifica que reciba los KEYS: NEWGALLERY
            if (json_object_get_string(tempNewGallery) != nullptr ) {
                ///JSON saliente del servidor
                string newGallery = sendNewGallery(json_object_get_string(tempNewGallery), "NEWGALLERY");
                ///Envio al cliente
                send(fd2, newGallery.c_str(), MAXDATASIZE, 0);
            }


            ///Obtendra un request para obtener la disponibilidad de la nueva imagen.
            ///Verifica que reciba los KEYS: NEWIMAGE, GALLERY & BINARYDATA
            if (json_object_get_string(tempNewImage) != nullptr &
                json_object_get_string(tempBinaryData) != nullptr) {

                cout << "Image recieved" << endl;

                ///JSON saliente del servidor
                string newImage = raidWrite(json_object_get_string(tempNewImage),
                        json_object_get_string(tempBinaryData), "NEWIMAGE");
                ///Envio al cliente
                send(fd2, newImage.c_str(), MAXDATASIZE, 0);
            }



        }

        ///Reestablece el buffer
        memset(buff, 0, MAXDATASIZE);

        cout <<
             "\n\n--------------------------------------------------------------------------------"
             "END-------------------------------------------------------------------------------\n"
             << endl;

    }

    close(fd2);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * Convierte un numero decimal a hexadecimal.
 * @param d
 * @return h
 */
string decimalToHexT(int d) {

    if (d == -1) {
        cout << "\n\nEOF\n\n" << endl;
        return "";
    }

    stringstream ss;
    ss<< hex << d;
    string h ( ss.str() );

    if (h.length() < 2) {
        h = "0" + h;
    }

    return h;

}


/**
 * Hace una lectura de los bytes de la imagen .bmp y guarda cada uno, en binario.
 * @return bitString
 */
string BMPtoBinaryDataT(string directory) {

    ///restauracion de binaryData

    ///Variables para el recorrido
    int byteValue;
    int index = 0;

    ///Para abrir la imagen
    FILE *file;
    file = fopen(directory.c_str(), "rb");


    if (file != NULL) {

        while (byteValue != EOF) {
            if (index >= 0) {

                byteValue = fgetc(file);

                //binaryData += decimalToBinary(byteValue);

                binaryData += decimalToHex(byteValue);

            }
            index++;
        }

        fclose(file);

    } else {

        printf("\nFile not found.");

        return "";

    }

    return binaryData;

}



/////////////////////////////////////////////////////////////////////////////////////////////////////






/**
 * Main del Programa.
 */
int main() {

    //cout << "MyInvincibleLibrary -> RAIDLibrary" << endl;

    raidController = new RAIDController();

    ///Corre el servidor
    //runServer();



    /////////////////////////////////////////////////////////////
    ///                         TESTS                         ///
    /////////////////////////////////////////////////////////////


    string name1 = "alps.bmp";
    string directory1 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/Media/" + name1;
    string hexData1 = BMPtoBinaryDataT(directory1);

    raidWrite( name1,  hexData1, "KEY");

/*
    string name2 = "batman.bmp";
    string directory2 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/Media/" + name2;
    string hexData2 = BMPtoBinaryDataT(directory2);

    raidWrite( name2,  hexData2, "KEY");

*/






/*
    ///HexTests
    int dec = 15;
    cout << "\ndecimalToHex(" << dec << "): " << decimalToHex(dec) << endl;

    string hex = "f";
    cout << "\nhexToDecimal(" << hex << "): " << hexToDecimal(hex) << endl;
*/

/*
    ///Test Image Directory 1
    string actualImageName1 = "sea";
    string actualDirectory1 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/Media/" + actualImageName1 + ".bmp";

    ///Test decimalData
    decimalData(actualDirectory1);

    ///Test Image BinaryData
    string actualBinaryData1 = BMPtoBinaryData(actualDirectory1);

    cout << actualBinaryData1 << endl;

    ///Test del recibimiento del JSON
    checkAvailability(actualImageName1, actualBinaryData1);
*/



/*
    ///Test Image Directory 2
    string actualImageName2 = "game";
    string actualDirectory2 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/Media/" + actualImageName2 + ".bmp";

    ///Test Image BinaryData
    string actualBinaryData2 = BMPtoBinaryData(actualDirectory2);

    cout << actualBinaryData2 << endl;

    ///Test del recibimiento del JSON
    checkAvailability(actualImageName2, actualBinaryData2);
*/

}
