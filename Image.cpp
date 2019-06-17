
#include <fstream>
#include <cmath>
#include <iomanip>
#include "Image.h"

using namespace std;


/**
 * Contenedor de la informacion necesaria para obtener una imagen.
 *
 * @since 03/06/19
 */


///Constructores

/**
 * Constructor solo con el nombre de la imagen
 * @param _nombre
 */
Image::Image(string _nombre) {
    name = _nombre;
}

/**
 * Constructor de Imagen
 * @param _nombre
 * @param _rawHexadecimalString
 */
Image::Image(string _nombre, string _rawHexadecimalString) {
    name = _nombre;
    rawHexString = _rawHexadecimalString;
    tempDirectory = "";
    byteQuantity = 0;
}


///Metodos


/**
 * Muestra el tamaño del archivo (cantidad de bytes)
 */
 /*
int Image::getFileSizeFromDirectory() {

    ///Para abrir la imagen
    ifstream image( tempDirectory.c_str(), ios::binary | ios::in);

    if (!image) {
        cout << "Couldn't get " << name << "'s length." << endl;
    }


    //Para obtener el size del archivo
    image.seekg(0,ifstream::end);
    long size = image.tellg();
    image.seekg(0);

    cout << "\nFile Length in Image: " << size << endl;

    image.close();

    return size;

}
*/



















/**
 * Convierte un numero decimal a binario.
 * @param d
 * @return binary
 */
