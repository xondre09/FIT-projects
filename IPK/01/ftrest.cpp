/* 
 * Name:    ftrest.cpp
 * Author:  Karel Ondřej
 * Email:   xondre09@stud.fit.vutbr.cz
 * Login:   xondre09
 * Date:    19. 03. 2017
 * 
 * Description: Klient pro stahování souborů.
 */

#include "ftrest.h"

/**
 * @brief Parse parametrs of command line.
 */
int parseArguments(int argc, const char* argv[], string *server_hostname, int *port_number, string *method, string *remotePath, string *file) {
    if (argc < 3) {
        return 1;
    }

    regex_t path;
    regmatch_t match[4];
    // URI regex
    if(regcomp(&path, "^http://([^:/]*):([0-9]*)(/.*)", REG_EXTENDED | REG_NEWLINE)) {
        cerr << "Regex 'parse URI' compile fail." << endl;
        exit(1);
    }

    string uri(argv[2]);
    string p("");

    // parse URI
    if(regexec(&path, argv[2], 4, match, 0) == 0) {
        string b2(uri.substr(match[2].rm_so, match[2].rm_eo - match[2].rm_so));
        *server_hostname = uri.substr(match[1].rm_so, match[1].rm_eo - match[1].rm_so);
        *port_number = b2.length() == 0 ? 6677 : stoi(b2);
        p += uri.substr(match[3].rm_so, match[3].rm_eo - match[3].rm_so);

    } else {
        return 1;
    }
    // set request method
    if(argc == 3 && strcmp(argv[1], "mkd") == 0) {
        *method ="PUT";
        p +="?type=folder";
    }
    else if(argc == 4 && strcmp(argv[1], "put") == 0){
        *method = "PUT";
        p +="?type=file";
        *file = argv[3];
    }
    else if(argc == 3 && strcmp(argv[1], "get") == 0){
        *method = "GET";
        p +="?type=file";
    }
    else if(argc == 3 && strcmp(argv[1],"lst") == 0) {
        *method = "GET";
        p +="?type=folder";
    }
    else if(argc == 3 && strcmp(argv[1], "del") == 0) {
        *method = "DELETE";
        p +="?type=file";
    }
    else if(argc == 3 && strcmp(argv[1],"rmd") == 0) {
        *method = "DELETE";
        p +="?type=folder";
    }
    else {
        return 1;
    }
    *remotePath = p;
    return 0;
}
/**
 * Main function. Complete message, send and recv.
 */
int main (int argc, const char * argv[]) {
    int clientSocket, port_number;
    struct hostent *server;
    struct sockaddr_in server_address;

    string server_hostname("");
    string file("");
    string remotePath("");
    string method("");
    if(parseArguments(argc, argv, &server_hostname, &port_number, &method, &remotePath, &file)) {

        fprintf(stderr,"ERROR: parse argumets\n");
        return 1;
    }

    if ((server = gethostbyname(server_hostname.c_str())) == NULL) {
        fprintf(stderr,"ERROR: no such host as %s\n", server_hostname.c_str());
        exit(EXIT_FAILURE);
    }
 

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number);
    // request header
    HTTPRequestWritter req;
    req.setMethod(method);
    req.setRemotePath(remotePath);
    // Accep-Type for get
    if(strcmp(argv[1], "get") == 0) {
        req.addHeaderField("Accept-Type", "application/octet-stream");
    }
    // Payload for PUT method
    if(strcmp(argv[1], "put") == 0) {
        req.addHeaderField("Content-Type", "application/octet-stream");
        req.addHeaderField("Content-Length", "0");

        ifstream infile { file, ios::binary };
        if(infile.good()) {
            string payload { istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };
            req.setPayload(payload);
            infile.close();
        } else {
            cerr << "File '" << file << "' not found." << endl;
            return EXIT_FAILURE;
        }

    }
    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        perror("ERROR: socket");
        return EXIT_FAILURE;
    }

    if (connect(clientSocket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0) {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);        
    }

    // send HTTP request
    req.setSocket(clientSocket);
    req.sendMsg();

    // read HTTP response
    HTTPResponseReader parser;
    parser.setSocket(clientSocket);
    parser.read();

    close(clientSocket);

    if(parser.isError()) {  // isn't HTTP response
        cerr << "Unknown error." << endl;
        return EXIT_FAILURE;        
    
    } else if(parser.getStatus() == 200) {  // status code 200 OK
        if(strcmp(argv[1], "get") == 0) {   // get file from server

            if(parser.getHeaderField("Content-Type") != "application/octen-stream") {   // isn't binary content
                cerr << "Unknown error." << endl;
                return EXIT_FAILURE; 
            }

            regex_t path;
            regmatch_t match[1];

            if(regcomp(&path, "[^/]*$", REG_EXTENDED | REG_NEWLINE)) {
                cerr << "Regex 'file name' compile fail." << endl;
                exit(1);
            }

            string uri(argv[2]);
            string name;
            //parse file name
            if(regexec(&path, argv[2], 4, match, 0) == 0) {
                name = uri.substr(match[0].rm_so, match[0].rm_eo - match[0].rm_so);
            } else {
                return EXIT_FAILURE;
            }
            ofstream outfile { name };
            outfile << parser.getPayload();

        } else if ( strcmp(argv[1], "lst") == 0 && parser.getHeaderField("Content-Type") == "text/plain"){  // list file in directory on server
            if(parser.getHeaderField("Content-Type") != "text/plain") {
                cerr << "Unknown error." << endl;
                return EXIT_FAILURE; 
            }

            cout << parser.getPayload();
        } else if(parser.getPayload() != "") {      // anything in payload
            cerr << "Unknown error." << endl;
            return EXIT_FAILURE;              
        }

    } else {
        cerr << parser.getPayload() << endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}