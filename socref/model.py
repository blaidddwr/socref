"""
Contains all model classes used by the core application.
"""
import traceback
import enum
from PySide2 import QtCore as qtc
from . import exception
from . import utility
from . import abstract
from . import block
from .command import *








class Role(enum.Enum):
    """
    This is the role enumeration class. It defines all custom data roles the project model
    implements.
    """


    #########################
    # PUBLIC - Enumerations #
    #########################


    #
    # Defines the block type role which represents a block's type name.
    #
    BLOCK_TYPE = qtc.Qt.UserRole + 0
    #
    # Defines the view role which represents a block's detailed rich text view.
    #
    VIEW = qtc.Qt.UserRole + 1
    #
    # Defines the build list role which represents a block's build list.
    #
    BUILD_LIST = qtc.Qt.UserRole + 2
    #
    # Defines the properties role which represents a block's properties.
    #
    PROPERTIES = qtc.Qt.UserRole + 3
    #
    # Defines the edit definitions role which represents a block's edit definitions.
    #
    EDIT_DEFS = qtc.Qt.UserRole + 4








@utility.Singleton
class ParserModel(qtc.QObject):
    """
    This is the singleton parser model class. It handles execution of a given abstract parser. A
    slot is provided for starting a new abstract parser. Signals are provided for informing when
    parsing begins, makes progress, finishes, and has remaining unknown code fragments. This class
    is designed to run on its own thread because parsing can be a long process. Because of this the
    GUI must interface with this class exclusively through its signals and slots.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new parser.
        """
        qtc.QObject.__init__(self)
        self.__progress = 0


    ##################
    # PUBLIC - Slots #
    ##################


    @qtc.Slot(abstract.AbstractParser)
    def start(self, parser):
        """
        Called to start execution of the given abstract parser, returning when execution is
        complete.

        parser : The abstract parser that is executed.
        """
        self.__progress = 0
        self.started.emit()
        try:
            parser.parse(self.__update_)
            unknown = parser.unknown()
            if unknown:
                self.remained.emit(unknown)
        except:
            #
            # Qt thread's event loop ignores python exceptions so catch any here and make it
            # visible.
            #
            traceback.print_exc()
        finally:
            self.finished.emit()


    ####################
    # PUBLIC - Signals #
    ####################


    #
    # Signals this parser has started parsing.
    #
    started = qtc.Signal()
    #
    # Signals this parser has made the given percentage progress parsing. The range given is from 0
    # to 100.
    #
    progressed = qtc.Signal(int)
    #
    # Signals this parser has finished parsing.
    #
    finished = qtc.Signal()
    #
    # Signals this parser had remaining unknown code fragments after finishing the last parsing.
    #
    remained = qtc.Signal(dict)


    #####################
    # PRIVATE - Methods #
    #####################


    def __update_(self, percent):
        """
        Called to by the abstract parser that this parser model is currently parsing to inform this
        parser that progress of the given percentage has been made in parsing.

        percent : The percentage progress made by this parser model's abstract parser ranging from 0
                  to 100.
        """
        if percent > self.__progress:
            self.__progress = percent
            self.progressed.emit(percent)








