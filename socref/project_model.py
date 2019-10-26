"""
todo
"""
from PySide2.QtCore import QAbstractItemModel






class Project_Model(QAbstractItemModel):
    """
    todo
    """

    def __init__(self,parent):
        """
        todo
        """
        QAbstractItemModel.__init__(self,parent)

    def flags(self,index):
        """
        todo
        """
        pass

    def index(self,row,column,parent):
        """
        todo
        """
        pass

    def parent(self,child):
        """
        todo
        """
        pass

    def rowCount(self,index):
        """
        todo
        """
        pass

    def columnCount(self,index):
        """
        todo
        """
        pass

    def data(self,index,role):
        """
        todo
        """
        pass

    def insertRows(self,row,blocks,parent):
        """
        todo
        """
        pass

    def removeRows(self,row,count,parent):
        """
        todo
        """
        pass

    def insertRow(self,row,block,parent):
        """
        todo
        """
        return self.insertRows(row,(block,),parent)
