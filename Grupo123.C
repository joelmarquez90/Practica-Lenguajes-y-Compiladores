
# line 2 "Grupo123.y"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include "TablaDeSimbolos.h"
#include "Arbol.h"

#define MAX_TOKEN 31
#define MAX_INT 65535  
#define MAX_REAL 157108198082446748462.29006590719
#define MAX_STRING 50
#define MAX_PILA 200
#define CANT_ESTADOS 32
#define CANT_COLUMNAS 27
#define CANT_CHARS 22
#define CANT_PAL_RES 15
#define EF CANT_ESTADOS
#define ERR 999
#define FIN_ARCHIVO -1
#define HOJA_VACIA "H_V"
#define CUERPO_PROGRAMA "C_P"
#define CUERPO_ELSE "C_E"
#define VAR_AUX "AUX"                             /*Numero asociado a tipo string en TS*/
#define LONG_AUX 3

//---------------------------------------------------------------
//-------------	DECLARACION DE FUNCIONES SEMANTICAS -------------
//---------------------------------------------------------------

void Ini_Id(void);
void Cont_Id(void);
void Fin_Id(void);
void Ini_Digito(void);
void Fin_Digito(void);
void P_Abre(void);
void Fin_P_Abre(void);
void P_Cierra(void);
void Fin_P_Cierra(void);
void L_Abre(void);
void Fin_L_Abre(void);
void L_Cierra(void);
void Fin_L_Cierra(void);
void C_Abre(void);
void Fin_C_Abre(void);
void C_Cierra(void);
void Fin_C_Cierra(void);
void Posible_Asig(void);
void Fin_Asig(void);
void Op_Igual(void);
void Fin_Op_Igual(void);
void Posible_Dist(void);
void Fin_Dist(void);
void Op_Dist(void);
void Posible_Mayor(void);
void Fin_Mayor(void);
void Op_Mayor_Igual(void);
void Fin_Mayor_Igual (void);
void Posible_Menor(void);
void Fin_Menor(void);
void Op_Menor_Igual(void);
void Fin_Menor_Igual (void);
void Op_Begin(void);
void Fin_Begin (void);
void Op_End(void);
void Fin_End(void);
void P_Coma(void);
void Fin_P_Coma(void);
void Coma(void);
void Fin_Coma(void);
void Dos_Puntos(void);
void Fin_Dos_Puntos(void);
void Ini_Cte_String(void);
void Posible_Coment(void);
void Continua_Coment(void); //No hace nada (es un comentario)
void Estado_Inicial(void);
void Estado_Final(void);
void Op_Suma(void);
void Fin_Op_Suma(void);
void Op_Resta(void);
void Fin_Op_Resta(void);
void Op_Mult(void);
void Fin_Op_Mult(void);
void Op_Div(void);
void Fin_Op_Div (void);
void Ini_Cte_Real(void);
void Ini_Cte_Real2(void);
void Cte_Real(void);
void Fin_Cte_Real(void);
void Cte_String(void);
void Fin_Cte_String(void);
void Error(void); //Error lexico. Agregar para cada caso especial si es que tenemos tiempo

//--------------------------------------------------------------------------
//-------------	DECLARACION DE FUNCIONES LEXICAS Y SINTACTICAS -------------
//--------------------------------------------------------------------------

int yylex(void);
int yyerror(char*);
int yyparse(void);
int get_evento(char);
void SetToken(int);
int EsLetra(char);
int EsDigito(char);
int EsBlanco(char);
void TerminarAplicacion(int, char *);
void LimpiarHeap(void);
void LimpiarArbol(void);
int tipoDelResultado(int,int,int);
char * negarComp(char *);

t_arbol apilarTermino (t_arbol);
t_arbol desapilarTermino(void);
t_arbol apilarExpresion (t_arbol);
t_arbol desapilarExpresion(void);
t_arbol apilarComparacion (t_arbol);
t_arbol desapilarComparacion(void);
t_arbol apilarListaSentencias (t_arbol);
t_arbol desapilarListaSentencias(void);
t_arbol apilarPrograma(t_arbol);
t_arbol desapilarPrograma(void);
t_arbol apilarComparacion(t_arbol);
t_arbol desapilarComparacion(void);
int apilarVariable(int);
int desapilarVariable(void);

t_arbol CrearHoja(char*,int);
t_arbol CrearNodo(t_info,t_arbol,t_arbol);

//--------------------------------------------------------------
//-------------	DECLARACION DE FUNCIONES ASSEMBLER -------------
//--------------------------------------------------------------

void GenerarAssembler(t_arbol);
void declararVariablesASM(void);
void comparacionAss(int *,t_arbol,const char *,const char *,int *,const char *);
void comparacionAssIEE(int *,t_arbol,const char *,char *,int *, int);
void liberarSubArbol(t_arbol);
void traerJumpContrario(char *,char *);
void traerJump(char *,char *);
void comprobarOverflowInt(t_arbol);
void escribInstrucComparar(void);
int operacionAss(const char *,const char *,t_arbol,int);
void comprobarFinControl(t_arbol);
void subirAlCopro(t_arbol, const char *);
void ponerAuxANodo(char *,int);
void agregarAux(int, t_arbol);
void comprobarDivisorCero(t_arbol);
t_arbol buscarSubArbolIzq (t_arbol);
const char *devolverInsInt(const char *);
void limpiarSecCond(void);
void imprimirError(void);
void darVueltaHijos(t_arbol);
void ConcatenarAssembler (void);
void cargarHijoAVectorAss(int, char*);
int crearVectorAss(void);
const char *devolverTipoLista(int);
const char *devolverTipoListaDescarga(int);
int esIEE(const char *);

t_arbol apilarCompAss(t_arbol);
t_arbol desapilarCompAss(void);
int apilarIntIf(int);
int desapilarIntIf(void);
int apilarIntWhile(int);
int desapilarIntWhile(void);

//----------------------------------------------------
//-------------	DECLARACION DE VARIABLES -------------
//----------------------------------------------------

int DEBUG = 0;
FILE *pfIn,
	 *pfArbol,
     *pfASM,
     *pfDASM,
     *pfCASM;
//lector_t lector;
char token[MAX_TOKEN] = {'\0'};
int tipoToken = 0;
int longId, longCteStr=0;
char c;
double cte;
int cantDec;
int yylval;
ts_t info;
tabla_simb_t tabla;
int cantNodos=0, finArchivo=0, carRetornado=0, cantLineas=1, cantEnTabla=0, cantCteString=0, cantVectores=0;
char secCond[6][MAX_TOKEN];
		
t_arbol p_prg = NULL, 
		p_programa = NULL, 
		p_lista_sentencias = NULL,
		p_sentencia_dec = NULL,
		p_sentencia = NULL, 
		p_iteracion = NULL, 
		p_seleccion = NULL, 
		p_asignacion = NULL, 
		p_tipo_de_dato = NULL, 
		p_lista_variables = NULL, 
		p_output = NULL, 
		p_condicion = NULL, 
		p_comparacion = NULL, 
		p_en_lista = NULL, 
		p_max_lista = NULL, 
		p_min_lista = NULL, 
		p_lista_expresiones = NULL, 
		p_expresion = NULL, 
		p_termino = NULL, 
		p_factor = NULL;
			
int		pilaVar[MAX_PILA],
		pilaIntWhile[MAX_PILA],
		pilaIntIf[MAX_PILA];
		
t_arbol pilaCompAss[MAX_PILA],
		pilaProg[MAX_PILA],
		pilaListaSent[MAX_PILA],
		pilaCond[MAX_PILA],
		pilaComp[MAX_PILA],
		pilaExp[MAX_PILA],
		pilaTer[MAX_PILA];

int topeTer=-1,
	topeExp=-1,
	topeComp=-1,
	topeCond=-1,
	topePilaSent=-1,
	topeProg=-1,
	topeVar=-1,
	topeCompAss=-1,
	topeIntWhile=-1,
	topeIntIf=-1;

//----------------------------------------------------------
//-------------	MATRIZ DE FUNCIONES SEMANTICAS -------------
//----------------------------------------------------------

void (*matFcSemanticas[CANT_ESTADOS][CANT_COLUMNAS])(void) =
	{
/*ESTADO 0*/	{	Ini_Id,Ini_Digito,P_Abre,P_Cierra,L_Abre,L_Cierra,C_Abre,C_Cierra,Posible_Asig,Op_Suma,Op_Resta,Op_Mult,Op_Div,Posible_Mayor,Posible_Menor,Posible_Dist,Op_Begin,Op_End,Ini_Cte_Real2,P_Coma,Coma,Dos_Puntos,Ini_Cte_String,Posible_Coment,Estado_Inicial,Estado_Inicial,Estado_Final},
/*ESTADO 1*/	{	Cont_Id,Cont_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id,Fin_Id},
/*ESTADO 2*/	{	Fin_Digito,Ini_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Ini_Cte_Real,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito,Fin_Digito},
/*ESTADO 3*/	{	Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre,Fin_P_Abre},
/*ESTADO 4*/	{	Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra,Fin_P_Cierra},
/*ESTADO 5*/	{	Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre,Fin_L_Abre},
/*ESTADO 6*/	{	Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra,Fin_L_Cierra},
/*ESTADO 7*/	{	Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre,Fin_C_Abre},
/*ESTADO 8*/	{	Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra,Fin_C_Cierra},
/*ESTADO 9*/	{	Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Op_Igual,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig,Fin_Asig},
/*ESTADO 10*/	{	Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma,Fin_Op_Suma},
/*ESTADO 11*/	{	Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta,Fin_Op_Resta},
/*ESTADO 12*/	{	Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult,Fin_Op_Mult},
/*ESTADO 13*/	{	Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div,Fin_Op_Div},
/*ESTADO 14*/	{	Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Op_Mayor_Igual,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor,Fin_Mayor},
/*ESTADO 15*/	{	Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual,Fin_Mayor_Igual},
/*ESTADO 16*/	{	Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Op_Menor_Igual,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor,Fin_Menor},
/*ESTADO 17*/	{	Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual,Fin_Menor_Igual},
/*ESTADO 18*/	{	Error,Error,Error,Error,Error,Error,Error,Error,Op_Dist,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error},
/*ESTADO 19*/	{	Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist,Fin_Dist},
/*ESTADO 20*/	{	Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin,Fin_Begin},
/*ESTADO 21*/	{	Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End,Fin_End},
/*ESTADO 22*/	{	Fin_Cte_Real,Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real,Fin_Cte_Real},
/*ESTADO 23*/	{	Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Ini_Cte_String,Cte_String,Ini_Cte_String,Error,Ini_Cte_String,Error},
/*ESTADO 24*/	{	Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String,Fin_Cte_String},
/*ESTADO 25*/	{	Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual,Fin_Op_Igual},
/*ESTADO 26*/	{	Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Continua_Coment,Error,Error,Error},
/*ESTADO 27*/	{	Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Continua_Coment,Estado_Inicial,Continua_Coment,Estado_Inicial},
/*ESTADO 28*/	{	Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma,Fin_P_Coma},
/*ESTADO 29*/	{	Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma,Fin_Coma},
/*ESTADO 30*/	{	Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos,Fin_Dos_Puntos},
/*ESTADO 31*/	{	Error,Cte_Real,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error,Error}
	};

//-----------------------------------------------------------
//-------------	MATRIZ DE TRANSICION DE ESTADOS -------------
//-----------------------------------------------------------

