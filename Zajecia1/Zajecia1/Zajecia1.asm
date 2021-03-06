// dane
.DSEG
.ORG 0x01FF

.EQU LEN = 2 // dlugosc liczb
 tab: .BYTE LEN
 tab1: .BYTE LEN

 // kod
.CSEG
CLC ; nie wiem po co to tu, ale boj� si� wykasowa�

; ustaw stos
LDI R16, high(RAMEND) ; Main program start
OUT SPH, r16 ; Set Stack Pointer to top of RAM
LDI R16, low(RAMEND)
OUT SPL, r16

LDI R16, 16
LDI R17, 17
LDI R18, 18

; wczytaj adresy liczb
LDI XL, LOW(tab)
LDI XH, HIGH(tab)

LDI YL, LOW(tab1)
LDI YH, HIGH(tab1)

; wczytuje dlugosc do rejestru
LDI ZL, LOW(LEN)   
LDI ZH, HIGH(LEN)

CALL DODAJ

END:
RJMP END


DODAJ:

PUSH R18
PUSH R17
PUSH R16
; Rejestr X, Y - adresy dodawanych bajt�w
; Rejestr Z - ile dodawan jeszcze nalezy wykonac 

; rejestr 18 - czy w ostatnim dodawaniu by�o ustawione carry (instrukcja sbiw modyfikuje carry)
; wyzerowaniu rejestru przechowywujacego stan flagi carry
LDI R18, 0

; dodawanie w ptli
LOOP:

; sprawdz, czy carry byla ustawiona w poprzednim dodawaniu
CPI R18, 0 

BRNE CARRY_SET

; jesli nie to wyzeruj
CLC
RJMP CARRY_AFTER_SET

; ew. ustaw
CARRY_SET:
SEC

CARRY_AFTER_SET:

; zaladuj bajty, ktore beda dodane
LD R16, X+
LD R17, Y

; dodaj
ADC R17,R16 

; zapamietaj, czy carry bylo ustawione
BRCC NOT_CARRY

; w R18 1 - carry ustawione w ostatniej opeacji, 0 - nie ustawione
LDI R18, 1
RJMP AFTER_CARRY_MEM

NOT_CARRY:
LDI R18, 0

AFTER_CARRY_MEM:

; zapisz wynik
ST Y+, R17

; zmniejszamy liczbe dodawan do wykonania
SBIW Z, 1 
BRBS 1, DODAJ_END ; jesli dodano wszystkie bajty skoncz program

; jesli nie, to kontynuuj
RJMP LOOP 

DODAJ_END:
	
POP R16
POP R17
POP R18

RET


IRQ:
	RETI