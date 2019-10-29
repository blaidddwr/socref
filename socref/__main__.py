"""
todo
"""
import sys
import os
from PySide2.QtWidgets import QApplication
from .main_window import Main_Window

from PySide2.QtCore import QByteArray,QXmlStreamWriter
from .block_factory import Block_Factory # TEMP




def main():
    print(os.getpid())
    Block_Factory().load("Python","socref_python")
    package = Block_Factory().create_root("Python")
    package2 = Block_Factory().create_root("Python")
    package2.set_default_properties()
    package.append(package2)
    ba = QByteArray()
    stream = QXmlStreamWriter(ba)
    stream.setAutoFormatting(True)
    stream.writeStartDocument()
    package.to_xml(stream)
    stream.writeEndDocument()
    print(str(ba,"utf-8"))
    application = QApplication(sys.argv)
    main_window = Main_Window()
    main_window.show()
    sys.exit(application.exec_())






if __name__ == "__main__":
    main()