int matTransEstados[CANT_ESTADOS][CANT_COLUMNAS] =
	{
			//		L 	D	( 	) 	{ 	} 	[ 	] 	= 	+ 	-	 * 	/ 	> 	< 	! 	@ 	$ 	. 	; 	,	:	" 	# 	EOL BL	EOF
/*ESTADO 0*/	{	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	16,	18,	20,	21,	31,	28,	29,	30,	23,	26,	0,	0,	EF	},
/*ESTADO 1*/	{	1,	1,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 2*/	{	EF,	2,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	22,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 3*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 4*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 5*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 6*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 7*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 8*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 9*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	25,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 10*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 11*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 12*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 13*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 14*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	15,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 15*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 16*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	17,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 17*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 18*/	{	ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,19,	ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR	},
/*ESTADO 19*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 20*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 21*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 22*/	{	EF,	22,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 23*/	{	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	23,	24,	23,	ERR,23,	ERR	},
/*ESTADO 24*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 25*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 26*/	{	ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,27,	ERR,ERR,ERR	},
/*ESTADO 27*/	{	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	27,	0,	27,	0	},
/*ESTADO 28*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 29*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 30*/	{	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF,	EF	},
/*ESTADO 31*/	{	ERR,22,	ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR	}
	};

// Array de caracteres v?lidos para estados

char caracteres[CANT_CHARS] = "(){}[]=+-*/><!@$.;,:\"#";

//Array de palabras reservadas del lenguaje

char* pReservadas[] = {"WHILE","IF","ELSE","DECLARE", "ENDDECLARE","OUT","AND","OR","NOT","INLIST","EQUMAX","EQUMIN","STRING","INT","REAL"};

//MATRICES DE COMPATIBILIDAD DE TIPOSS   
													
int tiposSuma[3][3]=					//PARA LA SUMA PORQUE PERMITE LA CONCATENACION DE STRINGS
{
	{TIPO_INT,ERROR_TIPO,ERROR_TIPO},
	{ERROR_TIPO,TIPO_REAL,ERROR_TIPO},
	{ERROR_TIPO,ERROR_TIPO,TIPO_STRING}
};

int tiposResto[3][3]=						//PARA EL RESTO DE OPERADORES
{
    {TIPO_INT,ERROR_TIPO,ERROR_TIPO},
    {ERROR_TIPO,TIPO_REAL,ERROR_TIPO},
    {ERROR_TIPO,ERROR_TIPO,ERROR_TIPO}
};


#define ID 257
#define CONST_ENTERA 258
#define CONST_REAL 259
#define CONST_STRING 260
#define P_ABRE 261
#define P_CIERRA 262
#define C_ABRE 263
#define C_CIERRA 264
#define L_ABRE 265
#define L_CIERRA 266
#define COMA 267
#define PUNTO_Y_COMA 268
#define DOS_PUNTOS 269
#define IGUAL 270
#define OP_ASIGNACION 271
#define OP_SUMA 272
#define OP_RESTA 273
#define OP_MULTIPLICACION 274
#define OP_DIVISION 275
#define DISTINTO 276
#define MENOR 277
#define MENOR_IGUAL 278
#define MAYOR 279
#define MAYOR_IGUAL 280
#define END 281
#define BEGIN 282
#define WHILE 283
#define IF 284
#define ELSE 285
#define DECLARE 286
#define ENDDECLARE 287
#define OUT 288
#define AND 289
#define OR 290
#define NOT 291
#define INLIST 292
#define EQUMAX 293
#define EQUMIN 294
#define STRING 295
#define INT 296
#define REAL 297
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
#define YYERRCODE 256

# line 869 "Grupo123.y"



int main (int argc, char** argv)
{
	if(argc == 1)
	{
		fprintf(stderr, "Debe especificar un archivo de entrada.\n");
		return 1;
	}
	
	pfIn = fopen(argv[1], "r");
	if(!pfIn)
	{
		perror("Error de apertura de archivo fuente");
		getchar();
		return 2;
	}
	
	CrearTS(&tabla);

	yyparse();
	
	LimpiarHeap();
	fclose(pfIn);

	printf("Compilacion exitosa!\n");
	getch();
    return 0;
}

//--------------------------------------------------------
//-------------	DEFINICION DE FUNCIONES BASE -------------
//--------------------------------------------------------

int yylex (void)
{
	int estadoAct = 0;
	int estadoAct_final = EF;
	int colMatEstado = 0;

	if(finArchivo)
		return FIN_ARCHIVO;
	
	while (estadoAct != estadoAct_final && !feof(pfIn))
	{
		if (!carRetornado)
			c = fgetc(pfIn);
			
		colMatEstado = get_evento (c);
		matFcSemanticas[estadoAct][colMatEstado]();
		estadoAct = matTransEstados[estadoAct][colMatEstado];
		
		carRetornado = 0;
	}
	if(feof(pfIn))
	{
		DEBUG ? printf("Fin de archivo\n") : printf("");
        finArchivo=1;
	}

	if(c!=' ' && c!='\t' && c!='\n')
        carRetornado = 1;
	else if (c == '\n')
		cantLineas++;
		
	return tipoToken;
}

int get_evento(char c)
{
	int i;

    if(EsLetra(c))
	{
		DEBUG ? printf("EsLetra %c\n", c) : printf("");
		return 0;
	}

    if(EsDigito(c))
	{
		DEBUG ? printf("EsDigito %c\n", c) : printf("");
		return 1;
	}

	for(i = 0; i < CANT_CHARS; i++)
	{
		if(caracteres[i] == c)
		{
			DEBUG ? printf("Caracter valido %c\n", c) : printf("");
			return i + 2; //Por el defasaje de Letra y Digito en la matriz de estados
		}
	}

    if(c == '\n')
	{
		DEBUG ? printf("Nueva linea %c\n", c) : printf("");
		return 24;
	}

    if(EsBlanco(c))
	{
		DEBUG ? printf("EsBlanco %c\n", c) : printf("");
		return 25;
	}

    if(c == EOF)
	{
		DEBUG ? printf("Fin de archivo %c\n", c) : printf("");
		return 26;
	}
	else
		TerminarAplicacion(0, "Syntax error");
}

void SetToken(int tipo)
{
	tipoToken = tipo;
	*token = '\0';
}

int EsLetra(char c){return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);}

int EsDigito(char c){return (c >= 48 && c <= 57);}

int EsBlanco(char c){return c == '\t' || c == ' ' || c == 27;}

void TerminarAplicacion(int exitCode, char *description)
{
	printf("ERROR %d: %s. Line %d.\nSe cerrara el compilador.", exitCode, description, cantLineas+1);
	LimpiarHeap();
	fclose(pfIn);
	fclose(pfArbol);
	fclose(pfCASM);
	exit(exitCode);
}

void LimpiarHeap(void)
{
	VaciarTS(&tabla);
	LimpiarArbol();
}

void LimpiarArbol(void)
{
	t_arbol nodoAEliminar = NULL;
	if(p_prg != NULL) nodoAEliminar = p_prg;
	if(p_programa != NULL && nodoAEliminar == NULL) nodoAEliminar = p_programa;
	if(p_iteracion != NULL && nodoAEliminar == NULL) nodoAEliminar = p_iteracion;	
	if(p_seleccion != NULL && nodoAEliminar == NULL) nodoAEliminar = p_seleccion; 
	if(p_asignacion != NULL && nodoAEliminar == NULL) nodoAEliminar = p_asignacion; 
	if(p_tipo_de_dato != NULL && nodoAEliminar == NULL) nodoAEliminar = p_tipo_de_dato; 
	if(p_lista_variables != NULL && nodoAEliminar == NULL) nodoAEliminar = p_lista_variables; 
	if(p_min_lista != NULL && nodoAEliminar == NULL) nodoAEliminar = p_min_lista;
	if(p_max_lista != NULL && nodoAEliminar == NULL) nodoAEliminar = p_max_lista; 
	if(p_en_lista != NULL && nodoAEliminar == NULL) nodoAEliminar = p_en_lista;
	if(p_output != NULL && nodoAEliminar == NULL) nodoAEliminar = p_output;
	if(p_comparacion != NULL && nodoAEliminar == NULL) nodoAEliminar = p_comparacion; 
	if(p_condicion != NULL && nodoAEliminar == NULL) nodoAEliminar = p_condicion; 	
	if(p_lista_expresiones != NULL && nodoAEliminar == NULL) nodoAEliminar = p_lista_expresiones;
	if(p_expresion != NULL && nodoAEliminar == NULL) nodoAEliminar = p_expresion;
	if(p_termino != NULL && nodoAEliminar == NULL) nodoAEliminar = p_termino;
	if(p_sentencia != NULL && nodoAEliminar == NULL) nodoAEliminar = p_sentencia;
	if(p_factor != NULL && nodoAEliminar == NULL) nodoAEliminar = p_factor;
	EliminarArbol(nodoAEliminar);
}

int yyerror(char * s)
{
	TerminarAplicacion(0, "Syntax error");
}

//--------------------------------------------------------------
//-------------	DEFINICION DE FUNCIONES SEMANTICAS -------------
//--------------------------------------------------------------

void Ini_Id(void)
{
	DEBUG ? printf("Ini_Id\n") : printf("");
    strcat(token, &c);
    longId = 1;
}

void Cont_Id(void)
{
	DEBUG ? printf("Cont_Id\n") : printf("");

	strcat(token, &c);
    longId++;

    if((longId + 1) > MAX_TOKEN)
		TerminarAplicacion(110, "La longitud del identificador excede la maxima permitida");
}

void Fin_Id(void)
{
	int i, pos;
	DEBUG ? printf("Fin_Id\n") : printf("");

	for(i = 0; i < CANT_PAL_RES; i++)
	{
		if(strcmp(token, pReservadas[i]) == 0)
		{
			SetToken(i + WHILE); //WHILE es la primera de las palabras reservadas
			return;
		}
	}

	strcpy(info.nombre, token);
	info.tipo=TIPO_NINGUNO; // No se de que tipo de dato son por ahora 
    info.valor.valCte = -1;
	info.valor.valCteStr[0] = '\0';
	info.longitud = 0;
	

	if((pos = BuscarPos(&tabla, &info)) != -1)
	{
		//El token est? en la tabla de s?mbolos
		yylval = pos;
	}
	else
	{
	    DEBUG ? printf("Agregue nuevo ID a las TS\n"): printf("");
		//No est?, se lo agrega a TS
		yylval = InsertarEnTS(&tabla, &info);
		cantEnTabla++;
	}



	SetToken(ID);
}

void Ini_Digito(void)
{
    int cte1=0;
	DEBUG ? printf("Ini_Digito\n") : printf("");
	cte = 0;
	cantDec = 0;
	strcat(token, &c);

	cte = atoi(token);

	if(cte > MAX_INT)
		TerminarAplicacion(100, "El valor ingresado excede el rango de representacion maximo de una constante entera");
}

void Fin_Digito(void)
{
    int pos;
    DEBUG ? printf("Fin_Digito\n") : printf("");

    info.nombre[0] = '\0';
	strcat(info.nombre, "__");
	itoa(cte, token, 10);
	strcat(info.nombre, token);

	info.tipo=TIPO_INT; 									 
	info.valor.valCte = cte;
	info.longitud = 0;

	if((pos = BuscarPos(&tabla, &info)) != -1)
	{
		//El token est? en la tabla de s?mbolos
		yylval = pos;
	}
	else
	{
		//No est?, se lo agrega a TS
		DEBUG ? printf("Agregue nueva CTEENTERA a la TS\n") : printf("");
		yylval = InsertarEnTS(&tabla, &info);
		cantEnTabla++;
	}

    SetToken(CONST_ENTERA);
}

void P_Abre(void){ DEBUG ? printf("P_Abre\n") : printf("");	strcat(token, &c);}

void Fin_P_Abre(void){DEBUG ? printf("Fin_P_Abre\n") : printf("");	SetToken(P_ABRE); yylval=-1;}

void P_Cierra(void){DEBUG ? printf("P_Cierra\n") : printf("");	strcat(token, &c);}

void Fin_P_Cierra(void){DEBUG ? printf("Fin_P_Cierra\n") : printf("");	SetToken(P_CIERRA); yylval=-1;}

void L_Abre(void){DEBUG ? printf("L_Abre\n") : printf("");	strcat(token, &c);}

void Fin_L_Abre(void){DEBUG ? printf("Fin_L_Abre\n") : printf("");	SetToken(L_ABRE); yylval=-1;}

void L_Cierra(void){DEBUG ? printf("L_Cierra\n") : printf("");	strcat(token, &c);}

void Fin_L_Cierra(void){DEBUG ? printf("Fin_L_Cierra\n") : printf("");	SetToken(L_CIERRA); yylval=-1;}

void C_Abre(void){DEBUG ? printf("C_Abre\n") : printf("");	strcat(token, &c);}

void Fin_C_Abre(void){DEBUG ? printf("Fin_C_Abre\n") : printf("");	SetToken(C_ABRE); yylval=-1;}

void C_Cierra(void){DEBUG ? printf("C_Cierra\n") : printf("");	strcat(token, &c);}

void Fin_C_Cierra(void){DEBUG ? printf("Fin_C_Cierra\n") : printf("");	SetToken(C_CIERRA); yylval=-1;}

void Posible_Asig(void){DEBUG ? printf("Posible_Asig\n") : printf("");	strcat(token, &c);}

void Fin_Asig(void){DEBUG ? printf("Fin_Asig\n") : printf("");	SetToken(OP_ASIGNACION); yylval=-1;}

void Op_Igual(void){DEBUG ? printf("Op_Igual\n") : printf("");	strcat(token, &c);}

void Fin_Op_Igual(void){DEBUG ? printf("Fin_Op_Igual\n") : printf("");	SetToken(IGUAL); yylval=-1;}
 
void Posible_Dist(void){DEBUG ? printf("Posible_Dist\n") : printf("");}
 
void Op_Dist(void){DEBUG ? printf("Op_Dist\n") : printf("");}
 
void Fin_Dist(void){DEBUG ? printf("Fin_Dist\n") : printf(""); SetToken(DISTINTO); yylval=-1;}
 
void Posible_Mayor(void){DEBUG ? printf("Posible_Mayor\n") : printf("");}
 
void Fin_Mayor(void){DEBUG ? printf("Fin_Mayor\n") : printf(""); SetToken(MAYOR); yylval=-1;}
 
void Op_Mayor_Igual(void){DEBUG ? printf("Op_Mayor_Igual\n") : printf("");}
 
void Fin_Mayor_Igual (void){DEBUG ? printf("Fin_Mayor_Igual\n") : printf(""); SetToken(MAYOR_IGUAL); yylval=-1;}
 
void Posible_Menor(void){DEBUG ? printf("Posible_Menor\n") : printf("");}
 
void Fin_Menor(void){DEBUG ? printf("Fin_Menor\n") : printf(""); SetToken(MENOR); yylval=-1;}
 
void Op_Menor_Igual(void){DEBUG ? printf("Op_Menor_Igual\n") : printf("");}
 
void Fin_Menor_Igual (void){DEBUG ? printf("Fin_Menor_Igual\n") : printf(""); SetToken(MENOR_IGUAL); yylval=-1;}
 
void Op_Begin(void){DEBUG ? printf("Op_Begin\n") : printf("");}
 
void Fin_Begin (void){DEBUG ? printf("Fin_Begin\n") : printf(""); SetToken(BEGIN); yylval=-1;}
 
void Op_End(void){DEBUG ? printf("Op_End\n") : printf("");}
 
void Fin_End(void){DEBUG ? printf("Fin_End\n") : printf("");SetToken(END); yylval=-1;}
 
void P_Coma(void){DEBUG ? printf("P_Coma\n") : printf("");}
 
void Fin_P_Coma(void){DEBUG ? printf("Fin_P_Coma\n") : printf(""); SetToken(PUNTO_Y_COMA); yylval=-1;}
 
void Coma(void){DEBUG ? printf("Coma\n") : printf("");}
 
void Fin_Coma(void){DEBUG ? printf("Fin_Coma\n") : printf(""); SetToken(COMA); yylval=-1;}
 
void Dos_Puntos(void){DEBUG ? printf("Dos_Puntos\n") : printf("");}
 
void Fin_Dos_Puntos(void){DEBUG ? printf("Fin_Dos_Puntos\n") : printf(""); SetToken(DOS_PUNTOS); yylval=-1;}
 
void Posible_Coment(void){DEBUG ? printf("Posible_Coment\n") : printf("");}
 
void Continua_Coment(void){DEBUG ? printf("Continua_Coment\n") : printf("");} //No hace nada (es un comentario)

void Estado_Inicial(void)
{
	DEBUG ? printf("Estado_Inicial\n") : printf("");
	*token = '\0';
	longId=0;
	tipoToken=0;
}
 
void Estado_Final(void)
{
	DEBUG ? printf("Estado_Final\n") : printf("");

	SetToken(FIN_ARCHIVO);
}
 
void Op_Suma(void){DEBUG ? printf("Op_Suma\n") : printf("");}
 
void Fin_Op_Suma(void){DEBUG ? printf("Fin_Op_Suma\n") : printf(""); SetToken(OP_SUMA); yylval=-1;}
 
void Op_Resta(void){DEBUG ? printf("Op_Resta\n") : printf("");}
 
void Fin_Op_Resta(void){DEBUG ? printf("Fin_Op_Resta\n") : printf(""); SetToken(OP_RESTA); yylval=-1;}
 
void Op_Mult(void){DEBUG ? printf("Op_Mult\n") : printf("");}
 
void Fin_Op_Mult(void){DEBUG ? printf("Fin_Op_Mult\n") : printf(""); SetToken(OP_MULTIPLICACION); yylval=-1;}
 
void Op_Div(void){DEBUG ? printf("Op_Div\n") : printf("");}
 
void Fin_Op_Div (void){DEBUG ? printf("Fin_Op_Div\n") : printf(""); SetToken(OP_DIVISION); yylval=-1;}
 
void Ini_Cte_Real(void)
{
    DEBUG ? printf("Ini_Cte_Real\n") : printf("");
    strcat(token,&c);
    cantDec=0;
}

void Ini_Cte_Real2(void)
{
	DEBUG ? printf("Ini_Cte_Real2\n") : printf("");
	cte = 0;
	cantDec = 0;
	strcat(token, &c);
}
 
void Cte_Real(void)
{
    DEBUG ? printf("Cte_Real\n") : printf("");
	strcat(token, &c);

	if(cte<MAX_REAL)
	{
		cantDec++;
		cte = cte + atoi(&c) * pow(10, -cantDec);
	}
}
 
void Fin_Cte_Real(void)
{
    int pos;
    DEBUG ? printf("Fin_Cte_Real\n") : printf("");

    info.nombre[0] = '\0';
	strcat(info.nombre, "__");
	DEBUG ? printf("el valor de constante es %d",cte) : printf("");
	sprintf(token, "%.2f", cte);
	*(token + strlen(token) - 3) = '_';
	strcat(info.nombre, token);

	info.tipo=TIPO_REAL;							
	info.valor.valCte = cte;
	info.longitud = 0;

	if((pos = BuscarPos(&tabla, &info)) != -1)
	{
		//El token est? en la tabla de s?mbolos
		yylval = pos;
	}
	else
	{
	    DEBUG ? printf("Agregue nueva CTEREAL a la TS\n") : printf("");
		//No est?, se lo agrega a TS
		yylval = InsertarEnTS(&tabla, &info);
		cantEnTabla++;
	}

    SetToken(CONST_REAL);
}
 
void Ini_Cte_String(void)
{	
	DEBUG ? printf("Ini_Cte_String\n") : printf("");
	longCteStr += 1;
	if (c != '"')
		strcat(token,&c);

	if(longCteStr > MAX_STRING)
		TerminarAplicacion(90, "Constante string de mayor longitud de la permitida");
} 
 
void Cte_String(void)
{
	DEBUG ? printf("Cte_String\n") : printf("");
	longCteStr += 1;
	if (c != ' ' && c != '"' )
		strcat(token,&c);

	if(longCteStr > MAX_STRING)
		TerminarAplicacion(90, "Constante string de mayor longitud de la permitida");
}
 
void Fin_Cte_String(void)
{
    int pos;
    DEBUG ? printf("Fin_Cte_String\n") : printf("");

    *(token + (strlen(token))) = '\0';

	info.nombre[0] = '\0';
	sprintf(info.nombre, "_CTE_STRING_%d", cantCteString);
	cantCteString++;

	info.tipo=TIPO_STRING;						
	strcpy(info.valor.valCteStr, token);
	info.longitud = strlen(token);

	if((pos = BuscarPos(&tabla, &info)) != -1)
	{
		//El token est? en la tabla de s?mbolos
		yylval = pos;
	}
	else
	{
		//No est?, se lo agrega a TS
		DEBUG ? printf("Agregue nueva CTESTRING a la TS\n"): printf("");
		yylval = InsertarEnTS(&tabla, &info);
		cantEnTabla++;
	}

	SetToken(CONST_STRING);
	longCteStr=0;
 }
 
void Error(void)
{
	TerminarAplicacion(0, "Syntax error");
}

///////

t_arbol apilarTermino (t_arbol ter)
{
	if (topeTer < MAX_PILA)
	{
		topeTer++;
		pilaTer[topeTer]=ter;
		return pilaTer[topeTer];
	}
	else
		return NULL;
}

t_arbol desapilarTermino(void)
{
	t_arbol ter = NULL;
	if (topeTer >= 0)
	{
		ter=pilaTer[topeTer];
		topeTer--;
	}
	return ter;
}

t_arbol apilarExpresion (t_arbol exp)
{
	if (topeExp < MAX_PILA)
	{
		topeExp++;
		pilaExp[topeExp]=exp;
		return pilaExp[topeExp];
	}
	else
		return NULL;
}

t_arbol desapilarExpresion()
{
	t_arbol exp = NULL;
	if (topeExp >= 0)
	{
		exp=pilaExp[topeExp];
		topeExp--;
	}
	return exp;
}

t_arbol apilarComparacion (t_arbol comp)
{
	if (topeComp < MAX_PILA)
	{
		topeComp++;
		pilaComp[topeComp]=comp;

		return pilaComp[topeComp];
	}
	else
		return NULL;
}

t_arbol desapilarComparacion()
{
	t_arbol comp = NULL;
	if (topeComp >= 0)
	{
		comp = pilaComp[ topeComp ];
		topeComp--;
	}
	return comp;
}

t_arbol apilarCondicion (t_arbol cond)
{
	if (topeCond < MAX_PILA)
	{
		topeCond++;
		pilaCond[topeCond]=cond;

		return pilaCond[topeCond];
	}
	else
		return NULL;
}

t_arbol desapilarCondicion(void)
{
	t_arbol cond = NULL;
	if (topeCond >= 0)
	{
		cond = pilaCond[ topeCond ];
		topeCond--;
	}
	return cond;
}

t_arbol apilarListaSentencias (t_arbol sent)
{
	if (topePilaSent < MAX_PILA)
	{
		topePilaSent++;
		pilaListaSent[topePilaSent]= sent;

		return pilaListaSent[topePilaSent];
	}
	else
		return NULL;
}

t_arbol desapilarListaSentencias()
{
	t_arbol sent = NULL;
	if (topePilaSent >= 0)
	{
		sent=pilaListaSent[topePilaSent];
		topePilaSent--;
	}
	return sent;
}

t_arbol apilarPrograma(t_arbol prog)
{
	if (topeProg < MAX_PILA)
	{
		topeProg++;
		pilaProg[topeProg]=prog;

		return pilaProg[topeProg];
	}
	else
		return NULL;
}

t_arbol desapilarPrograma()
{
	t_arbol prog = NULL;
	if (topeProg >= 0)
	{
		prog=pilaProg[topeProg];
		topeProg--;
	}
	return prog;
}

int apilarVariable(int var)
{
	if (topeVar < MAX_PILA)
	{
		topeVar++;
		pilaVar[topeVar]=var;
		
		return pilaVar[topeVar];
	}
	else
		return 0;
}

int desapilarVariable()
{
	int var=-1;
	if(topeVar>=0)
	{
		var=pilaVar[topeVar];
		topeVar--;
	}

	return var;
}

///

t_arbol apilarCompAss(t_arbol compAss)
{
	if (topeCompAss < MAX_PILA)
	{
		topeCompAss++;
		pilaCompAss[topeCompAss]=compAss;

		return pilaCompAss[topeCompAss];
	}
	else
		return NULL;
}

t_arbol desapilarCompAss()
{
	t_arbol compAss = NULL;
	if (topeCompAss >= 0)
	{
		compAss=pilaCompAss[topeCompAss];
		topeCompAss--;
	}
	return compAss;
}

int apilarIntWhile(int compIntWhile)
{
	if (topeIntWhile < MAX_PILA)
	{
		topeIntWhile++;
		pilaIntWhile[topeIntWhile]=compIntWhile;

		return pilaIntWhile[topeIntWhile];
	}
	else
		return 0;
}

int desapilarIntWhile()
{
	int compIntWhile = 0;
	if (topeIntWhile >= 0)
	{
		compIntWhile=pilaIntWhile[topeIntWhile];
		topeIntWhile--;
	}
	return compIntWhile;
}

int apilarIntIf(int compIntIf)
{
	if (topeIntIf < MAX_PILA)
	{
		topeIntIf++;
		pilaIntIf[topeIntIf]=compIntIf;

		return pilaIntIf[topeIntIf];
	}
	else
		return 0;
}

int desapilarIntIf()
{
	int compIntIf = 0;
	if (topeIntIf >= 0)
	{
		compIntIf=pilaIntIf[topeIntIf];
		topeIntIf--;
	}
	return compIntIf;
}

////////

int tipoDelResultado(int operador,int tipoIzq,int tipoDer)
{
	char izq[10], der[10];
	char error[200]="Incompatibilidad de tipos, no se puede realizar la operacion ";
	int tipo;
	if(operador==OP_SUMA)
        tipo=tiposSuma[tipoIzq][tipoDer];
    else
        tipo=tiposResto[tipoIzq][tipoDer];
	if(tipo != ERROR_TIPO)
		return tipo;
    else
	{
		switch(operador)
		{
			case OP_SUMA:
						strcat(error,"suma");
						break;
			case OP_RESTA:
						strcat(error,"resta");
						break;
			case OP_MULTIPLICACION:
						strcat(error,"multiplicacion");
						break;
			case OP_DIVISION:
						strcat(error,"division");
						break;
			default:
						break;
		}
		strcat(error," entre ");
		switch(tipoIzq)
		{
			case TIPO_INT:
						strcpy(izq,"entero y ");
						break;
			case TIPO_REAL:
						strcpy(izq,"real y ");
						break;
			case TIPO_STRING:
						strcpy(izq,"string y ");
						break;
		}
		strcat(error,izq);
		switch(tipoDer)
		{
			case TIPO_INT:
						strcpy(der,"entero");
						break;
			case TIPO_REAL:
						strcpy(der,"real");
						break;
			case TIPO_STRING:
						strcpy(der,"string");
						break;
		}
		strcat(error,der);
        TerminarAplicacion(80, error);
	}
}

t_arbol CrearHoja(char* val, int valTipo)
{
	t_info tempInfo;
	tempInfo.contenido[0]='\0';
	strcpy(tempInfo.contenido,val);
	tempInfo.tipo=valTipo;
	return CrearNodo(tempInfo, NULL, NULL);
}

t_arbol CrearNodo(t_info datos, t_arbol nodoIzq, t_arbol nodoDer)
{
	t_arbol nuevoNodo = (t_nodo_arbol *)malloc(sizeof (t_nodo_arbol));
	if (!nuevoNodo)
		TerminarAplicacion(10, "No se pudo pedir m?s memoria");
	
	(nuevoNodo)->info.id = cantNodos;
	(nuevoNodo)->info.contenido[0]='\0';
	strcpy((nuevoNodo)->info.contenido,datos.contenido);
	(nuevoNodo)->info.tipo = datos.tipo;
	(nuevoNodo)->izq = nodoIzq;
	(nuevoNodo)->der = nodoDer;
	
	if (nodoIzq != NULL)
		nodoIzq->padre = nuevoNodo;
	if (nodoDer != NULL)
		nodoDer->padre = nuevoNodo;
	
	cantNodos++;
	return nuevoNodo;
}

void GenerarAssembler(t_arbol pArbol)
{
    t_arbol pSubArbolIzq, nodoPadre,nodoPadrePadre;
    int cantAux=0, cantIf=0, cantWhile=0, cantElse=0, cantCuerpo=0, idVector=0, tipoLista=0, tipoNegado=0;
    char contFinal[20];
	t_info auxInfo;
	
    pfCASM=fopen("codigo.asm","wt");
    pfDASM=fopen("data.asm","wt");
    /*Comprobar si abrio bien el archivo*/
    if(!pfCASM || !pfDASM)
        TerminarAplicacion(160, "No se pudo crear el archivo necesario para generar Assembler");

    /*Cabecera con declaraciones*/
    fprintf(pfDASM,".MODEL LARGE\n.386\n.STACK 200h\n\n.DATA\n");

    /*Declarar todas las variables necesarias*/
    declararVariablesASM();
	
    /*Comienzo de codigo con librerias*/
    fprintf(pfCASM,"\n.CODE\n\ninclude strings.asm\ninclude reales.asm\n\nMAIN:\nMOV AX, @DATA\nMOV DS,AX\n");
    fprintf(pfDASM,"");
	  /*Zona de codigo assembler*/

    /*Encontrar el sub-arbol mas a la izquierda*/
    pSubArbolIzq=buscarSubArbolIzq(pArbol);
   /* //puts("Sub-arbol mas a la izquierda");
    mostrarArbol(pSubArbolIzq);
    getch();*/

    while(pSubArbolIzq)
    {
        /*//puts("ASSEMBLER: Sub-arbol mas a la izquierda: ");
        mostrarArbol(pSubArbolIzq);
        printf("\n");
        getch();*/
		////puts("while(pSubArbolIzq)");
        /*Contamos si es un while?*/
        if(pSubArbolIzq->info.esWhile == TIPO_WHILE)
        {
			//puts("WHILE");
            fprintf(pfCASM,"_while%d:\n",cantWhile);
            apilarIntWhile(cantWhile);
            cantWhile++;
        }
		//Si es una asignacion
		if(!strcmp(pSubArbolIzq->info.contenido,"="))
        {
			//puts("=");
            fprintf(pfCASM,";Se asigna %s a %s\n", pSubArbolIzq->der->info.contenido, pSubArbolIzq->izq->info.contenido);
			//SI ES UNA ASIGNACION DE INT O FLOAT   
            if(pSubArbolIzq->izq->info.tipo==TIPO_REAL||            
               pSubArbolIzq->izq->info.tipo==TIPO_INT)
            {
				//puts("asignacion int o real");
                if(pSubArbolIzq->izq->info.tipo==TIPO_INT)
                    comprobarOverflowInt(pSubArbolIzq);
				subirAlCopro(pSubArbolIzq, "FSTP");
				//puts("sali de subirAlCopro asignacion int o real");
            }
			//SI ES UNA ASIGNACION DE STRING
			else
            {
                /*Se preparan los registros y flags*/
                fprintf(pfCASM,"MOV AX, @DATA\n");
                fprintf(pfCASM,"MOV es, AX\n");
                /*Se copian los strings, de AUXSTR a variable en lado derecho*/
                fprintf(pfCASM,"MOV si, OFFSET %s\n",pSubArbolIzq->der->info.contenido);
                fprintf(pfCASM,"MOV di, OFFSET %s\n",pSubArbolIzq->izq->info.contenido);
                fprintf(pfCASM,"CALL COPIAR\n");
            }
			
			comprobarFinControl(pSubArbolIzq);
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"-"))
        {
			//puts("-");
            /*Cambia el nombre del nodo al nombre del auxiliar usado*/
            ponerAuxANodo((pSubArbolIzq->info.contenido),cantAux);

            /*Resta en Assembler*/
            cantAux=operacionAss("resta","FSUB",pSubArbolIzq,cantAux);
            comprobarFinControl(pSubArbolIzq);
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"+"))
        {
			//puts("+");
            if(pSubArbolIzq->izq->info.tipo==TIPO_REAL||
               pSubArbolIzq->izq->info.tipo==TIPO_INT)
            {
                ponerAuxANodo((pSubArbolIzq->info.contenido),cantAux);

                /*Suma en Assembler*/
                cantAux=operacionAss("suma","FADD",pSubArbolIzq,cantAux);
            }
		
			  /*Concatenacion de strings*/
            else
            {
                strcpy(pSubArbolIzq->info.contenido,"AUXSTR");
                fprintf(pfCASM,";Se concatena %s a %s\n", pSubArbolIzq->der->info.contenido, pSubArbolIzq->izq->info.contenido);
                /*Se preparan los registros y flags*/
                fprintf(pfCASM,"MOV AX, @DATA\n");
                fprintf(pfCASM,"MOV es, AX\n");

                /*Copia de cadena lado izquierdo a AUXSTR*/
                fprintf(pfCASM,"MOV si, OFFSET %s\n",pSubArbolIzq->izq->info.contenido);
                fprintf(pfCASM,"MOV di, OFFSET AUXSTR\n");
                fprintf(pfCASM,"CALL COPIAR\n");

                /*Concatenar cadena lado derecho a AUXSTR*/
                fprintf(pfCASM,"MOV si, OFFSET %s\n",pSubArbolIzq->der->info.contenido);
                fprintf(pfCASM,"MOV di, OFFSET AUXSTR\n");
                fprintf(pfCASM,"CALL CONCAT\n");
            }
            comprobarFinControl(pSubArbolIzq);
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"*"))
        {
			//puts("*");
            /*Cambia el nombre del nodo al nombre del auxiliar usado*/
            ponerAuxANodo((pSubArbolIzq->info.contenido),cantAux);

            /*Multiplicacion en Assembler*/
            cantAux=operacionAss("multiplica","FMUL",pSubArbolIzq,cantAux);
            comprobarFinControl(pSubArbolIzq);
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"/"))
        {
			//puts("/");
            /*Cambia el nombre del nodo al nombre del auxiliar usado*/
            ponerAuxANodo((pSubArbolIzq->info.contenido),cantAux);

            /*Division en Assembler*/
            cantAux=operacionAss("divide","FDIV",pSubArbolIzq,cantAux);
            comprobarFinControl(pSubArbolIzq);
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"IF"))
        {
			//puts("IF");
            comprobarFinControl(pSubArbolIzq);
        }
        else if(!strcmp(pSubArbolIzq->info.contenido,"WHILE"))
        {
			//puts("WHILE");
            comprobarFinControl(pSubArbolIzq);
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"OUT"))
        {
			//puts("OUT");
            fprintf(pfCASM,";Se imprime fin de linea y luego lo de adentro del print\n");

            /*Imprimir info.contenido*/
            if(pSubArbolIzq->izq->info.tipo==TIPO_STRING)
            {
                fprintf(pfCASM,"MOV DX, OFFSET %s\n", pSubArbolIzq->izq->info.contenido);
                fprintf(pfCASM,"MOV ah, 9\n");
                fprintf(pfCASM,"int 21h\n");
            }
            else
            {
				fprintf(pfCASM,"MOV eax, %s\n", pSubArbolIzq->izq->info.contenido);
                fprintf(pfCASM,"CALL print_num\n");
            }

            /*Imprimir fin de linea*/
            fprintf(pfCASM,"MOV DX, OFFSET ENDL\n");
            fprintf(pfCASM,"MOV ah, 9\n");
            fprintf(pfCASM,"int 21h\n");

            comprobarFinControl(pSubArbolIzq);
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,";"))
		{
			//puts(";");
			/*Carga el hijo izquierdo en el vector*/
			if(strcmp(pSubArbolIzq->izq->info.contenido,";"))	//carga el hijo izquierdo si no es ;
			{	
				pSubArbolIzq->info.tipo = crearVectorAss();
				
				cargarHijoAVectorAss(pSubArbolIzq->info.tipo, pSubArbolIzq->izq->info.contenido);
			}
			else		//tiene mas de un elemento el IEE, me copio el ID almacenado en el tipo
				pSubArbolIzq->info.tipo = pSubArbolIzq->izq->info.tipo;
			
			/*Carga el hijo derecho en el vector*/
			cargarHijoAVectorAss(pSubArbolIzq->info.tipo, pSubArbolIzq->der->info.contenido);
		}
		else if(!strcmp(pSubArbolIzq->info.contenido,"INLIST"))
		{
            char salto[5];
			//puts("INLIST");				
			if(pSubArbolIzq->info.tipo == TIPO_NEGADO)
				tipoNegado = 1;
			else
				tipoNegado = 0;
								
			if(strcmp(pSubArbolIzq->der->info.contenido,";"))    //si no es ; el hijo es porque solo hay una expresion en la lista y hay que cargarla en el vectoraux
			{
				pSubArbolIzq->info.tipo = crearVectorAss();
				idVector = pSubArbolIzq->info.tipo;
				
				cargarHijoAVectorAss(pSubArbolIzq->info.tipo, pSubArbolIzq->der->info.contenido);
			}
			else
			{
				idVector = pSubArbolIzq->der->info.tipo;
				pSubArbolIzq->info.tipo = pSubArbolIzq->der->info.tipo;
			}
			
			tipoLista=pSubArbolIzq->izq->info.tipo;
				
			//fue el ultimo elemento de la lista
			
			fprintf(pfCASM,";Comienzo del inlist %d\n", idVector);	
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);
			fprintf(pfCASM,"FISTP MAX_INDICE_%d\n", idVector);	//indice tiene la cantidad de elementos en el vector
			fprintf(pfCASM,"mov INDICE_%d, 0 \n", idVector);
				
			//recorrer el vector buscando la ocurrencia de la expresion para resolver el INLIST
			
			//guardo en AUX_ELEM el elemento de la lista a comparar con EXP que contiene la expresion a buscar en la lista
			
			fprintf(pfCASM,"FILD %s \n", pSubArbolIzq->izq->info.contenido);
			fprintf(pfCASM,"%s EXP\n", devolverTipoListaDescarga(tipoLista));
					
			fprintf(pfCASM,";Se compara con el primer elemento de la lista y comienza el ciclo\n");
			fprintf(pfCASM,"mov eax, INDICE_%d\n", idVector);  //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"mov bx,4 \n");												// cada elemento ocupa 4 bytes
			fprintf(pfCASM,"mul bx \n"); 												//cantidad de espacios de memoria desde el ppio del array
			fprintf(pfCASM,"mov bx, ax \n");											// el resultado queda en AX, pero se necesita en BX para indexar.Se mueve de ax a bx
			fprintf(pfCASM,"add bx, OFFSET VECAUX_%d \n", idVector); 								//suma la dirección de comienzo del array. BX ahora apunta al elemento en cuestion.
			fprintf(pfCASM,"mov eax, dword ptr[bx] \n"); 		//copia el dato a guardar en AX
			fprintf(pfCASM,"mov AUX_ELEM, eax \n");
			fprintf(pfCASM,"add INDICE_%d, 1 \n", idVector);
			
			//comienzo del WHILE
			//CONDICION
			fprintf(pfCASM,"ciclo_inlist_%d: \n", idVector);
			fprintf(pfCASM,"FILD INDICE_%d \n",idVector);
			fprintf(pfCASM,"FILD MAX_INDICE_%d \n",idVector);
			
			escribInstrucComparar();
			
			fprintf(pfCASM,"JNB fin_inlist_%d \n", idVector);										// cargar prox elemento a aux_elem
			
			fprintf(pfCASM,"%s EXP \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s AUX_ELEM \n", devolverTipoLista(tipoLista));
			
			escribInstrucComparar();		
			// indice++
			fprintf(pfCASM,"JE fin_inlist_%d\n", idVector);											// endWhile
																							//	preguntar si salio porque son iguales 
			//CUERPO WHILE
								
			fprintf(pfCASM,"mov eax, INDICE_%d\n", idVector);  //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"mov bx,4 \n");												// cada elemento ocupa 4 bytes
			fprintf(pfCASM,"mul bx \n"); 												//cantidad de espacios de memoria desde el ppio del array
			fprintf(pfCASM,"mov bx, ax \n");											// el resultado queda en AX, pero se necesita en BX para indexar.Se mueve de ax a bx
			fprintf(pfCASM,"add bx, OFFSET VECAUX_%d \n", idVector); 								//suma la dirección de comienzo del array. BX ahora apunta al elemento en cuestion.
			fprintf(pfCASM,"mov eax, dword ptr[bx] \n"); 		//copia el dato a guardar en AX
			fprintf(pfCASM,"mov AUX_ELEM, eax \n");
			fprintf(pfCASM,"add INDICE_%d, 1 \n", idVector);								
			fprintf(pfCASM,"JMP ciclo_inlist_%d \n", idVector);
								
			//FINWHILE, se hace un if para saber si salio porque lo encontro o no
			fprintf(pfCASM,"fin_inlist_%d: \n", idVector);
			
			fprintf(pfCASM,"%s EXP \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s AUX_ELEM \n", devolverTipoLista(tipoLista));
			
			strcpy(pSubArbolIzq->der->info.contenido,"AUX_ELEM");
			
			escribInstrucComparar();	

			fprintf(pfCASM,"JNE cont_list_%d\n", idVector);	 // en RESUL_%d queda almacenado un 1 en caso de que el inlist sea verdadero
			fprintf(pfCASM,"mov RESUL_%d, 1 \n", idVector);	 // o un 0 en caso de que sea falso
			fprintf(pfCASM,"cont_list_%d:\n", idVector);

			strcpy(salto,"JNE");
			comparacionAssIEE(&cantWhile,pSubArbolIzq,"igual",salto,&cantIf, tipoNegado);
		}	
		else if(!strcmp(pSubArbolIzq->info.contenido,"EQUMAX"))
		{
            char salto[5];
			//puts("EQUMAX");
			
			if(pSubArbolIzq->info.tipo == TIPO_NEGADO)
				tipoNegado = 1;
			else
				tipoNegado = 0;
				
			if(strcmp(pSubArbolIzq->der->info.contenido,";"))    //si no es ; el hijo es porque solo hay una expresion en la lista y hay que cargarla en el vectoraux
			{
				pSubArbolIzq->info.tipo = crearVectorAss();
				idVector = pSubArbolIzq->info.tipo;
				
				cargarHijoAVectorAss(pSubArbolIzq->info.tipo, pSubArbolIzq->der->info.contenido);
			}
			else
			{
				idVector = pSubArbolIzq->der->info.tipo;
				pSubArbolIzq->info.tipo = pSubArbolIzq->der->info.tipo;
			}
			
			tipoLista=pSubArbolIzq->izq->info.tipo;
						
			fprintf(pfCASM,";Comienzo del equmax %d\n", idVector);	
			fprintf(pfCASM,"mov EQU_AUX, 0 \n");	
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);
			fprintf(pfCASM,"FISTP MAX_INDICE_%d\n", idVector);	//indice tiene la cantidad de elementos en el vector
			fprintf(pfCASM,"mov INDICE_%d, 0 \n", idVector);
		 
			//recorrer el vector para encontrar el maximo
			
			//guardo en AUX_ELEM el elemento de la lista a comparar con EXP que contiene la expresion a buscar en la lista
			
			fprintf(pfCASM,"FILD %s \n", pSubArbolIzq->izq->info.contenido);
			fprintf(pfCASM,"%s EXP\n", devolverTipoListaDescarga(tipoLista));
					
			fprintf(pfCASM,";Se compara con el primer elemento de la lista y comienza el ciclo\n");
			fprintf(pfCASM,"mov eax, INDICE_%d\n", idVector);  //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"mov bx,4 \n");												// cada elemento ocupa 4 bytes
			fprintf(pfCASM,"mul bx \n"); 												//cantidad de espacios de memoria desde el ppio del array
			fprintf(pfCASM,"mov bx, ax \n");											// el resultado queda en AX, pero se necesita en BX para indexar.Se mueve de ax a bx
			fprintf(pfCASM,"add bx, OFFSET VECAUX_%d \n", idVector); 								//suma la dirección de comienzo del array. BX ahora apunta al elemento en cuestion.
			fprintf(pfCASM,"mov eax, dword ptr[bx] \n"); 		//copia el dato a guardar en AX
			fprintf(pfCASM,"mov AUX_ELEM, eax \n");	
						
			//comienzo del WHILE
			//CONDICION																					// While(indice < max_indice_lista )
			fprintf(pfCASM,"ciclo_equmax_%d: \n", idVector);	
			fprintf(pfCASM,"FILD INDICE_%d \n",idVector);
			fprintf(pfCASM,"FILD MAX_INDICE_%d \n",idVector);
			
			escribInstrucComparar();		
			
			fprintf(pfCASM,"JNB fin_equmax_%d\n", idVector);														//indice++																									
																											// cargar prox elemento a aux_elem																																		
			//CUERPO WHILE																				// endWhile
			//IF para guardar el maximo																			//	preguntar si equ_aux es igual a exp
			fprintf(pfCASM,"%s AUX_ELEM \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoLista(tipoLista));	
			
			escribInstrucComparar();		
																//	equmax=TRUE			
			fprintf(pfCASM,"JNA cargar_elem_%d \n", idVector);
			fprintf(pfCASM,"%s AUX_ELEM \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoListaDescarga(tipoLista));
			
			fprintf(pfCASM,"cargar_elem_%d: \n", idVector);	
			fprintf(pfCASM,"add INDICE_%d, 1 \n", idVector);	
			fprintf(pfCASM,"mov eax, INDICE_%d\n", idVector);  //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"mov bx,4 \n");												// cada elemento ocupa 4 bytes
			fprintf(pfCASM,"mul bx \n"); 												//cantidad de espacios de memoria desde el ppio del array
			fprintf(pfCASM,"mov bx, ax \n");											// el resultado queda en AX, pero se necesita en BX para indexar.Se mueve de ax a bx
			fprintf(pfCASM,"add bx, OFFSET VECAUX_%d \n", idVector); 								//suma la dirección de comienzo del array. BX ahora apunta al elemento en cuestion.
			fprintf(pfCASM,"mov eax, dword ptr[bx] \n"); 		//copia el dato a guardar en AX
			fprintf(pfCASM,"mov AUX_ELEM, eax \n");
					
			fprintf(pfCASM,"JMP ciclo_equmax_%d \n", idVector);	
																				
			//FINWHILE, se hace un if para saber si el que encontro como maximo es igual a la expresion a comparar
			fprintf(pfCASM,"fin_equmax_%d: \n", idVector);
			
			fprintf(pfCASM,"%s EXP \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoLista(tipoLista));
			
			strcpy(pSubArbolIzq->der->info.contenido,"AUX_ELEM");
			
			
			escribInstrucComparar();	

			fprintf(pfCASM,"JNE cont_list_%d\n", idVector);	 // en RESUL_%d queda almacenado un 1 en caso de que el inlist sea verdadero
			fprintf(pfCASM,"mov RESUL_%d, 1 \n", idVector);	 // o un 0 en caso de que sea falso
			fprintf(pfCASM,"cont_list_%d:\n", idVector);

			strcpy(salto,"JNE");
			comparacionAssIEE(&cantWhile,pSubArbolIzq,"igual",salto,&cantIf, tipoNegado);			
		}			
		else if(!strcmp(pSubArbolIzq->info.contenido,"EQUMIN"))
		{	
            char salto[5];
			//puts("EQUMIN");		
			if(pSubArbolIzq->info.tipo == TIPO_NEGADO)
				tipoNegado = 1;
			else
				tipoNegado = 0;
				
			if(strcmp(pSubArbolIzq->der->info.contenido,";"))    //si no es ; el hijo es porque solo hay una expresion en la lista y hay que cargarla en el vectoraux
			{
				pSubArbolIzq->info.tipo = crearVectorAss();
				idVector = pSubArbolIzq->info.tipo;
				
				cargarHijoAVectorAss(pSubArbolIzq->info.tipo, pSubArbolIzq->der->info.contenido);
			}
			else
			{
				idVector = pSubArbolIzq->der->info.tipo;
				pSubArbolIzq->info.tipo = pSubArbolIzq->der->info.tipo;
			}
			
			tipoLista=pSubArbolIzq->izq->info.tipo;			
			fprintf(pfCASM,";Comienzo del equmin %d\n", idVector);	
			fprintf(pfCASM,"FILD _cotaEnteros\n");
			fprintf(pfCASM,"FISTP EQU_AUX \n");
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);
			fprintf(pfCASM,"FISTP MAX_INDICE_%d\n", idVector);	//indice tiene la cantidad de elementos en el vector
			fprintf(pfCASM,"mov INDICE_%d, 0 \n", idVector);
		 
			//recorrer el vector para encontrar el maximo
			
			//guardo en AUX_ELEM el elemento de la lista a comparar con EXP que contiene la expresion a buscar en la lista
			
			fprintf(pfCASM,"FILD %s \n", pSubArbolIzq->izq->info.contenido);
			fprintf(pfCASM,"%s EXP\n", devolverTipoListaDescarga(tipoLista));
					
			fprintf(pfCASM,";Se compara con el primer elemento de la lista y comienza el ciclo\n");
			fprintf(pfCASM,"mov eax, INDICE_%d\n", idVector);  //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"mov bx,4 \n");												// cada elemento ocupa 4 bytes
			fprintf(pfCASM,"mul bx \n"); 												//cantidad de espacios de memoria desde el ppio del array
			fprintf(pfCASM,"mov bx, ax \n");											// el resultado queda en AX, pero se necesita en BX para indexar.Se mueve de ax a bx
			fprintf(pfCASM,"add bx, OFFSET VECAUX_%d \n", idVector); 								//suma la dirección de comienzo del array. BX ahora apunta al elemento en cuestion.
			fprintf(pfCASM,"mov eax, dword ptr[bx] \n"); 		//copia el dato a guardar en AX
			fprintf(pfCASM,"mov AUX_ELEM, eax \n");	
			
			//comienzo del WHILE
			//CONDICION																					// While(indice < max_indice_lista )
			fprintf(pfCASM,"ciclo_equmin_%d: \n", idVector);	
			fprintf(pfCASM,"FILD INDICE_%d \n",idVector);
			fprintf(pfCASM,"FILD MAX_INDICE_%d \n",idVector);
			
			escribInstrucComparar();		
			
			fprintf(pfCASM,"JNB fin_equmin_%d\n", idVector);														//indice++																									
																											// cargar prox elemento a aux_elem																																		
			//CUERPO WHILE																				// endWhile
			//IF para guardar el minimo																			//	preguntar si equ_aux es igual a exp
			fprintf(pfCASM,"%s AUX_ELEM \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoLista(tipoLista));	
			
			escribInstrucComparar();		
																//	equmax=TRUE			
			fprintf(pfCASM,"JNB cargar_elem_%d \n", idVector);
			fprintf(pfCASM,"%s AUX_ELEM \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoListaDescarga(tipoLista));
			
			fprintf(pfCASM,"cargar_elem_%d: \n", idVector);	
			fprintf(pfCASM,"add INDICE_%d, 1 \n", idVector);	
			fprintf(pfCASM,"mov eax, INDICE_%d\n", idVector);  //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"mov bx,4 \n");												// cada elemento ocupa 4 bytes
			fprintf(pfCASM,"mul bx \n"); 												//cantidad de espacios de memoria desde el ppio del array
			fprintf(pfCASM,"mov bx, ax \n");											// el resultado queda en AX, pero se necesita en BX para indexar.Se mueve de ax a bx
			fprintf(pfCASM,"add bx, OFFSET VECAUX_%d \n", idVector); 								//suma la dirección de comienzo del array. BX ahora apunta al elemento en cuestion.
			fprintf(pfCASM,"mov eax, dword ptr[bx] \n"); 		//copia el dato a guardar en AX
			fprintf(pfCASM,"mov AUX_ELEM, eax \n");
					
			fprintf(pfCASM,"JMP ciclo_equmin_%d \n", idVector);	
																				
			//FINWHILE, se hace un if para saber si el que encontro como maximo es igual a la expresion a comparar
			fprintf(pfCASM,"fin_equmin_%d: \n", idVector);
			
			fprintf(pfCASM,"%s EXP \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoLista(tipoLista));
			
			strcpy(pSubArbolIzq->der->info.contenido,"AUX_ELEM");
						
			escribInstrucComparar();	

			fprintf(pfCASM,"JNE cont_list_%d\n", idVector);	 // en RESUL_%d queda almacenado un 1 en caso de que el inlist sea verdadero
			fprintf(pfCASM,"mov RESUL_%d, 1 \n", idVector);	 // o un 0 en caso de que sea falso
			fprintf(pfCASM,"cont_list_%d:\n", idVector);

			strcpy(salto,"JNE");
			comparacionAssIEE(&cantWhile,pSubArbolIzq,"igual",salto,&cantIf, tipoNegado);	
		}   	 	
		else if(!strcmp(pSubArbolIzq->info.contenido,"<"))
        {
			//puts("<");
            /*Si es un while agrega la etiqueta para iterar*/
            t_arbol arbolAux;
			auxInfo.tipo=pSubArbolIzq->info.tipo;
			auxInfo.id=pSubArbolIzq->info.id;
			strcpy(auxInfo.contenido,pSubArbolIzq->info.contenido);
			
            arbolAux=CrearNodo(auxInfo,CrearHoja(pSubArbolIzq->izq->info.contenido,pSubArbolIzq->izq->info.tipo),CrearHoja(pSubArbolIzq->der->info.contenido,pSubArbolIzq->der->info.tipo));
            apilarCompAss(arbolAux);
            comparacionAss(&cantWhile,pSubArbolIzq,"menor","JNB",&cantIf,"<");
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,">"))
        {
			//puts(">");
            t_arbol arbolAux;
			auxInfo.tipo=pSubArbolIzq->info.tipo;
			auxInfo.id=pSubArbolIzq->info.id;
			strcpy(auxInfo.contenido,pSubArbolIzq->info.contenido);
			
            arbolAux=CrearNodo(auxInfo,CrearHoja(pSubArbolIzq->izq->info.contenido,pSubArbolIzq->izq->info.tipo),CrearHoja(pSubArbolIzq->der->info.contenido,pSubArbolIzq->der->info.tipo));
            apilarCompAss(arbolAux);
            comparacionAss(&cantWhile,pSubArbolIzq,"mayor","JBE",&cantIf,">");
        }
        else if(!strcmp(pSubArbolIzq->info.contenido,"=="))
        {
			//puts("==");
            t_arbol arbolAux;
			auxInfo.tipo=pSubArbolIzq->info.tipo;
			auxInfo.id=pSubArbolIzq->info.id;
			strcpy(auxInfo.contenido,pSubArbolIzq->info.contenido);
			
            arbolAux=CrearNodo(auxInfo,CrearHoja(pSubArbolIzq->izq->info.contenido,pSubArbolIzq->izq->info.tipo),CrearHoja(pSubArbolIzq->der->info.contenido,pSubArbolIzq->der->info.tipo));
            apilarCompAss(arbolAux);
            comparacionAss(&cantWhile,pSubArbolIzq,"igual","JNE",&cantIf,"==");
        }
        else if(!strcmp(pSubArbolIzq->info.contenido,"<="))
        {
			//puts("<=");
            t_arbol arbolAux;
			auxInfo.tipo=pSubArbolIzq->info.tipo;
			auxInfo.id=pSubArbolIzq->info.id;
			strcpy(auxInfo.contenido,pSubArbolIzq->info.contenido);
			
            arbolAux=CrearNodo(auxInfo,CrearHoja(pSubArbolIzq->izq->info.contenido,pSubArbolIzq->izq->info.tipo),CrearHoja(pSubArbolIzq->der->info.contenido,pSubArbolIzq->der->info.tipo));
            apilarCompAss(arbolAux);
            comparacionAss(&cantWhile,pSubArbolIzq,"menor o igual","JNBE",&cantIf,"<=");
        }
        else if(!strcmp(pSubArbolIzq->info.contenido,">="))
        {
			//puts(">=");
            t_arbol arbolAux;
			auxInfo.tipo=pSubArbolIzq->info.tipo;
			auxInfo.id=pSubArbolIzq->info.id;
			strcpy(auxInfo.contenido,pSubArbolIzq->info.contenido);
			
            arbolAux=CrearNodo(auxInfo,CrearHoja(pSubArbolIzq->izq->info.contenido,pSubArbolIzq->izq->info.tipo),CrearHoja(pSubArbolIzq->der->info.contenido,pSubArbolIzq->der->info.tipo));
            apilarCompAss(arbolAux);
            comparacionAss(&cantWhile,pSubArbolIzq,"mayor o igual","JNAE",&cantIf,">=");
        }
        else if(!strcmp(pSubArbolIzq->info.contenido,"!="))
        {
			//puts("!=");
			t_arbol arbolAux;
			auxInfo.tipo=pSubArbolIzq->info.tipo;
			auxInfo.id=pSubArbolIzq->info.id;
			strcpy(auxInfo.contenido,pSubArbolIzq->info.contenido);

			arbolAux=CrearNodo(auxInfo,CrearHoja(pSubArbolIzq->izq->info.contenido,pSubArbolIzq->izq->info.tipo),CrearHoja(pSubArbolIzq->der->info.contenido,pSubArbolIzq->der->info.tipo));
			apilarCompAss(arbolAux);
			comparacionAss(&cantWhile,pSubArbolIzq,"distinto","JE",&cantIf,"!=");
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"AND"))
        {
			//puts("AND");
            char salto[5];
            t_arbol arbolAux,arbolAux0;
            nodoPadre=pSubArbolIzq->padre;
			
			if(esIEE(pSubArbolIzq->izq->info.contenido) == OK)
			{
				//puts("IEE a la izq del AND");
				strcpy(salto,"JNE");
				fprintf(pfCASM, "FILD RESUL_%d\n",pSubArbolIzq->izq->info.tipo);
				fprintf(pfCASM, "FILD ___UNO___\n");
				escribInstrucComparar();
			}
			else
			{				
				//puts("comp a la izq del AND");
				arbolAux=desapilarCompAss();
				subirAlCopro(arbolAux,"FLD");
				/*Escribir Instrucciones para realizar comparacion entre ST0 y ST1*/
				escribInstrucComparar();
				/*Trae la instruccion correcta para el simbolo guardado en secCond[2]*/
				traerJumpContrario(arbolAux->info.contenido,salto);
				/*Si es falso que realice el salto al fin por ser un AND*/
			}

			if(!strcmp(nodoPadre->info.contenido,"IF"))
			{
				if(!strcmp(nodoPadre->der->info.contenido,CUERPO_ELSE))
					fprintf(pfCASM,"%s _else%d\n",salto,cantIf);
				else
					fprintf(pfCASM,"%s _fin_if%d\n",salto,cantIf);
			}
			else if(!strcmp(nodoPadre->info.contenido,"WHILE"))
			{
				fprintf(pfCASM,"%s _fin_while%d\n",salto,cantWhile-1);
			}
			
			if(esIEE(pSubArbolIzq->der->info.contenido) == OK)
			{
				//puts("IEE a la der del AND");				
				strcpy(salto,"JNE");
				fprintf(pfCASM, "FILD RESUL_%d\n",pSubArbolIzq->der->info.tipo);
				fprintf(pfCASM, "FILD ___UNO___\n");
				escribInstrucComparar();
			}
			else
			{			
				//puts("comp a la der del AND");	
				arbolAux0=desapilarCompAss();
				subirAlCopro(arbolAux0,"FLD");
				/*Escribir Instrucciones para realizar comparacion entre ST0 y ST1*/
				escribInstrucComparar();
				/*Trae la instruccion correcta para el simbolo guardado en secCond[2]*/
				traerJumpContrario(arbolAux0->info.contenido,salto);
			}
			
			if(!strcmp(nodoPadre->info.contenido,"IF"))
			{
				if(!strcmp(nodoPadre->der->info.contenido,CUERPO_ELSE))
					fprintf(pfCASM,"%s _else%d\n",salto,cantIf);
				else
					fprintf(pfCASM,"%s _fin_if%d\n",salto,cantIf);
			}
			else if(!strcmp(nodoPadre->info.contenido,"WHILE"))
			{
				fprintf(pfCASM,"%s _fin_while%d\n",salto,cantWhile-1);
			}
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"OR"))
        {
			//puts("OR");
            char salto[5];
            t_arbol arbolAux,arbolAux0;
            nodoPadre=pSubArbolIzq->padre;
			
			if(esIEE(pSubArbolIzq->izq->info.contenido) == OK)
			{
				//puts("IEE a la izq del OR");	
				strcpy(salto,"JE");
				fprintf(pfCASM, "FILD RESUL_%d\n",pSubArbolIzq->izq->info.tipo);
				fprintf(pfCASM, "FILD ___UNO___\n");
				escribInstrucComparar();
				fprintf(pfCASM,"%s _cuerpo_sent%d\n",salto,cantCuerpo);
				cantCuerpo++;
			}			
			else
			{
				//puts("comp a la izq del OR");	
				arbolAux0=desapilarCompAss();
				/*Evaluamos la primera comparacion*/
				subirAlCopro(arbolAux0,"FLD");
				/*Escribir Instrucciones para realizar comparacion entre ST0 y ST1*/
				escribInstrucComparar();
				/*Trae la sentencia de salto VERDADERO para el simbolo guardado en secCond[2]*/
				traerJump(arbolAux0->info.contenido,salto);
				/*Si es VERDADERO salta al cuerpo y no evalua la segunda condicion */
				fprintf(pfCASM,"%s _cuerpo_sent%d\n",salto,cantCuerpo);
				cantCuerpo++;				
			}
			
			if(esIEE(pSubArbolIzq->der->info.contenido) == OK)
			{			
				//puts("IEE a la der del OR");		
				strcpy(salto,"JNE");
				fprintf(pfCASM, "FILD RESUL_%d\n",pSubArbolIzq->der->info.tipo);
				fprintf(pfCASM, "FILD ___UNO___\n");
				escribInstrucComparar();
			}
			else
			{
				//puts("comp a la der del OR");	
				arbolAux=desapilarCompAss();
				/*Evaluamos la segunda condicion*/
				subirAlCopro(arbolAux,"FLD");
				/*Escribir Instrucciones para realizar comparacion entre ST0 y ST1*/
				escribInstrucComparar();
				traerJumpContrario(arbolAux->info.contenido,salto);
			}
			
            /*Si es falso que salte al fin*/
            if(!strcmp(nodoPadre->info.contenido,"IF"))
            {
                if(!strcmp(nodoPadre->der->info.contenido,CUERPO_ELSE))
                    fprintf(pfCASM,"%s _else%d\n",salto,cantIf);
                else
                    fprintf(pfCASM,"%s _fin_if%d\n",salto,cantIf);
                apilarIntIf(cantIf);
                cantIf++;
            }
            else if(!strcmp(nodoPadre->info.contenido,"WHILE"))
            {
                fprintf(pfCASM,"%s _fin_while%d\n",salto,cantWhile-1);
            }
			//fprintf(pfCASM,"JMP _cuerpo_sent%d:\n",cantCuerpo-1);
			fprintf(pfCASM,"_cuerpo_sent%d:\n",cantCuerpo-1);           
        }		
		else if(!strcmp(pSubArbolIzq->info.contenido,CUERPO_PROGRAMA))
		{
			//puts(CUERPO_PROGRAMA);
			comprobarFinControl(pSubArbolIzq);
		}
		  /*Liberar hojas*/
		liberarSubArbol(pSubArbolIzq);
		/*Siguiente subarbol a la izquierda*/
		pSubArbolIzq=buscarSubArbolIzq(pArbol);
		//puts("pSubArbolIzq=buscarSubArbolIzq(pArbol)");
	}
	/*Termina la ejecucion y fin de archivo*/
    fprintf(pfCASM,"JMP finProg\n");
    imprimirError();
    fprintf(pfCASM,";Fin del programa\n");
    fprintf(pfCASM,"finProg: \nMOV AX,4C00h\nint 21h\n\nEND MAIN\n");
    fclose(pfDASM);
    fclose(pfCASM);
}

