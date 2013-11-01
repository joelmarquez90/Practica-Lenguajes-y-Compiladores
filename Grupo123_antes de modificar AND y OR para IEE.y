%{
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
void comparacionAssIEE(int *,t_arbol,const char *,const char *,int *,const char *);
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


%}


%token ID
%token CONST_ENTERA 
%token CONST_REAL
%token CONST_STRING 
%token P_ABRE 
%token P_CIERRA
%token C_ABRE
%token C_CIERRA 
%token L_ABRE 
%token L_CIERRA 
%token COMA
%token PUNTO_Y_COMA 
%token DOS_PUNTOS 
%token IGUAL 
%token OP_ASIGNACION 
%token OP_SUMA
%token OP_RESTA
%token OP_MULTIPLICACION 
%token OP_DIVISION
%token DISTINTO 
%token MENOR 
%token MENOR_IGUAL 
%token MAYOR 
%token MAYOR_IGUAL
%token END
%token BEGIN
%token WHILE 
%token IF 
%token ELSE 
%token DECLARE 
%token ENDDECLARE
%token OUT
%token AND
%token OR
%token NOT
%token INLIST 
%token EQUMAX
%token EQUMIN 
%token STRING
%token INT 
%token REAL

%left OP_SUMA OP_RESTA
%left OP_DIVISION OP_MULTIPLICACION
%right OP_ASIGNACION


%%

prg : begin programa end 
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
};

programa : lista_sentencias 
{
    p_programa=apilarPrograma(p_lista_sentencias);   
};
programa : lista_sentencias sentencia_dec programa
{
	t_info datosNodo;
	strcpy(datosNodo.contenido,CUERPO_PROGRAMA);
    p_programa=apilarPrograma(CrearNodo(datosNodo,desapilarListaSentencias(),desapilarListaSentencias()));
};
programa : lista_sentencias sentencia_dec
{
    p_programa=apilarPrograma(p_lista_sentencias);
};    
programa : sentencia_dec;
programa : sentencia_dec programa
{
};   

lista_sentencias : sentencia
{
	p_lista_sentencias=apilarListaSentencias(p_sentencia);
};
lista_sentencias : lista_sentencias sentencia
{
	t_info datosNodo;
	strcpy(datosNodo.contenido,CUERPO_PROGRAMA);
    p_lista_sentencias=apilarListaSentencias(CrearNodo(datosNodo,desapilarListaSentencias(),p_sentencia));
};

sentencia_dec : super_declaracion;

sentencia : iteracion {p_sentencia=p_iteracion;};
sentencia : seleccion {p_sentencia=p_seleccion;};
sentencia : asignacion {p_sentencia=p_asignacion;};
sentencia : output {p_sentencia=p_output;};

iteracion : WHILE p_abre condicion p_cierra l_abre lista_sentencias l_cierra 
{
	t_info datosNodo;
	t_arbol arbolAux;
	datosNodo.tipo = TIPO_WHILE;
	strcpy(datosNodo.contenido,"WHILE");
	p_iteracion=CrearNodo(datosNodo,desapilarCondicion(),desapilarListaSentencias());
	
    arbolAux = buscarSubArbolIzq(p_iteracion);
    arbolAux->esWhile = TIPO_WHILE;
};

seleccion : IF p_abre condicion p_cierra l_abre lista_sentencias l_cierra 
{
	t_info datosNodoIf;
	datosNodoIf.tipo = TIPO_IF;
	strcpy(datosNodoIf.contenido,"IF");
	p_seleccion=CrearNodo(datosNodoIf,desapilarCondicion(),desapilarListaSentencias());
};
seleccion : IF p_abre condicion p_cierra l_abre lista_sentencias l_cierra ELSE l_abre lista_sentencias l_cierra 
{
	t_info datosNodoIf;
	t_info datosNodoElse;
	datosNodoIf.tipo = TIPO_IF;
	strcpy(datosNodoIf.contenido,"IF");
	strcpy(datosNodoElse.contenido,CUERPO_ELSE);
	p_seleccion=CrearNodo(datosNodoIf,desapilarCondicion(),CrearNodo(datosNodoElse,desapilarListaSentencias(),desapilarListaSentencias()));    
};

asignacion : identificador op_asignacion expresion            
{
	ts_t auxNodo;
	t_arbol p_expresion_aux;
	t_info datosNodo;
	ObtenerItemTS(&tabla, $1, &auxNodo);
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
};

