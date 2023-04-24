/**
 * @file    Multiplex.h
 * @author  Karel Ondřej
 * 
 * @brief
 */
#include <iostream>                     // std::ostream
#include <string>                       // std::string

#include "Infobox.h"                    // Infobox
#include "MPEGPacket.h"                 // MPEGPacket
#include "ServiceInformationTable.h"    // ServiceInformationTable

class Program
{
public:
    double pid;
    double serviceID;
    std::string providerName;
    std::string serviceName;
    int packetsCount;

    bool operator< (const Program& program) const;
};

class Multiplex
{
private:
    std::string networkName;
    int networkID;
    Infobox* bandwidth;
    Infobox* constellation;
    Infobox* codeRate;
    Infobox* guardInterval;

    double bitRate;

    std::vector<MPEGPacket*> packets;
    std::vector<ServiceInformationTable*> tables;
    std::vector<Program*> programs;
public:
    Multiplex() = default;
    ~Multiplex();

    void read(std::string filename);
    void write(std::ostream& stream);

private:
    void bind(std::vector<MPEGPacket*> packets, std::vector<ServiceInformationTable*> tables);
    void extractMultiplexHeader(std::vector<ServiceInformationTable*>);
    std::vector<Program*> extractPrograms();
};
