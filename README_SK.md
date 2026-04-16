# ATmegaProg
Premente svoju dosku Arduino Mega 2560 na univerzálny programátor

Myšlienkou tohto projektu bolo vytvoriť čo najviac univerzálny a hlavne jednoduchý programátor mikroprocesorov rady AVR. Pre podporu čo najviac procesorov bolo mojou podmienkou pätica ZIF40 a programovanie pomocou vysokého napätia 12V. Podporu vysokého napätia som zvolil preto, aby bolo možné zachraňovať mikroprocesory, ktoré boli znefunkčené nesprávnym nastavením poistiek, alebo ak ste predtým potrebovali ďalší pin natavením poistky RSTDISBL a potrebujete znovu nahrať nový program. Týmto bolo potrebné naprogramovať taký software, ktorý podporuje protokoly ISP (In-System Programming), HVPP (High Voltage Paralel Programming) a HVSP (High Voltage Serial Programming). Taktiež bolo mojou podmienkou podpora PC software (Avrdude, Atmel Studio, MPLAB IDE). Pre túto kompatibilitu som zvolil komunikačný protokol STK500 verzie 2, ktorý popisuje dokument AVR068 na stránkach Microchip, a podporujú ho všetky spomínané programy. Protokol STK500 komunikuje pomocou PC sériovým rozhraním, čo značne zjednodušuje výsledný programátor. Dá sa použiť Arduino doska, ktorá má vstavaný prevodník USB -> Serial pre programovanie a monitorovanie. Pre dostatočný počet pinov som zvolil dosku Arduino Mega 2560. Výsledná doska programátora obsahuje len 4 IO, niekoľko diód, odporov a kondenzátorov. Trochu skúsený elektronik si dosku vie zospájkovať aj sám (vyžaduje sa SMT spájkovanie). Celý programátor sa potom len zasunie do dosky Arduino Mega 2560, a po nahratí firmware do mega2560, sa môže hneď používať. Univerzálnosť dosky programátora ho predurčuje aj na implementáciu ďalších protokolov ako sú TPI, PDI a UPDI. Tieto budú doplnené postupne.

# Hlavné vlastnosti:
- podpora software Avrdude, Atmel Studio, MPLAB IDE a iné, ktoré podporujú protokol STK500v2.
- podpora protokolov ISP, HVPP a HVSP.
- pätica ZIF40 pre rôzne veľkosti puzdier od 8 pin až do 40 pin.
- pomocou vstavaného 6 pin ISP konektora je možné programovať aj MCU vo väčších puzdrách.
- vstavaný zdroj HV 12V je možné programovo aplikovať na jeden z piatich pinov pätice ZIF (1, 4, 9, 10 a 31).
- pripojenie malého displeja na zobrazenie stavov programátora.
- autodetekcia veľkosti puzdra.

## Príklad prepínania Vysokého napätia:
<img src="https://github.com/rocob/ATmegaProg/blob/main/images/4053_Example.webp" width="350">
Na prepínanie pinu medzi MCU mega2560 a Vysokým napätím 12V je použitý prepínač CMOS 4053. Štandardne je pin pripojený k mega2560, čo zaisťuje logická úroveň 1 pomocou rezistora R. Po privedení Log. 0 na vstup SEL, sa prepne TARGET pin na HV +12V. Takto sú zapojené piny 1, 4, 9, 10 a 31 pätice ZIF.

# Aktuálne podporované mikroprocesory:
```
ATmega168/A     tested ISP, HVPP
ATmega168P      tested ISP, HVPP
ATmega168PB
ATmega328       tested ISP, HVPP
ATmega328P      tested ISP, HVPP
ATmega328PB
ATmega48/A      tested ISP, HVPP
ATmega48P       tested ISP, HVPP
ATmega48PB
ATmega8/L/A     tested ISP, HVPP
ATmega88/A      tested ISP, HVPP
ATmega88P       tested ISP, HVPP
ATmega88PB

ATtiny11/L      only HVSP, not yet tested
ATtiny12/L/V
ATtiny13/A
ATtiny15/L
ATtiny22/L
ATtiny2313/A    tested ISP, HVPP
ATtiny24/A/V    tested ISP, HVPP
ATtiny25/V"     tested ISP, HVSP
ATtiny26/L      tested ISP, HVPP
ATtiny261/A
ATtiny28L/V     only HVPP, not yet tested
ATtiny4313      tested ISP, HVPP
ATtiny44/A/V    tested ISP, HVPP
ATtiny45/V      tested ISP, HVPP
ATtiny461/A
ATtiny48
ATtiny84/A/V    tested ISP, HVSP
ATtiny85/V      tested ISP, HVPP
ATtiny88
ATtiny861/A

// big mega, not yet tested
ATmega16/A
ATmega32/A
ATmega8535/L
ATmega164P/PA
ATmega164A
ATmega324P
ATmega324A
ATmega324PA
ATmega324PB
ATmega644P/PA
ATmega644A
ATmega1284P
ATmega1284
ATmega162/V
ATmega8515/L



// other tiny, not yet tested
ATtiny43U         // SO20
ATtiny87          // SO20
ATtiny167         // SO20
ATtiny441         // SO14
ATtiny841         // SO14
ATtiny828         // TQFP32
ATtiny1634        // SO20

// ATtiny 102 / 104 (TPI) - being prepared
ATtiny4          // SOT23-6
ATtiny5
ATtiny9
ATtiny10
ATtiny102        // SO8
ATtiny104        // SO14
ATtiny20         // SO14, TSSOP14
ATtiny40         // SO20, TSSOP20

```

## Chyby

Používatelia sa vyzývajú, aby svoje hlásenia o chybách zverejňovali pomocou príkazu git issues alebo otvorením problému v gite:

https://github.com/rocob/ATmegaProg/issues

Všetky hlásenia o problémoch musia byť sprevádzané správnou definíciou problému. Táto definícia by mala obsahovať popis vzniku problému, príkazový riadok a výstup programátora so správami pri spustení a prípadnými chybami. Užitočná je aj história, t. j. či to predtým fungovalo.

## Dary

ATmegaProg nemá žiadne poplatky, ale dary sú akceptované.

BTC: 16JvqnECPTZM9kr7VhNH2iUmBxb7WmnUsR

Prajeme príjemné programovanie!