super_declaracion: DECLARE bloque_declaracion ENDDECLARE;

bloque_declaracion: declaracion;
bloque_declaracion: bloque_declaracion declaracion;

declaracion :  tipo_dato dos_puntos c_abre lista_variables c_cierra               
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
			switch($1)
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
};

lista_variables : lista_variables coma identificador 
{
	t_info datosNodo;
	strcpy(datosNodo.contenido,",");
	datosNodo.tipo=TIPO_NINGUNO;	
	apilarVariable($3);	
};
lista_variables : identificador 
{
	apilarVariable($1);
};

output : OUT p_abre identificador p_cierra 
{
	ts_t auxNodo;
	t_info datosNodo;
	strcpy(datosNodo.contenido,"OUT");
	datosNodo.tipo=TIPO_NINGUNO;	        
	ObtenerItemTS(&tabla, $3, &auxNodo);
	if(fueDeclarado(auxNodo.tipo))
	{
		if (auxNodo.tipo != TIPO_REAL)
			p_output=CrearNodo(datosNodo,CrearHoja(auxNodo.nombre,auxNodo.tipo),CrearHoja(HOJA_VACIA,TIPO_NINGUNO));
		else
			TerminarAplicacion(180, "No se pueden imprimir variables tipo real por pantalla");
	}
	else
		TerminarAplicacion(60, "La variable no fue declarada");
};
output : OUT p_abre const_string p_cierra 
{
	ts_t auxNodo;
	t_info datosNodo;
	strcpy(datosNodo.contenido,"OUT");
	datosNodo.tipo=TIPO_NINGUNO;	                                            
	ObtenerItemTS(&tabla, $3, &auxNodo);	
	p_output=CrearNodo(datosNodo,CrearHoja(auxNodo.nombre,auxNodo.tipo),CrearHoja(HOJA_VACIA,TIPO_NINGUNO));
};

condicion : comparacion 
{
	p_condicion=apilarCondicion(desapilarComparacion());
};
condicion : comparacion AND comparacion 		
{
	t_info datosNodo;
	strcpy(datosNodo.contenido,"AND");
	datosNodo.tipo=TIPO_NINGUNO;
	
	p_condicion=apilarCondicion(CrearNodo(datosNodo,desapilarComparacion(),desapilarComparacion())); 
};
condicion : comparacion OR comparacion       
{
	t_info datosNodo;
	strcpy(datosNodo.contenido,"OR");
	datosNodo.tipo=TIPO_NINGUNO;
	
	p_condicion=apilarCondicion(CrearNodo(datosNodo,desapilarComparacion(),desapilarComparacion())); 
};
condicion : NOT comparacion				
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
};

comparacion : expresion comparador expresion 
{
	t_info datosNodo;
	switch($2)
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
}; 
comparacion : en_lista {p_comparacion=apilarComparacion(p_en_lista);};
comparacion : max_lista {p_comparacion=apilarComparacion(p_max_lista);};
comparacion : min_lista {p_comparacion=apilarComparacion(p_min_lista);};

en_lista : INLIST p_abre expresion punto_y_coma c_abre lista_expresiones c_cierra p_cierra 
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
};

max_lista : EQUMAX p_abre expresion punto_y_coma c_abre lista_expresiones c_cierra p_cierra 
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
};

min_lista : EQUMIN p_abre expresion punto_y_coma c_abre lista_expresiones c_cierra p_cierra 
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
};

lista_expresiones : lista_expresiones punto_y_coma expresion 
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
};

lista_expresiones : expresion  			
{
	t_arbol p_expresion_aux;
	
	p_expresion_aux=desapilarExpresion();                
	if(p_expresion_aux->info.tipo!=TIPO_STRING)                            
		p_lista_expresiones=p_expresion_aux;                
	else
		TerminarAplicacion(40, "Constante string no es un elemento valido de la lista de variables");
};

expresion : expresion op_suma termino  
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
};
expresion : expresion op_resta termino   								
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
};
expresion : termino 
{ 
	p_expresion=apilarExpresion(desapilarTermino());
};