void declararVariablesASM()
{
    int i;
	ts_t item;

    /*Variables del sistema*/
    fprintf(pfDASM,"\n;Variables del sistema:\n");
    fprintf(pfDASM,"\tMAX_TEXT equ 30\n"); /*Limite string*/
    fprintf(pfDASM,"\tMAX_LIST equ 100\n"); /*Limite vectores inlist - equmax - equmin*/
	fprintf(pfDASM,"\t___UNO___ dd 1\n");
    fprintf(pfDASM,"\tENDL db 0Dh, 0Ah, '$'\n"); /*Fin de linea*/
    fprintf(pfDASM,"\t_zero dd 0.0 \n"); /*Constante cero*/
    fprintf(pfDASM,"\tmsgError1 db \"Error: division por cero\",'$' \n"); /*Constante cero*/
    fprintf(pfDASM,"\tmsgError2 db \"Error: desbordamiento\",'$' \n"); /*Desbordamiento*/
    fprintf(pfDASM,"\tmsgError3 db \"Error: indice fuera de rango\",'$' \n"); /*Limite de vector*/
    fprintf(pfDASM,"\t_cotaEnteros dd %d\n",MAX_INT); /*Desbordamiento*/

    /*Variables del usuario*/
    fprintf(pfDASM,"\n;Variables del usuario:\n");

    /*Recorrer TS*/
    for(i = 0; i < cantEnTabla; i++)
    {
		ObtenerItemTS(&tabla, i, &item);
        fprintf(pfDASM,"\t%s ",item.nombre);
        /*Si es entero o flotante*/
        if(item.tipo==TIPO_INT||item.tipo==TIPO_REAL)
        {
				
			fprintf(pfDASM,"dd ");
			/*Si es constante imprime el valor, sino inicializa con 0*/
			if(item.valor.valCte > 0)
				if(item.tipo==TIPO_INT)
					fprintf(pfDASM,"%.0f",item.valor.valCte);
				else
					fprintf(pfDASM,"%.2f",item.valor.valCte);
			else
				fprintf(pfDASM,"0");
            
            /*Siguiente variable*/
            fprintf(pfDASM,"\n",item.nombre);
        }
        else if(item.tipo==TIPO_STRING)
		{
			/*Si es constante*/
			if(item.longitud > 0)
			{
				fprintf(pfDASM,"db \"%s\",'$'\n",item.valor.valCteStr);
			}
			else
				fprintf(pfDASM,"db MAX_TEXT dup (?),'$'\n",item.nombre);
		}
    }

    /*Variables auxiliares*/
    fprintf(pfDASM,"\n;Variables auxiliares:\n");
    fprintf(pfDASM,"\tAUXSTR db MAX_TEXT dup (?), '$'\n"); /*Auxiliar para strings*/
	fprintf(pfDASM,"\tEXP dd 0\n"); /*Auxiliar para IEE*/
	fprintf(pfDASM,"\tAUX_ELEM dd 0\n"); /*Auxiliar para IEE*/
	fprintf(pfDASM,"\tEQU_AUX dd 0 \n");					/*Variable para calcular el minimo o maximo de la lista*/
	fprintf(pfDASM,"\tAuxTrue db \"TRUE\",'$' \n");        /*Variables para setear resultado del inlist*/
	fprintf(pfDASM,"\tAuxFalse db \"FALSE\",'$' \n");
}

