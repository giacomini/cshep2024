.PHONY: all clean realclean

TEXFILES = $(shell echo *.tex)
LATEX = pdflatex -halt-on-error -file-line-error
TRANS = "\PassOptionsToClass{trans}{beamer}\input{cshep}"
#TRANS = "\input{pf}"

cshep2024.pdf: $(TEXFILES)
	rm -f contents.tex
	echo '\input{introduction.tex}' > contents.tex
	$(LATEX) -jobname=cshep2024 cshep
	$(LATEX) -jobname=cshep2024 cshep
	rm contents.tex

cshep2024p.pdf: $(TEXFILES)
	rm -f contents.tex
	echo '\input{introduction.tex}' > contents.tex
	$(LATEX) -jobname=cshep2024p $(TRANS)
	$(LATEX) -jobname=cshep2024p $(TRANS)
	rm contents.tex

introduction.pdf: cshep.tex frontmatter.tex introduction.tex
	echo '\input{introduction.tex}' > contents.tex
	$(LATEX) -jobname=introduction $(TRANS)
	$(LATEX) -jobname=introduction $(TRANS)
	rm contents.tex

clean:
	rm -f *.out *.aux *.nav *.toc *.vrb *.log *.snm contents.tex

realclean: clean
	rm -f *.pdf
