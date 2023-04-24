TARGET=formula-2-bdd
LOGIN=xondre09
PACK.FILE=flp-fun-$(LOGIN).zip
SRC=./src
APP=./app
BUILD=./build
MAIN=$(APP)/Main.hs
INCLUDE=$(SRC)

HC=ghc
HFLAGS=--make -i$(INCLUDE) -odir$(BUILD) -hidir$(BUILD)

$(TARGET): $(MAIN) $(SRC)/*.hs
	$(HC) $(HFLAGS) $< -o $@

test: $(TARGET)
	stack test --test-arguments=-q

clean:
	rm -rf $(BUILD) $(TARGET) $(PACK.FILE)
	stack clean

pack: clean
	zip -r $(PACK.FILE) *

.PHONY: test clean
