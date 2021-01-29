"""
Contains the main function where execution begins.
"""
from .Private.Controller import langController
from .Private.GUI.Dialog.CodeDialog import *
from .Private.GUI.Window.MainWindow import *
from .Private.Model import parserModel
from PySide2.QtWidgets import QApplication
from PySide2.QtCore import QCoreApplication
from PySide2.QtCore import QThread
from PySide2.QtCore import Qt
from sys import argv
from sys import exit


def main():
    """
    Begins execution of the program.
    """
    #langController.load("C++","socref_cpp")
    #langController.load("C++/Qt","socref_cppqt")
    #langController.load("GLSL","socref_glsl")
    #langController.load("Python","socref_python")
    QCoreApplication.setAttribute(Qt.AA_UseHighDpiPixmaps)
    QCoreApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
    application = QApplication(argv)
    QCoreApplication.setOrganizationName("Socrates' Gaming Republic")
    QCoreApplication.setApplicationName("Socrates' Reference")
    parserThread = QThread()
    parserModel.remained.connect(lambda code : CodeDialog(code).exec_())
    parserModel.moveToThread(parserThread)
    parserThread.start()
    try:
        main = MainWindow()
        if len(argv) > 1:
            main.open_(argv[1])
        main.show()
        ret = application.exec_()
    finally:
        parserThread.quit()
        parserThread.wait()
    exit(ret)


if __name__ == "__main__":
    main()
