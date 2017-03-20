 #include <reg51.h>
 sbit rs=P2^0;
 sbit en=P2^1;
 extern
 void delay(int x)
 {
   int i,j;
      for(i=0;i<=x;i++)
	    for(j=0;j<1275;j++);
 }
 void lcommand(char x)
 {
	 P1=x;
   rs=0;
   en=1;
   delay(1);
   en=0;
 }
 
 void ldata(char x)
 {
	 P1=x;
   rs=1;
   en=1;
   delay(1);
   en=0;
 }
 void lcd_init()
 {
   lcommand(0x38);
   lcommand(0x0e);
	 lcommand(0x06);
   lcommand(0x80);
   lcommand(0x01);   
 }
 
 void lcd_send(char *x)
 {  
	  int i=0;
	
	  while(*x!='\0')
		{
		ldata(*x);
		if(i>15)
		{
		  lcommand(0x1c);	
		}
		i++;x++;
   	}
				
	
 }