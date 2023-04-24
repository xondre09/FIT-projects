TARGET=isashark

CCPP=g++
CPPFLAGS=-std=c++11 -Wall -Wextra -pedantic -g -I$(INCDIR)

OBJDIR=src
INCDIR=include
SRCDIR=src

PACK=xondre09.tar

_DEPS=ethernet.h ipv4.h ipv6.h icmpv4.h icmpv6.h udp.h tcp.h packet.h parser.h level.h
_SRC=isashark.cc ethernet.cc ipv4.cc ipv6.cc icmpv4.cc icmpv6.cc udp.cc tcp.cc packet.cc parser.cc
_OBJ=$(_SRC:.cc=.o)


SRC  = $(patsubst %,$(SRCDIR)/%,$(_SRC))
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))
OBJ  = $(patsubst %,$(OBJDIR)/%,$(_OBJ))


all: $(OBJDIR) $(TARGET)

$(TARGET): $(OBJ)
	$(CCPP) $(CPPFLAGS) $^ -o $@ -lpcap

$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(DEPS)
	$(CCPP) $(CPPFLAGS) -c $< -o $@

test: all
	cd test && bash test.sh

pack: doc
	tar -cvf $(PACK) $(DEPS) $(SRC) Makefile manual.pdf README

doc:
	cd ./doc && make install

clean:
	rm -rf $(OBJ)

clean-all:
	rm -rf $(OBJ) $(PACK) $(TARGET) manual.pdf 

.PHONY: clean clean-all pack test doc
