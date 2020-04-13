#include<avr/io.h>
#include<util/delay.h>
void pot_init()
{
ADCSRA|=(1<<ADEN);     //enabling 
ADMUX|=(1<<REFS0);    //setting reference voltage = vcc
ADCSRA|=(1<<ADPS2);    //setting sampling rate
}
int pot_adc()
{
int p=0b00000001;       //setting input
ADCSRA|=(1<<ADSC);     //starting conversion
ADMUX|=p;               
while((ADCSRA&(1<<ADIF))==0)  //checking if conversion is complete
{
}
ADCSRA|=(1<<ADIF);             //clearing adif bit
return(ADC);                    //storing output
}
void main()
{
DDRD=0b00000000;    //input part
DDRB=0b11111111;    //output part
PORTD=0b11111111;   //pulling up
int c;
pot_init();
int p;
while(1)
{
c=PIND;             //reads input
p=pot_adc();
if(c==0b11110000)   //special case
PORTB=0b11111010;
if(c==0b11111100)   //line tracker: if both wheels on white, go forward
PORTB=0b11101010;
if(c==0b11111110)   //line tracker: if right wheel on white, take left turn
PORTB=0b11100110;
if(c==0b11111101)   //line tracker: if  left wheel on white, take right turn
PORTB=0b11101001;
if(c==0b11111111)   //marker: if both wheels on black,beep once and go front
PORTB=0b11111010;
if(c==0b11111000||c==0b11111001||c==0b11111010||c==0b11111011)   //obstacle detector: if obstacle there, beep and wait till obstacle is removed
PORTB=0b11110000;
if(c==0b11110100 && p<=256)   //cave detector: if it enters cave it should move forward with reduced speed
{
PORTB=0b00001010;
_delay_ms(8);
PORTB=0b00000000;
_delay_ms(12);
}
if(c==0b11110101  && p<=256)   //cave detector: if it enters cave it should turn right with reduced speed
{
PORTB=0b00001001;
_delay_ms(8);
PORTB=0b00000000;
_delay_ms(12);
}
if(c==0b11110110 && p<=256)   //cave detector: if it enters cave it should turn left with reduced speed
{
PORTB=0b00000110;
_delay_ms(8);
PORTB=0b00000000;
_delay_ms(12);
}
if(c==0b11110111 && p<=256)
{
PORTB=0b00011010;
_delay_ms(8);
PORTB=0b00000000;
_delay_ms(12);
}
}
}



