"""
Contains the main function where execution begins.
"""
from .Private.Controller.LangController import LangController
from .Private.Controller.ParseController import ParseController
from .Private.GUI.Dialog.CodeDialog import CodeDialog
from .Private.GUI.Window.MainWindow import MainWindow
from .Settings import Settings
from PySide6.QtWidgets import QApplication
from PySide6.QtCore import (
    QThread
    ,Qt
)
from sys import (
    argv
    ,exit
)


def main():
    """
    Begins execution of the program.
    """
    application = QApplication(argv)
    for lang in Settings.LANGS:
        LangController.s().load(*lang)
    QApplication.setOrganizationName(Settings.ORGANIZATION_NAME)
    QApplication.setApplicationName(Settings.APPLICATION_NAME)
    parserThread = QThread()
    ParseController.s().remained.connect(lambda code : CodeDialog(code).exec())
    ParseController.s().moveToThread(parserThread)
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
