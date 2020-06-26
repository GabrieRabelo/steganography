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

int main(int argc, char** argv)
{
    Img base, secreta;
    if(argc < 3) {
        printf("loader [img base] [img secreta]\n");
        exit(1);
    }
    load(argv[1], &base);
    load(argv[2], &secreta);

    printf("Primeiros 10 pixels da imagem base:\n");
    for(int i=0; i<10; i++)
        printf("[%02X %02X %02X] ", base.img[i].r, base.img[i].g, base.img[i].b);
    printf("\n\n");

    printf("Primeiros 10 pixels da imagem secreta:\n");
    for(int i=0; i<10; i++)
        printf("[%02X %02X %02X] ", secreta.img[i].r, secreta.img[i].g, secreta.img[i].b);
    printf("\n\n");

    printf("Teste: gravando imagem base em saida.bmp\n");
    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP,
     base.width, base.height, 3, (const unsigned char*) base.img);

    free(base.img);
    free(secreta.img);
}
