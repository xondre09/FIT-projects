/* 
 * Name:    xml.hh
 * Author:  Karel Ondřej
 * Email:   xondre09@stud.fit.vutbr.cz
 * Login:   xondre09
 * Date:    23. 04. 2017
 * 
 * Description: XML pro zarizeni.
 */

#include <vector>
#include <stdint.h>
#include <string>
#include <sstream>
#include  <iomanip>

#define TAB "   "
class IP {
	uint8_t addr[6];
	char version;
public:
	IP(uint8_t* _IP, char version);
	int compare(uint8_t* _IP, char v);

	std::string str(void);
};


class MAC {
	uint8_t addr[6];
	std::vector<IP> v;
private:
	void addIpUtil(uint8_t* _IP, char n);
public:
	MAC(uint8_t _MAC[6]);
	int compare(uint8_t _MAC[6]);
	void addIPv4(uint8_t _IPv4[4]);
	void addIPv6(uint8_t _IPv4[6]);
	std::string str(void);
};

class Devices {
	std::vector<MAC> v;

private:
	void addIpUtil(uint8_t _MAC[6], uint8_t* _IP, char n);
public:
	void addIPv4(uint8_t _MAC[6], uint8_t _IPv4[4]);
	void addIPv6(uint8_t _MAC[6], uint8_t _IPv6[6]);
	std::string str(void);
};