/*
 * AssemblerApplication3.asm
 *
 *  Created: 2014-10-22 17:39:45
 *   Author: student
 */

 #include "m32def.inc"


 // rejestry do komentowania




 /*
 .CSEG // pocz¹tek segmentu kodu
 LDI R18, 1 // load immidiet, wczytuje 1 do rejsetru r18
LOOP: // label, numer instrukcji
 INC R16 // inkrementacja o 1 rejestru r16
 ADD R17, R18 // dodaje rejestry
RJMP LOOP // skacze do labela (ew. do instrukcji nr xx)
*/

/*
.DSEG // segment danych, nie mo¿na inicjalizowa zmiennych
.ORG 0x01FF // w tym miejscu pamieci zaczyna sie segment

.EQU LEN = 128 // sta³a
 tab: .BYTE LEN // rozmiar 4 bajty
 tab1: .BYTE LEN


.CSEG

LDI XL, LOW(tab) // low - nizszy bajt
LDI XH, HIGH(tab) // hih - wyzszy bajt
LD R16, X
*/


.DSEG // segment danych, nie mo¿na inicjalizowa zmiennych
.ORG 0x01FF // w tym miejscu pamieci zaczyna sie segment

.EQU LEN = 128 // sta³a
 tab: .BYTE LEN // rozmiar 4 bajty
 tab1: .BYTE LEN

.CSEG
CLC


LDI XL, LOW(tab) // low - nizszy bajt
LDI XH, HIGH(tab) // hih - wyzszy bajt

LDI YL, LOW(tab1) // low - nizszy bajt
LDI YH, HIGH(tab1) // hih - wyzszy bajt

LDI ZL, LOW(LEN)    ; wczytuje do rejestru z d³ugoœc
LDI ZH, HIGH(LEN)   ; cd

; konczenie programu dla dlugosci 0
CPI  ZL, 0 ; porównaj nizszy bajt z 0, jesli jest rozny od zera to zacznij petle
BRNE LOOP:
CPI ZH, 0 ; j.w wyzszy bajt
BRNE LOOP:
RJMP END ; jezeli LEN == 0 to zakoncz program




LOOP:



; wczytaj mlodsze czesci liczb do rejestru
LD R16, X+
LD R17, YL

ADC R17,R16 ; dodaj
ST Y, R17


SBIW Z, 1 ; na koncu petli zdekrementuj dlugosc, aby odejmowac poprzednia komorke pamieci

; sprawdz czy liczba sie skonczyla, jesli nie to kontynuuj
CPI  ZL, 0 ; porównaj nizszy bajt z 0, jesli jest rozny od zera to zacznij petle
BRNE LOOP:
CPI ZH, 0 ; j.w wyzszy bajt
BRNE LOOP:
RJMP END ; jezeli LEN == 0 to zakoncz program

END: RJMP END ; koniec programu




// steorwanie dobre
LD R16, X+ // inkrementuje x po wyszstkim
LD R17, Y
ADC R17, R16 // wynik zostaje w R17
ST Y+, R17 // inkrementuje Y po wysztki 