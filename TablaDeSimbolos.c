
#include "TablaDeSimbolos.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void CrearTS(tabla_simb_t *p)
{
	*p = NULL;
}

int InsertarEnTS(tabla_simb_t *p,ts_t *d)
{
	t_nodo_tabla *nue;
	nue = (t_nodo_tabla*)malloc(sizeof(t_nodo_tabla));
	if(!nue)
		return 0;
	nue->info = *d;
	nue->sig = NULL;
	nue->ant = *p;
	if(*p)
		(*p)->sig = nue;
	*p = nue;
	return BuscarPos(p, d);
}

void VaciarTS(tabla_simb_t *p)
{
	t_nodo_tabla *pri = *p,*act = *p,*aux;
	if(*p)
		pri = (*p)->ant;
	while(act)
	{
		if(act->sig)
			act->sig->ant = act->ant;
		if(act->ant)
		{
			act->ant->sig = act->sig;
			*p = act->ant;
		}
		else
			*p = act->sig;
		aux = act;
		act = act->sig;
		free(aux);
	}
	act = pri;
	while(act)
	{
		if(act->sig)
			act->sig->ant = act->ant;
		if(act->ant)
		{
			act->ant->sig = act->sig;
			*p = act->ant;
		}
		else
			*p = act->sig;
		aux = act;
		act = act->ant;
		free(aux);
	}
}

int BuscarPos(tabla_simb_t* p, ts_t* nodo)
{
	int i = 0;
	t_nodo_tabla *act = *p;

	if(act)
	{
		while(act->ant)
			act = act->ant;
		while(act)
		{
			if(!strcmpi(act->info.nombre, nodo->nombre))
				return i;

			act = act->sig;
			i++;
		}
	}

	return -1;
}

void ObtenerItemTS(tabla_simb_t* p, int pos, ts_t* nodo)
{
	t_nodo_tabla *act = *p;
	int cont = 0;

	if(pos < 0)
		nodo = NULL;

	if(act)
	{
		while(act->ant)
			act = act->ant;
		while(act)
		{
			if(cont == pos)
			{
				*nodo = act->info;
				return;
			}
			else
			{
				cont++;
				act = act->sig;
			}
		}
	}

	nodo = NULL;
}

void MostrarTS(tabla_simb_t *p)
{
	t_nodo_tabla *act = *p;

	if(act)
	{
        printf("****************************** TABLA DE SIMBOLOS *******************************\n\n");
        printf("Nombre\t\t\t\tTipo\tValor\t\t\t\tLongitud\n\n");

		while(act->ant)
			act = act->ant;
		while(act)
		{
		    prinNombre(act->info.nombre);
            switch (act->info.tipo)									//  *****************AGREGADO **** MODIFICAMOS PARA MOSTRAR EL TIPO***
			{
                case TIPO_INT:	 	printf("%d\n\n",act->info.tipo);
                                    prinValorNS(act->info.valor.valCte);
                                    break;
                case TIPO_REAL:	 	printf("%d\n\n",act->info.tipo);
                                    prinValorNS(act->info.valor.valCte);
                                    break;
                case TIPO_STRING: 	printf("%d\n\n",act->info.tipo);
                                    prinValorS(act->info.valor.valCteStr);
                                    break;
                case TIPO_NINGUNO:	printf("\n\n Ninguno");
                                    break;
            }
            if(act->info.longitud != 0)
                printf("%d\n\n", act->info.longitud);
            else
                printf("-\n\n");

			act = act->sig;
		}
	}
	else
		printf("La tabla de simbolos esta vacia.\n");
}

void prinNombre(char* nombre)
{
    if(strlen(nombre) > 7 && strlen(nombre) <= 15)
        printf("%s\t\t\t", nombre);
    else if(strlen(nombre) > 15 && strlen(nombre) <= 23)
        printf("%s\t\t", nombre);
    else if(strlen(nombre) > 23)
        printf("%s\t", nombre);
    else
        printf("%s\t\t\t\t", nombre);
}

/*void prinTipo(int tipo)
{
    switch(tipo)
    {
        case TIPO_NINGUNO:
            printf("Ninguno\t");
            break;
        case TIPO_REAL:
            printf("Real\t");
            break;
        case TIPO_STRING:
            printf("String\t");
            break;
        case TIPO_INT:
            printf("Integer\t");
            break;
    }
}*/

void prinValorS(char* s)
{
    if(strlen(s) > 7 && strlen(s) <= 15)
        printf("%s\t\t\t", s);
    else if(strlen(s) > 15 && strlen(s) <= 23)
        printf("%s\t\t", s);
    else if(strlen(s) > 23)
        printf("%s\t", s);
    else
        printf("%s\t\t\t\t", s);
}

void prinValorNS(float f)
{
    if(f >= 0)
        if(f > 99.999999)
            printf("%.4f\t\t\t", f);
        else
            printf("%.4f\t\t\t\t", f);
    else
        printf("-\t\t\t\t");
}

int fueDeclarado(int tipo)				//**************************AGREGADO********************
{
    if(tipo==TIPO_NINGUNO)
        return 0;
    else
        return 1;
}

int verificarTipoComparacion(int tipoIzq,int tipoDer)
{
    if(tipoIzq==TIPO_STRING || tipoDer==TIPO_STRING)
        return BAD;
    else
        return OK;
}


void ModificarTipoDato(tabla_simb_t* p, int pos, int td)
{
	t_nodo_tabla *act = *p;
    int cont = 0;

	if(act)
	{
		while(act->ant)
			act = act->ant;
		while(act)
		{
			if(cont == pos)
			{
				act->info.tipo = td;
				return;
			}
			else
			{
				cont++;
				act = act->sig;
			}
		}
	}
}
/*
int HayVariablesNoUtilizadas(tabla_simb_t *p)
{
    t_nodo_tabla *act = *p;
    int cont = 0;

	if(act)
	{
		while(act->ant)
			act = act->ant;
		while(act)
		{
			if(act->info.idUtil == 'N')
                return cont;
			else
			{
				cont++;
				act = act->sig;
			}
		}
	}

	return -1; // Si retorna un -1 no se encontraron variables no utilizadas
}
*/
/*void VariableUtilizada(tabla_simb_t* p, int pos)
{
	t_nodo_tabla *act = *p;
    int cont = 0;

	if(act)
	{
		while(act->ant)
			act = act->ant;
		while(act)
		{
			if(cont == pos)
			{
				act->info.idUtil = 'S';
				return;
			}
			else
			{
				cont++;
				act = act->sig;
			}
		}
	}
}*/


void ModificarTamArray(tabla_simb_t* p, int pos, int tam)
{
	t_nodo_tabla *act = *p;
    int cont = 0;

	if(act)
	{
		while(act->ant)
			act = act->ant;
		while(act)
		{
			if(cont == pos)
			{
				act->info.longitud = tam;
				return;
			}
			else
			{
				cont++;
				act = act->sig;
			}
		}
	}
}

int VerificarTipos(int t1, int t2)
{
    if(t1 == TIPO_STRING && t2 == TIPO_STRING)
        return 1;
    else if(t1 != TIPO_STRING && t2 != TIPO_STRING)
        return 1;
    else
        return 0;
}
