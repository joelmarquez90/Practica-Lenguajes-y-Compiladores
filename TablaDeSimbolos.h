#pragma once
#define OK 1
#define BAD 0
#define TIPO_INT 0
#define TIPO_REAL 1
#define TIPO_STRING 2
#define ERROR_TIPO 3
#define TIPO_NINGUNO 4
#define TIPO_NEGADO 5
#define TIPO_IF 6
#define TIPO_WHILE 7

typedef struct
{
	double valCte;
	char valCteStr[31];
} valor_t;

typedef struct
{
	char nombre[35];
	int tipo;                      //AGREGADO **** modificado el formato de tipo *** antes era:char tipo[10];
	valor_t valor;
	int longitud;

} ts_t;

typedef struct s_nodo_tabla
{
	ts_t info;
	struct s_nodo_tabla *sig,*ant;
}t_nodo_tabla;

typedef t_nodo_tabla* tabla_simb_t;

void CrearTS(tabla_simb_t*);
int InsertarEnTS(tabla_simb_t*,ts_t*);
void VaciarTS(tabla_simb_t*);
void MostrarTS(tabla_simb_t*);
void prinNombre(char*);
void prinTipo(int);
void prinValorS(char*);
void prinValorNS(float);
void ObtenerItemTS(tabla_simb_t*,int,ts_t*);
int BuscarPos(tabla_simb_t*, ts_t*);
void ModificarTipoDato(tabla_simb_t*, int, int);
void ModificarTamArray(tabla_simb_t*, int, int);
int VerificarTipos(int, int);
int HayVariablesNoUtilizadas(tabla_simb_t *);
void VariableUtilizada(tabla_simb_t *, int);
int fueDeclarado(int);
int verificarTipoComp(int,int);
