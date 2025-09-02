all: pdf

pdf:
	@echo -e "Building PDF...\n"
	@latexmk -pdf -pdflatex="pdflatex -interaction=batchmode" -halt-on-error -use-make memoria.tex
	@echo -e "Done."

clean:
	@echo -e "Cleaning up...\n"
	@latexmk -CA
	@echo -e "Done."