void ConcatenarAssembler (void)
{
    char linea[200];
    pfCASM=fopen("codigo.asm","rt");
    pfDASM=fopen("data.asm","rt");
    pfASM=fopen("final.asm","wt");
	if (!pfCASM || !pfDASM || !pfASM)
		TerminarAplicacion(170, "No se pudieron concatenar los archivos de assembler");
    fgets(linea,200,pfDASM);
    while(!feof(pfDASM))
    {
        fputs(linea,pfASM);
        fgets(linea,200,pfDASM);
    }
    fgets(linea,200,pfCASM);
    while(!feof(pfCASM))
    {
        fputs(linea,pfASM);
        fgets(linea,200,pfCASM);
    }
    fclose(pfASM);
    fclose(pfCASM);
    fclose(pfDASM);
}

int crearVectorAss(void)
{
	fprintf(pfDASM,"\t;Variables auxiliares para el Inlist/Equmax/Equmin %d\n", cantVectores);
	fprintf(pfDASM,"\tVECAUX_%d dd MAX_LIST dup (0)\n", cantVectores); 		/*Vector Auxiliar para Inlist -Equmax - Equmin */
	fprintf(pfDASM,"\tINDICE_%d dd 0 \n", cantVectores); 						/*Variable para usar de indice del vector auxiliar de Inlist*/
	fprintf(pfDASM,"\tMAX_INDICE_%d dd 0 \n", cantVectores);			/*Variable para guardar la cantidad en lista*/
	fprintf(pfDASM,"\tRESUL_%d dd 0 \n", cantVectores);
	return cantVectores++;
}

