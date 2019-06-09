
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
 * Dividira la imagen en tres imagenes.
 */
void Image::split() {

    ///Creacion del nombre de los archivos nuevos
    string newFileName1 = "new1" + nombre + ".bmp";
    string newFileName2 = "new2" + nombre + ".bmp";
    string newFileName3 = "new3" + nombre + ".bmp";

    ///Creacion en maquina de los archivos nuevos
    FILE* newFile1 = fopen(newFileName1.c_str(),"a");
    FILE* newFile2 = fopen(newFileName2.c_str(),"a");
    FILE* newFile3 = fopen(newFileName3.c_str(),"a");

    ///Tamaño del header
    int headerLength = 54;
    ///Tamaño del commonData
    int commonDataLength = 138;

    ///Tamaño total de la imagen
    int fileLength = getFileLength();
    ///Tamaño total de la imagen sin el Header (54)
    int fileLengthWOH = fileLength - headerLength;
    ///Tamaño total de la imagen sin el CommonData (138)
    int fileLengthWOC = fileLength - commonDataLength;

    cout << "fileLengthWOC: " << fileLengthWOC << endl;

    ///Arrays para guardar la informacion de la imagen
    int headerData[headerLength];
    int commonDataTotal[commonDataLength];
    int pixelDataTotal[fileLengthWOC];

    ///Variables para el recorrido
    int byteValue;
    int index = 0;

    ///Para abrir la imagen original
    string directory = "./Media/" + nombre + ".bmp";
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




    cout << "\nCommonData: "<< endl;

    for (int i = 0; i < 138; i++) {
        //cout << i <<"C: " << commonDataTotal[i] << endl;
    }

    cout << "\nPixelData: "<< endl;

    for (int i = 0; i < fileLengthWOC; i++) {
        //cout << i + 138 << "P: " << pixelDataTotal[i] << endl;
    }





    ///Altura de la imagen original total
    int imageHeightTotal = commonDataTotal[24-1] * 256 + commonDataTotal[23-1];

    ///Ancho de la imagen original y para las parciales
    int imageWidth = commonDataTotal[20-1] * 256 + commonDataTotal[19-1];

    ///Bytes en cada fila
    int bytesPerRow = fileLengthWOC / imageHeightTotal;

    cout << "\nBytesPerRow: " << bytesPerRow << endl;

    float floatImageHeightTotal = imageHeightTotal;

    float exactThird = floatImageHeightTotal / 3;

    cout << "ExactThird: " << exactThird << endl;

    ///Cantidad de filas extra para guardar la imagen
    int extraRows = 3 - (imageHeightTotal % 3);

    if (extraRows == 3) {
        extraRows = 0;
    }

    cout << "ExtraRows: " << extraRows << endl;

    int imageHeightFixed = imageHeightTotal + extraRows;

    cout << "ImageHeightFixed: " << imageHeightFixed << endl;

    ///El tercio del height modificado para que sea exacto
    int aThirdHeightF = imageHeightFixed / 3;

    cout << "aThirdHeightF: " << aThirdHeightF << endl;

    ///Arrays que guardaran la informacion de los pixeles
    int pixelData1[aThirdHeightF];
    int pixelData2[aThirdHeightF];
    int pixelData3[aThirdHeightF];

    ///pixelDataN index
    int z1 = 0;
    int z2 = 0;
    int z3 = 0;

    ///pixelDataTotal index
    int pixelIndex = 0;


    for (int y = 0; y < imageHeightFixed; y++) {

        for (int x = 0; x < bytesPerRow; x++) {

            if (y < aThirdHeightF) {

                pixelData1[z1] = pixelDataTotal[pixelIndex];
                //cout << "Imagen1: " << z1 << ". FromTotalHeight: " << y << ". FromTotalWidth " << x << " -> " << pixelData1[z1] << endl;

                z1++;

                pixelIndex++;

            }

            else if (y < aThirdHeightF*2 ) {

                //cout << y << " == " << aThirdHeightF*2 - 1 << " || " << extraRows << " == " << 2 << endl;

                if ( ( y == (aThirdHeightF*2 - 1) ) && extraRows == 2 ) {
                    pixelData2[z2] = 0;
                    //cout << "02" << endl;
                } else {
                    pixelData2[z2] = pixelDataTotal[pixelIndex];
                    pixelIndex++;
                }

                //cout << "Imagen2: " << z2 << ". FromTotalHeight: " << y << ". FromTotalWidth " << x << " -> " << pixelData2[z2] << endl;

                z2++;

            }

            else {

                //cout << y + 1 << " == " << imageHeightFixed << " || " << extraRows << " >= " << 1 << endl;

                if (( y + 1 == (imageHeightFixed) ) && extraRows >= 1) {
                    pixelData3[z3] = 0;
                    //cout << "03" << endl;
                } else {
                    pixelData3[z3] = pixelDataTotal[pixelIndex];
                    pixelIndex++;
                }

                //cout << "Imagen3: " << z3 << ". FromTotalHeight: " << y << ". FromTotalWidth " << x << " -> " << pixelData3[z3] << endl;

                z3++;

            }

        }

    }

    cout << "\n\nDivision en arrays separados listos.\n" << endl;


    ///Arrays que guardaran la informacion completa de las nuevas imagenes
    int imageData1[fileLength];
    int imageData2[fileLength];
    int imageData3[fileLength];

    ///Agrega el commonData al array de cada imagen
    for (int i = 0; i < 137; i++) {
        imageData1[i] = commonDataTotal[i];
        imageData2[i] = commonDataTotal[i];
        imageData3[i] = commonDataTotal[i];
    }

    int fixedFileLengthWOC = fileLengthWOC + extraRows*imageWidth*3;

    cout << "fixedFileLengthWOC: " << fixedFileLengthWOC << endl;

    ///Agrega la informacion de los pixeles a el array de cada imagen, el resto se completa en blanco.
    for (int i = 0; i < fixedFileLengthWOC; i++) {

        if (i < (fixedFileLengthWOC/3) ) {
            imageData1[i + 137] = pixelData1[i];
            imageData2[i + 137] = 255;
            imageData3[i + 137] = 255;
        } else if (i < (fixedFileLengthWOC/3)*2 ) {
            imageData1[i + 137] = 255;
            imageData2[i + 137] = pixelData2[i + (fixedFileLengthWOC/3)];
            imageData3[i + 137] = 255;
        } else {
            imageData1[i + 137] = 255;
            imageData2[i + 137] = 255;
            imageData3[i + 137] = pixelData3[i + (fixedFileLengthWOC/3)*2];
        }

    }


    int fixedFileLength = fileLength + extraRows*imageWidth*3;


    for (int i = 0; i < fixedFileLength; i++) {
        //cout << i << "-> " << "1: " << imageData1[i] << " 2: " << imageData2[i] << " 3: " << imageData3[i] << endl;

        ///Escritura al nuevo archivo
        fputc(imageData1[i], newFile1);
        fputc(imageData2[i], newFile2);
        fputc(imageData3[i], newFile3);

    }

    cout << "Recreation complete: " + newFileName1 + " created." << endl;
    cout << "Recreation complete: " + newFileName2 + " created." << endl;
    cout << "Recreation complete: " + newFileName3 + " created." << endl;





    int image_width = commonDataTotal[20-1] * 256 + commonDataTotal[19-1];
    int image_height = commonDataTotal[24-1] * 256 + commonDataTotal[23-1];
    int image_bits = commonDataTotal[29-1];


    printf("\nImage width: %d \n", image_width);
    printf("Image height: %d \n", image_height);
    printf("%d bit image\n", image_bits);


}


