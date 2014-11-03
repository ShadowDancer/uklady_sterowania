; Przemys³aw Onak
; dane
.DSEG
.ORG 0x01FF

.EQU LEN = 2 // dlugosc liczb
 tab: .BYTE LEN
 tab1: .BYTE LEN

; kod 
.CSEG
 
; ustawianie wskaŸnika stosu
LDI R16, HIGH(RAMEND)
OUT SPH, R16
LDI R16, LOW(RAMEND)
OUT SPL, R16

; wczytywanie przyk³adowych wartoœci do rejestrów roboczych funkcji
LDI R16, 0x16
LDI R17, 0x17
LDI R18, 0x18

; wczytaj adresy liczb (dane)
LDI XL, LOW(tab)
LDI XH, HIGH(tab)
LDI YL, LOW(tab1)
LDI YH, HIGH(tab1)

; wczytaj d³ugoœæ liczby (dane)
LDI ZL, LOW(LEN)  
LDI ZH, HIGH(LEN)

; wywo³aj procedurê (dane znajduj¹ siê w rejestrach X, Y, Z)
CALL DODAJ

; koniec programu
END:
RJMP END

; dodawanie dwóch liczb
DODAJ:

; zapisuje rejestry na stos
PUSH R18
PUSH R17
PUSH R16
; Rejestr X, Y - adresy dodawanych bajtów
; Rejestr Z - ile dodawan jeszcze nalezy wykonac 

; rejestr 18 - czy w ostatnim dodawaniu by³o ustawione carry (instrukcja sbiw modyfikuje carry)
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
	
; przywroc rejestry ze stosu
POP R16
POP R17
POP R18

; koniec procedury
RET