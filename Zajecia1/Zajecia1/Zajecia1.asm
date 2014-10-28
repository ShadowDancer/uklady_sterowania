// dane
.DSEG
.ORG 0x01FF

.EQU LEN = 4 // dlugosc liczb
 tab: .BYTE LEN
 tab1: .BYTE LEN

 // kod
.CSEG
CLC ; nie wiem po co to tu, ale bojê siê wykasowaæ

; Rejestr X, Y - adresy dodawanych bajtów
; Rejestr Z - ile dodawan jeszcze nalezy wykonac 
; rejestr 18 - czy w ostatnim dodawaniu by³o ustawione carry (instrukcja sbiw modyfikuje carry)

; wczytuje dlugosc do rejestru
LDI ZL, LOW(LEN)   
LDI ZH, HIGH(LEN)

; konczenie programu dla dlugosci 0
CPI  ZL, 0
BRNE START
CPI ZH, 0
BRNE START
RJMP END

; wyzerowaniu rejestru przechowywujacego stan flagi carry
LDI R18, 0

START:

; wczytywanie adresów danych
LDI XL, LOW(tab)
LDI XH, HIGH(tab)

LDI YL, LOW(tab1)
LDI YH, HIGH(tab1)

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
BRBS 1, END ; jesli dodano wszystkie bajty skoncz program

; jesli nie, to kontynuuj
RJMP LOOP 
	
; koniec programu
END: 
RJMP END 