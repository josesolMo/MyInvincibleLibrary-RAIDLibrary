
#include "RAIDController.h"

#include <fstream>
#include <stdio.h>
#include <cmath>
#include <iomanip>




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

    actualImage = nullptr;

    TICDirectory = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/TemporalImageContainer/";
    DCDirectory = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/DisksContainer/";

    actualSplit1 = "";
    actualSplit2 = "";
    actualSplit3 = "";
    actualParity = "";

    parityDiskIndex = 0;
    imagePart1DiskIndex = 1;
    imagePart2DiskIndex = 2;
    imagePart3DiskIndex = 3;

}


///Metodos


/**
 * Guarda la imagen en los discos (RAID)
 * @return true, si es agregada
 */
bool RAIDController::write(Image* newImage) {

    ///Flag
    bool isWritten = true;

    ///Guarda la instancia de la nueva imagen como la imagen actual en el Controller.
    setActualImage(newImage);

    while (isWritten) {

        ///Convierte el hexData de la imagen y la guarda temporalmente en .
        isWritten = hexDataToBMP(actualImage->getNombre(), actualImage->getRawHexString());
        if (!isWritten) {
            break;
        }

        ///Divide la imagen en tres y las guarda en diferentes discos.
        isWritten = split();
        if (!isWritten) {
            break;
        }

        ///Pasa de .bmp a su BinaryData para calcular su paridad luego.
        isWritten = brokenBinary();
        if (!isWritten) {
            break;
        }

        ///Calcula el bit de paridad con XOR entre los BinaryData de las imagenes divididas.
        isWritten = XORParity();
        if (!isWritten) {
            break;
        }

        ///Calcula el siguiente disco por guardar la paridad
        nextParityDiskIndex();


        break;
    }

    ///Se reestablecen las variables luego de su uso
    actualImage = nullptr;
    actualSplit1 = "";
    actualSplit2 = "";
    actualSplit3 = "";
    actualParity = "";

    return isWritten;
}


/**
 * Toma el hexData de la imagen y la convierte a .bmp.
 * Es guardada en el folder "TemporalImageContainer".
 * @return bool
 */
bool RAIDController::hexDataToBMP(string name, string hexData) {

    ///Variables para la conversion
    int byteValue;
    int index = 0;
    string actualByte;

    cout << hexData << endl;

    float totalByteLength = hexData.length() / 2;

    //actualImage->setByteQuantity(totalByteLength);

    cout << "totalByteLength: " << totalByteLength << endl;

    ///Archivo nuevo por crear y escribir
    string newFileName = TICDirectory + "fH_" + name;
    FILE* newFile = fopen(newFileName.c_str(),"a");

    ///Guarda el archivo donde se encuentra temporalmente luego de haber sido reconstruida
    ///al provenir del cliente.
    actualImage->setTempDirectory(newFileName);

    ///Recorrera el binString hasta que desaparezca
    while (hexData.length() > 0) {

        if (true) {

            if (hexData.length() == 8) {

                ///Elimina los ultimos bits del bitString
                hexData.erase(0, 2);

                ///Ingresa EOF al final del nuevo archivo
                fputc(EOF, newFile);

                //cout << index << ": " << EOF << endl;

            } else {



                ///Lectura del bitString

                ///Toma los primeros 8 bits del bitString
                actualByte = hexData.substr(0, 2);

                ///Elimina esos bits del bitString
                hexData.erase(0, 2);

                ///Convierte el byte (2 bits) a decimal
                byteValue = hexToDecimal(actualByte);

                ///Escribe el valor al nuevo archivo
                fputc(byteValue, newFile);

                //cout << index << ": " << byteValue << endl;

            }

        }

        index++;

        ///Muestra el porcentaje completado de la conversion
        if (index % 1000 == 0) {

            float percentageOfCompletition = index/(totalByteLength) * 100;
            cout << "Recreation Completed: " << fixed << setprecision(2) << percentageOfCompletition << " %" << endl;

        }

    }

    cout << "Recreation Completed: " << 100.00 << "%" << endl;
    cout << "\nRecreation complete: " + newFileName + " created.\n" << endl;

    fclose(newFile);

    return true;
}


/**
 * Convierte un numero hexadecimal a decimal.
 * @param h
 * @return d
 */