termino : termino op_multiplicacion factor 
{	
	int tipo;
	t_info datosNodo;

	tipo=tipoDelResultado(OP_MULTIPLICACION,p_termino->info.tipo,p_factor->info.tipo); 
	strcpy(datosNodo.contenido,"*");
	datosNodo.tipo=tipo;
	p_termino=apilarTermino(CrearNodo(datosNodo,desapilarTermino(),p_factor));
};
termino : termino op_division factor 
{   
	int tipo;
	t_info datosNodo;

	tipo=tipoDelResultado(OP_DIVISION,p_termino->info.tipo,p_factor->info.tipo); 
	strcpy(datosNodo.contenido,"/");
	datosNodo.tipo=tipo;
	p_termino=apilarTermino(CrearNodo(datosNodo,desapilarTermino(),p_factor));
};	
termino : factor 
{
	p_termino=apilarTermino(p_factor);
};

factor : identificador 
{	
	ts_t auxNodo;														
	ObtenerItemTS(&tabla, $1, &auxNodo);
	if(fueDeclarado(auxNodo.tipo))												
		p_factor=CrearHoja(auxNodo.nombre,auxNodo.tipo);
	else
		TerminarAplicacion(30, "Variable no declarada");
};	
factor : const_entera
{	
	ts_t auxNodo;														
	ObtenerItemTS(&tabla, $1, &auxNodo);
	p_factor=CrearHoja(auxNodo.nombre,auxNodo.tipo);
};	
factor : const_real 
{
	ts_t auxNodo;														
	ObtenerItemTS(&tabla, $1, &auxNodo);
	p_factor=CrearHoja(auxNodo.nombre,auxNodo.tipo);
};
factor : const_string 
{
	ts_t auxNodo;														
	ObtenerItemTS(&tabla, $1, &auxNodo);
	p_factor=CrearHoja(auxNodo.nombre,auxNodo.tipo);
};
	
factor : p_abre expresion p_cierra 
{
	p_factor=desapilarExpresion();;
};

comparador : menor {$$=MENOR;};
comparador : menor_igual {$$=MENOR_IGUAL;};
comparador : mayor {$$=MAYOR;};
comparador : mayor_igual {$$=MAYOR_IGUAL;};
comparador : igual {$$=IGUAL;};
comparador : distinto {$$=DISTINTO;};

const_entera : CONST_ENTERA {$$=yylval;};

const_real : CONST_REAL {$$=yylval;};

const_string : CONST_STRING { $$=yylval;};

identificador : ID { $$=yylval;};



begin : BEGIN {};
end : END {};
p_abre :  P_ABRE {};
p_cierra : P_CIERRA {};
c_abre : C_ABRE {};
c_cierra : C_CIERRA {};
l_abre : L_ABRE {};
l_cierra : L_CIERRA {};
coma : COMA {};
punto_y_coma : PUNTO_Y_COMA {};
dos_puntos : DOS_PUNTOS {};
op_suma : OP_SUMA {};
op_resta : OP_RESTA {};
op_multiplicacion : OP_MULTIPLICACION {};
op_division : OP_DIVISION {};
op_asignacion : OP_ASIGNACION {};
menor : MENOR {};
menor_igual : MENOR_IGUAL {};
mayor : MAYOR {};
mayor_igual : MAYOR_IGUAL {};
igual : IGUAL {};
distinto : DISTINTO {};
tipo_dato : STRING {$$=STRING;};
tipo_dato : INT {$$=INT;};
tipo_dato : REAL {$$=REAL;}; 

%%


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

