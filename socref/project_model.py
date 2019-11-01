"""
todo
"""
from PySide2.QtCore import (Qt
                            ,Slot as QtSlot
                            ,QByteArray
                            ,QXmlStreamReader
                            ,QXmlStreamWriter
                            ,QAbstractItemModel
                            ,QModelIndex)
from .block_factory import Block_Factory
from .project_commands import *






class Project_Model(QAbstractItemModel):


    BUILD_LIST_ROLE = Qt.UserRole + 0


    def __init__(self,parent=None):
        QAbstractItemModel.__init__(self,parent)
        #
        self.__root = None
        #
        self.__lang_name = None
        #
        self.__undo_stack = []
        #
        self.__undo_stack_index = 0
        #:


    @QtSlot()
    def undo(self):
        if self.can_undo():
            self.__undo_stack_index -= 1
            self.__undo_stack[self.__undo_stack_index].undo()


    @QtSlot()
    def redo(self):
        if self.can_redo():
            self.__undo_stack[self.__undo_stack_index].redo()
            self.__undo_stack_index += 1


    def can_undo(self):
        return self.__undo_stack_index > 0


    def can_redo(self):
        return self.__undo_stack_index < len(self.__undo_stack)


    def headerData(self,section,orientation,role):
        if orientation == Qt.Horizontal and section == 0 and role == Qt.DisplayRole:
            return self.__lang_name
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
            if role == Qt.DisplayRole: return block.display_name()
            elif role == self.BUILD_LIST_ROLE: return block.build_list()
            else: return None
        else: return None


    def insertRows(self,row,block_names,parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or row > len(parent_block): return False
        blocks = []
        for block_name in block_names:
            block = Block_Factory().create(self.__lang_name,block_name)
            block.set_default_properties()
            blocks.append(block)
        self.__push_(Insert(row,blocks,parent,self))
        return True


    def removeRows(self,row,count,parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or count < 0 or (row + count) > len(parent_block):
            return False
        self.__push_(Remove(row,count,parent,self))
        return True


    def insertRow(self,row,block_name,parent):
        return self.insertRows(row,(block_name,),parent)


    def move_row(self,change,index):
        if not change or not index.isValid(): return False
        block = self.__block_(index.parent())
        if block is None: return False
        to_row = index.row() + change
        if to_row < 0 or to_row >= len(block): return False
        self.__push_(Move(change,index,self))
        return True


    def new(self,lang_name):
        self.beginResetModel()
        try:
            self.__lang_name = lang_name
            self.__root = Block_Factory().create_root(self.__lang_name)
        except:
            self.__lang_name = None
            self.__root = None
            raise
        finally:
            self.endResetModel()


    def copy_to_xml(self,indexes):
        block_names = set()
        ba = QByteArray()
        stream = QXmlStreamWriter(ba)
        stream.setAutoFormatting(True)
        stream.writeStartDocument()
        stream.writeStartElement(self.__COPY_TAG)
        stream.writeTextElement(self.__LANG_TAG,self.__lang_name)
        for index in indexes:
            block = self.__block_(index)
            if block is not None:
                block.to_xml(stream)
                block_names.add(block.type_name())
        stream.writeEndElement()
        stream.writeEndDocument()
        return str(ba,"utf-8"),block_names


    def insert_rows_from_xml(self,row,xml,parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or row > len(parent_block): return 0
        stream = QXmlStreamReader(xml.encode("utf-8"))
        stream.readNextStartElement()
        if not stream.isStartElement() or stream.name() != self.__COPY_TAG: return 0
        stream.readNextStartElement()
        if stream.name() != self.__LANG_TAG: return 0
        lang_name = stream.readElementText()
        if lang_name != self.__lang_name: return 0
        blocks = []
        while not stream.atEnd():
            stream.readNext()
            if stream.isStartElement():
                name = stream.name()
                if name in parent_block.build_list():
                    block = Block_Factory().create(lang_name,name)
                    block.set_from_xml(stream)
                    blocks.append(block)
        self.__push_(Insert(row,blocks,parent,self))
        return len(blocks)


    def _remove_rows_(self,row,count,parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or count < 0 or (row + count) > len(parent_block):
            raise RuntimeError("Parent index is not a valid block.")
        blocks = []
        self.beginRemoveRows(parent,row,row + count - 1)
        while count:
            blocks.append(parent_block.pop(row))
            count -= 1
        self.endRemoveRows()
        return blocks


    def _move_row_(self,from_row,to_row,parent):
        parent_block = self.__block_(parent)
        if parent_block is None: raise RuntimeError("Parent index is not a valid block.")
        self.beginMoveRows(parent
                           ,from_row,from_row
                           ,parent
                           ,to_row + 1 if to_row > from_row else to_row)
        parent_block.insert(to_row,parent_block.pop(from_row))
        self.endMoveRows()


    def _insert_rows_(self,row,blocks,parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or row > len(parent_block):
            raise RuntimeError("Parent index is not a valid block.")
        self.beginInsertRows(parent,row,row + len(blocks) - 1)
        for block in blocks:
            parent_block.insert(row,block)
            row += 1
        self.endInsertRows()
        return True


    def __push_(self,command):
        self.__undo_stack = self.__undo_stack[:self.__undo_stack_index]
        self.__undo_stack.append(command)
        self.__undo_stack_index += 1
        command.redo()


    def __block_(self,index):
        if index.isValid():
            return index.internalPointer()
        else:
            return self.__root


    #
    __COPY_TAG = "pysoref_copy"
    #
    __LANG_TAG = "language"
