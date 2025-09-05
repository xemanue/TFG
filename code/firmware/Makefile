all: docs

docs:
	@echo -e "Generating documentation...\n"
	@doxygen doxyconf > /dev/null
	@echo -e "Done."

clean:
	@echo -e "Cleaning docs...\n"
	@-rm -dr ./doxyfiles/html/
	@echo -e "Done."
