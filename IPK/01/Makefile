CC=g++
CFLAGS=-std=c++11 -Wall -Wextra -pedantic

all: ftrestd ftrest

ftrestd: ftrestd.o http.o
	$(CC) $(CFLAGS)  $^ -o $@

ftrest: ftrest.o http.o
	$(CC) $(CFLAGS)  $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<


ftrestd.o: ftrestd.cpp ftrestd.h http.h
ftrest.o: ftrest.cpp ftrest.h http.h
http.o: http.cpp http.h


tar:
	tar -cvzf xondre09.tgz http.cpp http.h ftrest.cpp ftrest.h ftrestd.cpp ftrestd.h Makefile readme.md

test:
	@echo Vytvoření adresáře bar na serveru bežícím na lokálním počítači a portu 12345;
	./ftrest mkd http://localhost:12345/tonda/foo/bar
 
	@echo Nahrání souboru doc.pdf na serveru do adresáře bar:;
	./ftrest put http://localhost:12345/tonda/foo/bar/doc.pdf ./proj2.pdf
	  
	@echo Stažení souboru doc.pdf do lokálního adresáře:;
	./ftrest get http://localhost:12345/tonda/foo/bar/doc.pdf
	 
	@echo Odstranění souboru doc.pdf:
	./ftrest del http://localhost:12345/tonda/foo/bar/doc.pdf
	  
	@echo Odstranění adresáře foo:
	./ftrest rmd http://localhost:12345/tonda/foo/bar

clean:
	rm -f *.o

clean-all:
	rm -f *.o ftrest ftrestd
