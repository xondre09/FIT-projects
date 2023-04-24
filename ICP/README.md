# Hra 2017 - Solitaire Klondike

Autoři: 
* Karel ONDŘEJ, FIT (xondre09)
* David SMÉKAL, FIT (xsmeka13)

Datum: 8. 4. 2017

## Popis:

Cílem projektu je implemntovat pravidla hry Solitaire Klondike v jazyce C++ včetně grafického i konsolového rozhraní.


Makefile:

	make 				Přeloží obě verze aplikace (hra2017, hra2017-cli) do složky ./src/
	make hra2017 		Přeloží grafické rozhraní hry
	make hra2017-cli 	Přeloží konsolové rozhraní hry
	make run			Zpustí obě aplikace hra2017 a hra2017-cli
	make doxygen		Vygeneruje dokumentaci ve složce ./doc/html/
	make pack			Zabalí vše potřebné do *.zip
	make clean 			Smaže vygenerované soubory při překladu
	make clean-all		Smaže vše vygnerované při překladu včetně binárních souborů a smaže dokumentaci

## Překlad:
V případě potíží při překladu je nutno přepsat v ```./src/Makefile``` proměnnou ```QMAKE``` na správnou verzi ```qmake```.
	
## Překlad na merlinovi:
	
	export LD_LIBRARY_PATH=:/usr/local/share/Qt-5.5.1/5.5/gcc_64/lib:${LD_LIBRARY_PATH}
	export QT_PLUGIN_PATH=:/usr/local/share/Qt-5.5.1/5.5/gcc_64/plugins:${QT_PLUGIN_PATH}

V ./src/Makefile je potřeba zadat celá cesta k qmake:

	QMAKE=/usr/local/share/Qt-5.5.1/5.5/gcc_64/bin/qmake