//pfIn = fopen("C:\\Users\\Joel\\Documents\\Joel\\Universidad\\8vo Cuatrimestre\\Lenguajes y Compiladores\\Compilador\\Grupo123\\bin\\Debug\\lenguaje.jsm", "r");
//if (!pfIn)
//{
//	perror("Error de apertura de archivo fuente");
//	getchar();
//	return 2;
//}
	
	CrearTS(&tabla);

	yyparse();

   	MostrarTS(&tabla);
	
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
    int cantAux=0, cantIf=0, cantWhile=0, cantElse=0, cantCuerpo=0, idVector=0, tipoLista=0;
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
   /* puts("Sub-arbol mas a la izquierda");
    mostrarArbol(pSubArbolIzq);
    getch();*/

    while(pSubArbolIzq)
    {
        /*puts("ASSEMBLER: Sub-arbol mas a la izquierda: ");
        mostrarArbol(pSubArbolIzq);
        printf("\n");
        getch();*/
		puts("while(pSubArbolIzq)");
        /*Contamos si es un while?*/
        if(pSubArbolIzq->esWhile == TIPO_WHILE)
        {
			puts("WHILE");
            fprintf(pfCASM,"_while%d:\n",cantWhile);
            apilarIntWhile(cantWhile);
            cantWhile++;
        }
		//Si es una asignacion
		if(!strcmp(pSubArbolIzq->info.contenido,"="))
        {
			puts("=");
            fprintf(pfCASM,";Se asigna %s a %s\n", pSubArbolIzq->der->info.contenido, pSubArbolIzq->izq->info.contenido);
			//SI ES UNA ASIGNACION DE INT O FLOAT   
            if(pSubArbolIzq->izq->info.tipo==TIPO_REAL||            
               pSubArbolIzq->izq->info.tipo==TIPO_INT)
            {
				puts("asignacion int o real");
                if(pSubArbolIzq->izq->info.tipo==TIPO_INT)
                    comprobarOverflowInt(pSubArbolIzq);
				subirAlCopro(pSubArbolIzq, "FSTP");
				puts("sali de subirAlCopro asignacion int o real");
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
			puts("-");
            /*Cambia el nombre del nodo al nombre del auxiliar usado*/
            ponerAuxANodo((pSubArbolIzq->info.contenido),cantAux);

            /*Resta en Assembler*/
            cantAux=operacionAss("resta","FSUB",pSubArbolIzq,cantAux);
            comprobarFinControl(pSubArbolIzq);
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"+"))
        {
			puts("+");
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
			puts("*");
            /*Cambia el nombre del nodo al nombre del auxiliar usado*/
            ponerAuxANodo((pSubArbolIzq->info.contenido),cantAux);

            /*Multiplicacion en Assembler*/
            cantAux=operacionAss("multiplica","FMUL",pSubArbolIzq,cantAux);
            comprobarFinControl(pSubArbolIzq);
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"/"))
        {
			puts("/");
            /*Cambia el nombre del nodo al nombre del auxiliar usado*/
            ponerAuxANodo((pSubArbolIzq->info.contenido),cantAux);

            /*Division en Assembler*/
            cantAux=operacionAss("divide","FDIV",pSubArbolIzq,cantAux);
            comprobarFinControl(pSubArbolIzq);
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"IF"))
        {
			puts("IF");
            comprobarFinControl(pSubArbolIzq);
        }
        else if(!strcmp(pSubArbolIzq->info.contenido,"WHILE"))
        {
			puts("WHILE");
            comprobarFinControl(pSubArbolIzq);
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"OUT"))
        {
			puts("OUT");
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
			puts(";");
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
			puts("INLIST");				
			if(strcmp(pSubArbolIzq->der->info.contenido,";"))    //si no es ; el hijo es porque solo hay una expresion en la lista y hay que cargarla en el vectoraux
			{
				pSubArbolIzq->info.tipo = crearVectorAss();
				
				cargarHijoAVectorAss(pSubArbolIzq->info.tipo, pSubArbolIzq->der->info.contenido);
			}
			else
				idVector = pSubArbolIzq->der->info.tipo;
			
			tipoLista=pSubArbolIzq->izq->info.tipo;
				
			//fue el ultimo elemento de la lista
			
			fprintf(pfCASM,";Comienzo del inlist \n", idVector, idVector);		//indice tiene la cantidad de elementos en el vector
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);	//indice tiene la cantidad de elementos en el vector
			fprintf(pfCASM,"FISTP MAX_INDICE_%d\n", idVector);		//indice tiene la cantidad de elementos en el vector
			fprintf(pfCASM,"MOV INDICE_%d, 0 \n", idVector);
				
			//recorrer el vector buscando la ocurrencia de la expresion para resolver el INLIST
			
			//guardo en AUX_ELEM el elemento de la lista a comparar con EXP que contiene la expresion a buscar en la lista
			
			fprintf(pfCASM,"FILD %s \n", pSubArbolIzq->izq->info.contenido);
			fprintf(pfCASM,"FISTP EXP \n");
					
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);
			fprintf(pfCASM,"FISTP AX\n", idVector);   //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"MOV BX,2 \n");
			fprintf(pfCASM,"MUL BX \n");
			fprintf(pfCASM,"MOV BX,AX \n");
			fprintf(pfCASM,"ADD BX, OFFSET VECAUX_%d \n", idVector);
			fprintf(pfCASM,"MOV AX, dword ptr [BX] \n");
			fprintf(pfCASM,"FILD AX\nFISTP AUX_ELEM\n");
			fprintf(pfCASM,"ADD INDICE_%d, 1 \n", idVector);
			
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
								
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);
			fprintf(pfCASM,"FISTP AX\n", idVector);   //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"MOV BX,2 \n");
			fprintf(pfCASM,"MUL BX \n");
			fprintf(pfCASM,"MOV BX,AX \n");
			fprintf(pfCASM,"ADD BX, OFFSET VECAUX_%d \n", idVector);
			fprintf(pfCASM,"MOV AX, dword ptr [BX] \n");
			fprintf(pfCASM,"FILD AX\nFISTP AUX_ELEM\n");
			fprintf(pfCASM,"ADD INDICE_%d, 1 \n", idVector);
			fprintf(pfCASM,"JMP ciclo_inlist_%d \n", idVector);
								
			//FINWHILE, se hace un if para saber si salio porque lo encontro o no
			fprintf(pfCASM,"fin_inlist_%d: \n", idVector);
			
			fprintf(pfCASM,"%s EXP \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s AUX_ELEM \n", devolverTipoLista(tipoLista));
			
			escribInstrucComparar();
			fprintf(pfCASM,"JNE condicion_inlist_%d \n", idVector);						

			comprobarFinControl(pSubArbolIzq);			
		}	
		else if(!strcmp(pSubArbolIzq->info.contenido,"EQUMAX"))
		{
			puts("EQUMAX");
			if(strcmp(pSubArbolIzq->der->info.contenido,";"))    //si no es ; el hijo es porque solo hay una expresion en la lista y hay que cargarla en el vectoraux
			{
				pSubArbolIzq->info.tipo = crearVectorAss();
				
				cargarHijoAVectorAss(pSubArbolIzq->info.tipo, pSubArbolIzq->der->info.contenido);
			}
			else
				idVector = pSubArbolIzq->der->info.tipo;
			
			//fue el ultimo elemento de la lista
			fprintf(pfCASM,"MOV MAX_INDICE_%d, INDICE_%d \n", idVector, idVector);   //indice tiene la cantidad de elementos del vector
			fprintf(pfCASM,"MOV INDICE_%d, 0 \n", idVector);
		 
			//recorrer el vector para encontrar el maximo
			
			//guardo en AUX_ELEM el elemento de la lista a comparar con EXP que contiene la expresion a buscar en la lista
			
			fprintf(pfCASM,"MOV EXP, %s \n", pSubArbolIzq->izq->info.contenido);
					
			fprintf(pfCASM,"MOV AX, INDICE_%d \n", idVector);   //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"MOV bx,2 \n");
			fprintf(pfCASM,"MUL bx \n");
			fprintf(pfCASM,"MOV bx,AX \n");
			fprintf(pfCASM,"ADD bx, OFFSET VECAUX_%d \n", idVector);
			fprintf(pfCASM,"MOV AX, dword ptr [bx] \n");
			fprintf(pfCASM,"MOV AUX_ELEM, AX \n");
		
			
			//comienzo del WHILE
			//CONDICION																					// While(indice < max_indice_lista )
			fprintf(pfCASM,"ciclo_equmax_%d: \n", idVector);																//   if(aux_elem>equ_aux)
			fprintf(pfCASM,"CMP INDICE_%d, MAX_INDICE_%d \n", idVector, idVector);													//equ_aux=aux_elem
			fprintf(pfCASM,"JGE fin_equmax_%d \n", idVector);															//indice++																									
																											// cargar prox elemento a aux_elem																																		
			//CUERPO WHILE																				// endWhile
			//IF para guardar el maximo																			//	preguntar si equ_aux es igual a exp
			fprintf(pfCASM,"CMP AUX_ELEM, EQU_AUX \n");														//	equmax=TRUE			
			fprintf(pfCASM,"JLE cargar_elem_%d \n", idVector);    															//  sino FALSE*/
			fprintf(pfCASM,"MOV EQU_AUX, AUX_ELEM \n");
			fprintf(pfCASM,"JMP cargar_elem_%d \n", idVector); 
			
			fprintf(pfCASM,"cargar_elem_%d: \n", idVector);	
			fprintf(pfCASM,"add INDICE_%d, 1 \n", idVector);
			fprintf(pfCASM,"MOV AX, INDICE_%d \n", idVector);   										
			fprintf(pfCASM,"MOV bx,2 \n");												
			fprintf(pfCASM,"MUL bx \n");																	
			fprintf(pfCASM,"MOV bx,AX \n");																		
			fprintf(pfCASM,"ADD bx, OFFSET VECAUX_%d \n", idVector);												
			fprintf(pfCASM,"MOV AX, dword ptr [bx] \n");																	
			fprintf(pfCASM,"MOV AUX_ELEM, AX \n");
					
			fprintf(pfCASM,"JMP ciclo_equmax_%d \n", idVector);	
																				
			//FINWHILE, se hace un if para saber si el que encontro como maximo es igual a la expresion a comparar
			fprintf(pfCASM,"fin_equmax_%d: \n", idVector);
			fprintf(pfCASM,"CMP EXP, EQU_AUX \n");
			fprintf(pfCASM,"JNE condicion_equmax_%d \n", idVector);
			
			fprintf(pfCASM,"MOV si,OFFSET AuxTrue \n"); 			//origen					
			fprintf(pfCASM,"MOV di,OFFSET %s \n", pSubArbolIzq->info.contenido);  		//destino
			fprintf(pfCASM,"call COPIAR \n");	
			
			fprintf(pfCASM,"condicion_equmax_%d; \n", idVector);
			fprintf(pfCASM,"MOV si,OFFSET AuxFalse \n"); 								//origen
			fprintf(pfCASM,"MOV di,OFFSET %s \n", pSubArbolIzq->info.contenido);  		//destino
			fprintf(pfCASM,"call COPIAR \n");		
			
			fprintf(pfCASM,"MOV EQU_AUX, 0 \n");							

			comprobarFinControl(pSubArbolIzq);			
		}			
		else if(!strcmp(pSubArbolIzq->info.contenido,"EQUMIN"))
		{
			puts("EQUMIN");
			if(strcmp(pSubArbolIzq->der->info.contenido,";"))    //si no es ; el hijo es porque solo hay una expresion en la lista y hay que cargarla en el vectoraux
			{
				pSubArbolIzq->info.tipo = crearVectorAss();
				
				cargarHijoAVectorAss(pSubArbolIzq->info.tipo, pSubArbolIzq->der->info.contenido);
			}
			else
				idVector = pSubArbolIzq->der->info.tipo;
			
			//fue el ultimo elemento de la lista
			fprintf(pfCASM,"MOV MAX_INDICE_%d, INDICE_%d \n", idVector, idVector);   //indice tiene la cantidad de elementos del vector
			fprintf(pfCASM,"MOV INDICE_%d, 0 \n", idVector);
		 
			//recorrer el vector para encontrar el minimo
			
			//guardo en AUX_ELEM el elemento de la lista a comparar con EXP que contiene la expresion a buscar en la lista
			
			fprintf(pfCASM,"MOV EXP, %s \n", pSubArbolIzq->izq->info.contenido);
					
			fprintf(pfCASM,"MOV AX, INDICE_%d \n", idVector);   //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"MOV bx,2 \n");
			fprintf(pfCASM,"MUL bx \n");
			fprintf(pfCASM,"MOV bx,AX \n");
			fprintf(pfCASM,"ADD bx, OFFSET VECAUX_%d \n", idVector);
			fprintf(pfCASM,"MOV AX, dword ptr [bx] \n");
			fprintf(pfCASM,"MOV AUX_ELEM, AX \n");
				
			//comienzo del WHILE
			//CONDICION																					// While(indice < max_indice_lista )
			fprintf(pfCASM,"ciclo_equmin_%d: \n", idVector);																//   if(aux_elem<equ_aux)
			fprintf(pfCASM,"CMP INDICE_%d, MAX_INDICE_%d \n", idVector, idVector);													//equ_aux=aux_elem
			fprintf(pfCASM,"JGE fin_equmin_%d \n", idVector);															//indice++																										
																											// cargar prox elemento a aux_elem																																	
			//CUERPO WHILE																				// endWhile
			//IF para guardar el minimo																			//	preguntar si equ_aux es igual a exp
			fprintf(pfCASM,"CMP AUX_ELEM, EQU_AUX \n");														//	equmax=TRUE			
			fprintf(pfCASM,"JGE cargar_elem_%d \n", idVector);    															//  sino FALSE*/
			fprintf(pfCASM,"MOV EQU_AUX, AUX_ELEM \n");
			fprintf(pfCASM,"JMP cargar_elem_%d \n", idVector); 
			
			fprintf(pfCASM,"cargar_elem_%d: \n", idVector);	
			fprintf(pfCASM,"add INDICE_%d, 1 \n", idVector);
			fprintf(pfCASM,"MOV AX, INDICE_%d \n", idVector);   										
			fprintf(pfCASM,"MOV bx,2 \n");												
			fprintf(pfCASM,"MUL bx \n");																	
			fprintf(pfCASM,"MOV bx,AX \n");																		
			fprintf(pfCASM,"ADD bx, OFFSET VECAUX_%d \n", idVector);												
			fprintf(pfCASM,"MOV AX, dword ptr [bx] \n");																	
			fprintf(pfCASM,"MOV AUX_ELEM, AX \n");
					
			fprintf(pfCASM,"JMP ciclo_equmin_%d \n", idVector);	
																				
			//FINWHILE, se hace un if para saber si el que encontro como minimo es igual a la expresion a comparar
			fprintf(pfCASM,"fin_equmin_%d: \n", idVector);
			fprintf(pfCASM,"CMP EXP, EQU_AUX \n");
			fprintf(pfCASM,"JNE condicion_equmin_%d \n", idVector);
			
			fprintf(pfCASM,"MOV si,OFFSET AuxTrue \n"); 			//origen					
			fprintf(pfCASM,"MOV di,OFFSET %s \n", pSubArbolIzq->info.contenido);  		//destino
			fprintf(pfCASM,"call COPIAR \n");	
			
			fprintf(pfCASM,"condicion_equmin_%d; \n", idVector);
			fprintf(pfCASM,"MOV si,OFFSET AuxFalse \n"); 								//origen
			fprintf(pfCASM,"MOV di,OFFSET %s \n", pSubArbolIzq->info.contenido);  		//destino
			fprintf(pfCASM,"call COPIAR \n");
			
			fprintf(pfCASM,"MOV EQU_AUX, 0 \n");							

			comprobarFinControl(pSubArbolIzq);			
		}   	
		else if(!strcmp(pSubArbolIzq->info.contenido,"<"))
        {
			puts("<");
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
			puts(">");
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
			puts("==");
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
			puts("<=");
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
			puts(">=");
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
			puts("!=");
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
			puts("AND");
            char salto[5];
            t_arbol arbolAux,arbolAux0;
            nodoPadre=pSubArbolIzq->padre;
            /*Evaluamos la primera comparacion*/
            arbolAux=desapilarCompAss();
            arbolAux0=desapilarCompAss();
            subirAlCopro(arbolAux0,"FLD");
            /*Escribir Instrucciones para realizar comparacion entre ST0 y ST1*/
            escribInstrucComparar();
            /*Trae la instruccion correcta para el simbolo guardado en secCond[2]*/
            traerJumpContrario(arbolAux0->info.contenido,salto);
            /*Si es falso que realice el salto al fin por ser un AND*/
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
            /*Evaluamos la segunda condicion*/
            subirAlCopro(arbolAux,"FLD");
            /*Escribir Instrucciones para realizar comparacion entre ST0 y ST1*/
            escribInstrucComparar();
             /*Trae la instruccion correcta para el simbolo guardado en secCond[2]*/
            traerJumpContrario(arbolAux->info.contenido,salto);
            /*Si es falso que salte al fin por ser un AND*/
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
        }
		else if(!strcmp(pSubArbolIzq->info.contenido,"OR"))
        {
			puts("OR");
            char salto[5];
            t_arbol arbolAux,arbolAux0;
            arbolAux=desapilarCompAss();
            arbolAux0=desapilarCompAss();
            nodoPadre=pSubArbolIzq->padre;
            /*Evaluamos la primera comparacion*/
            subirAlCopro(arbolAux0,"FLD");
            /*Escribir Instrucciones para realizar comparacion entre ST0 y ST1*/
            escribInstrucComparar();
            /*Trae la sentencia de salto VERDADERO para el simbolo guardado en secCond[2]*/
            traerJump(arbolAux0->info.contenido,salto);
            /*Si es VERDADERO salta al cuerpo y no evalua la segunda condicion */
            fprintf(pfCASM,"%s _cuerpo_sent%d\n",salto,cantCuerpo);
            cantCuerpo++;
            /*Evaluamos la segunda condicion*/
            subirAlCopro(arbolAux,"FLD");
            /*Escribir Instrucciones para realizar comparacion entre ST0 y ST1*/
            escribInstrucComparar();
            traerJumpContrario(arbolAux->info.contenido,salto);

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
			puts(CUERPO_PROGRAMA);
			comprobarFinControl(pSubArbolIzq);
		}
		  /*Liberar hojas*/
		liberarSubArbol(pSubArbolIzq);
		/*Siguiente subarbol a la izquierda*/
		pSubArbolIzq=buscarSubArbolIzq(pArbol);
		puts("pSubArbolIzq=buscarSubArbolIzq(pArbol)");
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
	fprintf(pfDASM,"\tVECAUX_%d dd MAX_LIST dup (0)\n", cantVectores); 		/*Vector Auxiliar para Inlist -Equmax - Equmin */
	fprintf(pfDASM,"\tINDICE_%d dd 0 \n", cantVectores); 						/*Variable para usar de indice del vector auxiliar de Inlist*/
	fprintf(pfDASM,"\tMAX_INDICE_%d dd 0 \n", cantVectores);			/*Variable para guardar la cantidad en lista*/		
	return cantVectores++;
}

