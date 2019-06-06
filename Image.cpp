
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

Image::Image(string _nombre) {
    nombre = _nombre;
}


///Metodos


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
    string directory = "./Media/" + nombre + ".bmp";
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
    rawBinString = binaryData;

    ///Length de binaryData
    int binLen = binaryData.length();

    cout << "\nrawBinString: " << endl;
    //cout << rawBinString << endl;
    cout << "Length: " << rawBinString.length() << endl;
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
    if (binBmp == rawBinString) {
        cout << "Ready for recreating :)\n" << endl;
    } else {
        cout << "rawBinString is not the same :(\n" << endl;
    }

    ///Variales para la conversion
    int byteValue;
    int index = 0;
    string actualByte;
    float totalByteLength = binBmp.length() / 8;

    ///Archivo nuevo por crear y escribir
    string newFileName = "new" + nombre;
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
 * Llama a los metodos para probar la imagen
 */
void Image::testImage() {

    printBytes();
    getHeader();
    getFileLength();
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
    string directory = "./Media/" + nombre + ".bmp";
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
    string directory = "./Media/" + nombre + ".bmp";
    FILE *file;
    file = fopen(directory.c_str(), "rb");


    if (file != NULL) {

        cout << "CONTENT START:" << endl;
        while (byteValue != EOF) {
            if (index >= 0) {
                byteValue = fgetc(file);
                //cout << index << ":   " << byteValue << endl;
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
 * Muestra el tamaño del archivo (cantidad de bytes)
 */
void Image::getFileLength() {

    ///Para abrir la imagen
    string directory = "./Media/" + nombre + ".bmp";
    ifstream image( directory.c_str(), ios::binary | ios::in);

    if (!image) {
        cout << "Could not open." << endl;
    } else {
        //cout << "Success." << endl;
    }


    // get size of file
    image.seekg(0,ifstream::end);
    long size = image.tellg();
    image.seekg(0);

    cout << "\nFile Length: " << size << endl;

    image.close();

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
    string directory = "./Media/" + nombre + ".bmp";
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
    return nombre;
}

/**
 * Setter del nombre de Image.
 * @param _nombre
 */
void Image::setNombre(string _nombre) {
    nombre = _nombre;
}

