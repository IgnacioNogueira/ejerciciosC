#ifndef TDA_DEFINICIONES_H_
#define TDA_DEFINICIONES_H_

/** etiquetas t�picas de TDA (salvo para primitivas booleanas) **/
#define SIN_MEMORIA     0
#define TODO_BIEN       1
#define CLAVE_DUPLICADA 2

/** etiquetas espec�ficas para Punto 1 a.- **/


/** Tipo de dato para TDA Pila con asignaci�n din�mica de memoria */
typedef struct sNodo
{
    void           *info;
    unsigned        tamInfo;
    struct sNodo   *sig;
} tNodo;
typedef tNodo *tPila;

#endif // TDA_DEFINICIONES_H_
