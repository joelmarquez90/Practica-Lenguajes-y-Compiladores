#pragma once
#include "TablaDeSimbolos.h"
#define MAXLONG 100

typedef struct
	{
       int id;
       char contenido[MAXLONG];
       int tipo;
       int esWhile;
   }t_info;

typedef struct s_nodo_arbol
	{
       t_info info;
       struct s_nodo_arbol 	*izq,
                            *der,
							*padre;
  	}t_nodo_arbol;

typedef t_nodo_arbol *t_arbol;

/////////////////////////////

t_arbol CrearHoja(char*,int);
t_arbol CrearNodo(t_info, t_arbol, t_arbol);    //******AGREGADO****MODIFICADO EL PARAM QUE RECIBE
void MostrarArbol (const t_info);
void RecorrerInOrden (const t_arbol);
void EliminarArbol (t_arbol);
void GrabarArbol (t_arbol, FILE *);

