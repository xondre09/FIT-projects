TARGET=flp19-log
LOGIN=xondre09
ARCHIVE=flp-log-$(LOGIN).zip

PROLOG=swipl
PROLOG.FLAGS=-q -g main

SRC.DIR=src
SOURCES=$(SRC.DIR)/turing-machine.pl

default: $(TARGET)

$(TARGET): $(SOURCES)
	$(PROLOG) $(PROLOG.FLAGS) -o $@ -c $<

clean:
	rm -f $(ARCHIVE) $(TARGET)

run: $(TARGET)
	@cat $(FILE)
	@echo "-------"
	@bash -c "./$(TARGET) < $(FILE)" | cat


run-color: $(TARGET)
	@cat $(FILE)
	@echo "-------"
	@bash -c "./$(TARGET) < $(FILE) | sed -e 's/\ /./g' | GREP_COLOR='01;31' grep --color=always -e '[A-Z]' | GREP_COLOR='01;4' grep --color=always -P '(?<=[A-Z])[a-z0-9. Δ]'"

test: $(TARGET)
	@stack test --test-arguments=-q

pack: clean
	zip -r $(ARCHIVE) $(SOURCES) Makefile data/ test/ package.yaml README.md

.PHONY: clean pack test
