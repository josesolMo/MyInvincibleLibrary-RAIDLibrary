
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
 * Hace una lectura de los bytes de la imagen .bmp y guarda cada uno, en binario.
 * @return bitString
 */
string BMPtoHexData(string directory) {

    ///restauracion de binaryData

    ///Variables para el recorrido
    string binaryData = "";
    int byteValue;
    int index = 0;

    ///Para abrir la imagen
    FILE *file;
    file = fopen(directory.c_str(), "rb");


    if (file != NULL) {

        while (byteValue != EOF) {
            if (index >= 0) {

                byteValue = fgetc(file);

                binaryData += decimalToHex(byteValue);

            }
            index++;
        }

        fclose(file);
        //
    } else {

        printf("\nFile not found.");

        return "";

    }

    return binaryData;

}





/**
 * Hace una lectura de los bytes de la imagen .bmp y guarda cada uno, en hexadecimal.
 * @return bitString
 */

/*
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

}*/





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
 *
 * @param name
 * @param key
 * @return
 */
string raidRead(string name, string key) {

    ///Se envia la imagen al raidController para que sea guardada en discos
    bool isWritten = raidController->read(name);

    ///Preparacion del JSON por enviar
    json_object *jobjAvailability = json_object_new_object();
    json_object *jstringAvailability = json_object_new_string( to_string( isWritten ).c_str() ); /// "1"
    json_object_object_add(jobjAvailability, key.c_str(), jstringAvailability);
    return json_object_to_json_string(jobjAvailability);

}


/**
 * Guardara una imagen en el RAID proveniente del cliente.
 * @param name
 * @param hexData
 * @param key
 * @return
 */
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
 * @param selectImage
 * @param keySelectImage
 * @return
 */
string sendSelectImage(string selectImage, string keySelectImage) {

    ///Se envia la imagen al raidController para que sea guardada en discos
    bool isWritten = raidController->read(selectImage);

    string hexData = raidController->seek(selectImage);

    ///Preparacion del JSON por enviar
    json_object *jobjSelectImage = json_object_new_object();
    json_object *jstringSelectImage = json_object_new_string( hexData.c_str() ); /// "Informacion hex de imagen"
    json_object_object_add(jobjSelectImage, keySelectImage.c_str(), jstringSelectImage);
    return json_object_to_json_string(jobjSelectImage);

}



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

        unsigned address_size = sizeof(client);

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


            ///KEY: NEWIMAGE
            ///Obtiene el nombre de la nueva imagen para verificar si puede ser guardada.
            struct json_object *tempNewImage;
            json_object *parsed_jsonNewImage = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewImage, "NEWIMAGE", &tempNewImage);

            ///KEY: BINARYDATA
            ///Obtiene un request para obtener el binaryData de la imagen por
            struct json_object *tempBinaryData;
            json_object *parsed_jsonBinaryData = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonBinaryData, "BINARYDATA", &tempBinaryData);

            ///KEY: SELECTIMAGE
            ///Obtiene un request para
            struct json_object *tempSelectImage;
            json_object *parsed_jsonSelectImage = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonSelectImage, "SELECTIMAGE", &tempSelectImage);


            /*
            ///KEY: TEMPLATE
            ///Obtiene un request para
            struct json_object *tempZZ;
            cout<<"ZZ"<<endl;
            json_object *parsed_jsonZZ = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonZZ, "ZZ", &tempZZ);
             */



            ///JSON Writes


            ///Obtendra un request para obtener la disponibilidad de la nueva imagen.
            ///Verifica que reciba los KEYS: NEWIMAGE & BINARYDATA
            if (json_object_get_string(tempNewImage) != nullptr &
                json_object_get_string(tempBinaryData) != nullptr) {
                ///JSON saliente del servidor
                string newImage = raidWrite(json_object_get_string(tempNewImage),
                                            json_object_get_string(tempBinaryData), "NEWIMAGE");
                ///Envio al cliente
                send(fd2, newImage.c_str(), MAXDATASIZE, 0);
            }

            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: SELECTIMAGE
            if (json_object_get_string(tempSelectImage) != nullptr ) {
                ///JSON saliente del servidor
                string selectImage = sendSelectImage(json_object_get_string(tempSelectImage), "SELECTIMAGE");
                ///Envio al cliente
                send(fd2, selectImage.c_str(), MAXDATASIZE, 0);
            }

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


/**
 * Main del Programa.
 */
int main() {

    ///Instanciacion del RAIDController
    raidController = new RAIDController();

    ///Corre el servidor
    runServer();



    /////////////////////////////////////////////////////////////
    ///                         TESTS                         ///
    /////////////////////////////////////////////////////////////

/*
    string name1 = "bob.bmp";
    string directory1 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/Media/" + name1;
    string hexData1 = BMPtoHexData(directory1);

    string testWrite1 = raidWrite( name1,  hexData1, "KEY" );



    string testRead = raidRead( name1, "KEY" );


    string testSeek = sendSelectImage( name1, "KEY" );

    cout << "TESTSEEK: " << testSeek << endl;
*/



    /*
    string name2 = "sea.bmp";
    string directory2 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/Media/" + name2;
    string hexData2 = BMPtoHexData(directory2);

    string testWrite2 = raidWrite( name2,  hexData2, "KEY");
*/


}