string Image::decimalToBinary(int d) {

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
 * Convierte un numero binario a decimal.
 * @param b
 * @return decimal
 */
int Image::binaryToDecimal(string b) {

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
 * Hace una lectura de los bytes de la imagen .bmp y guarda cada uno, en binario.
 * @return bitString
 */
string Image::toBinary() {

    string binaryData = "";

    int byteValue;
    int index = 0;

    ///Para abrir la imagen
    string directory = "./Media/" + name + ".bmp";
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

    ///Asigna el binaryData al atributo propio de la clase
    rawHexString = binaryData;

    ///Length de binaryData
    int binLen = binaryData.length();

    cout << "\nrawHexString: " << endl;
    //cout << rawHexString << endl;
    cout << "Length: " << rawHexString.length() << endl;
    cout << "Additional bytes: " << additionalBytes << "\n" << endl;

    ///Verifica que se pueda dividir exacto en tres discos, se suma 8 ya que siempre tendra el indicador
    while ((((binLen + 8) % 3) != 0)) {
        additionalBytes += 1;
        binaryData += decimalToBinary(0);
        binLen = binaryData.length();
    }

    ///Se agrega el Indicador de additionalBytes en bitString
    binaryData = decimalToBinary(additionalBytes) + binaryData;

    ///Guarda el binString modificado
    ///Este sera utilizado en el RAID
    modBinString = binaryData;


    cout << "modBinString: " << endl;
    //cout << modBinString << endl;
    cout << "Length: " << modBinString.length() << endl;
    cout << "Additional bytes: " << additionalBytes << " -> '" << modBinString.substr (0,8) << "'\n" << endl;

    return binaryData;

}


/**
 * Genrara un archivo .bmp a partir de un bitString.
 */
void Image::toBmp() {

    ///String binario con la informacion modificada proveniente de los discos.
    string binBmp = modBinString;

    ///Obtiene el primer byte: Indicador de cuantos bytes adicionales tiene el binString
    string additionalBytesIndicator = binBmp.substr (0,8);
    cout << "\nadditionalBytesIndicator(binary): '" << additionalBytesIndicator << "'" << endl;

    ///int con la cantidad de bytes adicionales al final del bitString
    int aBInd = binaryToDecimal(additionalBytesIndicator);
    cout << "additionalBytesIndicator(decimal): " << aBInd << endl;

    cout << "\nFirst 16 -> '" << binBmp.substr (0,16) << "'" << endl;

    ///Elimina el indicador del inicio del binString
    binBmp.erase(0,8);

    cout << "First 16 w/o -> '" << binBmp.substr (0,16) << "'\n" << endl;

    cout << "Last -> '" << binBmp.substr (binBmp.length()-(16 + 8*aBInd), (16 + 8*aBInd)) << "'" << endl;

    ///Por medio del indicador se sabe cuantos bits se deben eliminar, y son eliminados del final del binString
    binBmp.erase(binBmp.length()-(8*aBInd), (8*aBInd));

    cout << "Last w/o -> '" << binBmp.substr(binBmp.length()-24, 24) << "'\n" << endl;

    ///Verifica que su conversion sea igual al rawData guardado en su primera lectura
    if (binBmp == rawHexString) {
        cout << "Ready for recreating :)\n" << endl;
    } else {
        cout << "rawHexString is not the same :(\n" << endl;
    }//Image* tImage = new Image(actualImageName);

    ///Variales para la conversion
    int byteValue;
    int index = 0;
    string actualByte;
    float totalByteLength = binBmp.length() / 8;

    ///Archivo nuevo por crear y escribir
    string newFileName = "new" + name;
    newFileName = newFileName  + ".bmp";
    FILE* newFile = fopen(newFileName.c_str(),"a");

    ///Recorrera el binString hasta que desaparezca
    while (binBmp.length() > 0) {

        if (true) { //index >= 0) {

            if (binBmp.length() == 8) {

                ///Elimina los ultimos bits del bitString
                binBmp.erase(0, 8);

                ///Ingresa EOF al final del nuevo archivo
                fputc(EOF, newFile);

                //cout << index << ": " << EOF << endl;

            } else {

                ///Lectura del bitString

                ///Toma los primeros 8 bits del bitString
                actualByte = binBmp.substr(0, 8);

                ///Elimina esos bits del bitString
                binBmp.erase(0, 8);

                ///Convierte el byte (8 bits) a decimal
                byteValue = binaryToDecimal(actualByte);

                ///Escribe el valor al nuevo archivo
                fputc(byteValue, newFile);

                //cout << index << ": " << byteValue << endl;

            }

        }

        index++;

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
void Image::split() {

    string parityDisk = to_string(0); //getParity();
    string image1disk = to_string(1);
    string image2disk = to_string(2);
    string image3disk = to_string(3);

    ///Creacion del nombre de los archivos nuevos
    string newFileName1 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/DisksContainer/Disk" + image1disk + "/new1" + name + ".bmp";
    string newFileName2 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/DisksContainer/Disk" + image2disk + "/new2" + name + ".bmp";
    string newFileName3 = "/home/ruben/Desktop/Proyectos Git/MyInvincibleLibrary-RAIDLibrary/DisksContainer/Disk" + image2disk + "/new3" + name + ".bmp";

    ///Creacion en maquina de los archivos nuevos
    FILE* newFile1 = fopen(newFileName1.c_str(),"a");
    FILE* newFile2 = fopen(newFileName2.c_str(),"a");
    FILE* newFile3 = fopen(newFileName3.c_str(),"a");

    ///Tamaño del header
    int headerLength = 54;
    ///Tamaño del commonData
    int commonDataLength = 138;

    ///Tamaño total de la imagen
    int fileLength = 0;//getFileSizeFromDirectory();
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
    string directory = "./Media/" + name + ".bmp";
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
 * Llama a los metodos para probar la imagen
 */
void Image::testImage() {

    printBytes();
    getHeader();
    //getFileSizeFromDirectory();
    recreateFile();

}

/**
 * Mostrara la informacion del Header de la imagen .bmp.
 */
void Image::getHeader() {

    unsigned char imageData[54];
    int byteValue;
    int index = 0;

    ///Para abrir la imagen
    string directory = "./Media/" + name + ".bmp";
    FILE *file;
    file = fopen(directory.c_str(), "rb");

    if (file != NULL) {
        while (byteValue != EOF) {
            if (index == 54) {
                break;
            } else {
                byteValue = fgetc(file);
                imageData[index] = byteValue;

                index++;
            }
        }

        fclose(file);

    } else {
        for (int i = 0; i < 29; i++) {
            imageData[i]='\0';
        }

        printf("\nFile not found.");

    }

    int image_width = imageData[20-1] * 256 + imageData[19-1];
    int image_height = imageData[24-1] * 256 + imageData[23-1];
    int image_bits = imageData[29-1];

    printf("\nImage width: %d \n", image_width);
    printf("Image height: %d \n", image_height);
    printf("%d bit image\n", image_bits);

}

/**
 * Imprime todos los bytes de la imagen, uno por uno
 */
void Image::printBytes() {

    int byteValue;
    int index = 0;

    ///Para abrir la imagen
    string directory = "./Media/" + name + ".bmp";
    FILE *file;
    file = fopen(directory.c_str(), "rb");


    if (file != NULL) {

        cout << "CONTENT START:" << endl;
        while (byteValue != EOF) {
            if (index >= 0) {
                byteValue = fgetc(file);
                if (index == 18) {

                }
                cout << index << ":   " << byteValue << endl;
            }
            index++;
        }
        cout << "CONTENT END." << endl;

        fclose(file);

    } else {

        printf("\nFile not found.");

    }

}



/**
 * Crea un nuevo archivo .bmp a partir del archivo existente inmediatamente
 */
void Image::recreateFile() {

    int byteValue;
    int index = 0;

    ///Archivo nuevo por crear y escribir

    string newFileName = "test0";
    newFileName = newFileName  + ".bmp";

    FILE* newFile = fopen(newFileName.c_str(),"a");

    ///Para abrir la imagen
    string directory = "./Media/" + name + ".bmp";
    FILE *file;
    file = fopen(directory.c_str(), "rb");

    if (file != NULL) {

        while (byteValue != EOF) {
            if (index >= 0) {
                ///Lectura al archivo original
                byteValue = fgetc(file);
                ///Escritura al nuevo archivo
                fputc(byteValue, newFile);
            }
            index++;
        }

        fclose(file);

        cout << "Recreation complete: " + newFileName + " created." << endl;

    } else {

        printf("\nFile not found.");

    }

}



///Getters & Setters

/**
 * Getter del nombre de Image.
 * @return nombre
 */
string Image::getNombre() {
    return name;
}

/**
 * Setter del nombre de Image.
 * @param _nombre
 */
void Image::setNombre(string _nombre) {
    name = _nombre;
}

/**
 * Getter de rawBinString de Image.
 * @return string
 */
string Image::getRawHexString() {
    return rawHexString;
}

/**
 * Setter de rawBinString de Image.
 * @param _rawHexString
 */
void Image::setRawHexString(string _rawHexString) {
    rawHexString = _rawHexString;
}

/**
 * Getter de tempDirectory de Image.
 * @return string
 */
string Image::getTempDirectory() {
    return tempDirectory;
}

/**
 * Setter de tempDirectory de Image.
 * @param _tempDirectory
 */
void Image::setTempDirectory(string _tempDirectory) {
    tempDirectory = _tempDirectory;
}

/**
 * Getter de byteQuantity de Image.
 * @return int
 */
int Image::getByteQuantity() {
    return byteQuantity;
}

/**
 * Setter de byteQuantity de Image.
 * @param _byteQuantity
 */
void Image::setByteQuantity(int _byteQuantity) {
    byteQuantity = _byteQuantity;
}
