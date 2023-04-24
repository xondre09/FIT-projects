/* 
 * Name:    ftrestd.cpp
 * Author:  Karel Ondřej
 * Email:   xondre09@stud.fit.vutbr.cz
 * Login:   xondre09
 * Date:    19. 03. 2017
 * 
 * Description: Server pro stahování souborů.
 */

#include "ftrestd.h"

string ROOT_FOLDER = ".";

/**
 * @brief Get file from server.
 */
HTTPResponseWritter getFile(string remotePath) {

    HTTPResponseWritter response;

    DIR *dir;
    if ((dir = opendir(remotePath.c_str())) == NULL) {
        ifstream infile {remotePath};

        if(infile.is_open()) {
            response.setStatus(200);
            string payload { istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };
            response.addHeaderField("Content-Type", "application/octen-stream");
            response.setPayload(payload);
        }
        else {
            response.setStatus(404);
            response.setPayload(ERROR_FNF);
        }
    } else {
        closedir(dir);
        response.setStatus(400);
        response.setPayload(ERROR_NF);
    }
    return response;
}

/**
 * @brief Return list of file in directory on server.
 */
HTTPResponseWritter list(string remotePath) {
    HTTPResponseWritter response;
    
    DIR *dir;
    struct dirent *item;
    if ((dir = opendir (remotePath.c_str())) != NULL) {
        response.setStatus(200);
        
        set<string> DirList;
        while ((item = readdir (dir)) != NULL) {
            if( strcmp(item->d_name, ".") != 0 && strcmp(item->d_name, "..") != 0) {
                DirList.insert(string(item->d_name));
            }
        }
        ostringstream out("");
        while (!DirList.empty()) {
            out << *DirList.begin() << endl;
            DirList.erase(DirList.begin());
        }
        response.setPayload(out.str());
        closedir (dir);
    } else {
        ifstream infile {remotePath};

        if(infile.is_open()) {
            response.setStatus(400);
            response.setPayload(ERROR_ND);
        }
        else {
            response.setStatus(404);
            response.setPayload(ERROR_DNF);
        }
    }
    return response;
}

/**
 * @brief Put file on server.
 */
HTTPResponseWritter putFile(string remotePath, string payload) {

    HTTPResponseWritter response;

    DIR *dir;
    if ((dir = opendir(remotePath.c_str())) == NULL) {
        ifstream infile { remotePath };

        if(infile.good() == false) {
            ofstream outfile { remotePath };
            if(outfile.good() == true) {
                response.setStatus(200);
                outfile << payload;
            } else {
                response.setStatus(400);
                response.setPayload(ERROR_UE);
            }
        } else {
            infile.close();
            response.setStatus(400);
            response.setPayload(ERROR_AE);
        }
    } else {    // is directory
        closedir (dir);
        response.setStatus(400);
        response.setPayload(ERROR_UE); // TODO
    }
    return response;    
}

/**
 * @brief Make directory.
 */
