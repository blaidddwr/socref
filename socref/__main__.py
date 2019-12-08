"""
Contains the main function where execution begins.
"""
import sys
import os
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import block
from . import model
from . import gui_dialog
from . import gui_window




def main():
    """
    Begins execution of the program.
    """
    #
    # Qt likes to catch all exceptions without exiting.
    #
    print(os.getpid())
    block.BlockFactory().load("C++","socref_cpp")
    block.BlockFactory().load("Python","socref_python")
    application = qtw.QApplication(sys.argv)
    qtc.QCoreApplication.setOrganizationName("Socrates' Gaming Republic")
    qtc.QCoreApplication.setApplicationName("Socrates' Reference")
    parserThread = qtc.QThread()
    model.ParserModel().remained.connect(lambda code : gui_dialog.CodeDialog(code).exec_())
    model.ParserModel().moveToThread(parserThread)
    parserThread.start()
    gui_window.MainWindow().show()
    ret = application.exec_()
    parserThread.quit()
    parserThread.wait()
    sys.exit(ret)








if __name__ == "__main__": main()
