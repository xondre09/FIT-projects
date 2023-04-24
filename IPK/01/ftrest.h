/* 
 * Name:    ftrest.h
 * Author:  Karel Ondřej
 * Email:   xondre09@stud.fit.vutbr.cz
 * Login:   xondre09
 * Date:    19. 03. 2017
 * 
 * Description: Klient pro stahování souborů.
 */

// standart C library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// standart C++ library
#include <string>
#include <fstream>
#include <iostream>

// socket connection
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include "http.h"