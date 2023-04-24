/* 
 * Name:    ftrestd.h
 * Author:  Karel Ondřej
 * Email:   xondre09@stud.fit.vutbr.cz
 * Login:   xondre09
 * Date:    19. 03. 2017
 * 
 * Description: Server pro stahování souborů.
 */

// standart C library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <dirent.h>

// standart C++ library
#include <string>
#include <fstream>
#include <iostream>
#include <set>

// socket connection
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "http.h"

#define ERROR_ND "Not a directory."
#define ERROR_DNF "Directory not found."
#define ERROR_DNE "Directory not empty."
#define ERROR_AE "Already exists."
#define ERROR_NF "Not a file."
#define ERROR_FNF "File not found."
#define ERROR_UANF "User Account Not Found."
#define ERROR_UE "Unknown error."

using namespace std;