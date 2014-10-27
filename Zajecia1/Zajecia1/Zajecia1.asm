.DSEG
.ORG 0x01FF // w tym miejscu pamieci zaczyna sie segment

.EQU LEN = 0 // stala
 tab: .BYTE LEN // rozmiar 4 bajty
 tab1: .BYTE LEN

.CSEG
CLC

; Rejestr X, Y - adresy dodawanych bajtów
; Rejestr Z - ile dodawan jeszcze nalezy wykonac 
; rejestr 18 - czy w ostatnim dodawaniu by³o ustawione carry

LDI ZL, LOW(LEN)    ; wczytuje do rejestru z dlugosc
LDI ZH, HIGH(LEN)   ; cd

; konczenie programu dla dlugosci 0
CPI  ZL, 0 ; porownaj nizszy bajt z 0, jesli jest rozny od zera to zacznij petle
BRNE START
CPI ZH, 0 ; j.w wyzszy bajt
BRNE START
RJMP END ; jezeli LEN == 0 to zakoncz program

LDI R18, 0 ; w rejestrze bedzie przechowywana flaga carry po adc

START:

; wczytywanie danych
LDI XL, LOW(tab) // low - nizszy bajt
LDI XH, HIGH(tab) // hih - wyzszy bajt

LDI YL, LOW(tab1) // low - nizszy bajt
LDI YH, HIGH(tab1) // hih - wyzszy bajt

LOOP:

CPI R18, 0 ; sprawdz, czy carry byla ustawiona w poprzednim dodawaniu

BRNE CARRY_SET; carry powinno byc ustawione

; carry powyno byc wyzerowane
CLC
RJMP CARRY_AFTER_SET

CARRY_SET:
SEC

CARRY_AFTER_SET:

; zaladuj kolejne bajty
LD R16, X+
LD R17, Y

ADC R17,R16 ; dodaj

; zapisz wynik
ST Y+, R17

; zapamietaj, czy carry bylo ustawione
BRCC NOT_CARRY

LDI R18, 1
RJMP AFTER_CARRY_MEM

NOT_CARRY:
LDI R18, 0

AFTER_CARRY_MEM:

SBIW Z, 1 ; zmniejszamy liczbe dodawan do wykonania
BRBS 1, END ; jesli zostalo 0 dodwan do wykonania to skoncz

RJMP LOOP ; jesli nie, to kontynuuj
	
END: 
RJMP END ; koniec programu