int RAIDController::hexToDecimal(string h) {

    unsigned int d;

    h = "0x" + h;

    stringstream ss;
    ss << std::hex << h;
    ss >> d;

    return d;
}


/**
 * Parte la imagen en tres pedazos, en tres archivos diferentes.
 */
bool RAIDController::split() {

    string name = actualImage->getNombre();

    ///Se obtiene el disco en el cual se guardara cada parte
    string parityDisk = to_string(parityDiskIndex);
    string image1disk = to_string(imagePart1DiskIndex);
    string image2disk = to_string(imagePart2DiskIndex);
    string image3disk = to_string(imagePart3DiskIndex);

    ///Creacion del nombre de los archivos nuevos que separaran la imagen
    string image_1of3 = DCDirectory + "Disk" + image1disk + "/1_" + name;
    string image_2of3 = DCDirectory + "Disk" + image2disk + "/2_" + name;
    string image_3of3 = DCDirectory + "Disk" + image3disk + "/3_" + name;

    ///Creacion en folder "Disks Container/DiskN" los archivos nuevos
    FILE* file_1of3 = fopen(image_1of3.c_str(),"a");
    FILE* file_2of3 = fopen(image_2of3.c_str(),"a");
    FILE* file_3of3 = fopen(image_3of3.c_str(),"a");

    ///Tamaño del commonData
    int commonDataLength = 138;

    ///Tamaño total de la imagen
    int fileLength = getActualImageFileLength();
    ///Tamaño total de la imagen sin el CommonData (138)
    int fileLengthWOC = fileLength - commonDataLength;

    cout << "fileLengthWOC: " << fileLengthWOC << endl;

    ///Arrays para guardar la informacion de la imagen
    int commonDataTotal[commonDataLength];
    int pixelDataTotal[fileLengthWOC];

    ///Variables para el recorrido
    int byteValue;
    int index = 0;

    ///Para abrir la imagen original
    string directory = TICDirectory + "fH_" + name;
    FILE *tempFile;
    tempFile = fopen(directory.c_str(), "rb");

    ///Recorrido para obtener el commonData y el PixelData
    if (tempFile != nullptr) {
        while (byteValue != EOF) {
            byteValue = fgetc(tempFile);

            ///Para guardar el CommonData
            if (index < 138) {
                commonDataTotal[index] = byteValue;
            }
            ///Para guardar el PixelData
            else {
                pixelDataTotal[index - commonDataLength] = byteValue;
                //cout << "pixelDataTotal: " << index - commonDataLength << " -> " << pixelDataTotal[index - commonDataLength] << endl;
            }

            ///Incrementa el indice
            index++;
        }

        ///Cierra el file al terminar de leer
        fclose(tempFile);

    } else {

        printf("\nFile not found.");
        return false;

    }

    ///Escribira el commonData de la imagen original en las tres divisiones
    for (int i = 0; i < 138; i++) {

        ///Escritura al nuevo archivo
        fputc(commonDataTotal[i], file_1of3);
        fputc(commonDataTotal[i], file_2of3);
        fputc(commonDataTotal[i], file_3of3);

        //cout << i << "-> " << "1: " << commonDataTotal[i] << " 2: " << commonDataTotal[i] << " 3: " << commonDataTotal[i] << endl;

    }

    ///Calculara una aproximacion cercana a una igualdad en la particion de pixeles de las nuevas imagenes
    if ( (fileLength % 3) == 1) {
        //fileLength += 2;
    } else if ( (fileLength % 3) == 2) {
        //fileLength += 1;
    }

    cout << "fileLength: " << fileLength << ".   %3: " << (fileLength % 3) << endl;

    ///Size solo del tercio de la imagen
    int aFileThird = fileLength / 3;

    cout << "aFileThird: " << aFileThird << endl;


    for (int i = 0; i < fileLength; i++) {

        ///Para la primera imagen: La parte central y de mas abajo seran blancas.
        if (i < aFileThird) {

            fputc(pixelDataTotal[i], file_1of3);
            fputc(255, file_2of3);
            fputc(255, file_3of3);

        }
        ///Para la segunda imagen: La parte de mas arriba y de mas abajo seran blancas.
        else if (i < aFileThird*2) {

            fputc(255, file_1of3);
            fputc(pixelDataTotal[i], file_2of3);
            fputc(255, file_3of3);

        }
        ///Para la tercera imagen: La parte de mas arriba y central seran blancas.
        else {

            fputc(255, file_1of3);
            fputc(255, file_2of3);
            fputc(pixelDataTotal[i], file_3of3);

            //cout << i << "-> " << "1: " << 255 << " 2: " << 255 << " 3: " << pixelDataTotal[i] << endl;

        }

    }

    fputc(EOF, file_1of3);
    fputc(EOF, file_2of3);
    fputc(EOF, file_3of3);

    fclose(file_1of3);
    fclose(file_2of3);
    fclose(file_3of3);

    //delete(file_1of3);
    //delete(file_2of3);
    //delete(file_3of3);

    return true;
}


