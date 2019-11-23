"""
Contains all model classes used by this application. This includes related classes such as
exceptions and roles.
"""
import traceback
import enum
from PySide2 import QtCore as qtc
from . import util
from . import abstract
from .command import *








class LoadError(Exception):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Initializer #
    ########################


    def __init__(self, *args):
        """
        Detailed description.

        *args : Detailed description.
        """
        Exception.__init__(self,*args)








class Role(enum.Enum):
    """
    Detailed description.
    """


    #########################
    # PUBLIC - Enumerations #
    #########################


    #
    # Detailed description.
    #
    BUILD_LIST = qtc.Qt.UserRole + 0
    #
    # Detailed description.
    #
    VIEW = qtc.Qt.UserRole + 1
    #
    # Detailed description.
    #
    EDIT_DEFS = qtc.Qt.UserRole + 2
    #
    # Detailed description.
    #
    PROPERTIES = qtc.Qt.UserRole + 3
    #
    # Detailed description.
    #
    BLOCK_TYPE = qtc.Qt.UserRole + 4








@util.Singleton
class Parser(qtc.QObject):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Init/Delete #
    ########################


    def __init__(self):
        """
        Detailed description.
        """
        qtc.QObject.__init__(self)


    ##################
    # PUBLIC - Slots #
    ##################


    @qtc.Slot(abstract.Parser)
    def start(self, parser):
        """
        Detailed description.

        parser : Detailed description.
        """
        try:
            parser.parse()
        except:
            traceback.print_exc()








