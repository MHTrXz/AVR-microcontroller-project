#include <mega32.h>
#include <delay.h>
#include <alcd.h>

#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (1<<ADLAR))
unsigned char read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
delay_us(10);
ADCSRA|=(1<<ADSC);
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCH;
}

void main(void)
{

DDRA=0x00;
PORTA=0x00;

DDRB=0x00;
PORTB=0x00;

DDRC=0x03;
PORTC=0x00;

DDRD=0xff;
PORTD=0x00;

ASSR=0<<AS2;
TCCR2=(1<<PWM2) | (1<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (1<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2=0x00;

ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (1<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

OCR2=0;

lcd_init(40);
lcd_clear();
lcd_gotoxy(16, 0);
lcd_puts("WELCOME!");
lcd_gotoxy(5, 1);
lcd_puts("DC motor control with ATMEGA32");
delay_ms(200);
lcd_clear();
lcd_gotoxy(5, 0);
lcd_puts("Sayyed Mohammad Hosein Hashemi");
lcd_gotoxy(9, 1);
lcd_puts("Mohammad Hosein Maleky");
while (1)
      {
      OCR2=read_adc(0);       
             
      if(PINB.0==0)
       {
        PORTC.0=0;
        PORTC.1=1;
       }
       
      if(PINB.1==0)
       {
        PORTC.0=1;
        PORTC.1=0;
       }
       
      if(PINB.2==0)
       {       
        PORTC.0=0;
        PORTC.1=0;
       } 
      }
}
