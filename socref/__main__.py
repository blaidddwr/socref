"""
todo
"""
import sys
import os
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import main_window as mw
from . import parser_model as pm

from . import block_factory as bf # TEMP






def main():
    print(os.getpid())
    bf.Block_Factory().load("Python","socref_python")
    application = qtw.QApplication(sys.argv)
    parser_thread = qtc.QThread()
    pm.Parser_Model().moveToThread(parser_thread)
    parser_thread.start()
    mw.Main_Window().show()
    ret = application.exec_()
    parser_thread.quit()
    parser_thread.wait()
    sys.exit(ret)






if __name__ == "__main__":
    main()
