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
int* decToBin(int dec, int tam);

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

int* decToBin(int dec, int tam) {
    if( dec > 4095) {
        printf("Valor invalido para conversao, o valor maximo eh 4095");
        exit(1);
    }
    int i = 0;
    int binValue[tam];
    while (dec > 0) {

        binValue[i] = dec % 2;
        dec = dec / 2;
        i++;
    }

    int binReverse[tam];

    for(int j = i; j<tam; j++){
        binValue[j] = 0;
    }

    for(int j = 0; j<tam; j++) {
        int k = tam-1-j;
        binReverse[j] = binValue[k];
    }

    printf("\n[");
    for(int j = 0; j<tam; j++) {
        printf("%d", binReverse[j]);
    }
    printf("]\n");

    return *binReverse;
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

    if(areaBase/4+24 < areaSecreta) {
        printf("Imagem invalida, a imagem secreta deve ter no maximo 1/4 do tamanho da imagem base");
        exit(1);
    }

    printf("\nlargura: ");
    int binWidth = decToBin(secreta.width, 12);
    printf("altura: ");
    int binHeight = decToBin(secreta.height, 12);

    printf("\nlargura %d, altura %d\n", base.width, base.height);

    for(int i = 0;i<)
        int binRed = decToBin(base.img[i].r, 8);
        int binGreen = decToBin(base.img[i].g, 8);
        int binBlue = decToBin(base.img[i].b, 8);

        binRed[7] = binWidth[0];
        binRed[6] = binWidth[1];
        binGreen[7] = binWidth[2];
        binGreen[6] = binWidth[3];
        binBlue[7] = binWidth[4];
        binBlue[6] = binWidth[5];


    //end codigo

//    printf("Primeiros 10 pixels da imagem base:\n");
//    for(int i=0; i<10; i++)
//        printf("[%02X %02X %02X] ", base.img[i].r, base.img[i].g, base.img[i].b);
//    printf("\n\n");
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
