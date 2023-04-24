/**
 * @file    Multiplex.cpp
 * @author  Karel Ondřej
 * 
 * @brief
 */
#include <algorithm>
#include <iostream>
#include <iomanip>

#include "Multiplex.h"
#include "NetworkInformationTable.h"
#include "ServiceDescriptionTable.h"
#include "ProgramAssociationTable.h"

const double DOUBLE_BANDWIDTH_TABLE[] = {8., 7., 6., 5, -1., -1., -1., -1.};
const double DOUBLE_CONSTELLATION_TABLE[] = {3./4., 1./2., 1./4., -1., -1., -1., -1.};
const double DOUBLE_CODE_RATE_TABLE[] = {1./2., 2./3., 3./4., 5./6., 7./8., -1., -1., -1.};
const double DOUBLE_GUARD_INTERVAL_TABLE[] = {1./32., 1./16., 1./8., 1./4.};

const double BLACK_MAGIC_CONSTANT = 54000000.;

struct {
    bool operator() (Program* pt1, Program* pt2) { 
        return pt2 ? ((*pt1) < (*pt2)) : true;}
} pointerToProgramSort;

Multiplex::~Multiplex()
{
    for (auto packet: this->packets) {
        delete packet;
    }
    for (auto table: this->tables) {
        delete table;
    }
    for (auto program: this->programs) {
        delete program;
    }
}

void Multiplex::read(std::string filename)
{
    this->packets = MPEGPacket::read(filename);
    this->tables = ServiceInformationTable::tablesFromPackets(packets);

    this->bind(packets, tables);
}

void Multiplex::write(std::ostream& stream) {
    stream << "Network name: " << this->networkName << std::endl;
    stream << "Network ID: " << this->networkID << std::endl;
    stream << "Bandwidth: " << this->bandwidth->sVal << std::endl;
    stream << "Constellation: " << this->constellation->sVal << std::endl;
    stream << "Guard interval: " << this->guardInterval->sVal << std::endl;
    stream << "Code rate: " << this->codeRate->sVal << std::endl;
    stream << std::endl;

    for(auto program: this->programs) {
        std::streamsize savePrecision;
        double bitrate = (double)program->packetsCount 
                       / (double)this->packets.size() 
                       * this->bitRate / 1E6;
        stream << "0x" << std::setfill('0') << std::setw(4) << std::hex
               << (int)program->pid << "-" << program->providerName << "-" 
               << program->serviceName << ": ";
        savePrecision = stream.precision();
        stream << std::fixed << std::setprecision(2) << bitrate << " Mbps" 
               << std::endl;
        stream.precision(savePrecision);
    }
}

