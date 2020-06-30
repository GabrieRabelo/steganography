#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings

// SOIL é a biblioteca para leitura das imagens
#include "include/SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height;
    RGB* img;
} Img;

// Protótipos
void load(char* name, Img* pic);
int* decToBin8(int dec);
int* decToBin12(int dec);
int binToDec(int *bin, int tam);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (RGB*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: %s (%s)\n", SOIL_last_result(), name);
        exit(1);
    }
    printf("Load: %s (%d x %d x %d)\n", name, pic->width, pic->height, chan);
}

int* decToBin12(int dec) {
    if( dec > 4095) {
        printf("Valor invalido para conversao, o valor maximo eh 4095");
        exit(1);
    }
    int i = 0;
    int binValue[12];
    while (dec > 0) {

        binValue[i] = dec % 2;
        dec = dec / 2;
        i++;
    }

    static int binReverse[12];

    for(int j = i; j<12; j++){
        binValue[j] = 0;
    }

    for(int j = 0; j<12; j++) {
        int k = 12-1-j;
        binReverse[j] = binValue[k];
    }

//    printf("\n[");
//    for(int j = 0; j<12; j++) {
//        printf("%d", binReverse[j]);
//    }
//    printf("]\n");

    return binReverse;
}

int* decToBin8(int dec) {
    if( dec > 4095) {
        printf("Valor invalido para conversao, o valor maximo eh 4095");
        exit(1);
    }
    int i = 0;
    int binValue[8];
    while (dec > 0) {

        binValue[i] = dec % 2;
        dec = dec / 2;
        i++;
    }

    static int binReverse[8];

    for(int j = i; j<8; j++){
        binValue[j] = 0;
    }

    for(int j = 0; j<8; j++) {
        int k = 8-1-j;
        binReverse[j] = binValue[k];
    }

//    printf("\n[");
//    for(int j = 0; j<8; j++) {
//        printf("%d", binReverse[j]);
//    }
//    printf("]\n");

    return binReverse;
}

int binToDec(int *bin, int tam){
    int i, n, sum = 0;
    for (i = 0; i < tam; i++) {
        n = *(bin + i) - '0';
        sum += (n * (1 << (tam - (i + 1))));
    }
    return sum;
}

int main(int argc, char** argv)
{
    Img base, secreta;
    if(argc < 3) {
        printf("loader [img base] [img secreta]\n");
        exit(1);
    }

    load(argv[1], &base);
    load(argv[2], &secreta);

    //start codigo

    int areaBase = base.height * base.width;
    int areaSecreta = secreta.height * secreta.width;

    if(areaBase/4 < areaSecreta+24) {
        printf("Imagem invalida, a imagem secreta deve ter no maximo 1/4 do tamanho da imagem base");
        exit(1);
    }

    printf("\nlargura: ");
    int binWidth = decToBin12(secreta.width);
    printf("altura: ");
    int binHeight = decToBin12(secreta.height);

    printf("\nlargura %d, altura %d\n", base.width, base.height);
    for(int i = 0;i < 2; i ++) {
        printf("DECIMAL VALUES TO RGB: %d, %d, %d\n", base.img[i].r, base.img[i].g, base.img[i].b);
        int *binRed = decToBin8(base.img[i].r);
        int *binGreen= decToBin8(base.img[i].g);
        int *binBlue = decToBin8(base.img[i].b);

        printf("RED: \n [");
        for (int j = 0; j < 8; j ++) {
            printf("%d", *(binRed + j));
        }
        printf("]\n");

        printf("GREEN: \n [");
        for (int j = 0; j < 8; j ++) {
            printf("%d", *(binGreen+ j));
        }
        printf("]\n");

        printf("BLUE: \n [");
        for (int j = 0; j < 8; j ++) {
            printf("%d", *(binBlue + j));
        }
        printf("]\n");
//        binRed[7] = binWidth[0 + 6 * i];
//        binRed[6] = binWidth[1 + 6 * i];
//        binGreen[7] = binWidth[2 + 6 * i];
//        binGreen[6] = binWidth[3 + 6 * i];
//        binBlue[7] = binWidth[4 + 6 * i];
//        binBlue[6] = binWidth[5 + 6 * i];
//
//        base.img[i].r = binToDec(*binRed, 8);
//        base.img[i].g = binToDec(*binGreen, 8);
//        base.img[i].b = binToDec(*binBlue, 8);
    }



    //end codigo

    printf("Primeiros 2 pixels da imagem base:\n");
    for(int i=0; i<2; i++)
        printf("[%02X %02X %02X] ", base.img[i].r, base.img[i].g, base.img[i].b);
    printf("\n\n");
//
//    printf("Primeiros 10 pixels da imagem secreta:\n");
//    for(int i=0; i<10; i++)
//        printf("[%02X %02X %02X] ", secreta.img[i].r, secreta.img[i].g, secreta.img[i].b);
//    printf("\n\n");

    printf("Teste: gravando imagem base em saida.bmp\n");
    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP,
     base.width, base.height, 3, (const unsigned char*) base.img);

    free(base.img);
    free(secreta.img);
}
