CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic

CPP=g++
CPPFLAGS=-std=c++11 -Wall -Wextra -pedantic
DOC=ipk-scanner

ipk-scanner: ipk-scanner.o xml.o
	$(CPP) $(CPPFLAGS) $^ -o $@


xml.o: xml.cc xml.hh

%.o: %.cc
	$(CPP) $(CPPFLAGS) -c  $<

tar:
	tar cvf xondre09.tgz xml.cc xml.hh ipk-scanner.cc ipk-scanner.pdf Makefile

doc:
	latex  $(DOC).tex
	bibtex $(DOC)
	latex  $(DOC).tex
	latex  $(DOC).tex
	dvips -sPAPERSIZE=a4 $(DOC).dvi
	ps2pdf $(DOC).ps

clean:
	rm -f *.aux *.dvi *.log *.ps *.bbl *.blg *.te~ *.toc
	rm -f *.o

clean-all: clean
	rm -f ipk-scanner ipk-scanner.pdf xondre09.tgz
