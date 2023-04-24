/*!
 * \file    proj2017-cli.cc
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    20. 4. 2017
 * \brief   Consolová aplikace pro hru Solitaire. Potřeba >= g++ 4.9.
 */

#include "game.h"
#include <iostream>
#include <string>
#include <regex>
#include <stdlib.h>
#include <iomanip>

/*!
 * \brief Tisk nápovědy.
 */
std::string help(void) {
    return "\nG             Další karty z balíčku.\n"
           "D to          Z balíčku na hromádku to.\n"
           "M from to     Z hromádky from do hromádky to vrchní kartu.\n"
           "M from to idx Z hromádky from do hromádky to všechny karty od řádku idx.\n"
           "U             Krok zpátky.\n"
           "H             Nápověda tahu.\n"
           "SAVE file     Uložení hry.\n"
           "LOAD file     Načtení hry.\n"
           "HELP          Tahle nápověda.\n"
           "EXIT          Ukončení hry.";
}

/*!
 * \brief Vykreslení konzoloé aplikace.
 */
int main(void) {
    
    // Z balíčku
    std::regex D("^D\\s\\s*([0-9][0-9]*)\\s*$");
    // mezi hromádkami
    std::regex M("^M\\s*([0-9][0-9]*)\\s*([0-9][0-9]*)\\s*([0-9][0-9]*|)$");
    // načtení ze souboru
    std::regex LOAD("^LOAD\\s*([^\\s]*)$");
    // uložení do souboru
    std::regex SAVE("^SAVE\\s*([^\\s]*)$");
    // znaky a barvy
    std::string col[] = {"\u2660", "\u2663", "\033[0;31m\u2665", "\033[0;31m\u2666"};
    // hodnoty
    std::string value[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "X", "J", "Q", "K"};

    Game game;
    game.staffleCards();
    // prikaz
    std::string comm = "";
    // zprava
    std::string msg(help());

    while(comm != "EXIT" && game.isEnd() == false) {
        system("clear");
        bool flag = true;
        // horni lišta
        std::cout << "Score: " << game.getScore() << std::endl;
        std::cout << "     D              8    9   10   11" << std::endl;
        std::cout << "    --   --        --   --   --   --" << std::endl;
        std::cout << "    ";
        if(game.isDeckFlip()) {
            std::cout << "\033[0;33m->\033[0m";
        } else if(game[0].empty()) {
            std::cout << "  ";
        } else {
            std::cout << "\033[0;32m??\033[0m";
        }
        
        // vykreslení nabídky
        int i = 3;
        for(unsigned int r = 0; r < game[0].size(); ++r) {
            Card *c = game[0][r];
            if(c->isVisibility()) {
                i--;
                std::cout << "   " << col[c->getColor()] << value[c->getValue()] << "\033[0m";
                break;
            }
        }
        for(; i > 0; i--) {
            std::cout << "   ";
        }
        
        // vykreslení finálních hromádek
        std::cout << "     ";
        for(int i = 8; i < 12; i++) {
                            
            if(game[i].empty()) {
                std::cout << "     ";
            }
            else {
                Card *c = game[i][game[i].size() - 1];
                std::cout <<  "   "<< col[c->getColor()] << value[c->getValue()] << "\033[0m";
            }
        }
        // vykreslení hracích hromádek
        std::cout << std::endl << std::endl;
        std::cout << "     1    2    3    4    5    6    7" << std::endl;
        std::cout << "    --   --   --   --   --   --   --" << std::endl;
        for(unsigned int r = 0; flag == true; ++r) {
            flag = false;
            std::cout << std::setw(2) << r << "  ";
            for(int i= 1; i < 8; ++i) {
                
                if(r < game[i].size()) {
                    flag = true;
                    Card *c = game[i][r];
                    if(c->isVisibility() == true) {
                        std::cout << col[c->getColor()] << value[c->getValue()] << "\033[0m";
                    } else {
                        std::cout << "\033[0;32m??\033[0m";
                    }
                    std::cout << "   ";
                }
                else {
                    std::cout << "     ";
                }
            }
            std::cout << std::endl;
        }
        // zprava
        std::cout << msg << "\033[0m" << std::endl;
        msg = "";
        // Příkazová řádka
        std::cout << ">>> ";
        std::getline(std::cin, comm);
        std::smatch regMatch;
        if(std::regex_match(comm, regMatch, D)){        // z balíčku
            if(!game.move(0, stoi(regMatch[1])) ) {
                msg = "\033[0;31mInvalid move: D " + std::string(regMatch[1]);
            }
        }
        else if(comm == "G") {      // další karty
            game.rollDeck();
        }
        else if(std::regex_match(comm, regMatch, M)) {      // přesun karet
            if(regMatch[3] == "") {

                if(!game.move(stoi(regMatch[1]), stoi(regMatch[2]) )) {
                    msg = "\033[0;31mInvalid move: M " + std::string(regMatch[1]) + " " + std::string(regMatch[2]);
                }
            }
            else {
                if(!game.move(stoi(regMatch[1]), stoi(regMatch[2]), stoi(regMatch[3])) ) {
                    msg = "\033[0;31mInvalid move: M " + std::string(regMatch[1]) + " " + std::string(regMatch[2]) + " " + std::string(regMatch[3]);
                }
            }
        }
        else if(comm == "U") {      // tah zpět
            game.undo();
        } 
        else if(std::regex_match(comm, regMatch, SAVE)) {       // uložení hry
            std::ofstream out(regMatch[1]);
            if(out.good()) {
                out << game;
                out.close();
                msg = "Save: " + std::string(regMatch[1]);
            } else {
                msg = "\033[0;31mFail save: " + std::string(regMatch[1]);
            }
        } else if(std::regex_match(comm, regMatch, LOAD)) {     // načtení hry
            std::ifstream in(regMatch[1]);
            if(in.good()) {
                in >> game;
                in.close();
                msg = "Lload: " + std::string(regMatch[1]);
            } else {
                msg = "\033[0;31mFail load.";
            }
        } else if(comm == "HELP") {     // nápověda jak hrát
            msg = help();
        } else if(comm == "EXIT" || std::cin.eof()) {   // exit
            break;
        } else if(comm == "H") {        // nápověda tahu
            Move *m = game.getHint();

            if(m->getFrom() == 0) {
                if(m->getTo() == 0) {
                    msg = "\033[0;33mHint: G";
                } else {
                    msg = "\033[0;33mHint: D " + std::to_string(m->getTo());
                }
            } else {
                msg = "\033[0;33mHint: M " + std::to_string(m->getFrom()) + " " + std::to_string(m->getTo()) + " " + std::to_string(m->getIdx());
            }
        } else {
            msg = "\033[0;31mUnknown operation: " + comm;
        }
        std::cout << "-------------------------------------" << std::endl;
    }

    if(game.isEnd()) {
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "You win!" << std::endl; 
        std::cout << "Your score: " << game.getScore() << std::endl;
    }
}
