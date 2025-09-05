all: docs

docs:
	@echo -e "Generating documentation...\n"
	@doxygen doxyconf > /dev/null
	@echo -e "Done."

clean:
	@echo -e "Cleaning PySide6 files...\n"
	@-rm main_window.py
	@-rm pass_window.py
	@-rm save_window.py
	@-rm send_window.py
	@-rm rename_window.py
	@-rm wait_window.py
	@-rm res_rc.py
	@echo -e "Cleaning docs...\n"
	@-rm -dr ./doxyfiles/html/
	@echo -e "Done."