void cargarHijoAVectorAss(int idHijo, char *nombreHijo)
{
	fprintf(pfCASM,";Se carga en el vector numero %d el elemento %s\n", idHijo, nombreHijo);
	fprintf(pfCASM,"mov eax, INDICE_%d \n", idHijo);			// valor del indice
	fprintf(pfCASM,"mov bx,4 \n");												// cada elemento ocupa 4 bytes
	fprintf(pfCASM,"mul bx \n"); 												//cantidad de espacios de memoria desde el ppio del array
	fprintf(pfCASM,"mov bx, ax \n");											// el resultado queda en AX, pero se necesita en BX para indexar.Se mueve de ax a bx																							
	fprintf(pfCASM,"add bx, OFFSET VECAUX_%d \n", idHijo); 								//suma la dirección de comienzo del array. BX ahora apunta al elemento en cuestion.																							
	fprintf(pfCASM,"mov eax, %s \n", nombreHijo); 		//copia el dato a guardar en AX
	fprintf(pfCASM,"mov dword ptr[bx],eax \n"); 									// copia AX a la posición de memoria apuntada por BX.
	fprintf(pfCASM,"add INDICE_%d, 1 \n", idHijo);
}

//COMPROBAR SI EN LA ASIGNACION EL DE LA DERECHA NO SUPERA EL OVERFLOW DE INT
 
 /*Comparaciones*/
void comparacionAss(int *cantWhile,t_arbol pSubArbolIzq,const char *comparacionCad,const char *instruccion,int *cantIf,const char *comparacionSimb)
{
	//puts("comparacionAss");
    t_arbol nodoPadre=pSubArbolIzq->padre;
    if(!strcmp(nodoPadre->info.contenido,"AND")==0 && !strcmp(nodoPadre->info.contenido,"OR")==0 )
    {
		//puts("No hay AND ni OR, entro a una sola condicion");
		
        fprintf(pfCASM,";Salta si %s no es %s a %s\n", pSubArbolIzq->izq->info.contenido,comparacionCad, pSubArbolIzq->der->info.contenido);
        subirAlCopro(pSubArbolIzq, "FLD");
        escribInstrucComparar();
        if(!strcmp(nodoPadre->info.contenido,"IF"))
        {
            if(!strcmp(nodoPadre->der->info.contenido,CUERPO_ELSE))
                fprintf(pfCASM,"%s _else%d\n",instruccion,*cantIf);
            else
                fprintf(pfCASM,"%s _fin_if%d\n",instruccion,*cantIf);
            apilarIntIf(*cantIf);
            (*cantIf)++;
        }
        else if(!strcmp(nodoPadre->info.contenido,"WHILE"))
        {
            fprintf(pfCASM,"%s _fin_while%d\n",instruccion,*cantWhile-1);
        }
    }
    else
    {
		//puts("Hay AND o OR");
        /*Si es la primera condicion*/
        if(nodoPadre->izq == pSubArbolIzq)
        {
			//puts("Primera condicion");
            strcpy(secCond[0],pSubArbolIzq->izq->info.contenido);
            strcpy(secCond[1],pSubArbolIzq->der->info.contenido);
            strcpy(secCond[2],comparacionSimb);
        }
        else
        {
			//puts("Segunda condicion");
            strcpy(secCond[3],pSubArbolIzq->izq->info.contenido);
            strcpy(secCond[4],pSubArbolIzq->der->info.contenido);
            strcpy(secCond[5],comparacionSimb);
        }
    }
}

