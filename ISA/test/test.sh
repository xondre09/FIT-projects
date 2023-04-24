PROG=isashark
PCAP=pcap
OUT=output

t()
{
	if [ $? = 0 ]
	then
		echo -e "\e[32mTest$1\e[0m"
	else
		echo -e "\e[31mTest$1\e[0m"
		echo "$2"
	fi
}

output=`diff -u1 <( ../isashark $PCAP/eth_ipv4_tcp.pcap ) $OUT/eth_ipv4_tcp.out 2>&1`
t 01 "$output"

output=`diff -u1 <( ../isashark $PCAP/eth_dot1q_ipv6_udp.pcap ) $OUT/eth_dot1q_ipv6_udp.out`
t 02 "$output"

output=`diff -u1 <( ../isashark $PCAP/eth_dot1ad_ipv4_icmpv4.pcap | sed -e 's/ICMPv\(.*\)/ICMPv\L\1/') $OUT/eth_dot1ad_ipv4_icmpv4.out`
t 03 "$output"

output=`diff -u1 <( ../isashark $PCAP/eth_ipv6_icmpv6.pcap | sed -e 's/ICMPv\(.*\)/ICMPv\L\1/' ) $OUT/eth_ipv6_icmpv6.out`
t 04 "$output"

output=`diff -u1 <( ../isashark $PCAP/mix.pcap ) $OUT/mix.out`
t 05 "$output"

output=`diff -u1 <( ../isashark -f "src host 2001:db8::1" $PCAP/mix.pcap  ) $OUT/mix_filter.out`
t 06 "$output"

output=`diff -u1 <( ../isashark -l 3 $PCAP/mix.pcap  ) $OUT/mix_limit.out`
t 07 "$output"

output=`diff -u1 <( ../isashark -a srcip $PCAP/mix.pcap | sort ) <(sort  $OUT/mix_a.out  )`
t 08 "$output"

output=`diff -u1 <( ../isashark -a srcip -s bytes $PCAP/mix.pcap  ) $OUT/mix_a_s.out`
t 09 "$output"

output=`diff -u1 <( ../isashark -s bytes $PCAP/mix.pcap  ) $OUT/mix_s.out`
t 10 "$output"

output=`diff -u1 <( ../isashark $PCAP/fragmentation.pcap  ) $OUT/fragmentation.out`
t 11 "$output"