/**
 * Muestra el tamaño del archivo (cantidad de bytes)
 * @return int
 */
int RAIDController::getActualImageFileLength() {

    ///Nombre del archivo
    string name = actualImage->getNombre();

    ///Para abrir la imagen
    string directory = TICDirectory + "fH_" + name;
    ifstream image( directory.c_str(), ios::binary | ios::in);

    if (!image) {
        cout << "Could not open. (getActualImageFileLength)" << endl;
    } else {
        //cout << "Success." << endl;
    }


    ///Toma el tama;o del archivo
    image.seekg(0,ifstream::end);
    long size = image.tellg();
    image.seekg(0);

    cout << "\nFile Length: " << size << endl;

    image.close();

    return size;

}


/**
 * Tomara la imagen dividida y guardada por separado en discos para convertirse en binaryData cada una.
 * Esto tiene el fin de poder calcular el bit de paridad que se guarda en el disco sobrante.
 */
bool RAIDController::brokenBinary() {

    actualSplit1 = BMPtoBinaryData(to_string(imagePart1DiskIndex),"1");
    //cout << "actualSplit1: " + actualSplit1 << endl;

    actualSplit2 = BMPtoBinaryData(to_string(imagePart2DiskIndex),"2");
    //cout << "actualSplit2: " + actualSplit2 << endl;

    actualSplit3 = BMPtoBinaryData(to_string(imagePart3DiskIndex),"3");
    //cout << "actualSplit3: " + actualSplit3 << endl;

    return true;

}


/**
 * Hace una lectura de los bytes de la imagen .bmp y guarda cada uno, en binario.
 * @return bitString
 */
