#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Para archivo LF

#define TAM_CODPROD 1
#define TAM_CANT 2
#define TAM_OP 1

#define TAM_LINEA TAM_CODPROD + TAM_CANT + TAM_OP

#define NOMBRE_ARCHIVO_MAESTRO  "Maestro.bin"
#define NOMBRE_ARCHIVO_NOVEDADES "Novedades.txt"
#define NOMBRE_ARCHIVO_NOVEDADES2 "Novedades - copia.txt"
#define NOMBRE_ARCHIVO_SIN_STOCK "Sin Stock.txt"
#define NOMBRE_ARCHIVO_ERRORES "Archivo Errores.bin"
#define NOMBRE_ARCHIVOBIN_LONGFIJA "ArchivoBin Longitud fija.bin"
#define NOMBRE_ARCHIVOBIN_LONGVAR "ArchivoBin Longitud variable.bin"


#define NOMBRE_ARCHIVOBIN_MERGE "Archivo Merge.bin"
#define NOMBRE_ARCHIVOTXT_MERGE "Archivo Merge.txt"

#define TAM 100
#define SIN_STOCK 2

#define aMayus(X)((X)>='a' && (X)<='z' ? (X)-32:(X))
#define aMayusc(X)((X) >= 'a' && (X)<='z' ? (X)-32:(X))
#define OK 1
#define NO 0

typedef struct
{
    char codProd;
    char descrip[36];
    int cantidad;
    float precio;

} tProducto;

typedef struct
{
    char codProd;
    int cantidad;
    char tipoOp;

} tNovedades;


int crearArchivoMae(char * nombreBinario);
int crearArchivoMaeDes(char * nombreBinario);
int crearArchivoBin(char * nombreBinario);
int crearArchivoTxt(char * nombreTxt);
int crearLoteTxt(const char *nombreArchivo, char cad[][100]);
int abrirArchivo(FILE **fp,const char * nombreArchivo,const char * modoApertura,int mostrarError);

int leerYmostrarArchivoMast(const char * nombreArchivoMaes);
void mostrarMaesBin(tProducto * prod);

int leerYmostrarArchivoNov(char * nombreArchNov); //como no lo abro, no lo cierro (Lo hago en una funci�n anterior)
void mostrarNovTxt(const tNovedades * nov);

void trozarArchivoLV(tNovedades * nov,char *linea);
void trozarArchivoLF(tNovedades * nov,char *linea);
int actualizarMercaditoArchOrd(FILE * archMaster,FILE * archNov, FILE * sinStock, FILE * archErr);
//int estadoArchivo(FILE * archMaster, FILE *sinStock,tProducto *prod,tNovedades *nov,size_t *cantAct, int *i);
int estadoArchivo(FILE* archMaster, FILE * archNov, FILE * sinStock, tProducto *prod,tNovedades *nov,size_t *cantAct, unsigned int *i);
int actualizarMercaditoArchDes(FILE * archMaster,FILE * archNov, FILE * sinStock, FILE * archErr);

int pasarTxtABin(char * nombreArchivoTxt,char * nombreArchivoBin,char tipo); //el bin es lo mismo pero al rev�s con fprintf




#endif // ARCHIVO_H_INCLUDED
