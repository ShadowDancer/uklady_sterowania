#include <avr/io.h>
#include <avr/interrupt.h>

// liczby[x] = liczba jak¹ trzeba wypidaæ na porcie aby wyœwietliæ x
int liczby[] = {  0b00000011, 0b10011111, 0b00100101, 0b00001101, // 0 - 3
					0b10011001, 0b01001001, 0b01000001, 0b00011111, // 4 - 7
					0b00000001, 0b00001001 }; // 8 - 9


// czas od startu licznika
unsigned int miliseconds;
unsigned int seconds;

// cyfra na poszczególnym wyœwietlaczu
int digits[4] = {0, 0, 0, 0};

// który wyœwietlacz aktualnie wyœwietla
int x = 0;

// funkcja wywo³ywana co 1 ms
void Work(void* data)
{
	// ustawianie daty
	miliseconds++;
	if(miliseconds == 1000)
	{
		miliseconds = 0;
		seconds++;
		
		// jak doliczymy do koñca to stop
		if(seconds > 9999)
		{
			seconds = 9999;
		}
		
		// wyci¹gamy z liczby cyfry na poszczególnych pozycjach
		if(seconds < 1000)
		{
			digits[1] = seconds % 10;
			digits[2] = seconds / 10 % 10;
			digits[3] = seconds / 100 % 10;
		}
		// specjalny przypadek dla > 999 sekund
		else
		{
			digits[0] = seconds % 10;
			digits[1] = seconds / 10 % 10;
			digits[2] = seconds / 100 % 10;
			digits[3] = seconds / 1000 % 10;
		}
	}
	
	// liczba ms (ostatnia cyfra)
	if(seconds < 1000)
	{
		digits[0] = miliseconds / 100;
	}
	
	// kolejny wyyœwietlacz
	x = (x + 1) % 4;
	
	// przygotuj wartoœci wyœwietlaj¹ce kolejn¹ liczbê
	int display = ~(1 << x);
	int number = liczby[digits[x]];
	
	// wrzuæ przygotowane dane na porty
	PORTC = display; // wybór wyœwietlacza
	PORTD = number; // wybór segmentów
	
	// zapal / zgaœ kropkê
	if(x != 1 || seconds >= 1000)
	{
		PORTD |= 1;
	}
	else
	{
		PORTD &= ~1;
	}
	
}

// co 1 ms
ISR(TIMER0_COMP_vect)
{
	Work(0);
}

int main(void)
{
	// ustawienia portów
	DDRD |= 0b11111111;
	PORTD |= 0x0;
	
	DDRC |= 0xFF;
	PORTC |= 0x0;
	
	// Timer 0 w trybie OCR
	TIMSK |= (1 << OCIE0); // COMP overflow
	OCR0 = 250; // 250 * 64 = 16MHz = 1000 przerwa? na sekund?
	TCCR0 |= (1 << CS00) | (1 << CS01); // Prescaller 64
	

	// pocz¹tkowe ustawienia liczników
	miliseconds = 0;
	seconds = 0;
	
	/*
	seconds = 990;
	digits[2] = 9;
	digits[3] = 9;
	*/
	
	sei();
    while(1)
    {
    }
}
