DOC=doc.pdf
DOC.DIR=doc
DATA.DIR=data
LOGIN=xondre09

PYTHON=python3
MAKE=make
ZIP=zip
RM=rm -f

default: pack

$(DOC):
	$(MAKE) -s --directory  $(DOC.DIR) $@

pack: $(DOC)
	$(ZIP) $(LOGIN).zip solution.py solution_sat.py $(DOC.DIR)/$(DOC)

run:
	$(PYTHON) solution.py $(DATA.DIR)
	$(PYTHON) solution_sat.py $(DATA.DIR)

clean:
	$(MAKE) -s --directory $(DOC.DIR) $@
	$(RM) $(LOGIN).zip $(DOC)

.PHONY: $(DOC) pack run clean
