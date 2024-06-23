#include <mega32.h>
#include <delay.h>
#include <alcd.h>

#define no_0 0b10111111 // 0
#define no_1 0b10000110 // 1
#define no_2 0b11011011 // 2 
#define no_3 0b11001111 // 3 
#define no_4 0b11100110 // 4
#define no_5 0b11101101 // 5
#define no_6 0b11111101 // 6
#define no_7 0b10000111 // 7
#define no_8 0b11111111 // 8
#define no_9 0b11101111 // 9

char numbers[10] = {~no_0, ~no_1, ~no_2, ~no_3, ~no_4, ~no_5, ~no_6, ~no_7, ~no_8, ~no_9};

#define counter_delay 30

void counter_up(int status) {
    if (status == 0) {
        PORTB.6 = 0;
        PORTB.7 = 0;
    } else if (status == 1) {
        PORTB.6 = 1;
        PORTB.7 = 0;
    } else if (status == 2) {
        PORTB.6 = 0;
        PORTB.7 = 1;
    } else {
        PORTB.6 = 1;
        PORTB.7 = 1;
    }
}

#define maxTime 60

int l1, l2;
int l1s, l2s;
int i, h1h, l1l, h2h, l2l;
int adc_value;

void light_status(int light, int status) {
    if (light == 0) {
        if (status == 0) {
            PORTB.0 = 1;
            PORTB.1 = 0;
            PORTB.2 = 0;
        } else if (status == 1) {
            PORTB.0 = 0;
            PORTB.1 = 1;
            PORTB.2 = 0;
        } else {
            PORTB.0 = 0;
            PORTB.1 = 0;
            PORTB.2 = 1;
        }
    } else {
        if (status == 0) {
            PORTB.3 = 1;
            PORTB.4 = 0;
            PORTB.5 = 0;
        } else if (status == 1) {
            PORTB.3 = 0;
            PORTB.4 = 1;
            PORTB.5 = 0;
        } else {
            PORTB.3 = 0;
            PORTB.4 = 0;
            PORTB.5 = 1;
        }
    }
}



#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
delay_us(10);
ADCSRA|=(1<<ADSC);
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}


float read_temp;

void main(void)
{

DDRA=0b00000000;
PORTA=0b00000001;

DDRB=0xff; 
PORTB=0x00;

DDRC=0xff;
PORTC=0x00;

DDRD=0xff;
PORTD=0xff;

ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

lcd_init(20);
lcd_gotoxy(6, 1);
lcd_puts("WELCOME!");
lcd_gotoxy(3, 2);
lcd_puts("Traffic lights");
delay_ms(200);

lcd_clear();
lcd_gotoxy(3, 0);
lcd_puts("Traffic lights");
lcd_gotoxy(5, 1);
lcd_puts("4022363143");
lcd_gotoxy(5, 2);
lcd_puts("4003623035");
lcd_gotoxy(0, 3);
lcd_puts("l&7S 16p      lcd 6p");

l1s = 2, l2s = 0;
l1 = maxTime;
l2 = maxTime * 3 / 4;

while (1)
      { 
      l1 --;
      l2 --;
      if (l1 == 0) {
        if (l1s == 0) {
            l1 = adc_value;
            l1s = 2;
        } else {
            l1 = adc_value * 3 / 4;
            l1s = 0;
        } 
      }
        
      if (l1 <= 2 && l1s == 2) {
        l1s = 1;
      }
        
        
      if (l2 == 0) {
        if (l2s == 0) {
            l2 = adc_value;
            l2s = 2;
        } else {
            l2 = adc_value * 3 / 4;
            l2s = 0;
        } 
      }
        
      if (l2 <= 2 && l2s == 2) {
        l2s = 1;
      }
      
      
      light_status(0, l1s);
      light_status(1, l2s); 
        
        
      h1h = l1 / 10;
      l1l = l1 % 10; 
      h2h = l2 / 10;
      l2l = l2 % 10;  

      for(i = 0; i < 1; i ++){
        PORTD = numbers[h1h];
        counter_up(3);
        delay_ms(counter_delay);  
             
        PORTD = numbers[l1l];
        counter_up(2);
        delay_ms(counter_delay); 
             
        PORTD = numbers[h2h];
        counter_up(1);
        delay_ms(counter_delay); 
             
        PORTD = numbers[l2l];
        counter_up(0);
        delay_ms(counter_delay);  
      } 
      adc_value = read_adc(0) * maxTime / 1023;
    }
}
