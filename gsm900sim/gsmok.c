#include<reg51.h>
#include<string.h>
sbit rs=P2^0;
sbit en=P2^1;
sbit led=P0^3;
sbit blub=P0^0;
sbit motor=P0^1;
sbit fan=P0^2;

void lcd_cmd(unsigned char x1);
void lcd_data(unsigned char x2);
void delay(unsigned int time);
void lcd_string(unsigned char *str);
void send_byte(unsigned char ch);
unsigned char recv_byte();
void enter();
void signal();
void string_hypr(unsigned char *str);

unsigned char message[40],buff[4]; 

char i,j,l;
int as=0;
void main()
{
 P0=0x00;
 led=0;
 TMOD=0x20;
 SCON=0x50;
 TH1=0xfd;
 TR1=1;
 delay(50);
 lcd_cmd(0x38);
 lcd_cmd(0x0c);
 lcd_cmd(0x01);
 lcd_cmd(0x80);
	
 lcd_string("** Welcome **");
	delay(200);
lcd_cmd(0xC0);lcd_cmd(0x01);
	lcd_string("Initializing..");
	delay(200);
	
 string_hypr("AT");
 enter();
 delay(200);
	
 string_hypr("ATE0");
 enter();
 delay(200);
 
 string_hypr("AT+CMGF=1");
 enter();
 delay(200);
 
 while(1)
 {
  string_hypr("AT+CMGD=1");
  enter();
	 lcd_cmd(0x01);lcd_cmd(0xc0);
	lcd_string("No New MSG");
  delay(50);
  i=0;j=0;l=0;as=0;
 do
  {
   i=recv_byte();
  }while(i!='+');
  delay(300);
	lcd_string("MSG RECIEVED");
	delay(200);
	i=0;j=0;l=0;as=0;
  signal();
  string_hypr("AT+CMGR=1");
  enter();
  delay(2);
  do
  {
   i=recv_byte();
  }while(i!='+');
  
  do
  {
   j=recv_byte();
   message[l++]=j;
  }while(j!='K');
  message[--l]='\0';
	
  lcd_cmd(0x01);
	lcd_string("MSG Reading..");
	for(i=75;i<=77;i++)
	{
		buff[as]=message[i];
		as++;
	}
	
	if(strcmp(buff,"bON")==0) { blub=1; lcd_cmd(0x01); lcd_cmd(0xc3); lcd_string("Blub ON");}
  if(strcmp(buff,"bOF")==0) { blub=0; lcd_cmd(0x01); lcd_cmd(0xc3); lcd_string("Blub OFF"); }
	if(strcmp(buff,"mON")==0) { motor =1; lcd_cmd(0x01); lcd_cmd(0xc3); lcd_string("Motor ON");}
  if(strcmp(buff,"mOF")==0) { motor =0; lcd_cmd(0x01); lcd_cmd(0xc3); lcd_string("Motor OFF");}
	if(strcmp(buff,"fON")==0) { fan =1; lcd_cmd(0x01); lcd_cmd(0xc3); lcd_string("Fan ON");}
  if(strcmp(buff,"fOF")==0) { fan =0; lcd_cmd(0x01); lcd_cmd(0xc3); lcd_string("Fan OFF");}
	
  delay(500);
 }
}
void send_byte(unsigned char ch)
{
   
   SBUF = ch;
   while(!TI);
   TI = 0;
 
}
unsigned char recv_byte()
{
   unsigned char c;
   while(!RI);                 
   c = SBUF;
   RI = 0;
   return(c);
}
void string_hypr(unsigned char *str)
{
   while(*str != '\0')
   {
      send_byte(*str);
	  str++;
   }
}
void enter()
{
   send_byte(0x0d);
   send_byte(0x0a);
}

void signal()
{
  led=1;
  delay(20);
  led=0;
  delay(20);
  led=1;
  delay(20);
  led=0;
  delay(20);
  led=1;
  delay(20);
  led=0;
  delay(20);
}

void lcd_cmd(unsigned char x1)
{
 P1=x1;
 rs=0;
 en=1;
 en=0;
 delay(1);
}
void lcd_data(unsigned char x2)
{
 P1=x2;
 rs=1;
 en=1;
 en=0;
 delay(1);
}
void lcd_string(unsigned char *str)
{
 	int i,j,k;
	j=strlen(str);
	lcd_cmd(0x80);
	delay(10);
	lcd_cmd(0x01);
	delay(10);
	if(strlen(str)<16)
	{
		for(i=0;str[i]!='\0';i++)
		{
			lcd_data(str[i]);
			delay(1);
		}
	}
	else
	{
		for(i=0;i<16;i++)
		{
			lcd_data(str[i]);
			delay(1);
		}
		for(i=1,j=16;str[j]!='\0';i++,j++)
		{
		 lcd_cmd(0x80);
		 lcd_cmd(0x01);
		 for(k=i;k<=j;k++)
		 {
		 	lcd_data(str[k]);
			delay(1);
		 }
		 delay(10);
		}
	}
}
void delay(unsigned int time)
{ 
 int x,y;
 for(x=0;x<time;x++)
 for(y=0;y<1275;y++);
}