class Project(qtc.QAbstractItemModel):
    """
    Detailed description.
    """


    ########################
    # PUBLIC - Init/Delete #
    ########################


    def __init__(self, parent=None):
        """
        Detailed description.

        parent : Detailed description.
        """
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
        #


    ######################
    # PUBLIC - Operators #
    ######################


    def __len__(self):
        """
        Detailed description.

        return : Yes
        """
        return 0 if self.__root is None else 1


    ####################
    # PUBLIC - Methods #
    ####################


    def new(self, lang_name):
        """
        Detailed description.

        lang_name : Detailed description.
        """
        if self.__root is not None : self.close()
        self.beginResetModel()
        try:
            self.__lang_name = lang_name
            self.__root = abstract.Factory().create_root(self.__lang_name)
        except:
            self.__lang_name = None
            self.__root = None
            raise
        finally:
            self.endResetModel()


    def load(self, path):
        """
        Detailed description.

        path : Detailed description.
        """
        xml = None
        with open(path,"br") as ifile :
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
            if stream.name() != self.__root._TYPE_ :
                raise LoadError("Invalid/missing XML root block tag.")
            self.__root.set_from_xml(stream)
        except:
            self.close()
            raise


    def save(self, path):
        """
        Detailed description.

        path : Detailed description.
        """
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
            with open(path,"bw") as ofile :
                ofile.write(xml.data())
            self.__modified = False


    def close(self):
        """
        Detailed description.
        """
        self.beginResetModel()
        self.__name = None
        self.__lang_name = None
        self.__root = None
        self.__modified = False
        self.__undo_stack.clear()
        self.__undo_stack_index = 0
        self.endResetModel()


    def lang_name(self):
        """
        Detailed description.

        return : Yes
        """
        return self.__lang_name


    def name(self):
        """
        Detailed description.

        return : Yes
        """
        return self.__name


    def set_name(self, name):
        """
        Detailed description.

        name : Detailed description.
        """
        if name != self.__name :
            self.__name = name
            self.__modified_()
            self.name_changed.emit(name)


    def is_modified(self):
        """
        Detailed description.

        return : Yes
        """
        return self.__modified


    def can_undo(self):
        """
        Detailed description.

        return : Yes
        """
        return self.__undo_stack_index > 0


    def can_redo(self):
        """
        Detailed description.

        return : Yes
        """
        return self.__undo_stack_index < len(self.__undo_stack)


    def headerData(self, section, orientation, role):
        """
        Detailed description.

        section : Detailed description.

        orientation : Detailed description.

        role : Detailed description.

        return : Yes
        """
        if orientation == qtc.Qt.Horizontal and section == 0 and role == qtc.Qt.DisplayRole :
            return self.__lang_name
        else: return qtc.QAbstractItemModel.headerData(self,section,orientation,role)


    def index(self, row, column, parent):
        """
        Detailed description.

        row : Detailed description.

        column : Detailed description.

        parent : Detailed description.

        return : Yes
        """
        if row < 0 or column != 0 : return qtc.QModelIndex()
        parent_block = self.__block_(parent)
        if parent_block is None or row >= len(parent_block) : return qtc.QModelIndex()
        return self.createIndex(row,column,parent_block[row])


    def parent(self, child):
        """
        Detailed description.

        child : Detailed description.

        return : Yes
        """
        child_block = self.__block_(child)
        if child_block is None : return qtc.QModelIndex()
        parent_block = child_block.parent()
        if parent_block is None or parent_block.parent() is None : return qtc.QModelIndex()
        return self.createIndex(parent_block.index(),0,parent_block)


    def rowCount(self, index):
        """
        Detailed description.

        index : Detailed description.

        return : Yes
        """
        block = self.__block_(index)
        if block is None : return 0
        else: return len(block)


    def columnCount(self, index):
        """
        Detailed description.

        index : Detailed description.

        return : Yes
        """
        return 1


    def data(self, index, role):
        """
        Detailed description.

        index : Detailed description.

        role : Detailed description.

        return : Yes
        """
        block = self.__block_(index)
        if block is not None :
            if role == qtc.Qt.DisplayRole : return block.display_name()
            elif role == qtc.Qt.DecorationRole : return block.icon()
            elif role == Role.BUILD_LIST : return block.build_list()
            elif role == Role.VIEW : return block.display_view()
            elif role == Role.EDIT_DEFS : return block.edit_definitions()
            elif role == Role.PROPERTIES : return block.properties()
            elif role == Role.BLOCK_TYPE : return block._TYPE_


    def setData(self, index, value, role):
        """
        Detailed description.

        index : Detailed description.

        value : Detailed description.

        role : Detailed description.

        return : Yes
        """
        block = self.__block_(index)
        if block is not None and role == Role.PROPERTIES :
            self.__push_(Set(block.properties(),value,index,self))
            return True
        else:
            return False


    def insertRows(self, row, block_names, parent):
        """
        Detailed description.

        row : Detailed description.

        block_names : Detailed description.

        parent : Detailed description.

        return : Yes
        """
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or row > len(parent_block) : return False
        blocks = []
        for block_name in block_names :
            block = abstract.Factory().create(self.__lang_name,block_name)
            block.set_default_properties()
            blocks.append(block)
        self.__push_(Insert(row,blocks,parent,self))
        return True


    def removeRows(self, row, count, parent):
        """
        Detailed description.

        row : Detailed description.

        count : Detailed description.

        parent : Detailed description.

        return : Yes
        """
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or count < 0 or (row + count) > len(parent_block) :
            return False
        self.__push_(Remove(row,count,parent,self))
        return True


    def insertRow(self, row, block_name, parent):
        """
        Detailed description.

        row : Detailed description.

        block_name : Detailed description.

        parent : Detailed description.

        return : Yes
        """
        return self.insertRows(row,(block_name,),parent)


    def move_row(self, change, index):
        """
        Detailed description.

        change : Detailed description.

        index : Detailed description.

        return : Yes
        """
        if not change or not index.isValid() : return False
        block = self.__block_(index.parent())
        if block is None : return False
        to_row = index.row() + change
        if to_row < 0 or to_row >= len(block) : return False
        self.__push_(Move(change,index,self))
        return True


    def copy_to_xml(self, indexes):
        """
        Detailed description.

        indexes : Detailed description.

        return : Yes
        """
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
                block_names.add(block._TYPE_)
        stream.writeEndElement()
        stream.writeEndDocument()
        return xml,block_names


    def insert_from_xml(self, row, xml, parent):
        """
        Detailed description.

        row : Detailed description.

        xml : Detailed description.

        parent : Detailed description.

        return : Yes
        """
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
                    block = abstract.Factory().create(lang_name,name)
                    block.set_from_xml(stream)
                    blocks.append(block)
        self.__push_(Insert(row,blocks,parent,self))
        return len(blocks)


    def parser(self):
        """
        Detailed description.

        return : Yes
        """
        if self.__root is not None :
            ret = self.__root.parser()
            if not isinstance(ret,abstract.Parser) :
                raise RuntimeError("Generated parser is not an abstract parser.")
            return ret


    ####################
    # PUBLIC - Signals #
    ####################


    #
    # Detailed description.
    #
    modified = qtc.Signal()
    #
    # Detailed description.
    #
    name_changed = qtc.Signal(str)


    ##################
    # PUBLIC - Slots #
    ##################


    @qtc.Slot()
    def undo(self):
        """
        Detailed description.
        """
        if self.can_undo() :
            self.__undo_stack_index -= 1
            self.__undo_stack[self.__undo_stack_index].undo()


    @qtc.Slot()
    def redo(self):
        """
        Detailed description.
        """
        if self.can_redo() :
            self.__undo_stack[self.__undo_stack_index].redo()
            self.__undo_stack_index += 1


    #######################
    # PROTECTED - Methods #
    #######################


    def _insert_rows_(self, row, blocks, parent):
        """
        Detailed description.

        row : Detailed description.

        blocks : Detailed description.

        parent : Detailed description.
        """
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


    def _remove_rows_(self, row, count, parent):
        """
        Detailed description.

        row : Detailed description.

        count : Detailed description.

        parent : Detailed description.

        return : Yes
        """
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
        """
        Detailed description.

        from_row : Detailed description.

        to_row : Detailed description.

        parent : Detailed description.
        """
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


    def _set_props_(self, index, props):
        """
        Detailed description.

        index : Detailed description.

        props : Detailed description.
        """
        block = self.__block_(index)
        if block is None: raise RuntimeError("Index is not a valid block.")
        block.set_properties(props)
        self.dataChanged.emit(index,index)
        if block.is_volatile_above() : self.__push_volatile_above_(index.parent())
        if block.is_volatile_below() : self.__push_volatile_below_(index)
        self.__modified_()


    #####################
    # PRIVATE - Methods #
    #####################


    def __modified_(self):
        """
        Detailed description.
        """
        if not self.__modified :
            self.__modified = True
            self.modified.emit()


    def __push_(self, command):
        """
        Detailed description.

        command : Detailed description.
        """
        self.__undo_stack = self.__undo_stack[:self.__undo_stack_index]
        self.__undo_stack.append(command)
        self.__undo_stack_index += 1
        command.redo()


    def __push_volatile_above_(self, index):
        """
        Detailed description.

        index : Detailed description.
        """
        if index.isValid() :
            self.dataChanged.emit(index,index)
            block = self.__block_(index)
            if block is not None and block.is_volatile_above() :
                self.__push_volatile_above_(index.parent())


    def __push_volatile_below_(self, parent):
        """
        Detailed description.

        parent : Detailed description.
        """
        for row in range(self.rowCount(parent)) :
            index = self.index(row,0,parent)
            self.dataChanged.emit(index,index)
            block = self.__block_(index)
            if block.is_volatile_below() : self.__push_volatile_below_(index)


    def __block_(self, index):
        """
        Detailed description.

        index : Detailed description.

        return : Yes
        """
        if index.isValid() :
            return index.internalPointer()
        else:
            return self.__root


    #######################
    # PRIVATE - Constants #
    #######################


    #
    # Detailed description.
    #
    __PROJECT_TAG = "scp_project"
    #
    # Detailed description.
    #
    __NAME_TAG = "name"
    #
    # Detailed description.
    #
    __COPY_TAG = "pysoref_copy"
    #
    # Detailed description.
    #
    __LANG_TAG = "language"