void cargarHijoAVectorAss(int idHijo, char *nombreHijo)
{
	fprintf(pfCASM,"FILD INDICE_%d \n", idHijo);			// valor del indice
	fprintf(pfCASM,"FISTP AX \n");
	fprintf(pfCASM,"MOV bx,2 \n");												// cada elemento ocupa 2 bytes
	fprintf(pfCASM,"MUL bx \n"); 												//cantidad de espacios de memoria desde el ppio del array
	fprintf(pfCASM,"MOV bx, AX \n");											// el resultado queda en AX, pero se necesita en BX para indexar.Se mueve de ax a bx																							
	fprintf(pfCASM,"add bx, OFFSET VECAUX_%d \n", idHijo); 								//suma la dirección de comienzo del array. BX ahora apunta al elemento en cuestion.																							
	fprintf(pfCASM,"FILD %s \n", nombreHijo); 		//copia el dato a guardar en AX
	fprintf(pfCASM,"FISTP AX \n");
	fprintf(pfCASM,"MOV dword ptr[BX],AX \n"); 									// copia AX a la posición de memoria apuntada por BX.
	fprintf(pfCASM,"add INDICE_%d, 1 \n", idHijo);
}

//COMPROBAR SI EN LA ASIGNACION EL DE LA DERECHA NO SUPERA EL OVERFLOW DE INT
 
 /*Comparaciones*/
