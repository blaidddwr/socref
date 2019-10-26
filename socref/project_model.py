"""
todo
"""
from PySide2.QtCore import Qt,QAbstractItemModel,QModelIndex






class Project_Model(QAbstractItemModel):


    def __init__(self,parent):
        QAbstractItemModel.__init__(self,parent)
        # 
        self.__root = None
        # 
        self.__lang = None
        #:


    def flags(self,index):
        return QAbstractItemModel.flags(self,index)


    def index(self,row,column,parent):
        #.
        if row < 0 or column != 0: return QModelIndex()
        #.
        parent_block = self.block(parent)
        if parent_block is None or row >= len(parent_block): return QModelIndex()
        #.
        return self.createIndex(row,column,parent_block[row])


    def parent(self,child):
        #.
        child_block = self.block(child)
        if child_block is None: return QModelIndex()
        #.
        parent_block = child_block.parent()
        if parent_block is None or parent_block.parent() is None: return QModelIndex()
        #.
        return self.createIndex(parent_block.index(),0,parent_block)


    def rowCount(self,index):
        #.
        if self.__root is None: return 0
        else:
            return self.block(index).children_size()


    def columnCount(self,index):
        return 1


    def data(self,index,role):
        #.
        block = self.block(index)
        if block is not None:
            #.
            if role is Qt.DisplayRole: return block.name()
            else: return None
        #.
        else: return None


    def insertRows(self,row,blocks,parent):
        return False


    def removeRows(self,row,count,parent):
        return False


    def insertRow(self,row,block,parent):
        return self.insertRows(row,(block,),parent)


    def block(self,index):
        #.
        if index.isValid():
            return index.internalPointer()
        else:
            return self.__root
