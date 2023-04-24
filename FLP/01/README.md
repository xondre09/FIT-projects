# FORMULA-2-BDD
Projekt do předmětu Funkcionální a logické programování na Fakultě informačních technologií Vysokého učení technického v Brně.

# Zadání
Program převádí formule v disjunktní normální formě (DNF) na binární rozhodovací diagramy (BDD) a na redukované binární rozhodovací diagramy (RBDD).

# Překlad
Program lze přeložit příkazem 'make'. Alternativně lze program přeložit pomocí nástrojů stack nebo cabal.

# Testy
Pro správné spuštění testů je potřeba mít nainstalovaný nástroj 'stack'. Testy lze spustit pomocí 'make test'. Z neznámých důvodů některý z testů občas selže (broken pipe). V takovém případě proveďte testy znovu.
Testovací vstupy aplikace jsou uloženy v adresáři 'data'. Složka 'test' obsahuje unit testy a testovací skript celé aplikace.

# Struktura
* app - Main funkce aplikace.
* src - Funkce pro práci s formulemi a BDD.
* test - Unit testy a skript pro test celé aplikace.
* data - Vstupy testů.
