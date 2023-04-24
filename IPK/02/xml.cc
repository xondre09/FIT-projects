/* 
 * Name:    xml.cc
 * Author:  Karel Ondřej
 * Email:   xondre09@stud.fit.vutbr.cz
 * Login:   xondre09
 * Date:    23. 04. 2017
 * 
 * Description: XML pro zarizeni.
 */

#include "xml.hh"

// inicializace IP adresy
IP::IP(uint8_t* _IP, char version) {
	this->version = version;

	for (int i = 0; i < (version == 0 ? 4 : 6); ++i)
	{
		this->addr[i] = _IP[i];
	}
}
// porovnani adres
int IP::compare(uint8_t* _IP, char v) {
	if (version != v) 
	{
		return -2;
	}

	for(int i = 0; i < (version == 0 ? 4 : 6); i++) 
	{
		if(addr[i] < _IP[i]) return -1;
		if(addr[i] > _IP[i]) return 1;
	}
	return 0;
}
// prevod IP na xml format
std::string IP::str(void) {
	std::ostringstream out("");

	int n   = version == 0 ? 3   : 5;
	const char* ver = version == 0 ? "4" : "6";

	if( version == 0) 
	{
		out << TAB << TAB << "<ipv" << ver << ">";
		for(int i =  0; i < n; ++i)
		{
			out << unsigned(addr[i]) << ".";
		}
		out << unsigned(addr[n]);
		out << "</ipv" << ver << ">\n";
	} 

	return out.str();
}
// inicializace MAC
MAC::MAC(uint8_t _MAC[6]) {
	for(int i = 0; i < 6; ++i) {
		addr[i] = _MAC[i];
	}
}
// Porovnani MAC
int MAC::compare(uint8_t _MAC[6]) {
	for(int i = 0; i < 6; i++) {
		if(addr[i] < _MAC[i]) return -1;
		if(addr[i] > _MAC[i]) return 1;
	}
	return 0;
}
// pridani IP k MAC
void MAC::addIpUtil(uint8_t* _IPv4, char n) {
	for(auto i = v.begin(); i != v.end(); i++) {
		if((*i).compare(_IPv4, n) == 0) {
			return;
		}
	}
	IP ip(_IPv4, n);
	v.push_back(ip);
}
// pridani IPv4 adresy k MAC
void MAC::addIPv4(uint8_t _IPv4[4]) {
	this->addIpUtil(_IPv4, 0);
}
// pridani IPv6 adresy k MAC
void MAC::addIPv6(uint8_t _IPv6[6]) {
	this->addIpUtil(_IPv6, 1);
}
// prevod MAC na xml format
std::string MAC::str(void) {
	std::ostringstream out("");
	out << TAB << "<host mac=\"";
	out << std::setfill('0') << std::setw(2) << std::hex << unsigned(addr[0]);
	out << std::setfill('0') << std::setw(2) << std::hex << unsigned(addr[1]); 
	out << "."; 
	out << std::setfill('0') << std::setw(2) << std::hex << unsigned(addr[2]);
	out << std::setfill('0') << std::setw(2) << std::hex << unsigned(addr[3]);
	out << ".";
	out << std::setfill('0') << std::setw(2) << std::hex << unsigned(addr[4]);
	out << std::setfill('0') << std::setw(2) << std::hex << unsigned(addr[5]);
	out << "\">\n";

	for(auto i = v.begin(); i != v.end(); i++) {
		out << (*i).str();
	}
	out << TAB << "</host>\n";
	return out.str();
}
// Pridani MAC
void Devices::addIpUtil(uint8_t _MAC[6], uint8_t* _IP, char n) {
	for(auto i = v.begin(); i != v.end(); i++) {
		if((*i).compare(_MAC) == 0) {
			if( n == 0) 
			{
				(*i).addIPv4(_IP);
			}
			else
			{
				(*i).addIPv6(_IP);
			}
			return;
		}
	}

	MAC m(_MAC);
	if( n == 0) 
	{
		m.addIPv4(_IP);
	}
	else
	{
		m.addIPv6(_IP);
	}
	v.push_back(m);
}
// Pridani IPv4 adresy k MAC
void Devices::addIPv4(uint8_t _MAC[6], uint8_t _IPv4[4]) {
	this->addIpUtil(_MAC, _IPv4, 0);
}
// Pridani IPv6 adresy k MAC
void Devices::addIPv6(uint8_t _MAC[6], uint8_t _IPv6[6]) {
	this->addIpUtil(_MAC, _IPv6, 1);
}
// Prevod na sting
std::string Devices::str(void) {
	std::ostringstream out("");
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	out << "<devices>\n";
	for(auto i = v.begin(); i != v.end(); i++) {
		out <<(*i).str();
	}
	out << "</devices>\n";
	return out.str();
}
