
#include <fstream>
#include <cmath>
#include "Image.h"


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
 * Llama a los metodos para probar la imagen
 */
void Image::testImage() {

    printBytes();
    getHeader();
    getFileLength();
    recreateFile();

}

/**
 * Monstrara la informacion del Header de la imagen .bmp.
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
 * Crea un nuevo archivo .bmp a partir del archivo existente
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