void comparacionAss(int *cantWhile,t_arbol pSubArbolIzq,const char *comparacionCad,const char *instruccion,int *cantIf,const char *comparacionSimb)
{
	puts("comparacionAss");
    t_arbol nodoPadre=pSubArbolIzq->padre;
    if(!strcmp(nodoPadre->info.contenido,"AND")==0 && !strcmp(nodoPadre->info.contenido,"OR")==0 )
    {
		puts("No hay AND ni OR, entro a una sola condicion");
		
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
		puts("Hay AND o OR");
        /*Si es la primera condicion*/
        if(nodoPadre->izq == pSubArbolIzq)
        {
			puts("Primera condicion");
            strcpy(secCond[0],pSubArbolIzq->izq->info.contenido);
            strcpy(secCond[1],pSubArbolIzq->der->info.contenido);
            strcpy(secCond[2],comparacionSimb);
        }
        else
        {
			puts("Segunda condicion");
            strcpy(secCond[3],pSubArbolIzq->izq->info.contenido);
            strcpy(secCond[4],pSubArbolIzq->der->info.contenido);
            strcpy(secCond[5],comparacionSimb);
        }
    }
}

void comparacionAssIEE(int *cantWhile,t_arbol pSubArbolIzq,const char *comparacionCad,const char *instruccion,int *cantIf,const char *comparacionSimb)
{
	puts("comparacionAssIEE");
    t_arbol nodoPadre=pSubArbolIzq->padre;
    if(!strcmp(nodoPadre->info.contenido,"AND")==0 && !strcmp(nodoPadre->info.contenido,"OR")==0 )
    {
		puts("No hay AND ni OR, entro a una sola condicion IEE");
		
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
	printf("Liberando hijo izq: %s y der: %s de %s\n", pArbol->izq->info.contenido, pArbol->der->info.contenido, pArbol->info.contenido);
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
	puts("comprobarFinControl");
    t_arbol nodoPadre;
    int ret;
    if(p_prg != pSubArbolIzq)
    {
		nodoPadre = pSubArbolIzq->padre;
        /*Si es un if con else*/
        if(!strcmp(nodoPadre->info.contenido,CUERPO_ELSE))
        {
			puts("!strcmp(nodoPadre->info.contenido,CUERPO_ELSE)");
            /*Si es el fin de un then*/
            if(nodoPadre->izq==pSubArbolIzq)
            {
				puts("nodoPadre->izq==pSubArbolIzq");
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
	puts("subir al copro");
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
	puts("darVueltaHijos");
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
}