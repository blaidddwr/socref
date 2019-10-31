"""
todo
"""
import sys
import os
from PySide2.QtWidgets import QApplication
from .main_window import Main_Window

from .block_factory import Block_Factory # TEMP






def main():
    print(os.getpid())
    Block_Factory().load("Python","socref_python")
    application = QApplication(sys.argv)
    Main_Window().show()
    sys.exit(application.exec_())






if __name__ == "__main__":
    main()
