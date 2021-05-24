"""
Contains the main function where execution begins.
"""
from .Private.Controller import langController
from .Private.Controller import parseController
from .Private.GUI.Dialog.CodeDialog import *
from .Private.GUI.Window.MainWindow import *
from PySide6.QtWidgets import QApplication
from PySide6.QtCore import QThread
from PySide6.QtCore import Qt
from sys import argv
from sys import exit


def main():
    """
    Begins execution of the program.
    """
    application = QApplication(argv)
    #langController.load("C++","socref_cpp")
    #langController.load("C++/Qt","socref_cppqt")
    langController.load("GLSL","socref_glsl")
    langController.load("Python","socref_python")
    QApplication.setOrganizationName("Socrates' Gaming Republic")
    QApplication.setApplicationName("Socrates' Reference")
    parserThread = QThread()
    parseController.remained.connect(lambda code : CodeDialog(code).exec())
    parseController.moveToThread(parserThread)
    parserThread.start()
    try:
        main = MainWindow()
        if len(argv) > 1:
            main.open_(argv[1])
        main.show()
        ret = application.exec()
    finally:
        parserThread.quit()
        parserThread.wait()
    exit(ret)


if __name__ == "__main__":
    main()
