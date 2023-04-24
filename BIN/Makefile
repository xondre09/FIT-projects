LOGIN=xondre09
ARCHIVE=$(LOGIN).zip

SRC.DIR=src
INCLUDE.DIR=include
BUILD.DIR=build

CCPP=g++
CPPFLAGS=-std=c++17 -Wall -Wextra -pedantic -g -O2 -I$(INCLUDE.DIR)

TARGET=bin-gp

EXPERIMENT=gp-cryptography-`date +%y%m%d%H%M%S`.xml

FILES+=expression.cpp
FILES+=function.cpp
FILES+=individual.cpp
FILES+=gp.cpp
OBJECTS=$(FILES:%.cpp=$(BUILD.DIR)/%.o)

#---------------------------------------------------------------------------------
# PARAMETRY
#---------------------------------------------------------------------------------
DEEP.MIN=4
DEEP.MAX=4
POPULATION.SIZE=50
TERMINATE.EVALUATE=500000
RUN=50
#---------------------------------------------------------------------------------


default: $(TARGET)

$(TARGET): $(OBJECTS: %.o=$(BUILD.DIR)/%.o)
	$(CCPP) $(CPPFLAGS) $^ -o $@

$(BUILD.DIR)/%.o: $(SRC.DIR)/%.cpp $(INCLUDE.DIR)/%.h | $(BUILD.DIR)
	$(CCPP) $(CPPFLAGS) -c $< -o $@

$(BUILD.DIR):
	mkdir $@

run: $(TARGET)
	@echo "Run experiment '$(EXPERIMENT)'."
	@./$(TARGET) $(DEEP.MIN) $(DEEP.MAX) $(POPULATION.SIZE) $(RUN) $(TERMINATE.EVALUATE) > $(EXPERIMENT)

pack:
	rm -f $(ARCHIVE)
	zip -r $(ARCHIVE) prezentace.pdf README.md Makefile $(SRC.DIR) $(INCLUDE.DIR) prototyp/

clean:
	rm -f $(OBJECTS) $(TARGET) $(ARCHIVE)
	rmdir $(BUILD.DIR)

.PHONY: pack run clean default
