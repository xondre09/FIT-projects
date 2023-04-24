/* 
 * Name:    http.h
 * Author:  Karel Ondřej
 * Email:   xondre09@stud.fit.vutbr.cz
 * Login:   xondre09
 * Date:    19. 03. 2017
 * 
 * Description: Zpracovávání HTTP dotazů a odpovědí.
 */

#include <string>
#include <map>
#include <regex>
#include <iostream>

#include <sys/socket.h>

#include <time.h>

using namespace std;

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

class HTTP {
protected:
	int socket;
	string version = "HTTP/1.1";
	map<string, string> fields;
	string payload = "";

	int error = false;
public:
	void setSocket(int socket);
	virtual string str() = 0;
	int isError();
};

class HTTPWritter : public virtual HTTP {

public:
	int sendMsg();
	void addHeaderField(const string name, const string value);
	void setPayload(const string str);
	void setVersion(const string str);
};

class HTTPReader : public virtual HTTP {
protected:

	string buffer;
	int state = 1;
	int end = false;
private:
	int feed(const char* str, int res);
	virtual void firstLine(void) = 0;
	void headerFields(void);
	void addPayload(void);
public:
	string read();
	string getHeaderField(string name);
	string getPayload();
	string getVersion();
};

class HTTPResponse : public virtual HTTP{
protected:
	int status;
	string reason = "";
public:
	virtual string str();
};

class HTTPResponseWritter : public HTTPResponse, public HTTPWritter {
public:
	HTTPResponseWritter(void);
	void setStatus(int code);
	void setReason(const string str);
};

class HTTPResponseReader : public HTTPResponse, public HTTPReader {

private:
	virtual void firstLine(void);
public:
	int getStatus();
	string getReason();
};


class HTTPRequest : virtual public HTTP{
protected:
	string method = "";
	string remotePath;
public:
	virtual string str();
};

class HTTPRequestWritter : public HTTPRequest, public HTTPWritter {
public:
	HTTPRequestWritter(void);
    void setMethod(const string str);
    void setRemotePath(const string str);
};

class HTTPRequestReader : public HTTPRequest, public HTTPReader {

private:
	virtual void firstLine(void);
public:
    string getMethod();
    string getRemotePath();
};