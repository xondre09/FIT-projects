# BMS - Bezdrátové a mobilní sítě - 2019/2020

Naprogramujte v jazyce C++/Python jednoduchou konzolovou aplikaci (bms1), která bude realizovat demultiplexing a analýzu transportního streamu vysílání digitální televize.

**Parametry programu:**

```
./bms1 nazev_souboru.ts
```

**Funkce programu:**

Načítá vstupní soubor (nazev_souboru.ts), který obsahuje data transportního streamu DVB-T vysílání. Výstupem aplikace bude soubor s názvem nazev_souboru.txt, která bude obsahovat detaily pro každý vyextrahovaný kanál ze zpracovávaného transportního streamu.

**Obsah a formát výstupu:**
* Soubor bude obsahovat v hlavičce informace o zpracovávaném multiplexu získané z NIT servisní tabulky, detaily o jednotlivých programech získané z tabulek PAT a SDT, které budou doplněny o souhrnnou statistiku přenosové rychlosti pro všechny kanály, které patří k jednomu programu.
* Každý řádek popisující jeden program multiplexu bude mít následující formát:
    ```
    PID-service_provider-service_name: <bitrate> Mbps
    ```
* Jednotlivé řádky budou seřazeny podle hodnoty PID.
* Bitrate počítejte na základě počtu paketů daného programu vzhledem k celkovému počtu paketů. Teda podle vzorce:
    ```
    bitrate_programu = pocet_paketu_pro_program/celkovy_pocet_paketu*bitrate_streamu
    ```
* V případě, že program obsahuje více video/audio stop případně servisní informace, sčítejte bitrate všech těchto kanálu do společné hodnoty.
* Přenosovou rychlost zaokrouhlete na 2 desetinné místa.

**Příklad:**
```
Network name: SIT 1 CESKA TELEVIZE
Network ID: 12345
Bandwidth: 8 MHz
Constellation: 64-QAM
Guard interval: 1/4
Code rate: 2/3
              
0x0100-Ceska televize-CT 1 JM: 10.50 Mbps
...
```

## Instalace

```
make
```

## Spuštění

```
./bms1 nazev_souboru.ts
```

Demonstrační příklad:

```
make download
make run
```

Výstup:
```
./bms1 ./data/multiplex.ts
Network name: SIT 1 CESKA TELEVIZE
Network ID: 12560
Bandwidth: 8 MHz
Constellation: 64-QAM
Guard interval: 1/4
Code rate: 2/3

0x0100-Ceska televize-CT 1 JM: 4.31 Mbps
0x0200-Ceska televize-CT 2: 4.15 Mbps
0x0300-Ceska televize-CT 24: 3.72 Mbps
0x0400-Ceska televize-CT sport: 4.16 Mbps
0x1000-Ceska televize-CRo RADIOZURNAL: 0.18 Mbps
0x1100-Ceska televize-CRo DVOJKA: 0.18 Mbps
0x1200-Ceska televize-CRo VLTAVA: 0.21 Mbps
0x1300-Ceska televize-CRo RADIO WAVE: 0.18 Mbps
0x1400-Ceska televize-CRo D-DUR: 0.21 Mbps
0x1500-Ceska televize-CRo RADIO JUNIOR: 0.18 Mbps
0x1600-Ceska televize-CRo PLUS: 0.08 Mbps
0x1700-Ceska televize-CRo JAZZ: 0.21 Mbps
```