HTTPResponseWritter makeDirectory(string remotePath) {
    HTTPResponseWritter response;

    const int dir_err = mkdir(remotePath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if(dir_err == -1) {
        DIR *dir;
        if ((dir = opendir(remotePath.c_str())) == NULL) {
            response.setStatus(400);
            response.setPayload(ERROR_UE);
        } else {
            response.setStatus(400);
            response.setPayload(ERROR_AE);
        }
    } else {
        response.setStatus(200);
    }
    return response;    
}

/**
 * @brief Delete file on server.
 */
HTTPResponseWritter deleteFile(string remotePath) {
    HTTPResponseWritter response;
    
    DIR *dir;
    if ((dir = opendir(remotePath.c_str())) == NULL) {
        ifstream infile { remotePath };

        if(infile.good() == true) {
            infile.close();

            if( remove(remotePath.c_str()) == 0) {
                response.setStatus(200);
            }
            else {
                response.setStatus(400);
                response.setPayload(ERROR_UE);
            }
        }
        else {
            response.setStatus(404);
            response.setPayload(ERROR_FNF);
        }
    } else {
        closedir (dir);
        response.setStatus(400);
        response.setPayload( ERROR_NF );
    }

    return response;
}

/**
 * @brief Remove directory on server.
 */
HTTPResponseWritter removeDirectory(string remotePath) {
    HTTPResponseWritter response;
    
    DIR *dir;
    if ((dir = opendir(remotePath.c_str())) == NULL) {
        ifstream infile { remotePath };

        if(infile.good() == true) {
            infile.close();
            response.setStatus(400);
            response.setPayload(ERROR_ND);
        }
        else {
            response.setStatus(404);
            response.setPayload(ERROR_DNF);
        }
    } else {
        closedir(dir);

        if( remove(remotePath.c_str()) == 0) {
            response.setStatus(200);
        }
        else {
            response.setStatus(400);
            response.setPayload(ERROR_DNE);
        }
    }

    return response;    
}

/**
 * @brief Exect command from HTTP request.
 */
HTTPResponseWritter exectRequest(HTTPRequestReader req) {
    regmatch_t match[4];
    regex_t path;
    regex_t correct;
    regmatch_t corm[2];
    string tmpRemotePath = req.getRemotePath();

    if(regcomp(&path, "^/([^/]+)(/.*|)[?]type=(.*)$", REG_EXTENDED)) {
        cerr << "Regex 'parse remote path' compile fail." << endl;
        exit(1);
    }

    HTTPResponseWritter response;

    if (regexec(&path, tmpRemotePath.c_str(), 4, match, 0) != 0) {
        response.setStatus(500);
        response.setPayload(ERROR_UE);
        return response;
    }

    string user = tmpRemotePath.substr( match[1].rm_so, match[1].rm_eo - match[1].rm_so);
    string remotePath = tmpRemotePath.substr( match[2].rm_so, match[2].rm_eo - match[2].rm_so);
    string type = tmpRemotePath.substr( match[3].rm_so, match[3].rm_eo - match[3].rm_so);

    if(regcomp(&correct, "(/[.][.]/|/[.][.]$)", REG_EXTENDED)) {
        cerr << "Regex '.. in path' compile fail." << endl;
        exit(1);
    }

    if (regexec(&correct, remotePath.c_str(), 2, corm, 0) == 0) {
        response.setStatus(400);
        response.setPayload(ERROR_UE);
        return response;
    }

    // username '..' or '.'
    if(user == ".." || user == ".") {
        response.setStatus(400);
        response.setPayload(ERROR_UE);
        return response;
    }
    string p = ROOT_FOLDER +"/"+ user;
    
    DIR *dir;

    if ((dir = opendir(p.c_str())) == NULL) {   // Unknown user name
        response.setStatus(404);
        response.setPayload(ERROR_UANF);
    } else {
        closedir (dir);
        
        if( req.getMethod() == "GET") {
            if(type == "file") {
                return getFile(p + remotePath);
            } else if(type == "folder") {
                return list(p + remotePath);
            }
        }
        else if( req.getMethod() == "PUT") {
            if(type == "file") {
                return putFile(p + remotePath, req.getPayload());
            } else if(type == "folder" && remotePath != "/") {
                return makeDirectory(p + remotePath);
            } else {
                response.setStatus(400);
                response.setPayload(ERROR_UE); 
                return response;   
            }
        }
        else if( req.getMethod() == "DELETE") {
            if(type == "file") {
                return deleteFile(p + remotePath);
            } else if(type == "folder" && remotePath != "/" ) {
                return removeDirectory(p + remotePath);
            } else {
                response.setStatus(400);
                response.setPayload(ERROR_UE);
                return response;
            }
        }

        response.setStatus(500);
    }
    return response;
}

/**
 * @brief Main function. Recv HTTP response, exect command and send HTTP response.
 */
int main (int argc, char * argv[]) {
    int rc;
    int server_socket;
    struct sockaddr_in sa;
    struct sockaddr_in sa_client;
    char str[INET_ADDRSTRLEN];
    int port_number = 6677;
    
    // parse program's argumets
    int ch, n;
    while ((ch = getopt(argc, argv, "r:p:")) != -1) {
        switch (ch) {
        case 'r':
            ROOT_FOLDER = optarg;
            break;
        case 'p':
            n =  atoi(optarg);
            if(n > 0) {
                port_number = n;
            } else {
                cerr << "ERROR: Wrong port." << endl;
                return EXIT_FAILURE;
            }
            break;
        default:
            return EXIT_FAILURE;
            break;
        }
    }
    argc -= optind;
    argv += optind; 

    DIR* dir;
    if ((dir = opendir (ROOT_FOLDER.c_str())) != NULL) {
        closedir (dir);
    } else {
        cerr << "ERROR: ROOT_FOLDER" << endl; 
        return EXIT_FAILURE;
    }
    socklen_t sa_client_len=sizeof(sa_client);
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        cerr << "ERROR: socket" << endl;
        return EXIT_FAILURE;
    }
    
    memset(&sa,0,sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(port_number);   
        
    
    
    if ((rc = bind(server_socket, (struct sockaddr*)&sa, sizeof(sa))) < 0)
    {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);     
    }
    if ((listen(server_socket, 1)) < 0)
    {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);             
    }
    // infinity loop where accept HTTP request
    while(1)
    {
        int comm_socket = accept(server_socket, (struct sockaddr*)&sa_client, &sa_client_len);

        if (comm_socket > 0) {

        
            HTTPRequestReader request;

            request.setSocket(comm_socket);
            request.read();
            
            if(request.isError() == false && request.getMethod() != "") {
                if(inet_ntop(AF_INET, &sa_client.sin_addr, str, sizeof(str))) {
                    cout << "Connect: " << str << " - " << request.getMethod() << " " << request.getRemotePath() << endl;
                }
                HTTPResponseWritter response = exectRequest(request);
                response.setSocket(comm_socket);
                response.sendMsg();
            } else {
                if(inet_ntop(AF_INET, &sa_client.sin_addr, str, sizeof(str))) {
                    cout << "Connect: " << str << " - " << "Unknown protocol." << endl;
                }
                HTTPResponseWritter response;
                response.setStatus(400);
                response.setPayload(ERROR_UE);
                response.setSocket(comm_socket);
                response.sendMsg();
            }
        }
        close(comm_socket);
    }
}