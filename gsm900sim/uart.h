void uart_init()
{
	TMOD=0x20; //use Timer 1, mode 2
  TH1=0xFD; //9600 baud rate
  SCON=0x50;
  TR1=1;
	TI=1;
	
}
/*void uart_schar(char x)
{
	SBUF=x;
	while (TI==0); //wait until transmitted
  TI=0;
}
unsigned char uart_rchar()
{
	unsigned char a;
	 while (RI==0); //wait to receive
    a=SBUF;
    RI=0;
	  RI=0;
	return a;
}
void uart_send(char *x)
{
	while(*x!='\0')
	{
		 uart_schar(*x);
		x++;
	}
}*/