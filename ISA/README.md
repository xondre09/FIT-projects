# ISA - Síťové aplikace a správa sítí - 2017/2018

Program slouží k offline analýze síťového provozu uloženého v souboru formátu PCAP. Aplikace podporuje rodinu protokolů TCP/IP a to na L2 
vrstvě protokol Ethernet, na L3 vrstvě protokoly IPv4, IPv6, ICMPv4 a ICMPv6, na L4 vrstvě UDP s TCP protokolem. Vrstva L7 se neanalyzuje.

## Použití
```
isashark [-h] [-a aggr-key] [-s sort-key] [-l limit] [-f filter-expression] file ...

-h                      Vypíše nápovědu na standartí výstup a ukončí program.
-a aggr-key             Seskupí pakety do skupin podle zadaného kritéria a určí jejich celkovou velikost a počet. Klíč aggr-key může nabývat
                        hodnot srcmac pro zdrojovou MAC adresu, dstmac pro cílovou adresu, srcip pro zdrojovou IP adresu, dstip pro cílovou 
                        IP adresu, srcport pro zdrojový port a dstport pro cílový port. V případě použití srcport a dstport se protokoly ICMP
                        nebudou agregovat.
-s sort-key             Seřazení paketů nebo agregovaných skupin podle klíče sort-key. Hodnoty sort-key můžou být bytes pro seřazení podle 
                        velikosti nebo pakets pro seřazení podle počtu paketů. Seřazení paketů podle klíče pakets nebude mít žádný vliv.
-l limit                Vypíše první položky až do hodnoty limitu.
-f filter-expression    Pakety budou vyfiltrovány na základě zadaného výrazu.
file                    Jednotlivé soubory formátu PCAP obsahující záznam provozu na síti.
```

## Příklady


1. Seskupení paketů podle cílové MAC adresy bez řazení.

    ./isashark -a srcmac file.pcap

2. Seskupení paketů podle zdrojového portu a seřazení podle počtu paketů ze zadaných souborů.

    ./isashark -a dstport -s pakets file01.pcap file02.pcap file03.pcap 

3. Program analyzuje zadaný soubor, z kterého vyfiltruje všechny pakety s cílovou IP adresou 192.168.56.1.

    ./isashark -f "dst host 192.168.56.1" file.pcap 

4. Vypsání prvních 10 paketů seřazených podle velikosti.

    ./isashark -l 10 -s bytes file.pcap 
