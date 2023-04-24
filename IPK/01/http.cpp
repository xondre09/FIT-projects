/* 
 * Name:    http.cpp
 * Author:  Karel Ondřej
 * Email:   xondre09@stud.fit.vutbr.cz
 * Login:   xondre09
 * Date:    19. 03. 2017
 * 
 * Description: Zpracovávání HTTP dotazů a odpovědí.
 */

#include "http.h"
#include <regex.h>

/*********************************
 *
 * Date
 *
 *********************************/
string date() {
    char buff[70];
    time_t t = time(NULL);
    struct tm *lt = {0};

    time(&t);
    lt = gmtime(&t);

    strftime(buff, sizeof buff, "%a, %d %b %Y %T GMT", lt);

    return string(buff);
}
/*********************************
 *
 * HTTP
 *
 *********************************/
void HTTP::setSocket(int socket) {
    this->socket = socket;
}

int HTTP::isError() {
    return error;
}

/*********************************
 *
 * HTTP Writter
 *
 *********************************/

/**
 * @brief Send messange with TCP protocol.
 */
int HTTPWritter::sendMsg() {

    string message = this->str();
    std::vector<char> vector(message.begin(), message.end());
    int n = send(socket, &vector[0], vector.size(), 0);
    if( n < 0) {
        perror("ERROR send msg\n");
        error = true;
    }

    return error;
}

/**
 * @brief Set HTTP version (for example HTTP/1.1).
 */
void HTTPWritter::setVersion(const string version) {
    this->version = version;
}

/**
 * @brief Add header field.
 */
void HTTPWritter::addHeaderField(const string name, const string value) {
    this->fields[name] = value;
}

/**
 * @brief Set body od message.
 */
void HTTPWritter::setPayload(const string str) {
    this->payload = str;
    this->fields["Content-Length"] = to_string(this->payload.length());
}

/*********************************
 *
 * HTTP Reader
 *
 *********************************/
void HTTPReader::headerFields(void) {
    // use C library <regex.h> because gcc version 4.8.5 don't implement C++ library <regex>.
    if(error == true) return;
    regmatch_t m[2];
    regmatch_t m2[3];
    regex_t line;
    regex_t field;

    if(regcomp(&line, "^(.*)\r\n", REG_EXTENDED | REG_NEWLINE)) {
        cerr << "Regex 'line' compile fail." << endl;
        error = true;
        exit(1);
    }

    if(regcomp(&field, "^([a-zA-Z-]*): (.*)$", REG_EXTENDED)) {
        cerr << "Regex 'field' compile fail." << endl;
        error = true;
        exit(1);
    }

    if (regexec(&line, buffer.c_str(), 2, m, 0) == 0) {

        string b0(buffer.substr(m[1].rm_so, m[1].rm_eo - m[1].rm_so));
        buffer = buffer.substr(m[0].rm_eo, buffer.length());
        
        if(regexec(&field, b0.c_str(), 3, m2, 0) == 0) {

            string b1 = b0.substr(m2[1].rm_so, m2[1].rm_eo - m2[1].rm_so);
            string b2 = b0.substr(m2[2].rm_so, m2[2].rm_eo - m2[2].rm_so);
            fields[b1] = b2;

        } else if(b0 == "") {
            state = 3;
        } else {
            error = true;
        }
    }
}

void HTTPReader::addPayload() {
    if(error == true) return;
    if(this->fields["Content-Length"] == "") {
        end = true;
        return;
    }

    payload += buffer;
    int n = std::stoi(this->fields["Content-Length"]) - payload.length();
    
    buffer = "";
    if(n == 0) {
        end = true;
    } else if( n < 0) {
        error = true;
    }
}

int HTTPReader::feed(const char* str, int res) {
    if(error == true) {
        return error;
    }

    buffer += string(str, res);

    while(buffer != "" && end == false && error == false) {
        if(state == 1) {
            this->firstLine();
        }

        if(state == 2) {
            this->headerFields();
        }

        if(state == 3) {
            this->addPayload();
        }
    }
    return error;
}

string HTTPReader::read() {

    char buff[BUFFER_SIZE];
    int res;

    do {
        res = recv(socket, buff, BUFFER_SIZE-1, 0);
        if(res <= 0) {
            break;
        }
        buff[res] = '\0';
        this->feed(buff, res);

    } while(this->end == false && this->error == false);

    if(buffer != "") {
        error = true;
    }
    return this->str();
}

string HTTPReader::getVersion() {
    return version;
}

string HTTPReader::getHeaderField(string name) {
    return this->fields[name];
}

string HTTPReader::getPayload() {
    return this->payload;
}

/*********************************
 *
 * HTTP Response
 *
 *********************************/
string HTTPResponse::str() {
    ostringstream out("");

    out << version << " " << status << " " << reason << "\r\n";

    for(auto i=this->fields.begin(); i != this->fields.end(); ++i) {
        out << i->first << ": " << i->second << "\r\n";
    }
    
    out << "\r\n";

    out << payload;
    return out.str();
}

