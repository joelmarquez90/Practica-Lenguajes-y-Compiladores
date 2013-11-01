#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "Arbol.h"

void MostrarArbol (const t_info d)
{
	printf ("Numero nodo: %d\n", d.id);
	printf ("Contenido: %s\n", d.contenido);
}

void RecorrerInOrden (const t_arbol p)
{
	if (p)
	{
	   RecorrerInOrden (p->izq);
	   MostrarArbol (p->info);
	   RecorrerInOrden (p->der);
	}
}

void EliminarArbol (t_arbol p)
{
	if (p)
	{
		EliminarArbol (p->izq);
		EliminarArbol (p->der);
		if (!p->izq && !p->der)
		{
			free (p);
			p=NULL;
		}
	}
}

void GrabarArbol(t_arbol p,FILE *pf)
{
	if(p)
	{
        fprintf(pf,"Nodo: %d ",p->info.id);
        if(p->izq != NULL)
            fprintf(pf,"%d,",p->izq->info.id);
        else
            fprintf(pf,"-,");
        fprintf(pf," %s,",p->info.contenido);
        if(p->der != NULL)
            fprintf(pf," %d\n",p->der->info.id);
        else
            fprintf(pf," -\n");
        GrabarArbol(p->izq,pf);
        GrabarArbol(p->der,pf);
    }
}
