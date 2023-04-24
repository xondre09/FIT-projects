NAME=manual
LATEX=pdflatex
BIBTEX=bibtex 

default: doc

doc:
	$(LATEX) $(NAME).tex
	$(BIBTEX) $(NAME).aux
	$(LATEX) $(NAME).tex
	$(LATEX) $(NAME).tex

install: doc
	mv $(NAME).pdf ../

clean:
	rm -f $(NAME).pdf $(NAME).aux $(NAME).bbl $(NAME).blg $(NAME).log $(NAME).toc