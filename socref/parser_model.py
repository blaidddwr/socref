"""
todo
"""
import traceback
from PySide2 import QtCore as qtc
from . import abstract_parser as ap
from . import singleton as si






@si.Singleton
class Parser_Model(qtc.QObject):


    def __init__(self):
        qtc.QObject.__init__(self)


    @qtc.Slot(ap.Abstract_Parser)
    def start(self, parser):
        try:
            parser.parse()
        except:
            traceback.print_exc()
