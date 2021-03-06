                        TITLE   BINASC.ASM
; This procedure converts a signed binary number to a six-digit ASCII string 
; (or if the no. is neg. to a minus sign plus five digits) in the data segment.
; Enter with the number to be converted in AX and the starting address of a   
; 6-byte memory buffer in DX. Upon return, DX holds the address of the         
; leftmost nonblank, i.e the leftmost digit, or a minus sign if present.  The
; string is placed right-justified in the 6-byte buffer with blank padding if 
; necessary on the left, and CX holds the length of the string, i.e. the number
; of digits and the minus sign if present.  CX and DX are the only registers
; which are changed.
 
           PUBLIC BINASC  
           .MODEL SMALL                                                       
           .CODE                                                               
BINASC     PROC FAR                                                 	
	   PUSH	BX		;Save affected registers            	
	   PUSH	SI                                                  	
	   PUSH	AX		;Save binary value                  	
           MOV  BX,DX
	   MOV	CX,6		;Fill buffer with spaces            	
FILL_BUFF: MOV  BYTE PTR [BX],' '                                           
	   INC	BX                                                  	
	   LOOP	FILL_BUFF                                           	
	   MOV	SI,10	        ;Get ready to divide by 10          	
	   OR	AX,AX	        ;if value is negative,              	
	   JNS	CLR_DVD                                             	
	   NEG	AX		;make it positive                   	
CLR_DVD:   MOV  DX,0	        ;clear upper half of dividend       	
	   DIV	SI		;Divide AX by 10                    	
	   ADD	DX,'0'	        ;Convert remainde to ASCII digit       
	   DEC	BX		;Backup through buffer              	
	   MOV	[BX],DL	        ;store this char. in the string     	
	   INC	CX		;count converted character          	
	   OR	AX,AX	        ;All done ?                         	
	   JNZ	CLR_DVD	        ;No.  Get next digit                	
	   POP	AX		;Yes. Retrieve original             	
	   OR	AX,AX	        ;Was it negative?                   	
	   JNS	NO_MORE                                             	
	   DEC	BX		;Yes store sign                     	
	   MOV	BYTE PTR [BX],'-'                                   	
	   INC	CX		;and increase character count       	
NO_MORE:   MOV  DX,BX       
           POP	SI		;restore registers                  	
	   POP	BX                                                  	
	   RETF                                                        	
BINASC     ENDP                                                        	
           END                                                                                                            