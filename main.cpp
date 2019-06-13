
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

#include "RAIDController.h"

#define PORT 3550
#define BACKLOG 4
#define MAXDATASIZE 1000

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






string checkAvailability(string name, string binaryData) {

    bool isAvailable = raidController->isAvailable(name);

    if (isAvailable) {

        ///Se crea un objeto con el nombre y el binaryData de la nueva imagen
        Image* newImage = new Image(name, binaryData);

        ///Se envia la imagen al raidController para que sea guardada en discos
        bool saved = raidController->saveImage(newImage);

        ///Si no se guarda
        if (!saved) {
            cerr << "Error in checkAvailability()" << endl;
            isAvailable = false;
        }

    }

    ///Preparacion del JSON con el resultado de la disponibilidad
    ///del nombre de la imagen por agregar.

    json_object *jobjAvailability = json_object_new_object();
    json_object *jstringAvailability = json_object_new_string(to_string(isAvailable).c_str()); /// "1" o "0"
    json_object_object_add(jobjAvailability,"AVAILABILITY", jstringAvailability);
    return json_object_to_json_string(jobjAvailability);


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



            ///KEY: FILENAME
            ///Obtiene el nombre del archivo por guardar u obtener.
            struct json_object *tempFileName;
            cout<<"FileName"<<endl;
            json_object *parsed_jsonFileName = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonFileName, "FILENAME", &tempFileName);

            ///KEY: BINARYDATA
            ///Obtiene el binary data de una imagen completa para que sea guardada.
            struct json_object *tempBinaryData;
            cout<<"BinaryData"<<endl;
            json_object *parsed_jsonBinaryData = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonBinaryData, "BINARYDATA", &tempBinaryData);



            /*
            ///KEY: TEMPLATE
            ///Obtiene un request para
            struct json_object *tempZZ;
            cout<<"ZZ"<<endl;
            json_object *parsed_jsonZZ = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonZZ, "ZZ", &tempZZ);
             */



            ///JSON Writes



            ///Obtendra un request para guardar una imagen en el RAID
            ///Verifica que reciba los KEYS: "FILENAME" & "BINARYDATA"
            ///Envia keys: "AVAILABILITY"
            if (json_object_get_string(tempFileName) != nullptr && json_object_get_string(tempBinaryData) != nullptr) {
                ///JSON saliente del servidor
                string confirmation = checkAvailability(json_object_get_string(tempFileName), json_object_get_string(tempBinaryData));
                ///Envio al cliente
                send(fd2, confirmation.c_str(), MAXDATASIZE, 0);
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
             "END--------------------------------------------------------------------------------\n"
             << endl;

    }

    close(fd2);

}






/**
 * Main del Programa.
 */
int main() {

    cout << "MyInvincibleLibrary -> RAIDLibrary" << endl;

    raidController = new RAIDController();

    ///Corre el servidor
    //runServer();



    /////////////////////////////////////////////////////////////
    ///                         TESTS                         ///
    /////////////////////////////////////////////////////////////



    ///Test Image Directory
    string actualImageName = "coachella";
    string actualDirectory = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/Media/" + actualImageName + ".bmp";

    ///Test Image BinaryData
    string actualBinaryData = BMPtoBinaryData(actualDirectory);

    cout << actualBinaryData << endl;

    ///Test del recibimiento del JSON
    //checkAvailability(actualImageName, actualBinaryData);


}
