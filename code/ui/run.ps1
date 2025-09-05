pyside6-uic ui/main_window.ui -o main_window.py
pyside6-uic ui/pass_window.ui -o pass_window.py
pyside6-uic ui/save_window.ui -o save_window.py
pyside6-uic ui/send_window.ui -o send_window.py
pyside6-uic ui/rename_window.ui -o rename_window.py

pyside6-rcc res/res.qrc -o res_rc.py

python main.py