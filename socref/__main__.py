"""
Contains the main function where execution begins.
"""
import sys
import os
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import main_window as mw
from . import parser_model as pm
from . import block_factory as bf






def main():
    """
    Begin execution of the program.
    """
    #
    # print the PID so the application can be killed. Qt likes to catch all exceptions without
    # exiting.
    #
    print(os.getpid())
    #
    # Load the python language.
    #
    bf.Block_Factory().load("Python","socref_python")
    #
    # Create a new Qt application.
    #
    application = qtw.QApplication(sys.argv)
    #
    # Create the parser thread, move the parser model to it, and then start its event loop.
    #
    parser_thread = qtc.QThread()
    pm.Parser_Model().moveToThread(parser_thread)
    parser_thread.start()
    #
    # Create and show an initial main window.
    #
    mw.Main_Window().show()
    #
    # Start the GUI event loop, saving its exit code when it returns.
    #
    ret = application.exec_()
    #
    # Request the parser thread quit and wait for it to stop.
    #
    parser_thread.quit()
    parser_thread.wait()
    #
    # Exit the application with the return code provided by the Qt application.
    #
    sys.exit(ret)
