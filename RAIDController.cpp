
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
    imagePart1DiskIndex = 1;
    imagePart2DiskIndex = 2;
    imagePart3DiskIndex = 3;

}


///Metodos


/**
 * Busca en los archivos si existe alguno con el mismo nombre.
 * @param name
 * @return bool
 */
bool RAIDController::isAvailable(string name) {

    ///Buscar en archivos si existe una imagen con ese nombre

    return true;

}


bool RAIDController::saveImage(Image *newImage) {

    ///Guarda la instancia de la nueva imagen como la imagen actual en el Controller.
    setActualImage(newImage);

    ///Convierte el BinaryData de la imagen y la guarda temporalmente.
    binaryDataToBMP();

    ///Divide la imagen en tres y las guarda en diferentes discos.
    split();

    ///Recalcula el siguiente disco en guardar la paridad.
    nextParityDiskIndex();


    return false;

}


/**
 * Toma el BinaryData de la imagen y la convierte a .bmp.
 * Es guardada en el folder "TemporalImageContainer".
 * @return bool
 */
bool RAIDController::binaryDataToBMP() {

    ///Nombre de la imagen actual
    string name = actualImage->getNombre();

    ///BinaryData de la imagen actual
    string binaryData = actualImage->getRawBinString();

    ///Variables para la conversion
    int byteValue;
    int index = 0;
    string actualByte;
    float totalByteLength = binaryData.length() / 8;

    ///Archivo nuevo por crear y escribir
    string newFileName = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/TemporalImageContainer/fB_" + name + ".bmp";
    FILE* newFile = fopen(newFileName.c_str(),"a");

    ///Recorrera el binString hasta que desaparezca
    while (binaryData.length() > 0) {

        if (true) {

            if (binaryData.length() == 8) {

                ///Elimina los ultimos bits del bitString
                binaryData.erase(0, 8);

                ///Ingresa EOF al final del nuevo archivo
                fputc(EOF, newFile);

                //cout << index << ": " << EOF << endl;

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

                //cout << index << ": " << byteValue << endl;

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
 * Parte la imagen en tres pedazos, en tres archivos diferentes.
 */
void RAIDController::split() {

    string name = actualImage->getNombre();

    string parityDisk = to_string(parityDiskIndex);
    string image1disk = to_string(imagePart1DiskIndex);
    string image2disk = to_string(imagePart2DiskIndex);
    string image3disk = to_string(imagePart3DiskIndex);

    ///Creacion del nombre de los archivos nuevos
    string newFileName1 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/DisksContainer/Disk" + image1disk + "/new1" + name + ".bmp";
    string newFileName2 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/DisksContainer/Disk" + image2disk + "/new2" + name + ".bmp";
    string newFileName3 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/DisksContainer/Disk" + image3disk + "/new3" + name + ".bmp";

    ///Creacion en maquina de los archivos nuevos
    FILE* newFile1 = fopen(newFileName1.c_str(),"a");
    FILE* newFile2 = fopen(newFileName2.c_str(),"a");
    FILE* newFile3 = fopen(newFileName3.c_str(),"a");

    ///Tamaño del header
    int headerLength = 54;
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
    string directory = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/TemporalImageContainer/fB_" + name + ".bmp";
    FILE *file;
    file = fopen(directory.c_str(), "rb");

    ///Recorrido para obtener el commonData y el PixelData
    if (file != nullptr) {
        while (byteValue != EOF) {

            if (index < 138) {
                byteValue = fgetc(file);
                commonDataTotal[index] = byteValue;
            } else {
                byteValue = fgetc(file);
                pixelDataTotal[index - commonDataLength] = byteValue;
                //cout << "pixelDataTotal: " << index - commonDataLength << " -> " << pixelDataTotal[index - commonDataLength] << endl;
            }

            ///Incrementa el indice
            index++;
        }

        ///Cierra el file al terminar de leer
        fclose(file);

    } else {

        printf("\nFile not found.");

    }

    ///Escribira el commonData de la imagen original en las tres divisiones
    for (int i = 0; i < 138; i++) {

        ///Escritura al nuevo archivo
        fputc(commonDataTotal[i], newFile1);
        fputc(commonDataTotal[i], newFile2);
        fputc(commonDataTotal[i], newFile3);

        cout << i << "-> " << "1: " << commonDataTotal[i] << " 2: " << commonDataTotal[i] << " 3: " << commonDataTotal[i] << endl;

    }

    ///Calculara una aproximacion cercana a una igualdad en la particion de pixeles de las nuevas imagenes
    if ( (fileLength % 3) == 1) {
        fileLength += 2;
    } else if ( (fileLength % 3) == 2) {
        fileLength += 1;
    }

    cout << "fileLength: " << fileLength << ".   %3: " << (fileLength % 3) << endl;

    int third = fileLength / 3;

    cout << third << " third: " << third;


    for (int i = 0; i < fileLength; i++) {

        if (i < third) {

            fputc(pixelDataTotal[i], newFile1);
            fputc(255, newFile2);
            fputc(255, newFile3);

        }
        else if (i < third*2) {

            fputc(255, newFile1);
            fputc(pixelDataTotal[i], newFile2);
            fputc(255, newFile3);

        } else {

            fputc(255, newFile1);
            fputc(255, newFile2);
            fputc(pixelDataTotal[i], newFile3);

        }

    }

}





/**
 * Muestra el tamaño del archivo (cantidad de bytes)
 */
int RAIDController::getActualImageFileLength() {

    ///Nombre del archivo
    string name = actualImage->getNombre();

    ///Para abrir la imagen
    string directory = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/TemporalImageContainer/fB_" + name + ".bmp";
    ifstream image( directory.c_str(), ios::binary | ios::in);

    if (!image) {
        cout << "Could not open." << endl;
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
 * Hace una lectura de los bytes de la imagen .bmp y guarda cada uno, en binario.
 * @return bitString
 */
string RAIDController::BMPtoBinaryData() {

    string name = actualImage->getNombre();

    ///Contenedor del BinaryData
    string binaryData;

    ///Variables para el recorrido
    int byteValue;
    int index = 0;

    ///Para abrir la imagen
    string directory = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/TemporalImageContainer/fromBinary_" + name + ".bmp";
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

    ///Asigna el binaryData al atributo propio de la clase
    actualImage->setRawBinString(binaryData);

    cout << "\nrawBinString: " << endl;
    cout << "Length: " << actualImage->getRawBinString().length() << endl;

    return binaryData;

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

    if (imagePart1DiskIndex == 3) {
        imagePart1DiskIndex = 0;
    } else if (imagePart1DiskIndex >= 0) {
        imagePart1DiskIndex++;
    } else {
        cout << "ParityDiskIndex out of bounds: " << imagePart1DiskIndex << endl;
    }

    if (imagePart2DiskIndex == 3) {
        imagePart2DiskIndex = 0;
    } else if (imagePart2DiskIndex >= 0) {
        imagePart2DiskIndex++;
    } else {
        cout << "ParityDiskIndex out of bounds: " << imagePart2DiskIndex << endl;
    }

    if (imagePart3DiskIndex == 3) {
        imagePart3DiskIndex = 0;
    } else if (imagePart3DiskIndex >= 0) {
        imagePart3DiskIndex++;
    } else {
        cout << "ParityDiskIndex out of bounds: " << imagePart3DiskIndex << endl;
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
