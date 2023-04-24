MEMBERS=xondre09-xsmeka13
MAKEFILE=Makefile

all: 
	cd ./src/ && make -f $(MAKEFILE)

pack:
	zip -r $(MEMBERS).zip ./doc/ ./example/ ./src/*.h ./src/*.cc ./src/*.pro ./src/*.qrc ./src/Makefile ./cards/*.png Makefile README.txt

doxygen:
	doxygen ./doc/Doxyfile ./src

run:
	./src/hra2017
	./src/hra2017-cli

clean:
	cd ./src/ && make -f $(MAKEFILE) clean

clean-all: clean
	rm -rf ./doc/html
	cd ./src/ && make -f $(MAKEFILE) clean-all