void Multiplex::bind(std::vector<MPEGPacket*> packets, std::vector<ServiceInformationTable*> tables)
{
    std::map<int, std::vector<MPEGPacket*>> PID2Packets;
    std::map<int, std::vector<ServiceInformationTable*>> PID2Tables;
    std::map<int, Program*> servideID2Program;
    std::map<int, std::vector<int>> servideID2elementaryPIDs;

    int prc0;
    int prcDelta = 0;
    int gotPRC = 1;
    int pid = 0;
    int mpegPackets = 0;
    double lastBitRate = 0.0;

    for (auto packet: packets) {
        PID2Packets[packet->pid].push_back(packet);
        // výpočet bitrate podle algoritmu na url:
        // http://ecee.colorado.edu/~siewerts/extra/code/example_code_archive/a490dmis_code/example-mpeg2/bitrate.c?fbclid=IwAR0ufBqbtC5fOpx-DkFfGnWuvg_ASIAT70PV_HmUaXvFW31ISFE06fvc6R0
        if (packet->prcPresent) {
            if (gotPRC == 1) {
                prc0 = packet->prc;
                pid = packet->pid;
                mpegPackets = 0;
                gotPRC = 2;
            }
            else if (gotPRC == 2 && pid == packet->pid) {
                prcDelta = packet->prc - prc0;
                double bitRate=(double)(((double)(40608000000ULL)*((double)mpegPackets))/(double)(prcDelta));
                if (lastBitRate > 0.0) {
                    this->bitRate = (bitRate+lastBitRate)/2.0;
                }
                lastBitRate = bitRate;
                gotPRC = 1;
            }
        }
        mpegPackets++;
    }

    for (auto table: tables) {
        PID2Tables[table->pid].push_back(table);
        if (table->tableID == 0x02) {
            int programNumber = table->descriptors[0][0]->infos["program_number"]->iVal;
            for (auto pid: table->descriptors[1]) {
                servideID2elementaryPIDs[programNumber].push_back(pid->infos["elementary_pid"]->iVal);
            }
        }
    }

    this->extractMultiplexHeader(PID2Tables[ServiceInformationTable::NETWORK_INFORMATION_TABLE]);

    ServiceDescriptionTable* service = static_cast<ServiceDescriptionTable*>(PID2Tables[ServiceInformationTable::SERVICE_DESCRIPTION_TABLE][0]);
    ProgramAssociationTable* program = static_cast<ProgramAssociationTable*>(PID2Tables[ServiceInformationTable::PROGRAM_ASSOCIATION_TABLE][0]);

    for (auto infos: service->descriptors[Descriptor::SERVICE_TAG]) {
        int serviceID = infos->infos["serviceID"]->iVal;
        std::string providerName = infos->infos["service_provider_name"]->sVal;
        std::string serviceName = infos->infos["service_name"]->sVal;
        if (servideID2Program.find(serviceID) == servideID2Program.end()) {
            servideID2Program[serviceID] = new Program();
            servideID2Program[serviceID]->serviceID = serviceID;
            servideID2Program[serviceID]->providerName = providerName;
            servideID2Program[serviceID]->serviceName = serviceName;
            std::sort(servideID2elementaryPIDs[serviceID].begin(),
                    servideID2elementaryPIDs[serviceID].end());
            auto last = std::unique(servideID2elementaryPIDs[serviceID].begin(),
                    servideID2elementaryPIDs[serviceID].end());
            servideID2elementaryPIDs[serviceID].erase(last, 
                    servideID2elementaryPIDs[serviceID].end());

            for (auto pid: servideID2elementaryPIDs[serviceID]) {
                if (PID2Packets.find(pid) != PID2Packets.end()) {
                    servideID2Program[serviceID]->packetsCount += PID2Packets[pid].size();
                }
            }        
        }
    }
    for (auto infos: program->descriptors[0]) {
        int serviceID = infos->infos["program_number"]->iVal;
        int pid = infos->infos["program_map_id"]->iVal;
        if (servideID2Program.find(serviceID) != servideID2Program.end()) {
            servideID2Program[serviceID]->pid = pid;
            servideID2Program[serviceID]->packetsCount += PID2Packets[pid].size();
        }
    }
    for(auto item: servideID2Program) {
        this->programs.push_back(item.second);
    }

    std::sort(this->programs.begin(), this->programs.end(), pointerToProgramSort);
}

void Multiplex::extractMultiplexHeader(std::vector<ServiceInformationTable*> tables)
{
    NetworkInformationTable* nit = static_cast<NetworkInformationTable*>(tables[0]);

    this->networkID = nit->networkID;
    this->networkName = nit->networkName;
    this->bandwidth = nit->descriptors[Descriptor::TERRESTRIAL_DELIVERY_SYSTEM_TAG][0]->infos["bandwidth"];
    this->constellation = nit->descriptors[Descriptor::TERRESTRIAL_DELIVERY_SYSTEM_TAG][0]->infos["constellation"];
    this->codeRate = nit->descriptors[Descriptor::TERRESTRIAL_DELIVERY_SYSTEM_TAG][0]->infos["code_rate-HP_stream"];
    this->guardInterval = nit->descriptors[Descriptor::TERRESTRIAL_DELIVERY_SYSTEM_TAG][0]->infos["guard_interval"];
}   

bool Program::operator<(const Program& program) const
{
    return (this->pid < program.pid);
}