CCPP=g++
CPPFLAGS=-std=c++11 -Wall -Wextra -pedantic -g -fPIC -I$(INCLUDE.DIR)
TARGET=bms1
LOGIN=xondre09

SRC.DIR=src
INCLUDE.DIR=include
BUILD.DIR=build
DOC.DIR=doc
DATA.DIR=data
DATA.URI=https://www.fit.vutbr.cz/study/courses/BMS/public/proj2019/multiplex.zip


SOURCES = $(shell find $(SRC.DIR) -name '*.cpp')
HEADERS = $(shell find $(INCLUDE.DIR) -name '*.h')
OBJECTS = $(subst $(SRC.DIR),$(BUILD.DIR),$(SOURCES:%.cpp=%.o))

default: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CCPP) $(CPPFLAGS) $^ -o $@

$(BUILD.DIR)/%.o: $(SRC.DIR)/%.cpp $(HEADERS) | $(BUILD.DIR)
	$(CCPP) $(CPPFLAGS) -c $< -o $@

$(BUILD.DIR):
	mkdir $@

$(DATA.DIR):
	mkdir $@ 

download: | $(DATA.DIR)
	cd $(DATA.DIR) && wget $(DATA.URI) && unzip *.zip && rm *.zip

clean:
	rm -rf $(OBJECTS) $(TARGET) $(DOC.DIR) $(BUILD.DIR)

code-line:
	@wc -l $(SRC.DIR)/* $(INCLUDE.DIR)/* | tail -n 1

doc:
	doxygen

pack:
	rm -f $(LOGIN).zip
	zip $(LOGIN).zip Makefile $(SRC.DIR)/*.cpp $(INCLUDE.DIR)/*.h

run: $(TARGET)
	./$(TARGET) ./data/multiplex.ts
	@cat multiplex.txt

.PHONY: clean run code-line doc