void comparacionAssIEE(int *cantWhile, t_arbol pSubArbolIzq, const char *comparacionCad, char *instruccion, int *cantIf, int tipoNegado)
{
	//puts("comparacionAssIEE");
    t_arbol nodoPadre=pSubArbolIzq->padre;
    if(!strcmp(nodoPadre->info.contenido,"AND")==0 && !strcmp(nodoPadre->info.contenido,"OR")==0 )
    {
		//puts("No hay AND ni OR, entro a una sola condicion IEE");
		
		if(tipoNegado)
			strcpy(instruccion,"JE");
		
        fprintf(pfCASM,";Salta si %s no es %s a %s\n", pSubArbolIzq->izq->info.contenido,comparacionCad, pSubArbolIzq->der->info.contenido);
        if(!strcmp(nodoPadre->info.contenido,"IF"))
        {
            if(!strcmp(nodoPadre->der->info.contenido,CUERPO_ELSE))
                fprintf(pfCASM,"%s _else%d\n",instruccion,*cantIf);
            else
                fprintf(pfCASM,"%s _fin_if%d\n",instruccion,*cantIf);
            apilarIntIf(*cantIf);
            (*cantIf)++;
        }
        else if(!strcmp(nodoPadre->info.contenido,"WHILE"))
        {
            fprintf(pfCASM,"%s _fin_while%d\n",instruccion,*cantWhile-1);
        }
    }
}

/*Libera el subarbol de la memoria*/
void liberarSubArbol(t_arbol pArbol)
{
    free(pArbol->izq);
    free(pArbol->der);
    pArbol->izq=NULL;
    pArbol->der=NULL;
}

/*TRAE EL SALTO CONTRARIO AL COMPARADOR EVALUADO*/
void traerJumpContrario(char *comparador,char *pSalto)
{
    if(!strcmp(comparador,"<"))
        strcpy(pSalto,"JNB");
    if(strcmp(comparador,">")==0)
        strcpy(pSalto,"JNA");
    if(strcmp(comparador,"<=")==0)
        strcpy(pSalto,"JNBE");
    if(strcmp(comparador,">=")==0)
        strcpy(pSalto,"JNAE");
    if(strcmp(comparador,"==")==0)
       strcpy(pSalto,"JNE");
    if(strcmp(comparador,"!=")==0)
        strcpy(pSalto,"JE");
}

/*Trae el salto correspondiente en la comparacion*/
void traerJump(char *comparador,char *pSalto)
{
    if(!strcmp(comparador,"<"))
       strcpy(pSalto,"JNAE");
    if(strcmp(comparador,">")==0)
       strcpy(pSalto,"JNBE");
    if(strcmp(comparador,"<=")==0)
       strcpy(pSalto,"JNA");
    if(strcmp(comparador,">=")==0)
       strcpy(pSalto,"JNB");
    if(strcmp(comparador,"==")==0)
        strcpy(pSalto,"JE");
    if(strcmp(comparador,"!=")==0)
        strcpy(pSalto,"JNE");
}

int esIEE(const char *contenido)
{
	if(!strcmp(contenido,"INLIST"))
		return OK;
	else if(!strcmp(contenido,"EQUMAX"))
		return OK;
	else if(!strcmp(contenido,"EQUMIN"))
		return OK;
	else
		return BAD;
}

void comprobarOverflowInt(t_arbol pSubArbolIzq)
{
    fprintf(pfCASM,";Se comprueba si la asignacion supera la cota de enteros\n");
    fprintf(pfCASM,"FILD _cotaEnteros\n");
		//subo al copro el valor de la derecha
    fprintf(pfCASM,"FILD %s\n", pSubArbolIzq->der->info.contenido);

    escribInstrucComparar();
    fprintf(pfCASM,"JBE errorOverflow\n");
}

//INSTRUCCIONES COMUNES A TODAS LAS COMPARACIONES
void escribInstrucComparar()
{
    fprintf(pfCASM,"FXCH\n");
    fprintf(pfCASM,"FCOMP\n");
    fprintf(pfCASM,"FSTSW AX\n");
    fprintf(pfCASM,"FFREE ST(0)\n");
    fprintf(pfCASM,"SAHF\n");
}

void comprobarFinControl(t_arbol pSubArbolIzq)
{
	//puts("comprobarFinControl");
    t_arbol nodoPadre;
    int ret;
    if(p_prg != pSubArbolIzq)
    {
		nodoPadre = pSubArbolIzq->padre;
        /*Si es un if con else*/
        if(!strcmp(nodoPadre->info.contenido,CUERPO_ELSE))
        {
			//puts("!strcmp(nodoPadre->info.contenido,CUERPO_ELSE)");
            /*Si es el fin de un then*/
            if(nodoPadre->izq==pSubArbolIzq)
            {
				//puts("nodoPadre->izq==pSubArbolIzq");
                ret=desapilarIntIf();
                fprintf(pfCASM,"JMP _fin_if%d\n",ret);
                fprintf(pfCASM,"_else%d:\n",ret);
                apilarIntIf(ret);
            }
            /*Es el fin del else y del if*/
            else
            {
                ret=desapilarIntIf();
                fprintf(pfCASM,"_fin_if%d:\n",ret);
            }
        }
		        /*Es el fin de un if sin else*/
        else if(!strcmp(nodoPadre->info.contenido,"IF"))
        {
            ret=desapilarIntIf();
            fprintf(pfCASM,"_fin_if%d:\n",ret);
        }
        /*Fin del while*/
        else if(!strcmp(nodoPadre->info.contenido,"WHILE"))
        {
            ret=desapilarIntWhile();
            fprintf(pfCASM,"JMP _while%d\n",ret);
            fprintf(pfCASM,"_fin_while%d:\n",ret);
        }
	}
}

void subirAlCopro(t_arbol pSubArbolIzq, const char *instruccion)
{
	//puts("subir al copro");
    /*Si es asignacion*/
    if(!strcmp(instruccion,"FSTP"))
        darVueltaHijos(pSubArbolIzq);

    if(pSubArbolIzq->izq->info.tipo == TIPO_INT)
        fprintf(pfCASM,"FILD %s\n", pSubArbolIzq->izq->info.contenido);
    else
        fprintf(pfCASM,"FLD %s\n", pSubArbolIzq->izq->info.contenido);
		
    if(pSubArbolIzq->der->info.tipo == TIPO_INT)
        fprintf(pfCASM,"%s %s\n", devolverInsInt(instruccion), pSubArbolIzq->der->info.contenido);
    else
        fprintf(pfCASM,"%s %s\n", instruccion, pSubArbolIzq->der->info.contenido);
}

 int operacionAss(const char *operacion,const char *instruccion,t_arbol pSubArbolIzq,int cantAux)
{
    fprintf(pfCASM,";Se %s %s a %s\n",operacion,pSubArbolIzq->der->info.contenido, pSubArbolIzq->izq->info.contenido);
    if(!strcmp(instruccion,"FDIV"))
        comprobarDivisorCero(pSubArbolIzq);
    subirAlCopro(pSubArbolIzq, "FLD");  //SUBE LOS OPERANDOS AL COPRO
    fprintf(pfCASM,"%s\n",instruccion);
    if(pSubArbolIzq->info.tipo==TIPO_INT)
         fprintf(pfCASM,"FISTP %s\n", pSubArbolIzq->info.contenido);
    else
        fprintf(pfCASM,"FSTP %s\n", pSubArbolIzq->info.contenido);
    /*Se agrega el auxiliar a la TS y a la declaracion en ASM*/
    agregarAux(cantAux, pSubArbolIzq);
    /*Se uso otra variable auxiliar*/
    cantAux++;
    return cantAux;
}


/*Coloca el auxiliar en el info.contenido del nodo*/
void ponerAuxANodo(char *contenido,int cantAux)
{
    strcpy(contenido,VAR_AUX);
    if(cantAux<=9)
    {
        contenido[LONG_AUX]='0'+cantAux;
        contenido[LONG_AUX+1]='\0';
    }
    else if (cantAux>9 && cantAux <= 99)
    {
        contenido[LONG_AUX]='0'+cantAux/10;
        contenido[LONG_AUX+1]='0'+cantAux%10;
        contenido[LONG_AUX+2]='\0';
    }
}

void agregarAux(int nroAux, t_arbol pSubArbolIzq)
{
    /*Agregar a TS (para un control)*/
    /*Nombre*/
	ts_t auxTabla;
	strcpy(auxTabla.nombre, VAR_AUX);
	
    if(nroAux<=9)
    {
        auxTabla.nombre[LONG_AUX]='0'+nroAux;
		auxTabla.nombre[LONG_AUX+1]='\0';
    }
    else if (nroAux>9 && nroAux <= 99)
    {
        auxTabla.nombre[LONG_AUX]='0'+nroAux/10;
        auxTabla.nombre[LONG_AUX+1]='0'+nroAux%10;
        auxTabla.nombre[LONG_AUX+2]='\0';
    }   
	auxTabla.longitud=0;
	strcpy(auxTabla.valor.valCteStr,pSubArbolIzq->info.contenido);
	auxTabla.tipo=pSubArbolIzq->info.tipo;

	InsertarEnTS(&tabla,&auxTabla);
	
	/*Agregar a declaracion de variables en assembler*/
	fprintf(pfDASM,"\t%s dd 0\n",auxTabla.nombre);
	
	cantEnTabla++;
}

void comprobarDivisorCero(t_arbol pSubArbolIzq)
{
    fprintf(pfCASM,";Se comprueba si es cero el divisor\n");
    fprintf(pfCASM,"FLD _zero\n");
    fprintf(pfCASM,"FLD %s\n", pSubArbolIzq->der->info.contenido);
    escribInstrucComparar();
    fprintf(pfCASM,"JE errorDivision\n");
}

void imprimirError()
{
    fprintf(pfCASM,"errorDivision:\n");
    fprintf(pfCASM,"MOV DX, OFFSET msgError1\n");
    fprintf(pfCASM,"MOV ah, 9\n");
    fprintf(pfCASM,"int 21h\n");
    /*Imprimir fin de linea*/
    fprintf(pfCASM,"MOV DX, OFFSET ENDL\n");
    fprintf(pfCASM,"MOV ah, 9\n");
    fprintf(pfCASM,"int 21h\n");
    fprintf(pfCASM,"JMP finProg\n");
    //Error de Overflow
    fprintf(pfCASM,"errorOverflow:\n");
    fprintf(pfCASM,"MOV DX, OFFSET msgError2\n");
    fprintf(pfCASM,"MOV ah, 9\n");
    fprintf(pfCASM,"int 21h\n");
    /*Imprimir fin de linea*/
    fprintf(pfCASM,"MOV DX, OFFSET ENDL\n");
    fprintf(pfCASM,"MOV ah, 9\n");
    fprintf(pfCASM,"int 21h\n");
    fprintf(pfCASM,"JMP finProg\n");
    //Error de limite del vector
     fprintf(pfCASM,"errorLimite:\n");
    fprintf(pfCASM,"MOV DX, OFFSET msgError3\n");
    fprintf(pfCASM,"MOV ah, 9\n");
    fprintf(pfCASM,"int 21h\n");
    /*Imprimir fin de linea*/
    fprintf(pfCASM,"MOV DX, OFFSET ENDL\n");
    fprintf(pfCASM,"MOV ah, 9\n");
    fprintf(pfCASM,"int 21h\n");
}

t_arbol buscarSubArbolIzq (t_arbol pArbol)
{
    t_arbol nodo;

    if(pArbol)
    {
        /*Si es una hoja lo descarta*/
        if((!(pArbol->izq))&&(!(pArbol->der)))
            return NULL;

        /*Si es un subarbol con nodos hoja*/
        if((!(pArbol->izq->izq))&&(!(pArbol->izq->der))&&
            (!(pArbol->der->izq))&&(!(pArbol->der->der)))
            return pArbol;
        nodo=buscarSubArbolIzq(pArbol->izq);
        if(!nodo)
            nodo=buscarSubArbolIzq(pArbol->der);
        return nodo;
    }
    return NULL;
}

const char *devolverInsInt(const char *instruccion)
{
    if (!strcmp("FLD",instruccion))
        return "FILD";
    if (!strcmp("FSTP",instruccion))
        return "FISTP";
}

const char *devolverTipoLista(int tipo)
{
	if (tipo == TIPO_INT)
		return "FILD";
	else
		return "FLD";
}

const char *devolverTipoListaDescarga(int tipo)
{
	if (tipo == TIPO_INT)
		return "FISTP";
	else
		return "FSTP";
}

/*Limpiar condicional*/
void limpiarSecCond()
{
    int i,j;
    for(i=0;i<6;i++)
        for(j=0;j<MAX_TOKEN;j++)
            secCond[i][j]='\0';
}

void darVueltaHijos(t_arbol pSubArbolIzq)
{
	//puts("darVueltaHijos");
    t_arbol aux=pSubArbolIzq->der;
    pSubArbolIzq->der=pSubArbolIzq->izq;
    pSubArbolIzq->izq=aux;
}

char * negarComp(char *cad)
{
    if(strcmp(cad,">")==0)
        return "<=";
    if(strcmp(cad,"<")==0)
        return ">=";
    if(strcmp(cad,">=")==0)
        return "<";
    if(strcmp(cad,"<=")==0)
        return ">";
    if(strcmp(cad,"==")==0)
        return "!=";
    if(strcmp(cad,"!=")==0)
        return "==";
}FILE *yytfilep;
char *yytfilen;
int yytflag = 0;
int svdprd[2];
char svdnams[2][2];

int yyexca[] = {
  -1, 1,
  0, -1,
  -2, 0,
  0,
};

#define YYNPROD 85
#define YYLAST 370

int yyact[] = {
      18,      58,      59,      60,      31,      18,      58,      59,
      60,      31,      37,      27,      28,      29,      69,      70,
     137,      18,      27,      28,      29,       3,      20,      18,
      91,      92,     128,     122,      34,      18,      40,      81,
      82,     103,      43,      49,      50,      51,      66,      68,
      49,      50,      51,      14,      15,      81,      82,      68,
      17,      14,      15,      31,      13,      18,      17,      14,
      15,      81,      82,      87,      17,      81,      82,      48,
      42,      88,      83,      84,      85,      86,     118,     127,
     118,      25,     122,     119,      18,      58,      59,      60,
      31,      18,      53,      80,      60,      16,       7,      56,
      16,      16,      79,       6,      22,      41,      78,      77,
      67,      21,      38,      53,      76,     102,      75,       4,
      16,      55,      54,      90,      71,       7,      23,      52,
      25,       6,      89,      74,     133,      73,      63,      44,
       5,      47,      46,      64,      45,      36,      61,      21,
      72,     117,     100,      39,      26,     116,     104,     105,
      69,     107,     108,      24,       4,      33,      12,      11,
      10,       9,      65,       8,     101,      19,       2,       1,
      56,      62,      65,      41,       0,       0,      97,       0,
      98,      99,       0,       0,       0,       0,       0,       0,
     139,      70,      42,      74,      48,       0,       0,       0,
       0,      96,       0,       0,       0,       0,      57,       0,
     102,      16,       0,       0,       0,       0,       0,       0,
     106,     114,      63,      64,      96,     132,      16,     115,
     126,     109,     110,      16,     147,       0,       0,      22,
      16,     120,       0,     138,      22,     111,     112,     113,
      72,     137,       0,     148,     127,      90,      52,     120,
       0,      16,      93,       0,       0,       0,      16,      94,
      95,     129,      22,     143,     125,     145,     146,     142,
     140,     115,     141,      67,     102,       0,     142,     135,
     136,     134,     134,     134,     131,     133,     139,       0,
       0,       0,       0,     144,       0,       0,       0,     147,
      44,       0,       0,     138,       5,     140,     141,       0,
     135,      57,     136,     116,     129,       0,     130,     131,
     121,     123,     124,       0,     123,     124,      65,       0,
      30,      32,     112,      35,     113,     139,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
      15,       0,      17,       0,       0,      49,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,      93,      94,      95,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,      50,
      51,      57,
};

