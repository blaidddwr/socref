"""
Contains the main function where execution begins.
"""
import sys
from PySide2 import QtCore as qtc
from PySide2 import QtWidgets as qtw
from . import core
from . import gui




def main():
    """
    Begins execution of the program.
    """
    core.blockFactory.load("C++","socref_cpp")
    core.blockFactory.load("C++/Qt","socref_cppqt")
    #core.blockFactory.load("GLSL","socref_glsl")
    core.blockFactory.load("Python","socref_python")
    qtc.QCoreApplication.setAttribute(qtc.Qt.AA_UseHighDpiPixmaps)
    qtc.QCoreApplication.setAttribute(qtc.Qt.AA_EnableHighDpiScaling)
    application = qtw.QApplication(sys.argv)
    qtc.QCoreApplication.setOrganizationName("Socrates' Gaming Republic")
    qtc.QCoreApplication.setApplicationName("Socrates' Reference")
    core.initialize()
    parserThread = qtc.QThread()
    core.parser.remained.connect(lambda code : gui.CodeDialog(code).exec_())
    core.parser.moveToThread(parserThread)
    parserThread.start()
    try:
        main = gui.MainWindow()
        if len(sys.argv) > 1:
            main.open_(sys.argv[1])
        main.show()
        ret = application.exec_()
    finally:
        parserThread.quit()
        parserThread.wait()
    sys.exit(ret)








if __name__ == "__main__": main()
