//------------------------------------
// (c) Stainless Steel Rats Club'2010
// http://steelrats.net/
//------------------------------------

//#define F_CPU=1200000UL

#include <avr/io.h>
#include <util/delay.h>

#define RT PB3
#define RT_HI PORTB|=(1<<RT)
#define RT_LO PORTB&=~(1<<RT)

#define LED PB1
#define LED_ON PORTB|=(1<<LED)
#define LED_OFF PORTB&=~(1<<LED)

#define BUTTON PB0
#define BUTTON_STATE !(PINB&(1<<BUTTON))

#define CODE_MAX 4095
#define CODE_COUNT 4
#define DELAY 16 //ms
#define TE 320 //us

void came_send(unsigned int code)
{
  unsigned int mask=0b0001000000000000;

  while(mask)
  {
    _delay_us(TE);
    if(mask&code) RT_HI;
    _delay_us(TE);
    RT_HI;
    _delay_us(TE);
    RT_LO;

    mask>>=1;
  }
}

void blink()
{
  LED_ON;
  _delay_ms(100);
  LED_OFF;
  _delay_ms(100);
}

int main(void)
{
  DDRB=(1<<RT)|(1<<LED);
  PORTB=(1<<BUTTON);

  blink();
  blink();
  blink();

  unsigned char a;
  unsigned int code;

  while(1)
  {
    if(BUTTON_STATE)
    {
      blink();

      for(code=1; code<=CODE_MAX; code++)
      {
        while(BUTTON_STATE);

        for(a=0; a<CODE_COUNT; a++)
        {
          came_send(code);
          _delay_ms(DELAY);
        }

        if(!(code%32))
        {
          LED_ON;
          _delay_ms(50);
          LED_OFF;
        }
      }

      for(a=0; a<8; a++) blink();
    }
  }

  return 0;
}
