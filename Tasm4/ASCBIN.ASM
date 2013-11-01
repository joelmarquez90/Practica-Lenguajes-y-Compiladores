                        TITLE   ASCBIN.ASM
; This procedure converts an ASCII string in the data segment to its 16-bit,
; two's-complement binary equivalent.  The ASCII string may contain leading 
; blanks and/or a minus or plus sign.  Enter with the starting address of the 
; string in DX and the character count in CX.  Upon return, the 16-bit value 
; is in AX, and the address of the first nonconvertible character (i.e. not a
; decimal digit or leading blank or minus sign), if any, found within the 
; character count is in DI. If the character count is more than 7 characters,
; or the number is out of range (greater than 32767 or less than -32768), or
; the string contains a non-convertible character, the carry flag (CF) is set
; to 1 (and this is when the contents of DI may be useful, to ,e.g., pinpoint 
; the non-digit in the middle of the number).  If the conversion was made 
; without error, CF is set to 0 (and DI contains 0FFH).  Only registers AX and
; DI are changed.    
;
           PUBLIC ASCBIN                                             
           .MODEL  SMALL                                             
           .CODE                                                     
ASCBIN     PROC FAR                                                  
	   PUSH	BX		  ;Save BX and CX                    
           PUSH CX
	   PUSH DX
           PUSH SI
	   MOV  AX,0 	          ;To start, result = 0,             
	   MOV  BX,DX
	   MOV	DI,0FFH	          ;and assume no badcharacters       
	   CMP	CX,7		  ;String too long ?                 
	   JA	NO_GOOD	          ;If so, go set CF and exit         
BLANKS:	   CMP  BYTE PTR[BX],' '  ;Scan past leading blanks          
	   JNE	CHK_NEG                                              
	   INC 	BX                                                   
	   LOOP	BLANKS                                               
	   JMP  GOOD              ;field is all blank.  Answer is zero.
CHK_NEG:   CMP	BYTE PTR [BX],'-' ;Negative number?                  
	   JNE	CHK_POS                                              
	   INC	BX		  ;If so, increment pointer,         
	   DEC	CX		  ;decrement count,                  
	   CALL	CONV_AB	          ;and convert the string            
	   JC	THRU              ;CONV_AB sets carry flag to indicate error                                   
	   CMP	AX,32768	  ;Is the number too small?          
	   JA	NO_GOOD                                              
	   NEG	AX		  ;No.  Complement the result        
	   JMP  GOOD                                                 
CHK_POS:   CMP	BYTE PTR [BX],'+' ;Positive number?                  
	   JNE	GO_CONV                                              
	   INC	BX		  ;If so, increment pointer,         
	   DEC	CX		  ;decrement the count,              
GO_CONV:   CALL	CONV_AB	          ;and convert the string            
	   JC 	THRU              ;CONV_AB sets carry flag to indicate error
	   CMP	AX,32767	  ;Is the number too big?            
	   JA	NO_GOOD                                              
GOOD:	   CLC                                                       
	   JMP  THRU                                            
NO_GOOD:   STC			  ;If so, set Carry Flag             
THRU:      POP  SI                ;Restore registers                 
           POP  DX
           POP	CX                                       
	   POP	BX                                                   
	   RET                    ;and exit                          
ASCBIN     ENDP                                                      
                                                                     
;******************************************************************************
; This procedure performs the actual conversion. Š                                                                     
CONV_AB	   PROC                                                      
RANGE:	   CMP	BYTE PTR [BX],'0' ;if the character is not a digit
	   JB	NON_DIG	                                             
	   CMP	BYTE PTR [BX],'9'                                 
	   JBE	DIGIT                                                
NON_DIG:   MOV	DI,BX	          ;put its address in DI,            
	   STC		          ;set the carry flag,               
	   JMP  END_CONV	  ;and exit                          
DIGIT:	   MOV	SI,10	          ;The character is a digit,         
	   MUL	SI		  ;so multiply AX by 10              
           JC   END_CONV          ;result is too big, doesn't fit in AX
           MOV  DL,[BX]	          ;fetch ASCII code,                 
	   AND	DX,000FH         ;save only low bits,               
	   ADD	AX,DX	          ;and update partial result         
	   JC	END_CONV	  ;Exit if result is too big         
	   INC	BX		  ;Otherwise, increment BP           
	   LOOP RANGE 	          ;and continue                      
	   CLC			  ;When done, clear Carry Flag       
END_CONV:  RET                                                       
CONV_AB	   ENDP                                                      
           END
