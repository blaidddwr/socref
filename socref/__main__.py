"""
todo
"""
import sys
import os
from PySide2 import QtWidgets as qtw
from . import main_window as mw

from . import block_factory as bf # TEMP






def main():
    print(os.getpid())
    bf.Block_Factory().load("Python","socref_python")
    application = qtw.QApplication(sys.argv)
    mw.Main_Window().show()
    sys.exit(application.exec_())






if __name__ == "__main__":
    main()