string RAIDController::BMPtoBinaryData(string disk, string num) {

    ///Nombre de la imagen
    string name = actualImage->getNombre();

    ///Contenedor del BinaryData
    string binaryData;

    ///Variables para el recorrido
    int byteValue;
    int index = 0;

    string directory;

    if (disk == "-1") {
        directory = TICDirectory + "fB_" + name;
    } else {
        name = num + "_" + name;
        directory = DCDirectory + "Disk" + disk + "/" + name;
    }

    cout << "\n\nDirectory: " << directory << endl;

    ///Para abrir la imagen
    FILE *file;
    file = fopen(directory.c_str(), "rb");


    if (file != NULL) {

        while (byteValue != EOF) {
            if (index >= 0) {

                ///Value obtenido de la imagen
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

    // ///Asigna el binaryData al atributo propio de la clase
    //actualImage->setRawHexString(binaryData);

    cout << "rawBinString(" << name << "): " << endl;
    //cout << "Length: " << actualImage->getRawHexString().length() << endl;

    return binaryData;

}


/**
 * Genera el bit de paridad de las imagenes para que exista redundancia en el RAID.
 */
bool RAIDController::XORParity() {

    ///BinaryData's temporales para calcular la paridad
    string aS1 = actualSplit1;
    string aS2 = actualSplit2;
    string aS3 = actualSplit3;

    ///String del nombre de la imagen
    string name = actualImage->getNombre();

    string temporalDirectory = TICDirectory + "fH_" + name;

    ///Toma solo el nombre, sin ".bmp"
    name.resize( name.size() - 4 ) ;

    string savingDirectory = DCDirectory + "Disk" + to_string(parityDiskIndex) + "/parity_" + name + ".txt";

    ///Creacion en maquina de los archivos nuevos
    FILE* parityFile = fopen(savingDirectory.c_str(),"a");

    ///Variable donde guardar el resultado de la paridad actual
    string actualResult;
    int len = actualSplit1.length();

    //cout << "len: " << len << endl;

    ///Se empiezan a recorrer los binaryData's de los splits
    for (int i = 0; i < len; i++) {

        ///Compara inicialmente las primeras dos imagenes
        if (aS1.substr(0, 1) == aS2.substr(0, 1)) {
            actualResult = "0";
        } else {
            actualResult = "1";
        }

        //cout << "a: " << aS1.substr(0, 1) << " + " << aS2.substr(0, 1) << " = " << actualResult << endl;

        //cout << "c: " << aS3.substr(0, 1) << " + " << actualResult;

        ///Compara el resultado de la comparacion anterior con la tercera imagen
        if (actualResult == aS3.substr(0, 1)) {
            actualResult = "0";
        } else {
            actualResult = "1";
        }

        //cout << " = " << actualResult << endl;

        ///Lo agrega al archivo
        fputc(stoi(actualResult), parityFile);

        ///Se agrega a la variable
        actualParity += actualResult;

        ///Borra el numero ya comparado de los binaryData's temporales
        aS1.erase(0, 1);
        aS2.erase(0, 1);
        aS3.erase(0, 1);


        ///Muestra el porcentaje completado de la conversion
        if (i % 1000 == 0) {

            //cout << (i*100)/ len << endl;

            float percentageOfCompletition = (i*100)/ len;
            cout << "Parity Completed: " << fixed << setprecision(2) << percentageOfCompletition << " %" << endl;

        }


    }

    cout << "Parity Completed: " << 100.00 << "%" << endl;
    cout << "\nXOR: " + savingDirectory + " created.\n" << endl;

    ///Cierra el archivo abierto
    fclose(parityFile);

    ///Elimina la imagen entera ya que esta no se necesita en el RAID
    //remove(temporalDirectory.c_str());

    ///Tests de la paridad
    //verifyParity();

    return true;
}









void RAIDController::verifyParity() {

    ///Variables para el recorrido
    int byteValue = 0;
    int index = 0;



    ///String del nombre de la imagen
    string name = actualImage->getNombre();

    ///Toma solo el nombre, sin ".bmp"
    name.resize( name.size() - 4 ) ;

    string savedDirectory = DCDirectory + "Disk" + to_string(parityDiskIndex) + "/parity_" + name + ".txt";



    ///Para abrir la imagen original
    FILE *tempFile;
    tempFile = fopen(savedDirectory.c_str(), "rb");

    ///Recorrido para obtener el commonData y el PixelData
    if (tempFile != nullptr) {
        while (byteValue != EOF) {
            byteValue = fgetc(tempFile);

            //cout << "byteValue: " << byteValue << endl;

            ///Incrementa el indice
            index++;
        }

        ///Cierra el file al terminar de leer
        fclose(tempFile);

    } else {

        printf("\nFile not found.");

    }

    cout << actualSplit1 << "\n" << endl;
    cout << actualSplit2 << "\n" << endl;
    cout << actualSplit3 << "\n" << endl;
    cout << actualParity << "\n" << endl;

    cout << actualSplit1.length() << "\n" << endl;
    cout << actualSplit2.length() << "\n" << endl;
    cout << actualSplit3.length() << "\n" << endl;
    cout << actualParity.length() << "\n" << endl;

    fclose(tempFile);


}



/////////////////////////////////////////////////////READ////////////////////////////////////////////////////////////



/**
 *
 * @param name
 * @return
 */
bool RAIDController::read(string name) {

    joinBinary(name);

    return true;
}



/**
 *
 * @param name
 * @return
 */
bool RAIDController::joinBinary(string name) {

    string image1disk;
    string image2disk;
    string image3disk;

    for (int i = 0; i < 4; i++) {

        string tempDirectory = DCDirectory + "Disk" + to_string(i) + "/1_" + name;

        ///Para abrir la imagen original
        FILE *tempFile;
        tempFile = fopen(tempDirectory.c_str(), "rb");

        if (tempFile != nullptr) {
            cout << "Found: " << i << endl;

            fclose(tempFile);
            image1disk = to_string(i);
        }

    }

    if (image1disk == "1") {
        image2disk = "2";
        image3disk = "3";
    } else if (image1disk == "2") {
        image2disk = "3";
        image3disk = "0";
    } else if (image1disk == "3") {
        image2disk = "0";
        image3disk = "1";
    } else {
        image2disk = "1";
        image3disk = "2";
    }


    ///Tamaño total de la imagen
    int fileLength = getActualImageFileLength(image1disk, name);

    cout << "\n\n\n\nfileLength: " << fileLength << endl;


    ///Creacion del nombre de los archivos que poseen la imagen separada
    string image_1of3 = DCDirectory + "Disk" + image1disk + "/1_" + name;
    string image_2of3 = DCDirectory + "Disk" + image2disk + "/2_" + name;
    string image_3of3 = DCDirectory + "Disk" + image3disk + "/3_" + name;

    ///Creacion en folder "Disks Container/DiskN" los archivos nuevos
    FILE* file_1of3 = fopen(image_1of3.c_str(),"rb");
    FILE* file_2of3 = fopen(image_2of3.c_str(),"rb");
    FILE* file_3of3 = fopen(image_3of3.c_str(),"rb");



    ///Size solo del tercio de la imagen
    int aFileThird = fileLength / 3;

    cout << "aFileThird: " << aFileThird << endl;

    ///Variables para el recorrido
    int byteValue1;
    int byteValue2;
    int byteValue3;
    int index = 0;
    string actualbyte;

    ///Para crear la imagen original
    string directory = TICDirectory + "fB_" + name;
    FILE *originalFile;
    originalFile = fopen(directory.c_str(), "a");

    ///Primera parte por recorrer


    ///Recorrido para obtener el commonData y el PixelData
    if (file_1of3 != nullptr) {
        cout << "1index: " << index << endl;
        while (byteValue1 != EOF) {
            byteValue1 = fgetc(file_1of3);
            byteValue2 = fgetc(file_2of3);
            byteValue3 = fgetc(file_3of3);

            ///Para guardar el CommonData
            if (index < 138 + aFileThird) {

                ///Escribe el valor al nuevo archivo
                fputc(byteValue1, originalFile);

            }
                ///Para guardar el PixelData
            else {
                ///Incrementa el indice
                index++;
                break;
            }

            ///Incrementa el indice
            index++;
        }

        ///Cierra el file al terminar de leer
        fclose(file_1of3);

    } if (file_2of3 != nullptr) {

        cout << "2index: " << index << endl;
        while (byteValue2 != EOF) {
            //byteValue1 = fgetc(file_1of3);
            byteValue2 = fgetc(file_2of3);
            byteValue3 = fgetc(file_3of3);

            ///Para guardar el CommonData
            if (index < 138 + aFileThird*2) {

                ///Escribe el valor al nuevo archivo
                fputc(byteValue2, originalFile);

            }
                ///Para guardar el PixelData
            else {
                ///Incrementa el indice
                index++;
                break;
            }

            ///Incrementa el indice
            index++;
        }

        ///Cierra el file al terminar de leer
        fclose(file_2of3);

    } if (file_3of3 != nullptr) {
        cout << "3index: " << index << endl;
        while (byteValue3 != EOF) {
            //byteValue1 = fgetc(file_1of3);
            //byteValue2 = fgetc(file_2of3);
            byteValue3 = fgetc(file_3of3);

            ///Para guardar el CommonData
            if (index < 138 + aFileThird*3) {

                ///Escribe el valor al nuevo archivo
                fputc(byteValue3, originalFile);

            }
                ///Para guardar el PixelData
            else {
                ///Incrementa el indice
                index++;
                break;
            }

            ///Incrementa el indice
            index++;
        }

        fputc(EOF, originalFile);

        ///Cierra el file al terminar de leer
        fclose(file_3of3);

    }


    else {

        printf("\nFile not found.");
        return false;

    }

    cout << "ENDindex: " << index << endl;




    return false;
}




/**
 * Muestra el tamaño del archivo (cantidad de bytes)
 * @return int
 */
int RAIDController::getActualImageFileLength(string disk, string name) {

    ///Creacion del nombre de los archivos que poseen la imagen separada
    string image_1of3 = DCDirectory + "Disk" + disk + "/1_" + name;

    ///Para abrir la imagen
    ifstream image( image_1of3.c_str(), ios::binary | ios::in);

    if (!image) {
        cout << "Could not open. ( getActualImageFileLength(a,b) )" << endl;
    } else {
        //cout << "Success." << endl;
    }


    ///Toma el tama;o del archivo
    image.seekg(0,ifstream::end);
    long size = image.tellg();
    image.seekg(0);

    cout << "\nFile Length: " << size << endl;

    image.close();

    return size;

}


///////////////////////////////////////////////////////////SEEK//////////////////////////////////////////////////


/**
 *
 * @param name
 * @return
 */
string RAIDController::seek(string name) {

    string actualDirectory = getDirectory(name);

    string hexData = BMPtoHexData(actualDirectory);

    return hexData;

}


/**
 *
 * @param name
 * @return
 */
string RAIDController::getDirectory(string name) {

    string dir = TICDirectory + "fB_" + name;

    return dir;

}




/**
 * Hace una lectura de los bytes de la imagen .bmp y guarda cada uno, en binario.
 * @return bitString
 */
string RAIDController::BMPtoHexData(string directory) {

    ///restauracion de binaryData

    ///Variables para el recorrido
    string binaryData;
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

    } else {

        printf("\nFile not found.");

        return "";

    }

    return binaryData;

}



/**
 * Convierte un numero decimal a hexadecimal.
 * @param d
 * @return h
 */
string RAIDController::decimalToHex(int d) {

    stringstream ss;
    ss<< hex << d;
    string h ( ss.str() );

    if (h.length() < 2) {
        h = "0" + h;
    }

    return h;

}































bool RAIDController::saveImage(Image *newImage) {

    ///Guarda la instancia de la nueva imagen como la imagen actual en el Controller.
    setActualImage(newImage);

    cout << 1 << endl;

    ///Convierte el BinaryData de la imagen y la guarda temporalmente.
    binaryDataToBMP();

    cout << 2 << endl;

    ///Divide la imagen en tres y las guarda en diferentes discos.
    split();

    cout << 3 << endl;

    ///Pasa de .bmp a su BinaryData para ser
    brokenBinary();

    cout << 4 << endl;

    ///Calcular el bit de paridad con XOR
    XORParity();

    cout << 5 << endl;

    ///Recalcula el siguiente disco en guardar la paridad.
    //nextParityDiskIndex();

    cout << 6 << endl;


    return true;

}

/**
 * Muestra el tamaño del archivo (cantidad de bytes)
 * @return int
 */
 /*
int RAIDController::getActualImageFileLength(string name) {

    ///Para abrir la imagen
    string directory = TICDirectory + "fH_" + name;
    ifstream image( directory.c_str(), ios::binary | ios::in);

    if (!image) {
        cout << "Couldn't get ActualImageLength." << endl;
    }

    ///Toma el tamaño del archivo
    image.seekg(0,ifstream::end);
    long size = image.tellg();
    image.seekg(0);

    //cout << "\nFile Length: " << size << endl;

    image.close();

    return size;

}*/



/**
 * Toma el BinaryData de la imagen y la convierte a .bmp.
 * Es guardada en el folder "TemporalImageContainer".
 * @return bool
 */
bool RAIDController::binaryDataToBMP() {

    ///Nombre de la imagen actual
    string name = actualImage->getNombre();

    ///BinaryData de la imagen actual
    string binaryData = actualImage->getRawHexString();

    ///Variables para la conversion
    int byteValue;
    int index = 0;
    string actualByte;
    float totalByteLength = binaryData.length() / 8;

    ///Archivo nuevo por crear y escribir
    string newFileName = TICDirectory + "fB_" + name + ".bmp";
    FILE* newFile = fopen(newFileName.c_str(),"a");

    ///Recorrera el binString hasta que desaparezca
    while (binaryData.length() > 0) {

        if (true) {

            if (binaryData.length() == 8) {

                ///Elimina los ultimos bits del bitString
                binaryData.erase(0, 8);

                ///Ingresa EOF al final del nuevo archivo
                fputc(EOF, newFile);

                cout << index << ": " << EOF << endl;

            } else {

                ///Lectura del bitString

                ///Toma los primeros 8 bits del bitString
                actualByte = binaryData.substr(0, 8);

                ///Elimina esos bits del bitString
                binaryData.erase(0, 8);

                ///Convierte el byte (8 bits) a decimal
                byteValue = binaryToDecimal(actualByte);

                ///Escribe el valor al nuevo archivo
                fputc(byteValue, newFile);

                cout << index << ": " << byteValue << endl;

            }

        }

        index++;

        ///Muestra el porcentaje completado de la conversion
        if (index % 1000 == 0) {

            float percentageOfCompletition = index/(totalByteLength) * 100;
            cout << "Completed: " << fixed << setprecision(2) << percentageOfCompletition << " %" << endl;

        }

    }

    cout << "Completed: " << 100.00 << "%" << endl;
    cout << "\nRecreation complete: " + newFileName + " created." << endl;

}



/**
 * Cambia el parityDiskIndex para saber cual disk será el que continua guardando el
 * byte de paridad. Va en orden ascendente hasta llegar al 3, ahí se devuelve al 0.
 */
void RAIDController::nextParityDiskIndex() {

    ///Calculo del parityDiskIndex
    if (parityDiskIndex == 3) {
        parityDiskIndex = 0;
    } else if (parityDiskIndex >= 0) {
        parityDiskIndex++;
    } else {
        cout << "ParityDiskIndex out of bounds: " << parityDiskIndex << endl;
    }

    ///Calculo del imagePart1DiskIndex
    if (imagePart1DiskIndex == 3) {
        imagePart1DiskIndex = 0;
    } else if (imagePart1DiskIndex >= 0) {
        imagePart1DiskIndex++;
    } else {
        cout << "imagePart1DiskIndex out of bounds: " << imagePart1DiskIndex << endl;
    }

    ///Calculo del imagePart2DiskIndex
    if (imagePart2DiskIndex == 3) {
        imagePart2DiskIndex = 0;
    } else if (imagePart2DiskIndex >= 0) {
        imagePart2DiskIndex++;
    } else {
        cout << "imagePart2DiskIndex out of bounds: " << imagePart2DiskIndex << endl;
    }

    ///Calculo del imagePart3DiskIndex
    if (imagePart3DiskIndex == 3) {
        imagePart3DiskIndex = 0;
    } else if (imagePart3DiskIndex >= 0) {
        imagePart3DiskIndex++;
    } else {
        cout << "imagePart3DiskIndex out of bounds: " << imagePart3DiskIndex << endl;
    }

}


/**
 * Convierte un numero decimal a binario.
 * @param d
 * @return binary
 */
string RAIDController::decimalToBinary(int d) {

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

int RAIDController::binaryToDecimal(string b) {

    int bin = stoi(b);

    int dec = 0;
    int i = 0;
    int rem;

    while (bin > 0) {
        rem = bin % 10;
        dec = dec + rem * pow(2,i);
        i++;
        bin /= 10;
    }

    //cout << dec << endl;

    return dec;

}















/**
 * Parte la imagen en tres pedazos, en tres archivos diferentes.
 */
bool RAIDController::split(string name, int byteQuantity) {

    ///Se obtiene el disco en el cual se guardara cada parte
    string parityDisk = to_string(parityDiskIndex);
    string image1disk = to_string(imagePart1DiskIndex);
    string image2disk = to_string(imagePart2DiskIndex);
    string image3disk = to_string(imagePart3DiskIndex);

    ///Creacion del nombre de los archivos nuevos que separaran la imagen
    string image_1of3 = DCDirectory + "Disk" + image1disk + "/1_" + name;
    string image_2of3 = DCDirectory + "Disk" + image2disk + "/2_" + name;
    string image_3of3 = DCDirectory + "Disk" + image3disk + "/3_" + name;

    ///Creacion en folder "Disks Container/DiskN" los archivos nuevos
    FILE* file_1of3 = fopen(image_1of3.c_str(),"a");
    FILE* file_2of3 = fopen(image_2of3.c_str(),"a");
    FILE* file_3of3 = fopen(image_3of3.c_str(),"a");

    ///Tamaño del commonData
    int commonDataLength = 138;

    // ///Tamaño total de la imagen
    //int fileLength = fileSize;
    ///Tamaño total de la imagen sin el CommonData (138)
    int byteQuantityWOC = byteQuantity - commonDataLength;

    cout << "byteQuantity: " << byteQuantity << endl;



    cout << "byteQuantityWOC: " << byteQuantityWOC << endl;

    ///Arrays para guardar la informacion de la imagen
    int commonDataTotal[commonDataLength];
    int pixelDataTotal[byteQuantityWOC];

    ///Variables para el recorrido
    int byteValue;
    int index = 0;

    ///Para abrir la imagen original
    string directory = TICDirectory + "fH_" + name;
    FILE *tempFile;
    tempFile = fopen(directory.c_str(), "rb");

    ///Recorrido para obtener el commonData y el PixelData en sus propios arrays
    if (tempFile != nullptr) {
        while (byteValue != EOF) {
            byteValue = fgetc(tempFile);

            ///Para guardar el CommonData
            if (index < commonDataLength) {
                commonDataTotal[index] = byteValue;
            }
                ///Para guardar el PixelData
            else {
                pixelDataTotal[index - commonDataLength] = byteValue;
                cout << "pixelDataTotal: " << index - commonDataLength << " -> " << pixelDataTotal[index - commonDataLength] << endl;
            }

            ///Incrementa el indice
            index++;

        }

        //Cierra el tempFile al terminar de leer
        fclose(tempFile);

    }
    else {

        printf("\nFile not found.");
        return false;

    }

    ///Se escribe  el commonData de la imagen original en las tres divisiones
    for (int i = 0; i < commonDataLength; i++) {

        ///Escritura a cada nuevo archivo
        fputc(commonDataTotal[i], file_1of3);
        fputc(commonDataTotal[i], file_2of3);
        fputc(commonDataTotal[i], file_3of3);

        //cout << i << "-> " << "1: " << commonDataTotal[i] << " 2: " << commonDataTotal[i] << " 3: " << commonDataTotal[i] << endl;

    }

    byteQuantityWOC = index - commonDataLength;
    int byteQuantityWOCC = index - commonDataLength - 1;

    ///Calculara una aproximacion cercana a una igualdad en la particion de pixeles de las nuevas imagenes
    if ( (byteQuantityWOC % 3) == 1) {
        cout << "\n(%3: " << (byteQuantityWOC % 3) << ") -> +" << 2 <<  endl;
        byteQuantityWOC += 2;
        cout << "byteQuantityWOC(for split): " << byteQuantityWOC << endl;
    }
    else if ( (byteQuantityWOC % 3) == 2) {
        cout << "\n(%3: " << (byteQuantityWOC % 3) << ") -> +" << 1 <<  endl;
        byteQuantityWOC += 1;
        cout << "byteQuantityWOC(for split): " << byteQuantityWOC << endl;
    }
    else {
        cout << "\n(%3: " << (byteQuantityWOC % 3) << ") -> +" << 0 <<  endl;
    }

    ///Size del tercio de la imagen
    int aFileThird = byteQuantityWOC / 3;

    cout << "aFileThird(after split): " << aFileThird << endl;

    ///Se escribe  el pixelData de la imagen original en las tres divisiones por separado
    for (int i = 0; i < byteQuantityWOC; i++) {

        ///Para la primera imagen: La parte central y de mas abajo seran blancas.
        if (i < aFileThird) {

            fputc(pixelDataTotal[i], file_1of3);
            fputc(255, file_2of3);
            fputc(255, file_3of3);

        }
            ///Para la segunda imagen: La parte de mas arriba y de mas abajo seran blancas.
        else if (i < aFileThird*2) {

            fputc(255, file_1of3);
            fputc(pixelDataTotal[i], file_2of3);
            fputc(255, file_3of3);

        }
            //////Para la tercera imagen: La parte de mas arriba y central seran blancas.
        else {

            fputc(255, file_1of3);
            fputc(255, file_2of3);
            fputc(pixelDataTotal[i], file_3of3);

        }

    }

    ///Elimina la imagen entera ya que esta no se necesita en el RAID
    remove(directory.c_str());

    return true;
}






///Getters & Setters


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
    actualImage = _actualImage;
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











