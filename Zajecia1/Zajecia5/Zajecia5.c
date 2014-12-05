#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000ul
#include <util/delay.h>

// liczby[x] = liczba jak� trzeba wypida� na porcie aby wy�wietli� x
int liczby[] = { 0b00000011, 0b10011111, 0b00100101, 0b00001101, // 0 - 3
	0b10011001, 0b01001001, 0b01000001, 0b00011111, // 4 - 7
0b00000001, 0b00001001 }; // 8 - 9


// liczba do wy�wietlenia na wy�wietlaczu
unsigned int displayNum = 0;

// cyfry na wy�wietlaczu
int digits[4] = {0, 0, 0, 0};

// obs�uga klawiatury
void Keyboard();

// kt�ry wy�wietlacz jest aktualnie obs�ugiwany
int x = 0;

void Work()
{
	// sprawd� jakie klawisze s� wci�ni�te
	Keyboard();

	// ustaw odpowiednie cyfry
	digits[0] = displayNum % 10;
	digits[1] = displayNum / 10 % 10;
	digits[2] = displayNum / 100 % 10;
	digits[3] = displayNum / 1000 % 10;

	// kolejny wy�wietlacz
	x = (x + 1) % 4;

	// wypisz cyfre na wy�wietlaczu
	int display = ~(1 << x);
	int number = liczby[digits[x]];

	PORTC = display; // wyb�r wy�wietlacza
	PORTD = number; // wyb�r segment�w
}
// co 1 ms
ISR(TIMER0_COMP_vect)
{
	Work();
}

int main(void)
{
	// ustawienia port�w dla wy�wietlacza
	DDRD |= 0b11111111;
	PORTD |= 0x0;
	DDRC |= 0xFF;
	PORTC |= 0x0;
	// Timer 0 w trybie OCR
	TIMSK |= (1 << OCIE0); // COMP overflow
	OCR0 = 250; // 250 * 64 = 16MHz = 1000 przerwa? na sekund?
	TCCR0 |= (1 << CS00) | (1 << CS01); // Prescaller 64
	sei();

	// ustaw porty kolawaitury
	PORTB = 0x00;
	DDRB = 0x0F;
	PORTB = 0xF0;

	while(1)
	{

	}
}

// obs�uga klawiatury
void Keyboard()
{	
	// wiersze
	PORTB = 0;
	DDRB = 0xF0;
	PORTB = 0x0F;
	_delay_us(5);

	// pobierz dane z odpowiedniej cz�ci portu
	unsigned char row = ~PINB & 0b00001111;
	unsigned char rowNum = 0;
	
	// sprawd� kt�ry bit jest ustawiony
	for(int i = 0; i < 4; i++)
	{
		if(row & (1 << i))
		{
			rowNum = i + 1;
		}
	}

	// kolumny
	PORTB = 0x00;
	DDRB = 0x0F;
	PORTB = 0xF0;
	_delay_us(5);
	// pobierz dane z odpowiedniej cz�ci portu
	unsigned char column = ((~PINB) >> 4) & 0b00001111 ;
	
	// sprawd�, kt�ry bit jest ustawiony
	unsigned char columnNum = 0;
	for(int i = 0; i < 4; i++)
	{
		if(column & (1 << i))
		{
			columnNum = i + 1;
		}
	}

	// oblicz numer przycisku
	displayNum = rowNum * 4 + columnNum;

	// we� poprawk� na to, �e pierwszy przycisk ma numer 5
	if(displayNum > 0)
	displayNum -= 4;
}