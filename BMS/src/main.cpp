// #include <filesystem> 
#include <fstream>      // std::ofstream
#include <iostream>     // std::cerr
#include <string>       // std::string

#include "Multiplex.h"

/* 
 * Nacte se vstupni soubor (cesta/nazev.ts), vytvori se statistika a vystup se 
 * ulozi do souboru (nazev.ts). 
 */
int main(int ac, char** av)
{
    if (ac != 2) {
        std::cerr << "usage: ./bms1 multiplex.ts" << std::endl;
        std::cerr << "Nacte se vstupni soubor (cesta/nazev.ts), vytvori se "
                  << "statistika a vystup se ulozi do souboru (nazev.txt)."
                  << std::endl;
        return 1;
    }

    std::string filePath(av[1]);
    Multiplex multiplex;
    size_t start = filePath.find_last_of("/");
	size_t end = filePath.find_last_of(".");
	std::string outFileName = filePath.substr(start+1, end-start-1).append(".txt");
    std::ofstream output;
    
    multiplex.read(filePath);
    output.open(outFileName);
    multiplex.write(output);
    output.close();

    return 0;
}