class ProjectModel(qtc.QAbstractItemModel):
    """
    This is the project model class. It implements the qt abstract item model class. It provides
    additional methods above and beyond the basic model methods for added functionality. It provides
    basic properties that are universal to a project. It has built in support for undo and redo of
    any modification done to it.

    It implements the qt abstract item model class. The header data, index, parent, row count,
    column count, data, set data, insert rows, remove rows, and insert row interfaces are
    implemented from the abstract class. The header data provides a single horizontal title that is
    the language name of the model's project if any. The column count always returns 1 since a
    project model only has only column for a block's display name. Custom roles are implemented
    using the roles defined in the role enumeration class. The only role supported for set data is
    the property role. The model is structures like a tree, returning the number of children an
    indexes block has for the row count interface.

    It provides additional methods and signals above and beyond the basic model methods for added
    functionality. Methods are provided for making a new project, loading a project, saving the
    current project, and closing the current project. A method and signal is provided to indicate if
    the model has unsaved changes. Methods and a signal are provided for a project name property of
    the current model's project. A read only method is also provided to query the language of the
    currently loaded project, if any. A move row method is provided to move the row of a given
    index. Methods are provided to copy any number of indexes to XML or insert copied XML blocks
    back into the model. A parser method is provided to build a new abstract parser for the model's
    current project, if any.

    It provides basic properties that are universal to a project. There is a project name that is
    purely for the convenience of the user to identify a project. A relative parse path property is
    provided that informs how a project should be parsed. The parsing path used is the directory
    path of a project's file with the relative parsing path appended to it.

    It has built in support for undo and redo of any modification done to it. Methods are provided
    to test if an undo or redo can be done on the model. Slots are provided to undo or redo a
    modification to the project model.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self, parent=None):
        """
        Initializes a new project model with the given optional parent.

        parent : Optional qt object parent of this new model.
        """
        qtc.QAbstractItemModel.__init__(self,parent)
        self.__name = None
        self.__parsePath = None
        self.__root = None
        self.__langName = None
        self.__undoStack = []
        self.__undoStackIndex = 0
        self.__modified = False


    ######################
    # PUBLIC - Operators #
    ######################


    def __len__(self):
        """
        Implements the length operator.

        return : 1 if this model has a project or 0 if it does not have a project.
        """
        return 0 if self.__root is None else 1


    ####################
    # PUBLIC - Methods #
    ####################


    def headerData(self, section, orientation, role):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        section : See qt docs.

        orientation : See qt docs.

        role : See qt docs.

        return : See qt docs.
        """
        if orientation == qtc.Qt.Horizontal and section == 0 and role == qtc.Qt.DisplayRole:
            return self.__langName
        else:
            return qtc.QAbstractItemModel.headerData(self,section,orientation,role)


    def index(self, row, column, parent):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        row : See qt docs.

        column : See qt docs.

        parent : See qt docs.

        return : See qt docs.
        """
        if row < 0 or column != 0:
            return qtc.QModelIndex()
        parentBlock = self.__block_(parent)
        if parentBlock is None or row >= len(parentBlock):
            return qtc.QModelIndex()
        return self.createIndex(row,column,parentBlock[row])


    def parent(self, child):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        child : See qt docs.

        return : See qt docs.
        """
        childBlock = self.__block_(child)
        if childBlock is None:
            return qtc.QModelIndex()
        parentBlock = childBlock.parent()
        if parentBlock is None or parentBlock.parent() is None:
            return qtc.QModelIndex()
        return self.createIndex(parentBlock.index(),0,parentBlock)


    def rowCount(self, index):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        index : See qt docs.

        return : See qt docs.
        """
        block = self.__block_(index)
        return 0 if block is None else len(block)


    def columnCount(self, index):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        index : See qt docs.

        return : See qt docs.
        """
        return 1


    def data(self, index, role):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        index : See qt docs.

        role : See qt docs.

        return : See qt docs.
        """
        block = self.__block_(index)
        if block is not None:
            if role == qtc.Qt.DisplayRole:
                return block.displayName()
            elif role == qtc.Qt.DecorationRole:
                return block.icon()
            elif role == Role.BUILD_LIST:
                return block.buildList()
            elif role == Role.VIEW:
                return block.displayView()
            elif role == Role.EDIT_DEFS:
                return block.editDefinitions()
            elif role == Role.PROPERTIES:
                return block.properties()
            elif role == Role.BLOCK_TYPE:
                return block._TYPE_


    def setData(self, index, value, role):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        index : See qt docs.

        value : See qt docs.

        role : See qt docs.

        return : See qt docs.
        """
        block = self.__block_(index)
        if block is not None and role == Role.PROPERTIES:
            self.__push_(SetCommand(block.properties(),value,index,self))
            return True
        else:
            return False


    def insertRows(self, row, blockTypes, parent):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        row : See qt docs.

        blockTypes : A list of block types, where each one represents a new block that is created
                     and inserted with the given type name.

        parent : See qt docs.

        return : See qt docs.
        """
        parentBlock = self.__block_(parent)
        if parentBlock is None or row < 0 or row > len(parentBlock):
            return False
        blocks = []
        for blockType in blockTypes:
            block_ = block.BlockFactory().create(self.__langName,blockType)
            block_.setDefaultProperties()
            blocks.append(block_)
        self.__push_(InsertCommand(row,blocks,parent,self))
        return True


    def removeRows(self, row, count, parent):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        row : See qt docs.

        count : See qt docs.

        parent : See qt docs.

        return : List of blocks that were successfully removed from this model.
        """
        parentBlock = self.__block_(parent)
        if parentBlock is None or row < 0 or count < 0 or (row + count) > len(parentBlock):
            return False
        self.__push_(RemoveCommand(row,count,parent,self))
        return True


    def insertRow(self, row, blockType, parent):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        row : See qt docs.

        blockType : The block type that is created and inserted into this model.

        parent : See qt docs.

        return : See qt docs.
        """
        return self.insertRows(row,(blockType,),parent)


    def name(self):
        """
        Getter method.

        return : The name of this model's currently loaded project or none if it has no project.
        """
        return self.__name


    def parsePath(self):
        """
        Getter method.

        return : The relative parsing path of this model's currently loaded project or none if it
                 has no project.
        """
        return self.__parsePath


    def langName(self):
        """
        Getter method.

        return : The language name of this model's currently loaded project or none if it has no
                 project.
        """
        return self.__langName


    def isModified(self):
        """
        Getter method.

        return : True if this model's currently loaded project has unsaved changes or false
                 otherwise.
        """
        return self.__modified


    def canUndo(self):
        """
        Getter method.

        return : True if this model has a modification that can be undone or false otherwise.
        """
        return self.__undoStackIndex > 0


    def canRedo(self):
        """
        Getter method.

        return : True if this model has a modification that can be redone or false otherwise.
        """
        return self.__undoStackIndex < len(self.__undoStack)


    def parser(self):
        """
        Builds a new abstract parser that can be used to parse the source code of this model's
        project. If this model has no project that this does nothing.

        return : The newly built abstract parser or none if this model has no project.
        """
        if self.__root is not None:
            ret = self.__root.parser()
            if not isinstance(ret,abstract.AbstractParser):
                raise RuntimeError("Generated parser is not an abstract parser.")
            return ret


    def new(self, langName):
        """
        Creates a new project for this model with the given language name, closing any currently
        loaded project. The given language name must be valid.

        langName : The name of the language used to create a new project.
        """
        if self.__root is not None:
            self.close()
        self.beginResetModel()
        try:
            self.__name = "New Project"
            self.__parsePath = "."
            self.__langName = langName
            self.__root = block.BlockFactory().createRoot(self.__langName)
        except:
            self.__name = None
            self.__langName = None
            self.__root = None
            raise
        finally:
            self.endResetModel()


    def load(self, path):
        """
        Loads a project to this model from the given file path, closing any currently loaded
        project.

        path : The file path of the project that is loaded into this model.
        """
        xml = None
        with open(path,"br") as ifile:
            xml = ifile.read()
        stream = qtc.QXmlStreamReader(xml)
        stream.readNextStartElement()
        if not stream.isStartElement() or stream.name() != self.__PROJECT_TAG:
            raise exception.LoadError("Invalid XML project tag.")
        stream.readNextStartElement()
        if stream.name() != self.__LANG_TAG:
            raise exception.LoadError("Invalid/missing XML language tag.")
        lang_name = stream.readElementText()
        stream.readNextStartElement()
        if stream.name() != self.__NAME_TAG:
            raise exception.LoadError("Invalid/missing XML name tag.")
        name = stream.readElementText()
        stream.readNextStartElement()
        if stream.name() != self.__PARSE_PATH_TAG:
            raise exception.LoadError("Invalid/missing XML parse path tag.")
        parse_path = stream.readElementText()
        self.new(lang_name)
        self.__name = name
        self.__parsePath = parse_path
        try:
            stream.readNextStartElement()
            if stream.name() != self.__root._TYPE_:
                raise exception.LoadError("Invalid/missing XML root block tag.")
            self.__root.setFromXml(stream)
        except:
            self.close()
            raise


    def save(self, path):
        """
        Saves this model's currently loaded project to the file with the given path. If this model
        has no project then this does nothing.

        path : The path to the save file of this model's project.
        """
        if self.__root is not None:
            xml = qtc.QByteArray()
            stream = qtc.QXmlStreamWriter(xml)
            stream.setAutoFormatting(True)
            stream.writeStartDocument()
            stream.writeStartElement(self.__PROJECT_TAG)
            stream.writeTextElement(self.__LANG_TAG,self.__langName)
            stream.writeTextElement(self.__NAME_TAG,self.__name)
            stream.writeTextElement(self.__PARSE_PATH_TAG,self.__parsePath)
            self.__root.toXml(stream)
            stream.writeEndElement()
            stream.writeEndDocument()
            with open(path,"bw") as ofile:
                ofile.write(xml.data())
            self.__modified = False


    def close(self):
        """
        Closes any currently loaded project of this model. If this model has no loaded project then
        this does nothing.
        """
        if self.__root is not None:
            self.beginResetModel()
            self.__name = None
            self.__parsePath = None
            self.__langName = None
            self.__root = None
            self.__modified = False
            self.__undoStack.clear()
            self.__undoStackIndex = 0
            self.endResetModel()


    def setName(self, name):
        """
        Sets the name of this model's currently loaded project to the one given. This model must
        have a loaded project.

        name : The new name for this model's project.
        """
        if self.__root is None:
            raise RuntimeError("Cannot set name of no project.")
        if name != self.__name:
            self.__name = name
            self.__modified_()
            self.nameChanged.emit(name)


    def setParsePath(self, path):
        """
        Sets the relative parsing path of this model's currently loaded project to the path given.
        This model must have a loaded project.

        path : The new parsing path of this model's project.
        """
        if self.__root is None:
            raise RuntimeError("Cannot set name of no project.")
        if path != self.__parsePath:
            self.__parsePath = path
            self.__modified_()
            self.parsePathChanged.emit(path)


    def moveRow(self, change, index):
        """
        Moves the given index's row by the amount given.

        change : The row change for the index that is moved. Negative is up and positive is down.

        index : The qt model index that is moved.

        return : True if the move was successful or false otherwise.
        """
        if not change or not index.isValid():
            return False
        block = self.__block_(index.parent())
        if block is None:
            return False
        toRow = index.row() + change
        if toRow < 0 or toRow >= len(block):
            return False
        self.__push_(MoveCommand(change,index,self))
        return True


    def copyToXml(self, indexes):
        """
        Copies the given indexes to XML.

        indexes : List of qt model indexes whose blocks are copied to XML.

        return : A tuple of two items. The first item is copied blocks at the given indexes in the
                 form of XML. The XML format is copy with each index an element in the root XML copy
                 tag. The second item is a set of all block types that were copied.
        """
        blockTypes = set()
        xml = qtc.QByteArray()
        stream = qtc.QXmlStreamWriter(xml)
        stream.setAutoFormatting(True)
        stream.writeStartDocument()
        stream.writeStartElement(self.__COPY_TAG)
        stream.writeTextElement(self.__LANG_TAG,self.__langName)
        for index in indexes:
            block = self.__block_(index)
            if block is not None:
                block.toXml(stream)
                blockTypes.add(block._TYPE_)
        stream.writeEndElement()
        stream.writeEndDocument()
        return (xml,blockTypes)


    def insertFromXml(self, row, xml, parent):
        """
        Inserts blocks into this model at the given parent index and row loaded from the given XML.

        row : The row in the given parent index where loaded blocks are inserted.

        xml : XML byte array containing the loaded blocks. This must conform to the XML copy
              standard produced by the copy to XML method of this class.

        parent : The parent index where the loaded blocks are inserted.

        return : Total number of blocks that were inserted into this model.
        """
        parentBlock = self.__block_(parent)
        if parentBlock is None or row < 0 or row > len(parentBlock):
            return 0
        stream = qtc.QXmlStreamReader(xml)
        stream.readNextStartElement()
        if not stream.isStartElement() or stream.name() != self.__COPY_TAG:
            return 0
        stream.readNextStartElement()
        if stream.name() != self.__LANG_TAG:
            return 0
        langName = stream.readElementText()
        if langName != self.__langName:
            return 0
        blocks = []
        while not stream.atEnd():
            stream.readNext()
            if stream.isStartElement():
                name = stream.name()
                block_ = block.BlockFactory().create(langName,name)
                block_.setFromXml(stream)
                if name in parentBlock.buildList():
                    blocks.append(block_)
        self.__push_(InsertCommand(row,blocks,parent,self))
        return len(blocks)


    ####################
    # PUBLIC - Signals #
    ####################


    #
    # Signals this model's project has been modified with unsaved changes.
    #
    modified = qtc.Signal()
    #
    # Signals this model's project name has changed to the value given.
    #
    nameChanged = qtc.Signal(str)
    #
    # Signals this model's relative parsing path has changed to the value given.
    #
    parsePathChanged = qtc.Signal(str)


    ##################
    # PUBLIC - Slots #
    ##################


    @qtc.Slot()
    def undo(self):
        """
        Called to undo the last modification that was done to this model. If this model cannot undo
        then this does nothing.
        """
        if self.canUndo():
            self.__undoStackIndex -= 1
            self.__undoStack[self.__undoStackIndex].undo()


    @qtc.Slot()
    def redo(self):
        """
        Called to redo the last undone modification of this model. If this model cannot redo then
        this does nothing.
        """
        if self.canRedo():
            self.__undoStack[self.__undoStackIndex].redo()
            self.__undoStackIndex += 1


    #######################
    # PROTECTED - Methods #
    #######################


    def _insertRows_(self, row, blocks, parent):
        """
        Inserts the given blocks into the given parent index at the given row. The given parent
        index and row must be valid.

        row : The row where the given blocks are inserted into the given parent.

        blocks : List of blocks that are inserted into this model.

        parent : The parent index where the given blocks are inserted into as children.
        """
        parentBlock = self.__block_(parent)
        if parentBlock is None or row < 0 or row > len(parentBlock):
            raise RuntimeError("Parent index is not a valid block.")
        volatile = False
        self.beginInsertRows(parent,row,row + len(blocks) - 1)
        for block in blocks:
            parentBlock.insert(row,block)
            if block.isVolatileAbove():
                volatile = True
            row += 1
        self.endInsertRows()
        if volatile:
            self.__pushVolatileAbove_(parent)
        self.__modified_()


    def _removeRows_(self, row, count, parent):
        """
        Removes the given count of blocks starting at the given row in the given parent index. The
        given parent index, row, and count must all be valid.

        row : The starting row where blocks are removed.

        count : The total number of blocks that are removed.

        parent : The parent index where child blocks are removed.

        return : List of removed blocks.
        """
        parentBlock = self.__block_(parent)
        if parentBlock is None or row < 0 or count < 0 or (row + count) > len(parentBlock):
            raise RuntimeError("Parent index is not a valid block.")
        volatile = False
        blocks = []
        self.beginRemoveRows(parent,row,row + count - 1)
        while count:
            if parentBlock[row].isVolatileAbove():
                volatile = True
            blocks.append(parentBlock.pop(row))
            count -= 1
        self.endRemoveRows()
        self.__pushVolatileAbove_(parent)
        self.__modified_()
        return blocks


    def _moveRow_(self, fromRow, toRow, parent):
        """
        Moves the block at the given from row to the given to row in the given parent index. The
        given rows and parent index must all be valid.

        fromRow : The row of the block that is moved.

        toRow : The new row of the moved block.

        parent : The parent index where a child block is moved.
        """
        parentBlock = self.__block_(parent)
        if parentBlock is None:
            raise RuntimeError("Parent index is not a valid block.")
        volatile = False
        self.beginMoveRows(
            parent
            ,fromRow,fromRow
            ,parent
            ,toRow + 1 if toRow > fromRow else toRow
        )
        if parentBlock[fromRow]:
            volatile = True
        parentBlock.insert(toRow,parentBlock.pop(fromRow))
        self.endMoveRows()
        self.__pushVolatileAbove_(parent)
        self.__modified_()


    def _setProperties_(self, index, properties):
        """
        Sets the properties of the block at the given index in this model. The given index must be
        valid.

        index : The qt model index of the updated block.

        properties : The new properties dictionary of the block at the given index.
        """
        block = self.__block_(index)
        if block is None:
            raise RuntimeError("Index is not a valid block.")
        block.setProperties(properties)
        self.dataChanged.emit(index,index)
        if block.isVolatileAbove():
            self.__pushVolatileAbove_(index.parent())
        if block.isVolatileBelow():
            self.__pushVolatileBelow_(index)
        self.__modified_()


    #####################
    # PRIVATE - Methods #
    #####################


    def __modified_(self):
        """
        Marks this model as modified, emitting a signal if it was not modified before calling this
        method.
        """
        if not self.__modified:
            self.__modified = True
            self.modified.emit()


    def __push_(self, command):
        """
        Pushes the given command to this model, immediately calling its redo interface and then
        adding it to this model's undo stack. Any undone commands on this model's undo stack are
        removed.

        command : The command that is immediately executed and added to this model's undo stack. Any
                  commands that have been undone on the stack are erased.
        """
        self.__undoStack = self.__undoStack[:self.__undoStackIndex]
        self.__undoStack.append(command)
        self.__undoStackIndex += 1
        command.redo()


    def __pushVolatileAbove_(self, index):
        """
        Signals the given index of this model has changed its data if it is valid, recursively
        calling itself on the given index's parent index if its block is also volatile above.

        index : The index of this model whose data has changed due to volatile children.
        """
        if index.isValid():
            self.dataChanged.emit(index,index)
            block = self.__block_(index)
            if block is not None and block.isVolatileAbove():
                self.__pushVolatileAbove_(index.parent())


    def __pushVolatileBelow_(self, parent):
        """
        Signals the given parent index of this model has changed its children's data if it is valid,
        recursively calling itself on any child that is also volatile below.

        parent : The parent index of this model whose children's data has changed due to the
                 volatile below parent.
        """
        for row in range(self.rowCount(parent)):
            index = self.index(row,0,parent)
            self.dataChanged.emit(index,index)
            block = self.__block_(index)
            if block.isVolatileBelow():
                self.__pushVolatileBelow_(index)


    def __block_(self, index):
        """
        Getter method.

        index : The index whose block is returned.

        return : The block of the given index. If the given index is invalid the root block is
                 returned, which is none if this model has no project.
        """
        if index.isValid():
            return index.internalPointer()
        else: return self.__root


    #######################
    # PRIVATE - Constants #
    #######################


    #
    # The root tag used for XML project files.
    #
    __PROJECT_TAG = "scp_project"
    #
    # The name tag used for the project name text element of XML project files.
    #
    __NAME_TAG = "name"
    #
    # The parse path tag used for the parse path text element of XML project files.
    #
    __PARSE_PATH_TAG = "parse_path"
    #
    # The language tag used for the language name text element of XML project files.
    #
    __LANG_TAG = "language"
    #
    # The root copy tag used for internal copy XML byte arrays.
    #
    __COPY_TAG = "pysoref_copy"