int yypact[] = {
    -261,   -1000,    -234,   -1000,    -259,    -234,    -234,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,    -284,    -210,    -210,
    -243,    -210,   -1000,   -1000,   -1000,    -234,   -1000,   -1000,
    -277,   -1000,    -239,   -1000,   -1000,   -1000,    -257,   -1000,
    -257,    -181,   -1000,    -176,   -1000,   -1000,   -1000,    -225,
   -1000,    -223,    -275,    -252,    -211,   -1000,   -1000,   -1000,
    -250,    -210,    -210,    -210,   -1000,   -1000,   -1000,   -1000,
   -1000,    -181,   -1000,   -1000,   -1000,    -223,    -227,    -223,
    -223,    -204,   -1000,    -232,   -1000,    -252,    -252,   -1000,
    -181,    -181,    -181,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,    -181,    -181,   -1000,   -1000,    -181,    -181,    -181,
    -215,    -232,   -1000,   -1000,    -192,   -1000,    -228,   -1000,
   -1000,   -1000,    -227,    -250,    -250,   -1000,   -1000,    -241,
    -241,    -241,   -1000,    -228,   -1000,    -204,   -1000,   -1000,
    -240,    -225,   -1000,    -225,    -225,    -240,   -1000,   -1000,
   -1000,    -181,    -181,    -181,    -269,    -194,    -227,    -194,
    -194,    -232,    -223,    -181,    -223,    -223,    -228,   -1000,
    -227,   -1000,   -1000,    -240,   -1000,
};

int yypgo[] = {
       0,     151,     150,     103,     149,     120,      91,      86,
     147,     145,     144,     143,     142,     273,      93,      96,
     101,      71,      82,     141,     119,     139,      73,     132,
     131,     154,     130,     133,     129,      87,      64,     128,
     124,     122,     121,     168,     116,     117,      63,     115,
     114,     111,     107,     106,     105,     102,     100,      95,
      94,      90,      83,
};

int yyr1[] = {
       0,       1,       3,       3,       3,       3,       3,       5,
       5,       6,       7,       7,       7,       7,       9,      10,
      10,      11,       8,      21,      21,      22,      26,      26,
      12,      12,      14,      14,      14,      14,      30,      30,
      30,      30,      32,      33,      34,      36,      36,      20,
      20,      20,      38,      38,      38,      41,      41,      41,
      41,      41,      31,      31,      31,      31,      31,      31,
      43,      44,      29,      18,       2,       4,      13,      15,
      25,      27,      16,      17,      28,      35,      24,      37,
      39,      40,      42,      19,      45,      46,      47,      48,
      49,      50,      23,      23,      23,
};

int yyr2[] = {
       2,       3,       1,       3,       2,       1,       2,       1,
       2,       1,       1,       1,       1,       1,       7,       7,
      11,       3,       3,       1,       2,       5,       3,       1,
       4,       4,       1,       3,       3,       2,       3,       1,
       1,       1,       8,       8,       8,       3,       1,       3,
       3,       1,       3,       3,       1,       1,       1,       1,
       1,       3,       1,       1,       1,       1,       1,       1,
       1,       1,       1,       1,       1,       1,       1,       1,
       1,       1,       1,       1,       1,       1,       1,       1,
       1,       1,       1,       1,       1,       1,       1,       1,
       1,       1,       1,       1,       1,
};

int yychk[] = {
   -1000,      -1,      -2,     282,      -3,      -5,      -6,      -7,
      -8,      -9,     -10,     -11,     -12,     286,     283,     284,
     -18,     288,     257,      -4,     281,      -6,      -7,      -3,
     -21,     -22,     -23,     295,     296,     297,     -13,     261,
     -13,     -19,     271,     -13,      -3,     287,     -22,     -24,
     269,     -14,     -30,     291,     -20,     -32,     -33,     -34,
     -38,     292,     293,     294,     -41,     -18,     -43,     -44,
     -29,     -13,     258,     259,     260,     -14,     -20,     -18,
     -29,     -25,     263,     -15,     262,     289,     290,     -30,
     -31,     -37,     -39,     -45,     -46,     -47,     -48,     -49,
     -50,     272,     273,     277,     278,     279,     280,     270,
     276,     -40,     -42,     274,     275,     -13,     -13,     -13,
     -20,     -15,     -15,     -15,     -26,     -18,     -16,     265,
     -30,     -30,     -20,     -38,     -38,     -41,     -41,     -20,
     -20,     -20,     -15,     -16,     -27,     -28,     264,     267,
      -5,     -35,     268,     -35,     -35,      -5,     -18,     -17,
     266,     -25,     -25,     -25,     -17,     -36,     -20,     -36,
     -36,     285,     -27,     -35,     -27,     -27,     -16,     -15,
     -20,     -15,     -15,      -5,     -17,
};

int yydef[] = {
       0,      -2,       0,      60,       0,       2,       5,       7,
       9,      10,      11,      12,      13,       0,       0,       0,
       0,       0,      59,       1,      61,       4,       8,       6,
       0,      19,       0,      82,      83,      84,       0,      62,
       0,       0,      75,       0,       3,      18,      20,       0,
      70,       0,      26,       0,       0,      31,      32,      33,
      41,       0,       0,       0,      44,      45,      46,      47,
      48,       0,      56,      57,      58,       0,      17,       0,
       0,       0,      64,       0,      63,       0,       0,      29,
       0,       0,       0,      50,      51,      52,      53,      54,
      55,      71,      72,      76,      77,      78,      79,      80,
      81,       0,       0,      73,      74,       0,       0,       0,
       0,       0,      24,      25,       0,      23,       0,      66,
      27,      28,      30,      39,      40,      42,      43,       0,
       0,       0,      49,       0,      21,       0,      65,      68,
       0,       0,      69,       0,       0,       0,      22,      14,
      67,       0,       0,       0,      15,       0,      38,       0,
       0,       0,       0,       0,       0,       0,       0,      34,
      37,      35,      36,       0,      16,
};

int *yyxi;


/*****************************************************************/
/* PCYACC LALR parser driver routine -- a table driven procedure */
/* for recognizing sentences of a language defined by the        */
/* grammar that PCYACC analyzes. An LALR parsing table is then   */
/* constructed for the grammar and the skeletal parser uses the  */
/* table when performing syntactical analysis on input source    */
/* programs. The actions associated with grammar rules are       */
/* inserted into a switch statement for execution.               */
/*****************************************************************/


#ifndef YYMAXDEPTH
#define YYMAXDEPTH 200
#endif
#ifndef YYREDMAX
#define YYREDMAX 1000
#endif
#define PCYYFLAG -1000
#define WAS0ERR 0
#define WAS1ERR 1
#define WAS2ERR 2
#define WAS3ERR 3
#define yyclearin pcyytoken = -1
#define yyerrok   pcyyerrfl = 0
YYSTYPE yyv[YYMAXDEPTH];     /* value stack */
int pcyyerrct = 0;           /* error count */
int pcyyerrfl = 0;           /* error flag */
int redseq[YYREDMAX];
int redcnt = 0;
int pcyytoken = -1;          /* input token */


yyparse()
{
  int statestack[YYMAXDEPTH]; /* state stack */
  int      j, m;              /* working index */
  YYSTYPE *yypvt;
  int      tmpstate, tmptoken, *yyps, n;
  YYSTYPE *yypv;


  tmpstate = 0;
  pcyytoken = -1;
#ifdef YYDEBUG
  tmptoken = -1;
#endif
  pcyyerrct = 0;
  pcyyerrfl = 0;
  yyps = &statestack[-1];
  yypv = &yyv[-1];


  enstack:    /* push stack */
#ifdef YYDEBUG
    printf("at state %d, next token %d\n", tmpstate, tmptoken);
#endif
    if (++yyps - &statestack[YYMAXDEPTH] > 0) {
      yyerror("pcyacc internal stack overflow");
      return(1);
    }
    *yyps = tmpstate;
    ++yypv;
    *yypv = yyval;


  newstate:
    n = yypact[tmpstate];
    if (n <= PCYYFLAG) goto defaultact; /*  a simple state */


    if (pcyytoken < 0) if ((pcyytoken=yylex()) < 0) pcyytoken = 0;
    if ((n += pcyytoken) < 0 || n >= YYLAST) goto defaultact;


    if (yychk[n=yyact[n]] == pcyytoken) { /* a shift */
#ifdef YYDEBUG
      tmptoken  = pcyytoken;
#endif
      pcyytoken = -1;
      yyval = yylval;
      tmpstate = n;
      if (pcyyerrfl > 0) --pcyyerrfl;
      goto enstack;
    }


  defaultact:


    if ((n=yydef[tmpstate]) == -2) {
      if (pcyytoken < 0) if ((pcyytoken=yylex())<0) pcyytoken = 0;
      for (yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=tmpstate); yyxi += 2);
      while (*(yyxi+=2) >= 0) if (*yyxi == pcyytoken) break;
      if ((n=yyxi[1]) < 0) { /* an accept action */
        if (yytflag) {
          int ti; int tj;
          yytfilep = fopen(yytfilen, "w");
          if (yytfilep == NULL) {
            fprintf(stderr, "Can't open t file: %s\n", yytfilen);
            return(0);          }
          for (ti=redcnt-1; ti>=0; ti--) {
            tj = svdprd[redseq[ti]];
            while (strcmp(svdnams[tj], "$EOP"))
              fprintf(yytfilep, "%s ", svdnams[tj++]);
            fprintf(yytfilep, "\n");
          }
          fclose(yytfilep);
        }
        return (0);
      }
    }


    if (n == 0) {        /* error situation */
      switch (pcyyerrfl) {
        case WAS0ERR:          /* an error just occurred */
          yyerror("syntax error");
          yyerrlab:
            ++pcyyerrct;
        case WAS1ERR:
        case WAS2ERR:           /* try again */
          pcyyerrfl = 3;
	   /* find a state for a legal shift action */
          while (yyps >= statestack) {
	     n = yypact[*yyps] + YYERRCODE;
	     if (n >= 0 && n < YYLAST && yychk[yyact[n]] == YYERRCODE) {
	       tmpstate = yyact[n];  /* simulate a shift of "error" */
	       goto enstack;
            }
	     n = yypact[*yyps];


	     /* the current yyps has no shift on "error", pop stack */
#ifdef YYDEBUG
            printf("error: pop state %d, recover state %d\n", *yyps, yyps[-1]);
#endif
	     --yyps;
	     --yypv;
	   }


	   yyabort:
            if (yytflag) {
              int ti; int tj;
              yytfilep = fopen(yytfilen, "w");
              if (yytfilep == NULL) {
                fprintf(stderr, "Can't open t file: %s\n", yytfilen);
                return(1);              }
              for (ti=1; ti<redcnt; ti++) {
                tj = svdprd[redseq[ti]];
                while (strcmp(svdnams[tj], "$EOP"))
                  fprintf(yytfilep, "%s ", svdnams[tj++]);
                fprintf(yytfilep, "\n");
              }
              fclose(yytfilep);
            }
	     return(1);


	 case WAS3ERR:  /* clobber input char */
#ifdef YYDEBUG
          printf("error: discard token %d\n", pcyytoken);
#endif
          if (pcyytoken == 0) goto yyabort; /* quit */
	   pcyytoken = -1;
	   goto newstate;      } /* switch */
    } /* if */


    /* reduction, given a production n */
#ifdef YYDEBUG
    printf("reduce with rule %d\n", n);
