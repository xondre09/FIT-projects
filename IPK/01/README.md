FREST(1)    User Commands
## JMÉNO
ftrestd, frest - vzdálená práce ze soubory

## PŘEHLED
**ftrestd** [-r ROOT-FOLDER] [-p PORT]
**ftrest** COMMAND REMOTE-PATH [LOCAL-PATH]

## POPIS
**frestd**

* **-r** ROOT-FOLDER
    specifikuje kořenový adresář. Pokud není zadán tak se bere aktuální.
* **-p** PORT
    port na kterém server naslouchá. Pokud není zadán tak 6677.

**frest**

* COMMNAD 
	smazání souboru **del**, stažení souboru **get**, nahrání souboru **put**, obsah adresáře **lst**, vytvoření adresáře **mkd**, odstranění adresáře **rmd**.
* REMOTE-PATH 
	URI ukazující k souboru nebo adresáři na serveru. V cestě je zakázano použít název složky '..'.
* LOCAL-PATH 
	cesta k lokálnímu souboru pro příkaz put.

## IMPLEMENTACE
Projekt byl implementován v jazyku C a C++. Zpracovávání HTTP dotazů a odpovědí bylo implementováno pomocí 4 tříd **HTTPRequestWritter**, **HTTPRequestReader**, **HTTPResponseWritter** a **HTTPResponceReader**. **HTTPRequestWritter** a **HTTPResponseWritter** slouží k vytvoření dotazu/odpovědi kterou následně pošle pomocí TCP protokolu. **HTTPRequestReader** a **HTTPResponceReader** přijímají příchozí zprávu a zpracovávají ji pro snadné následné použití.

**ftrestd**

* Je implementován jako iterativní server který komunikuje pomocí TCP protokolu. Po navázaní spojení je HTTP dotaz zpracována pomocí třídy **HTTPRequestReader**. Pokud se nejednalo o HTTP dotaz vrací server chybu 400. Dále podle přijaté metody a zda se jedná o adresář nebo soubor se provede požadovaná akce. Pro stažení souboru, pokud nedojde k chybě, vrací odpověď s binárním obsahem, pro operaci výpisu obsahu adresáře text/plain se seznamem souborů/adresářů oddělené znakem konce řádku. V případě chyby vrací odpověď s stavovým kódem 400, 404 nebo 500, které jako náklad obsahují upřesnění chyby. V případě vytvoření/smazání uživatele vrací se chyba 400 s detailem 'Unknown error.'.

**frest**

* Pro každou metodu vytvoří a pomocí TCP protokolu odešle HTTP dotaz. Pro operaci **put** posílá také binární data zadaného souboru. V případě operace **get** uloží příchozí data do aktuální složky a pro **lst** vypíše na standartní výstup seznam souborů/adresářů. Pokud stavový kód odpovědi je různý od 200, tak vypíše na standartní chybový výstup doplňující informace o chybě.

## PŘÍKLADY

$ ftrest mkd http://localhost:12345/karel/dir

* Vytvoření adresáře bar na serveru bežícím na lokálním počítači a portu 12345:

$ ftrest put http://localhost:12345/karel/dir/doc.pdf ~/doc.pdf

* Nahrání souboru doc.pdf do adresáře bar

$ ftrest get http://localhost:12345/karel/dir/doc.pdf

* Stažení souboru doc.pdf ze serveru

$ ftrest del http://localhost:12345/karel/dir/doc.pdf

* Odstranění souboru

$ ftrest lst http://localhost:12345/karel/dir

* Výpis obsahu adresáře

$ ftrest rmd http://localhost:12345/karel/dir

* Odstranění adresáře


## AUTOR
Napsal Karel Ondřej <xondre09@stud.fit.vutbr.cz> roku 2017.