/*********************************
 *
 * HTTP Response Writter
 *
 *********************************/
HTTPResponseWritter::HTTPResponseWritter(void) {
    this->fields["Date"] = date();
    this->fields["Content-Type"] = "text/plain";
    this->fields["Content-Length"] = "0";
    this->fields["Content-Encoding"] = "identity";
}

void HTTPResponseWritter::setStatus(int code) {
    this->status = code;
    switch(code) {
        case 200:
            this->reason = "OK";
            break;
        case 400:
            this->reason = "Bad Request";
            break;
        case 404:
            this->reason = "Not Found";
            break;
        case 500:
            this->reason = "Internal Server Error";
            break;
        case 501:
            this->reason = "Not Implemented";
            break;
        default:
            this->reason = "Unknown Status Code";
            break;
    }
}

void HTTPResponseWritter::setReason(const string reason) {
    this->reason = reason;
}

/*********************************
 *
 * HTTP Response Reader
 *
 *********************************/
void HTTPResponseReader::firstLine(void) {
    // use C library <regex.h> because gcc version 4.8.5 don't implement C++ library <regex>.
    regmatch_t m[2];
    regmatch_t m2[4];
    regex_t line;
    regex_t head;

    if(regcomp(&line, "^(.*)\r\n", REG_EXTENDED | REG_NEWLINE)) {
        cerr << "Regex 'line' compile fail." << endl;
        exit(1);
    }

    if(regcomp(&head, "^(HTTP/1\\.[01]) ([0-9]{3}) (.*)$", REG_EXTENDED)) {
        cerr << "Regex 'head' compile fail." << endl;
        exit(1);
    }

    if (regexec(&line, buffer.c_str(), 2, m, 0) == 0) {

        string b0(buffer.substr(m[1].rm_so, m[1].rm_eo));
        buffer = buffer.substr(m[0].rm_eo, buffer.length());
        
        if(regexec(&head, b0.c_str(), 4, m2, 0) == 0) {
            this->version = b0.substr(m2[1].rm_so, m2[1].rm_eo);
            this->status = stoi(b0.substr(m2[2].rm_so, m2[2].rm_eo));
            this->reason = b0.substr(m2[3].rm_so, m2[3].rm_eo);
            state = 2;
        } else {
            error = true;
        }
    }
}

int HTTPResponseReader::getStatus() {
    return status;
}

string HTTPResponseReader::getReason() {
    return reason;
}

/*********************************
 *
 * HTTP Request
 *
 *********************************/
string HTTPRequest::str() {
    ostringstream out;

    out << method << " " << this->remotePath << " " << version << "\r\n";

    for(auto i=this->fields.begin(); i != this->fields.end(); ++i) {
        out << i->first << ": " << i->second << "\r\n";
    }
    
    out << "\r\n";
    out << this->payload;

    return out.str();
}

/*********************************
 *
 * HTTP Request Reader
 *
 *********************************/
void HTTPRequestReader::firstLine(void) {

    // use C library <regex.h> because gcc version 4.8.5 don't implement C++ library <regex>.
    regmatch_t m[2];
    regmatch_t m2[4];
    regex_t line;
    regex_t head;

    if(regcomp(&line, "^(.*)\r\n", REG_EXTENDED | REG_NEWLINE)) {
        cerr << "Regex 'line' compile fail." << endl;
        exit(1);
    }

    if(regcomp(&head, "^([A-Z]*) (..*) (HTTP/1\\.[01])$", REG_EXTENDED)) {
        cerr << "Regex 'HTTP request head' compile fail." << endl;
        exit(1);
    }

    if (regexec(&line, buffer.c_str(), 2, m, 0) == 0) {

        string b0(buffer.substr(m[1].rm_so, m[1].rm_eo - m[1].rm_so));
        buffer = buffer.substr(m[0].rm_eo, buffer.length());
        if(regexec(&head, b0.c_str(), 4, m2, 0) == 0) {
            this->method     = b0.substr(m2[1].rm_so, m2[1].rm_eo - m2[1].rm_so);
            this->remotePath = b0.substr(m2[2].rm_so, m2[2].rm_eo - m2[2].rm_so);
            this->version    = b0.substr(m2[3].rm_so, m2[3].rm_eo - m2[3].rm_so);

            state = 2;
        } else {
            error = true;
        }
    }
}

string HTTPRequestReader::getMethod() {
    return method;
}

string HTTPRequestReader::getRemotePath() {
    return remotePath;
}

/*********************************
 *
 * HTTP Request Writter
 *
 *********************************/
HTTPRequestWritter::HTTPRequestWritter(void) {
    this->addHeaderField("Date", date());
    this->addHeaderField("Accept-Type", "text/plain");
    this->addHeaderField("Accept-Encoding", "identity");
}

void HTTPRequestWritter::setMethod(const string method) {
    this->method = method;
}

void  HTTPRequestWritter::setRemotePath(const string remotePath) {
    this->remotePath = remotePath;
}