#endif
    if (yytflag && redcnt<YYREDMAX) redseq[redcnt++] = n;
    yyps -= yyr2[n];
    yypvt = yypv;
    yypv -= yyr2[n];
    yyval = yypv[1];
    m = n;
    /* find next state from goto table */
    n = yyr1[n];
    j = yypgo[n] + *yyps + 1;
    if (j>=YYLAST || yychk[ tmpstate = yyact[j] ] != -n) tmpstate = yyact[yypgo[n]];
    switch (m) { /* actions associated with grammar rules */
      
      case 1:
# line 392 "Grupo123.y"
      {
      	p_prg=desapilarPrograma();
      	pfArbol=fopen("Intermedia.txt","wt");
      	if(!pfArbol)
      		TerminarAplicacion(120, "No se pudo crear el archivo Intermedia.txt");
          fprintf(pfArbol,"%d\n",cantNodos);
      	GrabarArbol(p_prg,pfArbol);
      	fclose(pfArbol);
      	GenerarAssembler(p_prg);
      	ConcatenarAssembler();
      	EliminarArbol(p_prg);
      } break;
      case 2:
# line 406 "Grupo123.y"
      {
          p_programa=apilarPrograma(p_lista_sentencias);   
      } break;
      case 3:
# line 410 "Grupo123.y"
      {
      	t_info datosNodo;
      	strcpy(datosNodo.contenido,CUERPO_PROGRAMA);
          p_programa=apilarPrograma(CrearNodo(datosNodo,desapilarListaSentencias(),desapilarListaSentencias()));
      } break;
      case 4:
# line 416 "Grupo123.y"
      {
          p_programa=apilarPrograma(p_lista_sentencias);
      } break;
      case 6:
# line 421 "Grupo123.y"
      {
      } break;
      case 7:
# line 425 "Grupo123.y"
      {
      	p_lista_sentencias=apilarListaSentencias(p_sentencia);
      } break;
      case 8:
# line 429 "Grupo123.y"
      {
      	t_info datosNodo;
      	strcpy(datosNodo.contenido,CUERPO_PROGRAMA);
          p_lista_sentencias=apilarListaSentencias(CrearNodo(datosNodo,desapilarListaSentencias(),p_sentencia));
      } break;
      case 10:
# line 437 "Grupo123.y"
      {p_sentencia=p_iteracion;} break;
      case 11:
# line 438 "Grupo123.y"
      {p_sentencia=p_seleccion;} break;
      case 12:
# line 439 "Grupo123.y"
      {p_sentencia=p_asignacion;} break;
      case 13:
# line 440 "Grupo123.y"
      {p_sentencia=p_output;} break;
      case 14:
# line 443 "Grupo123.y"
      {
      	t_info datosNodo;
      	t_arbol arbolAux;
      	datosNodo.tipo = TIPO_WHILE;
      	strcpy(datosNodo.contenido,"WHILE");
      	p_iteracion=CrearNodo(datosNodo,desapilarCondicion(),desapilarListaSentencias());
      	
          arbolAux = buscarSubArbolIzq(p_iteracion);
          arbolAux->info.esWhile = TIPO_WHILE;
      } break;
      case 15:
# line 455 "Grupo123.y"
      {
      	t_info datosNodoIf;
      	datosNodoIf.tipo = TIPO_IF;
      	strcpy(datosNodoIf.contenido,"IF");
      	p_seleccion=CrearNodo(datosNodoIf,desapilarCondicion(),desapilarListaSentencias());
      } break;
      case 16:
# line 462 "Grupo123.y"
      {
      	t_info datosNodoIf;
      	t_info datosNodoElse;
      	datosNodoIf.tipo = TIPO_IF;
      	strcpy(datosNodoIf.contenido,"IF");
      	strcpy(datosNodoElse.contenido,CUERPO_ELSE);
      	p_seleccion=CrearNodo(datosNodoIf,desapilarCondicion(),CrearNodo(datosNodoElse,desapilarListaSentencias(),desapilarListaSentencias()));    
      } break;
      case 17:
# line 472 "Grupo123.y"
      {
      	ts_t auxNodo;
      	t_arbol p_expresion_aux;
      	t_info datosNodo;
      	ObtenerItemTS(&tabla, yypvt[-2], &auxNodo);
      	if(fueDeclarado(auxNodo.tipo))
      	{
      		p_expresion_aux=desapilarExpresion();
      		if(auxNodo.tipo==p_expresion_aux->info.tipo)                                 
      		{
      			strcpy(datosNodo.contenido,"=");
      			datosNodo.tipo=p_expresion_aux->info.tipo;
      			p_asignacion=CrearNodo(datosNodo,CrearHoja(auxNodo.nombre,auxNodo.tipo),p_expresion_aux);
      		}
      		else
      			TerminarAplicacion(80, "No puede realizarse una asignacion entre valores de distintos tipos");
      	}
      	else
      		TerminarAplicacion(70, "La variable a la cual se aplica la asignacion no fue declarada");
      } break;
      case 21:
# line 499 "Grupo123.y"
      {
      	ts_t auxNodo;
      	int var = -1;
      	var = desapilarVariable();
      	int auxTipo;
      	while (var != -1)
      	{
      		ObtenerItemTS(&tabla, var, &auxNodo);
      		if(fueDeclarado(auxNodo.tipo))
      			TerminarAplicacion(130, "Variable declarada anteriormente");
      		else
      		{
      			switch(yypvt[-4])
      			{
      				case INT: 		
      						auxTipo=TIPO_INT;
      						ModificarTipoDato(&tabla, var ,auxTipo);
      						break;
      				case STRING: 	
      						auxTipo=TIPO_STRING;
      						ModificarTipoDato(&tabla, var ,auxTipo);
      						break;
      				case REAL: 		
      						auxTipo=TIPO_REAL;
      						ModificarTipoDato(&tabla, var ,auxTipo);
      						break;
      				default: break;
      			}
      		}
      		var = desapilarVariable();
      	}
      } break;
      case 22:
# line 533 "Grupo123.y"
      {
      	t_info datosNodo;
      	strcpy(datosNodo.contenido,",");
      	datosNodo.tipo=TIPO_NINGUNO;	
      	apilarVariable(yypvt[-0]);	
      } break;
      case 23:
# line 540 "Grupo123.y"
      {
      	apilarVariable(yypvt[-0]);
      } break;
      case 24:
# line 545 "Grupo123.y"
      {
      	ts_t auxNodo;
      	t_info datosNodo;
      	strcpy(datosNodo.contenido,"OUT");
      	datosNodo.tipo=TIPO_NINGUNO;	        
      	ObtenerItemTS(&tabla, yypvt[-1], &auxNodo);
      	if(fueDeclarado(auxNodo.tipo))
      	{
      		if (auxNodo.tipo != TIPO_REAL)
      			p_output=CrearNodo(datosNodo,CrearHoja(auxNodo.nombre,auxNodo.tipo),CrearHoja(HOJA_VACIA,TIPO_NINGUNO));
      		else
      			TerminarAplicacion(180, "No se pueden imprimir variables tipo real por pantalla");
      	}
      	else
      		TerminarAplicacion(60, "La variable no fue declarada");
      } break;
      case 25:
# line 562 "Grupo123.y"
      {
      	ts_t auxNodo;
      	t_info datosNodo;
      	strcpy(datosNodo.contenido,"OUT");
      	datosNodo.tipo=TIPO_NINGUNO;	                                            
      	ObtenerItemTS(&tabla, yypvt[-1], &auxNodo);	
      	p_output=CrearNodo(datosNodo,CrearHoja(auxNodo.nombre,auxNodo.tipo),CrearHoja(HOJA_VACIA,TIPO_NINGUNO));
      } break;
      case 26:
# line 572 "Grupo123.y"
      {
      	p_condicion=apilarCondicion(desapilarComparacion());
      } break;
      case 27:
# line 576 "Grupo123.y"
      {
      	t_info datosNodo;
      	strcpy(datosNodo.contenido,"AND");
      	datosNodo.tipo=TIPO_NINGUNO;
      	
      	p_condicion=apilarCondicion(CrearNodo(datosNodo,desapilarComparacion(),desapilarComparacion())); 
      } break;
      case 28:
# line 584 "Grupo123.y"
      {
      	t_info datosNodo;
      	strcpy(datosNodo.contenido,"OR");
      	datosNodo.tipo=TIPO_NINGUNO;
      	
      	p_condicion=apilarCondicion(CrearNodo(datosNodo,desapilarComparacion(),desapilarComparacion())); 
      } break;
      case 29:
# line 592 "Grupo123.y"
      {
      	t_info datosNodo;
      	t_arbol auxComp;
      	
      	auxComp=desapilarComparacion();
      	if (strcmp(auxComp->info.contenido,"INLIST") &&
      		strcmp(auxComp->info.contenido,"EQUMAX") &&
      		strcmp(auxComp->info.contenido,"EQUMIN"))
      	{
      		strcpy(auxComp->info.contenido,negarComp(auxComp->info.contenido));
      	}
      	else
      	{
      		auxComp->info.tipo = TIPO_NEGADO;
      	}		
      	p_condicion=apilarCondicion(auxComp);
      } break;
      case 30:
# line 611 "Grupo123.y"
      {
      	t_info datosNodo;
      	switch(yypvt[-1])
      	{
      		case MENOR: strcpy(datosNodo.contenido,"<");
      							break;
      		case MENOR_IGUAL : strcpy(datosNodo.contenido,"<=");
      							break;
      		case MAYOR : strcpy(datosNodo.contenido,">");
      							break;
      		case MAYOR_IGUAL : strcpy(datosNodo.contenido,">=");
      							break;
      		case IGUAL : strcpy(datosNodo.contenido,"==");
      							break;
      		case DISTINTO : strcpy(datosNodo.contenido,"!=");
      							break;
      		default: break;
      	}
      	datosNodo.tipo=TIPO_NINGUNO;
      	p_comparacion=apilarComparacion(CrearNodo(datosNodo,desapilarExpresion(),desapilarExpresion()));
      	if  (!verificarTipoComparacion(p_comparacion->izq->info.tipo,p_comparacion->der->info.tipo))
      		TerminarAplicacion(50, "No se puede realizar comparaciones con strings");
      } break;
      case 31:
# line 634 "Grupo123.y"
      {p_comparacion=apilarComparacion(p_en_lista);} break;
      case 32:
# line 635 "Grupo123.y"
      {p_comparacion=apilarComparacion(p_max_lista);} break;
      case 33:
# line 636 "Grupo123.y"
      {p_comparacion=apilarComparacion(p_min_lista);} break;
      case 34:
# line 639 "Grupo123.y"
      {
      	t_info datosNodo;
      	t_arbol p_expresion_aux;
      	
      	p_expresion_aux=desapilarExpresion();                     
      	if(p_expresion_aux->info.tipo!=TIPO_STRING) 
      	{
      		if (p_lista_expresiones->info.tipo != TIPO_NINGUNO && p_expresion_aux->info.tipo == p_lista_expresiones->info.tipo
      			|| (p_lista_expresiones->der != NULL && p_lista_expresiones->der->info.tipo == p_expresion_aux->info.tipo)) 
      		{
      			strcpy(datosNodo.contenido,"INLIST");
      			datosNodo.tipo=TIPO_NINGUNO;	
      			p_en_lista=CrearNodo(datosNodo,p_expresion_aux,p_lista_expresiones);
      		}
      		else
      			TerminarAplicacion(140, "La expresion a comparar en INLIST debe ser del mismo tipo que la lista de expresiones");
      	}
      	else
      		TerminarAplicacion(40, "Constante o variable string no es un elemento valido de la lista de variables");
      } break;
      case 35:
# line 661 "Grupo123.y"
      {
      	t_info datosNodo;
      	t_arbol p_expresion_aux;
      	
      	p_expresion_aux=desapilarExpresion();                     
      	if(p_expresion_aux->info.tipo!=TIPO_STRING) 
      	{
      		if (p_lista_expresiones->info.tipo != TIPO_NINGUNO && p_expresion_aux->info.tipo == p_lista_expresiones->info.tipo
      			|| (p_lista_expresiones->der != NULL && p_lista_expresiones->der->info.tipo == p_expresion_aux->info.tipo)) 
      		{
      			strcpy(datosNodo.contenido,"EQUMAX");
      			datosNodo.tipo=TIPO_NINGUNO;		
      			p_max_lista=CrearNodo(datosNodo,p_expresion_aux,p_lista_expresiones);
      		}
      		else
      			TerminarAplicacion(140, "La expresion a comparar en EQUMAX debe ser del mismo tipo que la lista de expresiones");
      	}
      	else
      		TerminarAplicacion(40, "Constante o variable string no es un elemento valido de la lista de variables");
      } break;
      case 36:
# line 683 "Grupo123.y"
      {
      	t_info datosNodo;
      	t_arbol p_expresion_aux;
      	
      	p_expresion_aux=desapilarExpresion();                     
      	if(p_expresion_aux->info.tipo!=TIPO_STRING) 
      	{
      		if (p_lista_expresiones->info.tipo != TIPO_NINGUNO && p_expresion_aux->info.tipo == p_lista_expresiones->info.tipo
      			|| (p_lista_expresiones->der != NULL && p_lista_expresiones->der->info.tipo == p_expresion_aux->info.tipo)) 
      		{
      			strcpy(datosNodo.contenido,"EQUMIN");
      			datosNodo.tipo=TIPO_NINGUNO;			
      			p_min_lista=CrearNodo(datosNodo,p_expresion_aux,p_lista_expresiones);
      		}
      		else
      			TerminarAplicacion(140, "La expresion a comparar en EQUMIN debe ser del mismo tipo que la lista de expresiones");
      	}
      	else
      		TerminarAplicacion(40, "Constante o variable string no es un elemento valido de la lista de variables");
      } break;
      case 37:
# line 705 "Grupo123.y"
      {
      	t_info datosNodo;
      	t_arbol p_expresion_aux;
      	
      	p_expresion_aux=desapilarExpresion();                     
      	if(p_expresion_aux->info.tipo!=TIPO_STRING) 
      	{	
      		if (p_lista_expresiones->info.tipo != TIPO_NINGUNO && p_expresion_aux->info.tipo == p_lista_expresiones->info.tipo
      			|| (p_lista_expresiones->der != NULL && p_lista_expresiones->der->info.tipo == p_expresion_aux->info.tipo)) 			
      		{
      			strcpy(datosNodo.contenido,";");
      			datosNodo.tipo=TIPO_NINGUNO;
      			p_lista_expresiones=CrearNodo(datosNodo,p_lista_expresiones,p_expresion_aux);
      		}
      		else
      			TerminarAplicacion(150, "Las expresiones dentro de la lista de expresiones deben ser del mismo tipo");
      	}
      	else
      		TerminarAplicacion(40, "Constante string no es un elemento valido de la lista de variables");
      } break;
      case 38:
# line 727 "Grupo123.y"
      {
      	t_arbol p_expresion_aux;
      	
      	p_expresion_aux=desapilarExpresion();                
      	if(p_expresion_aux->info.tipo!=TIPO_STRING)                            
      		p_lista_expresiones=p_expresion_aux;                
      	else
      		TerminarAplicacion(40, "Constante string no es un elemento valido de la lista de variables");
      } break;
      case 39:
# line 738 "Grupo123.y"
      { 
      	int tipo;
      	t_info datosNodo;
      	t_arbol p_expresion_aux, p_termino_aux;
      	p_expresion_aux=desapilarExpresion();
      	p_termino_aux=desapilarTermino();
      	
      	tipo=tipoDelResultado(OP_SUMA,p_expresion_aux->info.tipo,p_termino_aux->info.tipo);
      	strcpy(datosNodo.contenido,"+");
      	datosNodo.tipo=tipo;
      	p_expresion = apilarExpresion(CrearNodo(datosNodo,p_expresion_aux,p_termino_aux)) ;
      } break;
      case 40:
# line 751 "Grupo123.y"
      {	
      	int tipo;
      	t_info datosNodo;
      	t_arbol p_expresion_aux, p_termino_aux;
      	p_expresion_aux=desapilarExpresion();
      	p_termino_aux=desapilarTermino();
      	
      	tipo=tipoDelResultado(OP_RESTA,p_expresion_aux->info.tipo,p_termino_aux->info.tipo);
      	strcpy(datosNodo.contenido,"-");
      	datosNodo.tipo=tipo;
      	p_expresion = apilarExpresion(CrearNodo(datosNodo,p_expresion_aux,p_termino_aux));
      } break;
      case 41:
# line 764 "Grupo123.y"
      { 
      	p_expresion=apilarExpresion(desapilarTermino());
      } break;
      case 42:
# line 769 "Grupo123.y"
      {	
      	int tipo;
      	t_info datosNodo;
      
	tipo=tipoDelResultado(OP_MULTIPLICACION,p_termino->info.tipo,p_factor->info.tipo); 
      	strcpy(datosNodo.contenido,"*");
      	datosNodo.tipo=tipo;
      	p_termino=apilarTermino(CrearNodo(datosNodo,desapilarTermino(),p_factor));
      } break;
      case 43:
# line 779 "Grupo123.y"
      {   
      	int tipo;
      	t_info datosNodo;
      
	tipo=tipoDelResultado(OP_DIVISION,p_termino->info.tipo,p_factor->info.tipo); 
      	strcpy(datosNodo.contenido,"/");
      	datosNodo.tipo=tipo;
      	p_termino=apilarTermino(CrearNodo(datosNodo,desapilarTermino(),p_factor));
      } break;
      case 44:
# line 789 "Grupo123.y"
      {
      	p_termino=apilarTermino(p_factor);
      } break;
      case 45:
# line 794 "Grupo123.y"
      {	
      	ts_t auxNodo;														
      	ObtenerItemTS(&tabla, yypvt[-0], &auxNodo);
      	if(fueDeclarado(auxNodo.tipo))												
      		p_factor=CrearHoja(auxNodo.nombre,auxNodo.tipo);
      	else
      		TerminarAplicacion(30, "Variable no declarada");
      } break;
      case 46:
# line 803 "Grupo123.y"
      {	
      	ts_t auxNodo;														
      	ObtenerItemTS(&tabla, yypvt[-0], &auxNodo);
      	p_factor=CrearHoja(auxNodo.nombre,auxNodo.tipo);
      } break;
      case 47:
# line 809 "Grupo123.y"
      {
      	ts_t auxNodo;														
      	ObtenerItemTS(&tabla, yypvt[-0], &auxNodo);
      	p_factor=CrearHoja(auxNodo.nombre,auxNodo.tipo);
      } break;
      case 48:
# line 815 "Grupo123.y"
      {
      	ts_t auxNodo;														
      	ObtenerItemTS(&tabla, yypvt[-0], &auxNodo);
      	p_factor=CrearHoja(auxNodo.nombre,auxNodo.tipo);
      } break;
      case 49:
# line 822 "Grupo123.y"
      {
      	p_factor=desapilarExpresion();;
      } break;
      case 50:
# line 826 "Grupo123.y"
      {yyval=MENOR;} break;
      case 51:
# line 827 "Grupo123.y"
      {yyval=MENOR_IGUAL;} break;
      case 52:
# line 828 "Grupo123.y"
      {yyval=MAYOR;} break;
      case 53:
# line 829 "Grupo123.y"
      {yyval=MAYOR_IGUAL;} break;
      case 54:
# line 830 "Grupo123.y"
      {yyval=IGUAL;} break;
      case 55:
# line 831 "Grupo123.y"
      {yyval=DISTINTO;} break;
      case 56:
# line 833 "Grupo123.y"
      {yyval=yylval;} break;
      case 57:
# line 835 "Grupo123.y"
      {yyval=yylval;} break;
      case 58:
# line 837 "Grupo123.y"
      { yyval=yylval;} break;
      case 59:
# line 839 "Grupo123.y"
      { yyval=yylval;} break;
      case 60:
# line 843 "Grupo123.y"
      {} break;
      case 61:
# line 844 "Grupo123.y"
      {} break;
      case 62:
# line 845 "Grupo123.y"
      {} break;
      case 63:
# line 846 "Grupo123.y"
      {} break;
      case 64:
# line 847 "Grupo123.y"
      {} break;
      case 65:
# line 848 "Grupo123.y"
      {} break;
      case 66:
# line 849 "Grupo123.y"
      {} break;
      case 67:
# line 850 "Grupo123.y"
      {} break;
      case 68:
# line 851 "Grupo123.y"
      {} break;
      case 69:
# line 852 "Grupo123.y"
      {} break;
      case 70:
# line 853 "Grupo123.y"
      {} break;
      case 71:
# line 854 "Grupo123.y"
      {} break;
      case 72:
# line 855 "Grupo123.y"
      {} break;
      case 73:
# line 856 "Grupo123.y"
      {} break;
      case 74:
# line 857 "Grupo123.y"
      {} break;
      case 75:
# line 858 "Grupo123.y"
      {} break;
      case 76:
# line 859 "Grupo123.y"
      {} break;
      case 77:
# line 860 "Grupo123.y"
      {} break;
      case 78:
# line 861 "Grupo123.y"
      {} break;
      case 79:
# line 862 "Grupo123.y"
      {} break;
      case 80:
# line 863 "Grupo123.y"
      {} break;
      case 81:
# line 864 "Grupo123.y"
      {} break;
      case 82:
# line 865 "Grupo123.y"
      {yyval=STRING;} break;
      case 83:
# line 866 "Grupo123.y"
      {yyval=INT;} break;
      case 84:
# line 867 "Grupo123.y"
      {yyval=REAL;} break;    }
    goto enstack;
}
