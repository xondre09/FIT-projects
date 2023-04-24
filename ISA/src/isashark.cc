/*!
 * \file    isashark.c
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   Aplikace analyzujici offline provoz na siti ze souboru
 */

// #include <iostream>

#include <unistd.h>

#include "parser.h"

#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE (1500)
#endif

// Zprava help, ktera se vypise na vystup
#define HELP \
"Usage: isashark [-h] [-a KEY] [-s KEY] [-l LIMIT] [-f EXPRESSION] FILE [FILE ...]\n\n" \
"Positional arguments:\n" \
"  FILE           pcap file\n\n" \
"Optional arguments:\n" \
"  -h             help\n" \
"  -a KEY         aggr (srcmac, dstmac, srcip, dstip, srcport, dstport)\n" \
"  -s KEY         sort (bytes, pakets)\n" \
"  -l LIMIT       limit of output line\n" \
"  -f EXPRESSION  packet filtering by expression" \


using namespace std;

int main( int argc, char *argv[] ) {
    pcap_t *fd;
    char errbuf[PCAP_ERRBUF_SIZE];
    int groupBy = 0;
    int c;
    int sortType = NO_SORT;
    int limit = 0;
    char *filterExpression = NULL;

    // Zpracovani parametru
    while ((c = getopt(argc, argv, ":ha:l:s:f:")) != -1) {
        switch( c ) {
            case 'h':   // help
                std::cout << HELP << std::endl;
                return 0;
                break;
            case 'a':   // agregace
                if( !strcmp( optarg, "srcmac" ) )       groupBy = SRC_MAC;
                else if( !strcmp( optarg, "dstmac" ) )  groupBy = DST_MAC;
                else if( !strcmp( optarg, "srcip" ) )   groupBy = SRC_IP;
                else if( !strcmp( optarg, "dstip" ) )   groupBy = DST_IP;
                else if( !strcmp( optarg, "srcport" ) ) groupBy = SRC_PORT;                
                else if( !strcmp( optarg, "dstport" ) ) groupBy = DST_PORT;
                else return 1;
                break;
            case 'l':   // limit
                limit = atoi( optarg );
                if( limit <= 0) {
                    std::cerr << "Wrong limit format." <<std::endl;
                    return 1;
                }
                break;
            case 's':   // serazeni
                if( !strcmp( optarg, "packet" ) )     sortType = PACKET_SORT;
                else if( !strcmp( optarg, "bytes" ) ) sortType = LEN_SORT;
                else return 1;
                break;
            case 'f':   // filter
                filterExpression = optarg;
                break;
            case ':':
                std::cerr << optarg << std::endl;
                return 1;
                break;
            case '?':
                return 1;
                break;
        }    

    }
    // Chybi vstupni soubor
    if( optind == argc ) {
        std::cerr << "Missing input file." << std::endl;
        return 1;
    }

    Parser parser = Parser( sortType );
    // Zpracovani vsech souboru
    while( optind < argc ) {
        struct bpf_program filter;
        fd = pcap_open_offline( argv[optind], errbuf);
        if (fd == NULL) {
            cerr << errbuf << endl;
            return 1;
        }
        // Filtry
        if( filterExpression != NULL ) {
            if (pcap_compile(fd, &filter, filterExpression, 0, 0) == -1) {
                std::cerr << "Couldn't parse filter " << filterExpression << ": " << pcap_geterr(fd) << std::endl;
                return 1;
            }

            if (pcap_setfilter(fd, &filter) == -1) {
                std::cerr << "Couldn't set filter " << filterExpression << ": " << pcap_geterr(fd) << std::endl;
                return 1;
            }

            pcap_freecode(&filter);
        }

        struct pcap_pkthdr pkthdr;
        const u_char* packet;
        // Zpracovani paketu ze souboru

        while ((packet = pcap_next(fd, &pkthdr)) != NULL) {
            parser.parse( pkthdr, packet );
        }

        optind++;
    }  

    int idx = 0;
    if( groupBy ) { // byla zadana agregace
        std::vector<Parser::Group*> v = parser.groupBy( groupBy );
        for( auto item: v ) {
            idx++;
            std::cout << item->toString() << std::endl;
            if( limit != 0 && idx == limit ) break;
        }
    } else {
        std::vector<Packet*> v = parser.getPackets() ;  
        for( auto item: v ) {
            idx++;
            std::cout << item->toString() << std::endl;
            if( limit != 0 && idx == limit ) break;
        }   
    }
    pcap_close( fd );
    return 0;
}