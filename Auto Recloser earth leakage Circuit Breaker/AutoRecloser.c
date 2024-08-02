// LCD module connections
sbit LCD_RS at PORTD2_bit;
sbit LCD_EN at PORTD3_bit;
sbit LCD_D4 at PORTD4_bit;
sbit LCD_D5 at PORTD5_bit;
sbit LCD_D6 at PORTD6_bit;
sbit LCD_D7 at PORTD7_bit;

sbit LCD_RS_Direction at DDD2_bit;
sbit LCD_EN_Direction at DDD3_bit;
sbit LCD_D4_Direction at DDD4_bit;
sbit LCD_D5_Direction at DDD5_bit;
sbit LCD_D6_Direction at DDD6_bit;
sbit LCD_D7_Direction at DDD7_bit;
// End LCD module connections
unsigned int ad_read;ad_read2;
float ad_cal;
char adc_dis[4];
unsigned short VREF=5;
analog00(){
PRADC_bit=0;          // Power reduction for ADC
ADSC_bit=1;           // ADC start convertion
Delay_ms(1);
if(ADSC_bit==0){
ad_read=ADCL;         //lower ADC value
ad_read2=ADCH;        // higher ADC value
ad_read=ad_read2*256+ad_read;    // combining lower and higher data value
ad_cal=(ad_read*VREF)/10.240;  // alternatively can use ad_read=ad_read/2;
}
}
void fsend(){
UART1_Write_Text("AT+CMGS=");
Delay_ms(500);
UART1_Write(0x22);
Delay_ms(500);
}

void msend(){
Delay_ms(500);
UART1_Write(0x22);
Delay_ms(500);
UART1_Write(0x0D);
}

void lsend(){
Delay_ms(500);
UART1_Write(0x1A);
Delay_ms(500);
UART1_Write(0x0D);
Delay_ms(500);
}

void wait(){
Delay_ms(500);
}

void main() { 
MCUCR.PUD=1;
//DDRD=0xFE;
DDD0_bit=0;
DDD1_bit=1;
DDRB=0x20;
ADCSRA=0x87;
ADMUX=0X00;            // AN0 channel selection written in main code
ADCSRB=0X00;
//DDC0_bit=1;         // port set as output
//PORTC0_bit=0;
PORTD=0x00;
PORTB=0x00;
PORTC=0x00;
Lcd_Init();   // Lcd initialization
Lcd_Cmd(_Lcd_Cursor_Off);
Lcd_Cmd(_Lcd_Clear);
Delay_ms(10);
Lcd_Out(1,2,"SMART RECLOSER");
UART1_Init(9600);
Delay_ms(500);
UART1_Write_Text("ate0");
wait();
UART1_WRITE(0x0D);
wait();
UART1_Write_Text("AT");
wait();
UART1_Write(0x0D);
wait();
UART1_Write_Text("AT+CMGF=1");
wait();
UART1_Write(0x0D);
wait();
Delay_ms(5);
fsend();          //initial sending
UART1_Write_Text("0767522684");
msend();          // middle sending
UART1_Write_Text("SMART RECLOSER");
lsend();
Delay_ms(10);
Delay_ms(1);
Lcd_Out(2,2,"test end");
while(1){
analog00();
if(ad_cal>=490){
PORTB5_bit=0;
Lcd_out(2,2,"HIGH   CURRENT");
analog00();
fsend();          //initial sending
UART1_Write_Text("0767522684");
msend();          // middle sending
UART1_Write_Text("High Current");
lsend();
Delay_ms(100);
}


if(PINB0_bit==1&PINB1_bit==1){
PORTB5_bit=0;
Lcd_Out(1,2,"SMART RECLOSER");
Lcd_Out(2,2,"HIGH   VOLTAGE");
fsend();          //initial sending
UART1_Write_Text("0767522684");
msend();          // middle sending
UART1_Write_Text("High Voltage");
lsend();
Delay_ms(10);
Delay_ms(1);
}
if(PINB0_bit==0&PINB1_bit==0){
PORTB5_bit=0;
Lcd_Out(1,2,"SMART RECLOSER");
Lcd_Out(2,2,"LOW    VOLTAGE");
fsend();          //initial sending
UART1_Write_Text("0767522684");
msend();          // middle sending
UART1_Write_Text("Low Voltage");
lsend();
Delay_ms(10);
Delay_ms(1);
}
if(PINB0_bit==1&PINB1_bit==0){   //&ad_cal<=400
PORTB5_bit=1;
Lcd_Out(1,2,"SMART RECLOSER");
Lcd_Out(2,2,"NORMAL VOLTAGE");
Delay_ms(1);
}
//PORTB7_bit=1;
}
}