"""
todo
"""
from PySide2.QtCore import Qt,QAbstractItemModel,QModelIndex
from .block_factory import Block_Factory






class Project_Model(QAbstractItemModel):


    def __init__(self,parent):
        QAbstractItemModel.__init__(self,parent)
        #
        self.__root = None
        #
        self.__lang = None
        #:


    def headerData(self,section,orientation,role):
        if orientation == Qt.Horizontal and section == 0 and role == Qt.DisplayRole:
            return self.__lang
        else: return QAbstractItemModel.headerData(self,section,orientation,role)


    def flags(self,index):
        flags = QAbstractItemModel.flags(self,index)
        if index.isValid(): return Qt.ItemIsDragEnabled|Qt.ItemIsDropEnabled|flags
        else: return Qt.ItemIsDropEnabled|flags


    def index(self,row,column,parent):
        if row < 0 or column != 0: return QModelIndex()
        parent_block = self.__block_(parent)
        if parent_block is None or row >= len(parent_block): return QModelIndex()
        return self.createIndex(row,column,parent_block[row])


    def parent(self,child):
        child_block = self.__block_(child)
        if child_block is None: return QModelIndex()
        parent_block = child_block.parent()
        if parent_block is None or parent_block.parent() is None: return QModelIndex()
        return self.createIndex(parent_block.index(),0,parent_block)


    def rowCount(self,index):
        block = self.__block_(index)
        if block is None: return 0
        else: return len(block)


    def columnCount(self,index):
        return 1


    def data(self,index,role):
        block = self.__block_(index)
        if block is not None:
            if role == Qt.DisplayRole: return block.name()
            else: return None
        else: return None


    def insertRows(self,row,block_names,parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or row > len(parent_block): return False
        self.beginInsertRows(parent,row,row + len(block_names) - 1)
        for block_name in block_names:
            new_block = Block_Factory().create(self.__lang,block_name)
            new_block.set_default_properties()
            parent_block.insert(row,new_block)
            row += 1
        self.endInsertRows()
        return True


    def removeRows(self,row,count,parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or count < 0 or (row + count) > len(parent_block):
            return False
        self.beginRemoveRows(parent,row,row + count - 1)
        while count:
            del parent_block[row]
            count -= 1
        self.endRemoveRows()
        return True


    def insertRow(self,row,block_name,parent):
        return self.insertRows(row,(block_name,),parent)


    def new(self,lang_name):
        self.beginResetModel()
        try:
            self.__lang = lang_name
            self.__root = Block_Factory().create_root(self.__lang)
        except:
            self.__lang = None
            self.__root = None
            raise
        finally:
            self.endResetModel()


    def __block_(self,index):
        #.
        if index.isValid():
            return index.internalPointer()
        else:
            return self.__root
