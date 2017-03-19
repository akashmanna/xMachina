?SU?PUTC SEGMENT CODE
?SU?GETC SEGMENT CODE
 
PUBLIC _putc
PUBLIC getc
 
txd_pin	EQU	P3.1		;Transmit on this pin
rxd_pin	EQU	P3.0		;Receive on this pin
 
;Formula to calculate the bit time delay constant
;This constant is calculated as: (((crystal/baud)/12) - 5) / 2
;crystal is the frequency of crystal in Hz
;baud is required baudrate
;Please try to keep baudrate below 9600
;to get best results :)
 
BITTIM	EQU	45;		(((11059200/9600)/12) - 5) / 2
 
;--------------------------------------------
;To send data serially
;For C programs
;Protype definition:
;		void putc(unsigned char);
;Usage:
;		putc(data);
;Return:
;		This function returns nothing
;
;For Assembly Programs:
;
;Usage:
;	data to be send has to be moved to R7
;	for example:
;		mov R7,#'a'
;		lcall _putc
;--------------------------------------------
RSEG ?SU?PUTC
_putc:
	push ACC
	Push PSW
	mov a,r7
	CLR txd_pin			;Drop line for start bit
	MOV R0,#BITTIM		;Wait full bit-time
	DJNZ R0,$			;For START bit
	MOV R1,#8			;Send 8 bits
putc1:
	RRC A				;Move next bit into carry
	MOV txd_pin,C		;Write next bit
	MOV R0,#BITTIM		;Wait full bit-time
	DJNZ R0,$			;For DATA bit
	DJNZ R1,putc1		;write 8 bits
	SETB txd_pin		;Set line high
	RRC A				;Restore ACC contents
	MOV R0,#BITTIM		;Wait full bit-time
	DJNZ R0,$			;For STOP bit
	POP PSW
	pop ACC
	RET
 
;--------------------------------------------
;To receive data Serially
;If you want to use this routine in your
;C program then define function prototype
; as:
;	unsigned char getc(void);
;
;	Usage:
;		data = getc();
;	Return value:
;		Returns data received
;
;
;If you are using it in assembly program
;	Usage:
;		lcall getc
;	Return:
;		data received is stored in R7
;--------------------------------------------
 
RSEG ?SU?GETC
getc:	
	Push ACC
	Push PSW
	JB rxd_pin,$		;Wait for start bit
	MOV R0,#BITTIM/2	;Wait 1/2 bit-time
	DJNZ R0,$			;To sample in middle
	JB rxd_pin,getc		;Insure valid
	MOV R1,#8			;Read 8 bits
getc1:
	MOV R0,#BITTIM		;Wait full bit-time
	DJNZ R0,$			;For DATA bit
	MOV C,rxd_pin		;Read bit
	RRC A				;Shift it into ACC
	DJNZ R1,getc1		;read 8 bits
	mov r7,a
	POP PSW
	pop ACC
	RET					;go home
	
END	
	