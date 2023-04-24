# Turingův stroj
Projekt do předmětu Funkcionální a logické programování na Fakultě informačních technologií Vysokého učení technického v Brně.

# Zadání
Program simuluje běh turingova stroje.

# Překlad
Program lze přeložit příkazem 'make'. Pro spuštění testů je zapotřebí interpret jazyka Haskell a nástroj 'stack'.

# Testy
Pro správné spuštění testů je potřeba mít nainstalovaný nástroj 'stack', který vyřeší závislosti. Testy lze spustit pomocí 'make test'.
Testovací vstupy aplikace jsou uloženy v adresáři 'data'. Složka 'test' obsahuje testovací skript celé aplikace. Program nad vstupy ve složce 'data/test/pass' proběhne <1 s, a ve složce 'data/test/fail' se buď zacyklí nebo skončí neúspěšně.

# Struktura
* src - Funkce pro práci s formulemi a BDD.
* test - Skript pro test celé aplikace.
* data - Data pro testy.

