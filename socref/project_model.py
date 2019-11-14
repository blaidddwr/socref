"""
todo
"""
from PySide2 import QtCore as qtc
from . import block_factory as bf
from . import project_commands as pc






class LoadError(Exception):


    def __init__(self,*args):
        Exception.__init__(self,*args)






class Project_Model(qtc.QAbstractItemModel):


    #
    BUILD_LIST_ROLE = qtc.Qt.UserRole + 0
    #
    VIEW_ROLE = qtc.Qt.UserRole + 1
    #
    EDIT_DEFS_ROLE = qtc.Qt.UserRole + 2
    #
    PROPERTIES_ROLE = qtc.Qt.UserRole + 3
    #
    BLOCK_TYPE_ROLE = qtc.Qt.UserRole + 4


    def __init__(self, parent=None):
        qtc.QAbstractItemModel.__init__(self,parent)
        #
        self.__name = None
        #
        self.__root = None
        #
        self.__lang_name = None
        #
        self.__undo_stack = []
        #
        self.__undo_stack_index = 0
        #
        self.__modified = False
        #:


    def __len__(self):
        return 0 if self.__root is None else 1


    def load(self, path):
        xml = None
        with open(path,"br") as ifile:
            xml = ifile.read()
        stream = qtc.QXmlStreamReader(xml)
        stream.readNextStartElement()
        if not stream.isStartElement() or stream.name() != self.__PROJECT_TAG :
            raise LoadError("Invalid XML project tag.")
        stream.readNextStartElement()
        if stream.name() != self.__LANG_TAG :
            raise LoadError("Invalid/missing XML language tag.")
        lang_name = stream.readElementText()
        stream.readNextStartElement()
        if stream.name() != self.__NAME_TAG :
            raise LoadError("Invalid/missing XML name tag.")
        name = stream.readElementText()
        self.new(lang_name)
        self.__name = name
        try:
            stream.readNextStartElement()
            if stream.name() != self.__root.type_name() :
                raise LoadError("Invalid/missing XML root block tag.")
            self.__root.set_from_xml(stream)
        except:
            self.close()
            raise


    def save(self, path):
        if self.__root is not None:
            xml = qtc.QByteArray()
            stream = qtc.QXmlStreamWriter(xml)
            stream.setAutoFormatting(True)
            stream.writeStartDocument()
            stream.writeStartElement(self.__PROJECT_TAG)
            stream.writeTextElement(self.__LANG_TAG,self.__lang_name)
            stream.writeTextElement(self.__NAME_TAG,self.__name)
            self.__root.to_xml(stream)
            stream.writeEndElement()
            stream.writeEndDocument()
            with open(path,"bw") as ofile:
                ofile.write(xml.data())
            self.__modified = False


    def lang_name(self):
        return self.__lang_name


    def name(self):
        return self.__name


    def set_name(self, name):
        if name != self.__name :
            self.__name = name
            self.__modified_()
            self.name_changed.emit(name)


    def is_modified(self):
        return self.__modified


    def can_undo(self):
        return self.__undo_stack_index > 0


    def can_redo(self):
        return self.__undo_stack_index < len(self.__undo_stack)


    def headerData(self, section, orientation, role):
        if orientation == qtc.Qt.Horizontal and section == 0 and role == qtc.Qt.DisplayRole :
            return self.__lang_name
        else : return qtc.QAbstractItemModel.headerData(self,section,orientation,role)


    def flags(self, index):
        flags = qtc.QAbstractItemModel.flags(self,index)
        if index.isValid() : return qtc.Qt.ItemIsDragEnabled | qtc.Qt.ItemIsDropEnabled | flags
        else : return qtc.Qt.ItemIsDropEnabled | flags


    def index(self, row, column, parent):
        if row < 0 or column != 0 : return qtc.QModelIndex()
        parent_block = self.__block_(parent)
        if parent_block is None or row >= len(parent_block) : return qtc.QModelIndex()
        return self.createIndex(row,column,parent_block[row])


    def parent(self, child):
        child_block = self.__block_(child)
        if child_block is None : return qtc.QModelIndex()
        parent_block = child_block.parent()
        if parent_block is None or parent_block.parent() is None : return qtc.QModelIndex()
        return self.createIndex(parent_block.index(),0,parent_block)


    def rowCount(self, index):
        block = self.__block_(index)
        if block is None : return 0
        else : return len(block)


    def columnCount(self, index):
        return 1


    def data(self, index, role):
        block = self.__block_(index)
        if block is not None :
            if role == qtc.Qt.DisplayRole : return block.display_name()
            elif role == qtc.Qt.DecorationRole : return block.icon()
            elif role == self.BUILD_LIST_ROLE : return block.build_list()
            elif role == self.VIEW_ROLE : return block.display_view()
            elif role == self.EDIT_DEFS_ROLE : return block.edit_definitions()
            elif role == self.PROPERTIES_ROLE : return block.properties()
            elif role == self.BLOCK_TYPE_ROLE : return block.type_name()


    def parser(self):
        if self.__root is not None :
            return self.__root.parser()


    def setData(self, index, value, role):
        block = self.__block_(index)
        if block is not None and role == self.PROPERTIES_ROLE :
            self.__push_(pc.Set(block.properties(),value,index,self))
            return True
        else :
            return False


    def insertRows(self, row, block_names, parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or row > len(parent_block) : return False
        blocks = []
        for block_name in block_names :
            block = bf.Block_Factory().create(self.__lang_name,block_name)
            block.set_default_properties()
            blocks.append(block)
        self.__push_(pc.Insert(row,blocks,parent,self))
        return True


    def removeRows(self, row, count, parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or count < 0 or (row + count) > len(parent_block) :
            return False
        self.__push_(pc.Remove(row,count,parent,self))
        return True


    def insertRow(self, row, block_name, parent):
        return self.insertRows(row,(block_name,),parent)


    def move_row(self, change, index):
        if not change or not index.isValid() : return False
        block = self.__block_(index.parent())
        if block is None : return False
        to_row = index.row() + change
        if to_row < 0 or to_row >= len(block) : return False
        self.__push_(pc.Move(change,index,self))
        return True


    def new(self, lang_name):
        if self.__root is not None : self.close()
        self.beginResetModel()
        try:
            self.__lang_name = lang_name
            self.__root = bf.Block_Factory().create_root(self.__lang_name)
        except:
            self.__lang_name = None
            self.__root = None
            raise
        finally:
            self.endResetModel()


    def close(self):
        self.beginResetModel()
        self.__name = None
        self.__lang_name = None
        self.__root = None
        self.__modified = False
        self.__undo_stack.clear()
        self.__undo_stack_index = 0
        self.endResetModel()


    def copy_to_xml(self, indexes):
        block_names = set()
        xml = qtc.QByteArray()
        stream = qtc.QXmlStreamWriter(xml)
        stream.setAutoFormatting(True)
        stream.writeStartDocument()
        stream.writeStartElement(self.__COPY_TAG)
        stream.writeTextElement(self.__LANG_TAG,self.__lang_name)
        for index in indexes :
            block = self.__block_(index)
            if block is not None :
                block.to_xml(stream)
                block_names.add(block.type_name())
        stream.writeEndElement()
        stream.writeEndDocument()
        return xml,block_names


    def insert_rows_from_xml(self, row, xml, parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or row > len(parent_block) : return 0
        stream = qtc.QXmlStreamReader(xml)
        stream.readNextStartElement()
        if not stream.isStartElement() or stream.name() != self.__COPY_TAG : return 0
        stream.readNextStartElement()
        if stream.name() != self.__LANG_TAG : return 0
        lang_name = stream.readElementText()
        if lang_name != self.__lang_name : return 0
        blocks = []
        while not stream.atEnd() :
            stream.readNext()
            if stream.isStartElement() :
                name = stream.name()
                if name in parent_block.build_list() :
                    block = bf.Block_Factory().create(lang_name,name)
                    block.set_from_xml(stream)
                    blocks.append(block)
        self.__push_(pc.Insert(row,blocks,parent,self))
        return len(blocks)


    modified = qtc.Signal()


    name_changed = qtc.Signal(str)


    @qtc.Slot()
    def undo(self):
        if self.can_undo() :
            self.__undo_stack_index -= 1
            self.__undo_stack[self.__undo_stack_index].undo()


    @qtc.Slot()
    def redo(self):
        if self.can_redo() :
            self.__undo_stack[self.__undo_stack_index].redo()
            self.__undo_stack_index += 1


    def _remove_rows_(self, row, count, parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or count < 0 or (row + count) > len(parent_block) :
            raise RuntimeError("Parent index is not a valid block.")
        volatile = False
        blocks = []
        self.beginRemoveRows(parent,row,row + count - 1)
        while count :
            blocks.append(parent_block.pop(row))
            if blocks[-1].is_volatile_above() : volatile = True
            count -= 1
        self.endRemoveRows()
        self.__push_volatile_above_(parent)
        self.__modified_()
        return blocks


    def _move_row_(self, from_row, to_row, parent):
        parent_block = self.__block_(parent)
        if parent_block is None : raise RuntimeError("Parent index is not a valid block.")
        volatile = False
        self.beginMoveRows(parent
                           ,from_row,from_row
                           ,parent
                           ,to_row + 1 if to_row > from_row else to_row)
        block = parent_block.pop(from_row)
        if block.is_volatile_above() : volatile = True
        parent_block.insert(to_row,block)
        self.endMoveRows()
        self.__push_volatile_above_(parent)
        self.__modified_()


    def _insert_rows_(self, row, blocks, parent):
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or row > len(parent_block) :
            raise RuntimeError("Parent index is not a valid block.")
        volatile = False
        self.beginInsertRows(parent,row,row + len(blocks) - 1)
        for block in blocks :
            parent_block.insert(row,block)
            if block.is_volatile_above() : volatile = True
            row += 1
        self.endInsertRows()
        if volatile : self.__push_volatile_above_(parent)
        self.__modified_()


    def _set_props_(self, index, props):
        block = self.__block_(index)
        if block is None: raise RuntimeError("Index is not a valid block.")
        block.set_properties(props)
        self.dataChanged.emit(index,index)
        if block.is_volatile_above() : self.__push_volatile_above_(index.parent())
        if block.is_volatile_below() : self.__push_volatile_below_(index)
        self.__modified_()


    def __modified_(self):
        if not self.__modified :
            self.__modified = True
            self.modified.emit()


    def __push_(self, command):
        self.__undo_stack = self.__undo_stack[:self.__undo_stack_index]
        self.__undo_stack.append(command)
        self.__undo_stack_index += 1
        command.redo()


    def __push_volatile_above_(self, index):
        if index.isValid() :
            self.dataChanged.emit(index,index)
            block = self.__block_(index)
            if block is not None and block.is_volatile_above() :
                self.__push_volatile_above_(index.parent())


    def __push_volatile_below_(self, parent):
        for row in range(self.rowCount(parent)) :
            index = self.index(row,0,parent)
            self.dataChanged.emit(index,index)
            block = self.__block_(index)
            if block.is_volatile_below() : self.__push_volatile_below_(index)


    def __block_(self, index):
        if index.isValid() :
            return index.internalPointer()
        else :
            return self.__root


    #
    __PROJECT_TAG = "scp_project"
    #
    __NAME_TAG = "name"
    #
    __COPY_TAG = "pysoref_copy"
    #
    __LANG_TAG = "language"
