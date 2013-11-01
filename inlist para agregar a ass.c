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
			fprintf(pfCASM,"MOV $MAX_INDICE_%d, $INDICE_%d \n", idVector, idVector);		//indice tiene la cantidad de elementos en el vector
			fprintf(pfCASM,"MOV $INDICE_%d, 0 \n", idVector);
				
			//recorrer el vector buscando la ocurrencia de la expresion para resolver el INLIST
			
			//guardo en $AUX_ELEM el elemento de la lista a comparar con $EXP que contiene la expresion a buscar en la lista
			
			fprintf(pfCASM,"MOV $EXP, %s \n", pSubArbolIzq->izq->info.contenido);
					
			fprintf(pfCASM,"MOV AX, $INDICE_%d \n", idVector);   //$INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"MOV bx,2 \n");
			fprintf(pfCASM,"MUL bx \n");
			fprintf(pfCASM,"MOV bx,AX \n");
			fprintf(pfCASM,"ADD bx, OFFSET $VECAUX_%d \n", idVector);
			fprintf(pfCASM,"MOV AX, dword ptr [bx] \n");
			fprintf(pfCASM,"MOV $AUX_ELEM, AX \n");
			fprintf(pfCASM,"ADD $INDICE_%d, 1 \n", idVector);
			
			//comienzo del WHILE
			//CONDICION
			fprintf(pfCASM,"ciclo_inlist_%d: \n", idVector);
			if(tipoLista==TIPO_INT)
			{
				 fprintf(pfCASM,"FILD $INDICE_%d \n",idVector);
				 fprintf(pfCASM,"FILD $MAX_INDICE_%d \n",idVector);
			}
			else																			// While(indice < max_indice_lista && exp!=aux_elem )
			{	
				 fprintf(pfCASM,"FLD $INDICE_%d \n",idVector);
				 fprintf(pfCASM,"FLD $MAX_INDICE_%d \n",idVector);
			}
			
			escribInstrucComparar();
			fprintf(pfCASM,"JNB fin_inlist_%d \n", idVector);										// cargar prox elemento a aux_elem
			
			if(tipoLista==TIPO_INT)
			{
				 fprintf(pfCASM,"FILD $EXP \n");
				 fprintf(pfCASM,"FILD $AUX_ELEM \n");
			}
			else																			
			{	
				 fprintf(pfCASM,"FLD $EXP \n");
				 fprintf(pfCASM,"FLD $AUX_ELEM \n");
			}
			
			escribInstrucComparar();														// indice++
			fprintf(pfCASM,"JE fin_inlist_%d\n", idVector);											// endWhile
																							//	preguntar si salio porque son iguales 
			//CUERPO WHILE
			fprintf(pfCASM,"MOV AX, $INDICE_%d \n", idVector);   										//	inlist=TRUE
			fprintf(pfCASM,"MOV bx,2 \n");													//  sino FALSE*/
			fprintf(pfCASM,"MUL bx \n");																	
			fprintf(pfCASM,"MOV bx,AX \n");																		
			fprintf(pfCASM,"ADD bx, OFFSET $VECAUX_%d \n", idVector);												
			fprintf(pfCASM,"MOV AX, dword ptr [bx] \n");																	
			fprintf(pfCASM,"MOV $AUX_ELEM, AX \n");																	
			fprintf(pfCASM,"add $INDICE_%d, 1 \n", idVector);		
			fprintf(pfCASM,"JMP ciclo_inlist_%d \n", idVector);
																				
			//FINWHILE, se hace un if para saber si salio porque lo encontro o no
			fprintf(pfCASM,"fin_inlist_%d: \n", idVector);
			if(tipoLista==TIPO_INT)
			{
				 fprintf(pfCASM,"FILD $EXP \n");
				 fprintf(pfCASM,"FILD $AUX_ELEM \n");
			}
			else																			
			{	
				 fprintf(pfCASM,"FLD $EXP \n");
				 fprintf(pfCASM,"FLD $AUX_ELEM \n");
			}
			escribInstrucComparar();
			fprintf(pfCASM,"JNE condicion_inlist_%d \n", idVector);
			
			fprintf(pfCASM,"MOV si,OFFSET AuxTrue\n"); 			//origen					
			fprintf(pfCASM,"MOV di,OFFSET %s \n", pSubArbolIzq->info.contenido);  		//destino
			fprintf(pfCASM,"call COPIAR \n");					
			
			fprintf(pfCASM,"condicion_inlist_%d: \n", idVector);
			fprintf(pfCASM,"MOV si,OFFSET AuxFalse\n"); 								//origen
			fprintf(pfCASM,"MOV di,OFFSET %s \n", pSubArbolIzq->info.contenido);  		//destino
			fprintf(pfCASM,"call COPIAR \n");								

			comprobarFinControl(pSubArbolIzq);			
		}		
		else if(!strcmp(pSubArbolIzq->info.contenido,"EQUMAX"))
		{
			puts("EQUMAX");
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
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);			  //indice tiene la cantidad de elementos del vector
			fprintf(pfCASM,"FISTP MAX_INDICE_%d\n", idVector);
			fprintf(pfCASM,"MOV $INDICE_%d, 0 \n", idVector);
		 
			//recorrer el vector para encontrar el maximo
			
			//guardo en $AUX_ELEM el elemento de la lista a comparar con $EXP que contiene la expresion a buscar en la lista
			
			fprintf(pfCASM,"FILD %s \n", pSubArbolIzq->izq->info.contenido);
			fprintf(pfCASM,"FISTP EXP \n");
					
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);
			fprintf(pfCASM,"FISTP AX\n", idVector);   //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"MOV BX,2 \n");
			fprintf(pfCASM,"MUL BX \n");
			fprintf(pfCASM,"MOV BX,AX \n");
			fprintf(pfCASM,"ADD BX, OFFSET $VECAUX_%d \n", idVector);
			fprintf(pfCASM,"MOV AX, dword ptr [BX] \n");
			fprintf(pfCASM,"FILD AX\nFISTP AUX_ELEM\n");
		
			
			//comienzo del WHILE
			//CONDICION																					// While(indice < max_indice_lista )
			fprintf(pfCASM,"ciclo_equmax_%d: \n", idVector);																//   if(aux_elem>equ_aux)
			fprintf(pfCASM,"FILD INDICE_%d \n",idVector);
			fprintf(pfCASM,"FILD MAX_INDICE_%d \n",idVector);
			escribInstrucComparar();																									//equ_aux=aux_elem
			fprintf(pfCASM,"JNB fin_equmax_%d \n", idVector);															//indice++																									
																											// cargar prox elemento a aux_elem																																		
			//CUERPO WHILE																				// endWhile
			//IF para guardar el maximo	
			fprintf(pfCASM,"%s AUX_ELEM \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoLista(tipoLista));																							//	preguntar si equ_aux es igual a exp
			escribInstrucComparar();																					//	equmax=TRUE			
			fprintf(pfCASM,"JNA cargar_elem_%d \n", idVector);    															//  sino FALSE*/
			
			fprintf(pfCASM,"%s AUX_ELEM \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoListaDescarga(tipoLista));
			
						
			fprintf(pfCASM,"cargar_elem_%d: \n", idVector);	
			fprintf(pfCASM,"add $INDICE_%d, 1 \n", idVector);
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);
			fprintf(pfCASM,"FISTP AX\n", idVector);
			fprintf(pfCASM,"MOV BX,2 \n");												
			fprintf(pfCASM,"MUL BX \n");																	
			fprintf(pfCASM,"MOV BX,AX \n");																		
			fprintf(pfCASM,"ADD BX, OFFSET $VECAUX_%d \n", idVector);												
			fprintf(pfCASM,"MOV AX, dword ptr [BX] \n");																	
			fprintf(pfCASM,"FILD AX\nFISTP AUX_ELEM\n");
					
			fprintf(pfCASM,"JMP ciclo_equmax_%d \n", idVector);	
																				
			//FINWHILE, se hace un if para saber si el que encontro como maximo es igual a la expresion a comparar
			fprintf(pfCASM,"fin_equmax_%d: \n", idVector);
			fprintf(pfCASM,"%s EXP \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoLista(tipoLista));
			
			strcpy(pSubArbolIzq->der->info.contenido,"EQU_AUX");
			
			escribirInstruccionComparar();
					
			
			fprintf(pfCASM,"JNE resul_falso_%d\n", idVector); // en RESUL_%d queda almacenado un 1 en caso de que el inlist sea verdadero
			fprintf(pfCASM,"mov RESUL_%d, 1 \n", idVector); // o un 0 en caso de que sea falso
			fprintf(pfCASM,"JMP cont_list_%d\n", idVector);
			fprintf(pfCASM,"resul_falso_%d:\n", idVector);
			fprintf(pfCASM,"mov RESUL_%d, 0\n", idVector);
			fprintf(pfCASM,"cont_list_%d:\n", idVector);
	
			fprintf(pfCASM,"MOV $EQU_AUX, 0 \n");							
			
			comparacionAssIEE(&cantWhile,pSubArbolIzq,"igual","JNE",&cantIf);			
		}			
		else if(!strcmp(pSubArbolIzq->info.contenido,"EQUMIN"))
		{
			puts("EQUMIN");
			if(strcmp(pSubArbolIzq->der->info.contenido,";"))    //si no es ; el hijo es porque solo hay una expresion en la lista y hay que cargarla en el vectoraux
			{
				pSubArbolIzq->info.tipo = crearVectorAss();
				idVector = pSubArbolIzq->info.tipo;
				
				cargarHijoAVectorAss(pSubArbolIzq->info.tipo, pSubArbolIzq->der->info.contenido);
			}
			else
			{
				pSubArbolIzq->info.tipo = pSubArbolIzq->der->info.tipo;
				idVector = pSubArbolIzq->der->info.tipo;
			}
			
			tipoLista=pSubArbolIzq->izq->info.tipo;
			
			//fue el ultimo elemento de la lista
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);			  //indice tiene la cantidad de elementos del vector
			fprintf(pfCASM,"FISTP MAX_INDICE_%d\n", idVector);
			fprintf(pfCASM,"MOV $INDICE_%d, 0 \n", idVector);
			
			//recorrer el vector para encontrar el minimo
			
			//guardo en $AUX_ELEM el elemento de la lista a comparar con $EXP que contiene la expresion a buscar en la lista
			
			fprintf(pfCASM,"FILD %s \n", pSubArbolIzq->izq->info.contenido);
			fprintf(pfCASM,"FISTP EXP \n");
					
			
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);
			fprintf(pfCASM,"FISTP AX\n", idVector);   //INDICE_%d esta en 0, comenzamos a comparar con el primer elemento de la lista
			fprintf(pfCASM,"MOV BX,2 \n");
			fprintf(pfCASM,"MUL BX \n");
			fprintf(pfCASM,"MOV BX,AX \n");
			fprintf(pfCASM,"ADD BX, OFFSET $VECAUX_%d \n", idVector);
			fprintf(pfCASM,"MOV AX, dword ptr [BX] \n");
			fprintf(pfCASM,"FILD AX\nFISTP AUX_ELEM\n");
			
					
			//comienzo del WHILE
			//CONDICION																					// While(indice < max_indice_lista )
			fprintf(pfCASM,"ciclo_equmin_%d: \n", idVector);																//   if(aux_elem<equ_aux)
			fprintf(pfCASM,"FILD INDICE_%d \n",idVector);
			fprintf(pfCASM,"FILD MAX_INDICE_%d \n",idVector);
			escribInstrucComparar();																					//equ_aux=aux_elem
			fprintf(pfCASM,"JNB fin_equmin_%d \n", idVector);															//indice++																										
			
																												//   if(aux_elem>equ_aux)
																											// cargar prox elemento a aux_elem																																	
			//CUERPO WHILE																				// endWhile
			//IF para guardar el minimo																			//	preguntar si equ_aux es igual a exp
			fprintf(pfCASM,"%s AUX_ELEM \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoLista(tipoLista));																							//	preguntar si equ_aux es igual a exp
			escribInstrucComparar();																									//	equmax=TRUE			
			fprintf(pfCASM,"JNB cargar_elem_%d \n", idVector);    															//  sino FALSE*/
			
			fprintf(pfCASM,"%s AUX_ELEM \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoListaDescarga(tipoLista));
		
			
			fprintf(pfCASM,"cargar_elem_%d: \n", idVector);	
			fprintf(pfCASM,"add $INDICE_%d, 1 \n", idVector);
			fprintf(pfCASM,"FILD INDICE_%d\n", idVector);
			fprintf(pfCASM,"FISTP AX\n", idVector);   										
			fprintf(pfCASM,"MOV BX,2 \n");												
			fprintf(pfCASM,"MUL BX \n");																	
			fprintf(pfCASM,"MOV BX,AX \n");																		
			fprintf(pfCASM,"ADD BX, OFFSET $VECAUX_%d \n", idVector);												
			fprintf(pfCASM,"MOV AX, dword ptr [BX] \n");																	
			fprintf(pfCASM,"FILD AX\nFISTP AUX_ELEM\n");
					
			fprintf(pfCASM,"JMP ciclo_equmin_%d \n", idVector);	
																				
				
			
			
			//FINWHILE, se hace un if para saber si el que encontro como minimo es igual a la expresion a comparar
			fprintf(pfCASM,"fin_equmin_%d: \n", idVector);
			fprintf(pfCASM,"%s EXP \n", devolverTipoLista(tipoLista));
			fprintf(pfCASM,"%s EQU_AUX \n", devolverTipoLista(tipoLista));
			
			strcpy(pSubArbolIzq->der->info.contenido,"EQU_AUX");
			escribirInstruccionComparar();
					
				
			fprintf(pfCASM,"JNE resul_falso_%d\n", idVector); // en RESUL_%d queda almacenado un 1 en caso de que el inlist sea verdadero
			fprintf(pfCASM,"mov RESUL_%d, 1 \n", idVector); // o un 0 en caso de que sea falso
			fprintf(pfCASM,"JMP cont_list_%d\n", idVector);
			fprintf(pfCASM,"resul_falso_%d:\n", idVector);
			fprintf(pfCASM,"mov RESUL_%d, 0\n", idVector);
			fprintf(pfCASM,"cont_list_%d:\n", idVector);
						
			
			fprintf(pfCASM,"MOV $EQU_AUX, 0 \n");							
			
			comparacionAssIEE(&cantWhile,pSubArbolIzq,"igual","JNE",&cantIf);		
		
		}

const char *devolverTipoListaDescarga(int tipo)
{
	if (tipo == TIPO_INT)
		return "FISTP";
	else
		return "FSTP";
}