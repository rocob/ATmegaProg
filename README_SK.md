# ATmegaProg
AVR Programmer for Arduino Mega 2560 boards

Myšlienkou tohto projektu bolo vytvoriť čo najviac univerzálny a hlavne jednoduchý programátor mikroprocesorov rady AVR. Pre podporu čo najviac procesorov bolo mojou podmienkou pätica ZIF40 a programovanie pomocou vysokého napätia 12V. Podporu vysokého napätia som zvolil preto, aby bolo možné zachraňovať mikroprocesory, ktoré boli znefunkčené nesprávnym nastavením poistiek, alebo ak ste predtým potrebovali ďalší pin natavením poistky RSTDISBL a potrebujete znovu nahrať nový program. Týmto bolo potrebné naprogramovať taký software, ktorý podporuje protokoly ISP (In-System Programming), HVPP (High Voltage Paralel Programming) a HVSP (High Voltage Serial Programming). Taktiež bolo mojou podmienkou podpora PC software (Avrdude, Atmel Studio, MPLAB Studio). Pre túto kompatibilitu som zvolil komunikačný protokol STK500 verzie 2, ktorý popisuje dokument AVR068 na stránkach Microchip, a podporujú ho všetky spomínané programy. Protokol STK500 komunikuje pomocou PC sériovým rozhraním, čo značne zjednodušuje výsledný programátor. Dá sa použiť Arduino doska, ktorá má vstavaný prevodník USB -> Serial pre programovanie a monitorovanie. Pre dostatočný počet pinov som zvolil dosku Arduino Mega 2560. Výsledná doska programátora obsahuje len 4 IO, niekoľko diód, odporov a kondenzátorov. Trochu skúsený elektronik si dosku vie zospájkovať aj sám (vyžaduje sa SMT spájkovanie). Celý programátor sa potom len zasunie do dosky Arduino Mega 2560, a po nahratí firmware do mega2560, sa môže hneď používať. Univerzálnosť dosky programátora ho predurčuje aj na implementáciu ďalších protokolov ako sú TPI, PDI a UPDI. Tieto budú doplnené postupne.

# Hlavné vlastnosti:
- podpora software Avrdude, Atmel Studio, MPLAB Studio a iné, ktoré podporujú protokol STK500v2.
- podpora protokolov ISP , HVPP a HVSP.
- pätica ZIF40 pre rôzne veľkosti puzdier od 8 pin až do 40 pin.
- pomocou vstavaného 6 pin ISP konektora je možné programovať aj MCU vo väčších puzdrách.
- vstavaný zdroj HV 12V je možné programovo aplikovať na jeden z piatich pinov pätice ZIF (1, 4, 9, 10 a 31).
- pripojenie malého displeja na zobrazenie stavov programátora.
- autodetekcia veľkosti puzdra.


# Aktuálne podporované mikroprocesory:
```
  // ATmega 8 (DIP28)
  {0x93, 0x07, 0 , 0 , "ATmega8/L/A"    },

  // ATmega 48 / 88 / 168 / 328 (DIP28)
  {0x92, 0x05, 0 , 0 , "ATmega48/A"     },
  {0x92, 0x0A, 0 , 0 , "ATmega48P"      },
  {0x92, 0x10, 0 , 0 , "ATmega48PB"     },
  {0x93, 0x0A, 0 , 0 , "ATmega88/A"     },
  {0x93, 0x0F, 0 , 0 , "ATmega88P"      },
  {0x93, 0x16, 0 , 0 , "ATmega88PB"     },
  {0x94, 0x06, 0 , 0 , "ATmega168/A"    },
  {0x94, 0x0B, 0 , 0 , "ATmega168P"     },
  {0x94, 0x15, 0 , 0 , "ATmega168PB"    },
  {0x95, 0x14, 0 , 0 , "ATmega328"      },
  {0x95, 0x0F, 0 , 0 , "ATmega328P"     },  // 328P tested ISP, HVPP
  {0x95, 0x16, 0 , 0 , "ATmega328PB"    },

  // ATtiny 48 / 88 (DIP28)
  {0x92, 0x09, 0 , 0 , "ATtiny48"       },  // not yet tested
  {0x93, 0x11, 0 , 0 , "ATtiny88"       },
  
  // ATtiny 2313 / 4313 (DIP20)
  {0x91, 0x0A, 1 , 0 , "ATtiny2313/A"   },
  {0x92, 0x0D, 1 , 0 , "ATtiny4313"     },  // 4313 tested ISP, HVPP

  // ATtiny 26 / 261 / 461 / 861 (DIP20)
  {0x91, 0x09, 2 , 0 , "ATtiny26/L"     },  // 26L tested ISP, HVPP
  {0x91, 0x0C, 2 , 0 , "ATtiny261/A"    },
  {0x92, 0x08, 2 , 0 , "ATtiny461/A"    },
  {0x93, 0x0D, 2 , 0 , "ATtiny861/A"    },

  // ATtiny 24 / 44 / 84 (DIP14)
  {0x91, 0x0B, 3 , 0 , "ATtiny24/A"     },
  {0x92, 0x07, 3 , 0 , "ATtiny44/A"     },
  {0x93, 0x0C, 3 , 0 , "ATtiny84/A"     },  // 84A tested ISP, HVSP

  // ATtiny 25 / 45 / 85 (DIP8)
  {0x91, 0x08, 4 , 0 , "ATtiny25/V"     },  // 25 tested ISP, HVSP
  {0x92, 0x06, 4 , 0 , "ATtiny45/V"     },
  {0x93, 0x0B, 4 , 0 , "ATtiny85/V"     },

  // ATmega 16 / 32 / 8535 (DIP40)
  {0x94, 0x03, 5 , 0 , "ATmega16/A"     }, // not yet tested
  {0x95, 0x02, 5 , 0 , "ATmega32/A"     },
  {0x93, 0x08, 5 , 0 , "ATmega8535/L"   },

  // ATmega 164 / 324 / 644 / 1284 (DIP40)
  {0x94, 0x0A, 5 , 0 , "ATmega164P/PA"  }, // not yet tested
  {0x94, 0x0F, 5 , 0 , "ATmega164A"     },
  {0x95, 0x08, 5 , 0 , "ATmega324P"     },
  {0x95, 0x15, 5 , 0 , "ATmega324A"     },
  {0x95, 0x11, 5 , 0 , "ATmega324PA"    },
  {0x95, 0x17, 5 , 0 , "ATmega324PB"    },
  {0x96, 0x0A, 5 , 0 , "ATmega644P/PA"  },
  {0x96, 0x09, 5 , 0 , "ATmega644A"     },
  {0x97, 0x05, 5 , 0 , "ATmega1284P"    },
  {0x97, 0x06, 5 , 0 , "ATmega1284"     },

  // ATmega 162 / 8515  (DIP40)
  {0x94, 0x04, 6 , 0 , "ATmega162/V"    }, // not yet tested
  {0x93, 0x06, 6 , 0 , "ATmega8515/L"   },

  // ATtiny 13A (DIP8)
  {0x90, 0x07, 4 , 0 , "ATtiny13/A"     }, // not yet tested

  // ATtiny 102 / 104 (TPI)
  {0x90, 0x0C, 8 , 0 , "ATtiny102"      },  // DIL8 - is being prepared
  {0x90, 0x0B, 9 , 0 , "ATtiny104"      }   // DIL14
```