void Image::splitV2() {

    ///Creacion del nombre de los archivos nuevos
    string newFileName1 = "new1" + nombre + ".bmp";
    string newFileName2 = "new2" + nombre + ".bmp";
    string newFileName3 = "new3" + nombre + ".bmp";

    ///Creacion en maquina de los archivos nuevos
    FILE* newFile1 = fopen(newFileName1.c_str(),"a");
    FILE* newFile2 = fopen(newFileName2.c_str(),"a");
    FILE* newFile3 = fopen(newFileName3.c_str(),"a");




    ///Tamaño del header
    int headerLength = 54;
    ///Tamaño del commonData
    int commonDataLength = 138;

    ///Tamaño total de la imagen
    int fileLength = getFileLength();
    ///Tamaño total de la imagen sin el Header (54)
    int fileLengthWOH = fileLength - headerLength;
    ///Tamaño total de la imagen sin el CommonData (138)
    int fileLengthWOC = fileLength - commonDataLength;

    cout << "fileLengthWOC: " << fileLengthWOC << endl;

    ///Arrays para guardar la informacion de la imagen
    int headerData[headerLength];
    int commonDataTotal[commonDataLength];
    int pixelDataTotal[fileLengthWOC];

    ///Variables para el recorrido
    int byteValue;
    int index = 0;

    ///Para abrir la imagen original
    string directory = "./Media/" + nombre + ".bmp";
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



    int aThird = fileLengthWOC / 3;

    cout << "fileLengthWOC: " << fileLengthWOC << endl;

    cout << "aThird: " << aThird << endl;




    ///Arrays que guardaran la informacion completa de las nuevas imagenes
    int imageData1[fileLength];
    int imageData2[fileLength];
    int imageData3[fileLength];


    for (int i = 0; i < 138; i++) {

        ///Escritura al nuevo archivo
        fputc(commonDataTotal[i], newFile1);
        fputc(commonDataTotal[i], newFile2);
        fputc(commonDataTotal[i], newFile3);

        cout << i << "-> " << "1: " << commonDataTotal[i] << " 2: " << commonDataTotal[i] << " 3: " << commonDataTotal[i] << endl;

    }


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


void Image::compare() {



    ///Para abrir la imagen original
    string directoryBlack = "./Media/black.bmp";
    FILE *black;
    black = fopen(directoryBlack.c_str(), "rb");

    string directorySpiral = "./Media/spiral.bmp";
    FILE *spiral;
    spiral = fopen(directorySpiral.c_str(), "rb");

    string directoryFive = "./Media/five.bmp";
    FILE *five;
    five = fopen(directoryFive.c_str(), "rb");

    string directoryGame = "./Media/game.bmp";
    FILE *game;
    game = fopen(directoryGame.c_str(), "rb");

    string directoryCoachella = "./Media/coachella.bmp";
    FILE *coachella;
    coachella = fopen(directoryCoachella.c_str(), "rb");

    string directoryColor = "./Media/color.bmp";
    FILE *color;
    color = fopen(directoryColor.c_str(), "rb");

    string directoryCorvette = "./Media/corvette.bmp";
    FILE *corvette;
    corvette = fopen(directoryCorvette.c_str(), "rb");


    int byteValue;
    int index = 0;

    ///Recorrido para obtener el commonData y el PixelData
    if (black != nullptr) {
        while (byteValue != EOF) {

            byteValue = fgetc(spiral);

            cout << index << ":  ->  " <<
                    "black: " << fgetc(black) << ".  spiral: " << byteValue << ".  five: " << fgetc(five)
                 << ".  game: " << fgetc(game) << ".  coachella: " << fgetc(coachella)
                 << ".  color: " << fgetc(color) << ".  corvette: " << fgetc(corvette) << endl;

            index++;

        }

        ///Cierra el file al terminar de leer
        fclose(black);
        fclose(spiral);
        fclose(five);
        fclose(game);
        fclose(coachella);
        fclose(color);
        fclose(corvette);

    } else {

        printf("\nFile not found.");

    }


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
 * Muestra el tamaño del archivo (cantidad de bytes)
 */
int Image::getFileLength() {

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

    return size;

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





