"""
Contains the main function where execution begins.
"""
import sys
import os
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import abstract
from . import model
from . import gui_window




def main():
    """
    Begins execution of the program.
    """
    #
    # Qt likes to catch all exceptions without exiting.
    #
    print(os.getpid())
    abstract.Factory().load("Python","socref_python")
    application = qtw.QApplication(sys.argv)
    qtc.QCoreApplication.setOrganizationName("Socrates' Gaming Republic")
    qtc.QCoreApplication.setApplicationName("Socrates' Reference")
    parser_thread = qtc.QThread()
    model.Parser().moveToThread(parser_thread)
    parser_thread.start()
    gui_window.Main().show()
    ret = application.exec_()
    parser_thread.quit()
    parser_thread.wait()
    sys.exit(ret)








if __name__ == "__main__" : main()
