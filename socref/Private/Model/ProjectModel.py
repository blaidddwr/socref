"""
Contains the ProjectModel class.
"""
from ...Abstract.AbstractParser import AbstractParser
from ...Error.LoadError import LoadError
from ...Settings import Settings
from ..Factory.BlockFactory import BlockFactory
from ..Factory.ParserFactory import ParserFactory
from .Command.InsertCommand import InsertCommand
from .Command.MoveCommand import MoveCommand
from .Command.RemoveCommand import RemoveCommand
from .Command.SetCommand import SetCommand
from PySide6.QtCore import (
    QAbstractItemModel
    ,QByteArray
    ,QModelIndex
    ,QXmlStreamReader
    ,QXmlStreamWriter
    ,Qt
    ,Signal
    ,Slot
)
from enum import IntEnum




class ProjectModel(QAbstractItemModel):
    """
    This is the project model class. It implements the Qt abstract item model
    class. The header data, index, parent, row count, column count, data, set
    data, insert rows, remove rows, and insert row interfaces are implemented
    from the abstract class. The header data provides a single horizontal title
    that is the language name of the model's project. The column count always
    returns 1 since a project model only has only column for a block's display
    name. Custom roles are implemented using the roles defined in the role
    enumeration class. The only role supported for set data is the property
    role.

    It provides additional methods and signals above and beyond the basic model
    methods for added functionality. Methods are provided for making a new
    project, loading a project, saving the current project, and closing the
    current project. A method and signal is provided to indicate if the model
    has unsaved changes. Methods and a signal are provided for project name and
    relative parse path properties of the current model's project. A read only
    method is provided to query the language of the currently loaded project. A
    move row method is provided to move the row of a given index. Methods are
    provided to copy any number of indexes to XML or insert copied XML blocks
    into the model. A parser method is provided to build a new abstract parser
    for the model's current project.

    Methods are provided to support undoing any previous action done to this
    model's project or redoing any previous undone action.
    """


    class Role(IntEnum):
        """
        This enumerates all custom data roles the project model implements.
        """
        BlockType = Qt.UserRole+0
        View = Qt.UserRole+1
        BuildList = Qt.UserRole+2
        Properties = Qt.UserRole+3
        EditDefs = Qt.UserRole+4


    #
    # Signals this model's project has been modified with unsaved changes.
    #
    modified = Signal()


    #
    # Signals this model's project name has changed to the value given.
    #
    nameChanged = Signal(str)


    #
    # Signals this model's relative parsing path has changed to the value given.
    #
    parsePathChanged = Signal(str)


    def __init__(
        self
        ,parent=None
    ):
        """
        Initializes this new project model with the given optional Qt object
        parent.

        Parameters
        ----------
        parent : QObject
                 The optional Qt object parent.
        """
        super().__init__(parent)
        self.__name = None
        self.__parsePath = None
        self.__root = None
        self.__langName = None
        self.__undoStack = []
        self.__undoStackIndex = 0
        self.__modified = False


    def __len__(
        self
    ):
        return 0 if self.__root is None else 1


    def canRedo(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this model has a modification that can be redone or
                 false otherwise.
        """
        return self.__undoStackIndex < len(self.__undoStack)


    def canUndo(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this model has a modification that can be undone or
                 false otherwise.
        """
        return self.__undoStackIndex > 0


    def close(
        self
    ):
        """
        Closes any currently loaded project of this model. If this model has no
        loaded project then this does nothing.
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


    def columnCount(
        self
        ,index
    ):
        return 1


    def copyToXml(
        self
        ,indexes
    ):
        """
        Copies and returns the blocks at the given Qt model indexes to a Qt byte
        array XML document, along with returning a set of all copied block
        types.

        Parameters
        ----------
        indexes : list
                  The Qt model indexes.

        Returns
        -------
        data : QByteArray
               The Qt byte array XML document.
        copied : set
                 The copied block types.
        """
        blockTypes = set()
        xml = QByteArray()
        stream = QXmlStreamWriter(xml)
        stream.setAutoFormatting(True)
        stream.writeStartDocument()
        stream.writeStartElement(Settings.ProjectModel.COPY_TAG)
        stream.writeTextElement(Settings.ProjectModel.LANG_TAG,self.__langName)
        for index in indexes:
            block = self.__block_(index)
            if block is not None:
                block.toXml(stream)
                blockTypes.add(block._TYPE_)
        stream.writeEndElement()
        stream.writeEndDocument()
        return (xml,blockTypes)


    def data(
        self
        ,index
        ,role
    ):
        block = self.__block_(index)
        if block is not None:
            if role == Qt.DisplayRole:
                return block.displayName()
            elif role == Qt.DecorationRole:
                return block.icon()
            elif role == Role.BuildList:
                return block.buildList()
            elif role == Role.View:
                return str(block.displayView())
            elif role == Role.EditDefs:
                return block.editDefinitions()
            elif role == Role.Properties:
                return block.properties()
            elif role == Role.BlockType:
                return block._TYPE_


    def headerData(
        self
        ,section
        ,orientation
        ,role
    ):
        if orientation == Qt.Horizontal and section == 0 and role == Qt.DisplayRole:
            return self.__langName
        else:
            return QAbstractItemModel.headerData(self,section,orientation,role)


    def index(
        self
        ,row
        ,column
        ,parent
    ):
        if row < 0 or column != 0:
            return QModelIndex()
        parentBlock = self.__block_(parent)
        if parentBlock is None or row >= len(parentBlock):
            return QModelIndex()
        return self.createIndex(row,column,parentBlock[row])


    def insertFromXml(
        self
        ,row
        ,xml
        ,parent
    ):
        """
        Inserts blocks into the given parent index of this model, at the given
        row, and loaded from the given Qt byte array XML document. The total
        number of blocks that were inserted is returned.

        Parameters
        ----------
        row : int
              The row.
        xml : QByteArray
              The Qt byte array XML document.
        parent : QModelIndex
                 The parent index.

        Returns
        -------
        result : int
                 The total number of inserted blocks.
        """
        parentBlock = self.__block_(parent)
        if parentBlock is None or row < 0 or row > len(parentBlock):
            return 0
        stream = QXmlStreamReader(xml)
        stream.readNextStartElement()
        if not stream.isStartElement() or stream.name() != Settings.ProjectModel.COPY_TAG:
            return 0
        stream.readNextStartElement()
        if stream.name() != Settings.ProjectModel.LANG_TAG:
            return 0
        langName = stream.readElementText()
        if langName != self.__langName:
            return 0
        blocks = []
        while not stream.atEnd():
            stream.readNext()
            if stream.isStartElement():
                name = stream.name()
                block_ = BlockFactory.s().create(langName,name)
                block_.setFromXml(stream)
                if name in parentBlock.buildList():
                    blocks.append(block_)
        self.__push_(InsertCommand(row,blocks,parent,self))
        return len(blocks)


    def insertRow(
        self
        ,row
        ,blockType
        ,parent
    ):
        """
        Implements the QAbstractItemModel interface, adding one additional
        argument that is described. The given block type is used to create a new
        block inserted.

        Parameters
        ----------
        blockType : string
                    The block type.
        """
        return self.insertRows(row,(blockType,),parent)


    def insertRows(
        self
        ,row
        ,blockTypes
        ,parent
    ):
        """
        Implements the QAbstractItemModel interface, adding one additional
        argument that is described. The list of given block types are used to
        create the new blocks inserted.

        Parameters
        ----------
        blockTypes : list
                     The Block types.
        """
        parentBlock = self.__block_(parent)
        if parentBlock is None or row < 0 or row > len(parentBlock):
            return False
        blocks = []
        for blockType in blockTypes:
            block_ = BlockFactory.s().create(self.__langName,blockType)
            block_.setDefaultProperties()
            blocks.append(block_)
        self.__push_(InsertCommand(row,blocks,parent,self))
        return True


    def isModified(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : bool
                 True if this model's currently loaded project has unsaved
                 changes or false otherwise.
        """
        return self.__modified


    def langName(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The language name of this model's project or none if it has no
                 project.
        """
        return self.__langName


    def load(
        self
        ,path
    ):
        """
        Loads a project to this model from the given file path, closing any
        currently loaded project.

        Parameters
        ----------
        path : string
               The file path.
        """
        xml = None
        with open(path,"br") as ifile:
            xml = ifile.read()
        stream = QXmlStreamReader(xml)
        stream.readNextStartElement()
        if not stream.isStartElement() or stream.name() != Settings.ProjectModel.PROJECT_TAG:
            raise LoadError("Invalid XML project tag.")
        stream.readNextStartElement()
        if stream.name() != Settings.ProjectModel.LANG_TAG:
            raise LoadError("Invalid/missing XML language tag.")
        lang_name = stream.readElementText()
        stream.readNextStartElement()
        if stream.name() != Settings.ProjectModel.NAME_TAG:
            raise LoadError("Invalid/missing XML name tag.")
        name = stream.readElementText()
        stream.readNextStartElement()
        if stream.name() != Settings.ProjectModel.PARSE_PATH_TAG:
            raise LoadError("Invalid/missing XML parse path tag.")
        parse_path = stream.readElementText()
        self.new(lang_name)
        self.__name = name
        self.__parsePath = parse_path
        try:
            stream.readNextStartElement()
            if stream.name() != self.__root._TYPE_:
                raise LoadError("Invalid/missing XML root block tag.")
            self.__root.setFromXml(stream)
            if stream.hasError():
                raise LoadError("Failed loading XML: "+stream.errorString())
        except:
            self.close()
            raise


    def moveRow(
        self
        ,change
        ,index
    ):
        """
        Moves the given Qt model index's row by the given row change, where
        negative is up and positive is down.

        Parameters
        ----------
        change : int
                 The row change.
        index : QModelIndex
                The Qt model index.

        Returns
        -------
        result : bool
                 True if the move was successful or false otherwise.
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


    def name(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The name of this model's project or none if it has no project.
        """
        return self.__name


    def new(
        self
        ,langName
    ):
        """
        Creates a new project for this model with the given language name,
        closing any currently loaded project. The given language name must be
        valid.

        Parameters
        ----------
        langName : string
                   The language name.
        """
        if self.__root is not None:
            self.close()
        self.beginResetModel()
        try:
            self.__name = "New Project"
            self.__parsePath = "."
            self.__langName = langName
            self.__root = BlockFactory.s().create(self.__langName)
        except:
            self.__name = None
            self.__langName = None
            self.__root = None
            raise
        finally:
            self.endResetModel()


    def parent(
        self
        ,child
    ):
        childBlock = self.__block_(child)
        if childBlock is None:
            return QModelIndex()
        parentBlock = childBlock.parent()
        if parentBlock is None or parentBlock.parent() is None:
            return QModelIndex()
        return self.createIndex(parentBlock.index(),0,parentBlock)


    def parsePath(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : string
                 The relative parsing path of this model's project or none if it
                 has no project.
        """
        return self.__parsePath


    def parser(
        self
    ):
        """
        Getter method.

        Returns
        -------
        result : object
                 A newly built parser for this model's project or none if it has
                 no project.
        """
        if self.__root is not None:
            ret = ParserFactory.s().create(self.__root)
            assert(isinstance(ret,AbstractParser))
            return ret


    @Slot()
    def redo(
        self
    ):
        """
        Called to redo the last undone modification of this model. If this model
        cannot redo then this does nothing.
        """
        if self.canRedo():
            self.__undoStack[self.__undoStackIndex].redo()
            self.__undoStackIndex += 1


    def removeRows(
        self
        ,row
        ,count
        ,parent
    ):
        parentBlock = self.__block_(parent)
        if parentBlock is None or row < 0 or count < 0 or (row + count) > len(parentBlock):
            return False
        self.__push_(RemoveCommand(row,count,parent,self))
        return True


    def rowCount(
        self
        ,index
    ):
        block = self.__block_(index)
        return 0 if block is None else len(block)


    def save(
        self
        ,path
    ):
        """
        Saves this model's currently loaded project to the file with the given
        path. If this model has no project then this does nothing.

        Parameters
        ----------
        path : string
               The path.
        """
        if self.__root is not None:
            xml = QByteArray()
            stream = QXmlStreamWriter(xml)
            stream.setAutoFormatting(True)
            stream.writeStartDocument()
            stream.writeStartElement(Settings.ProjectModel.PROJECT_TAG)
            stream.writeTextElement(Settings.ProjectModel.LANG_TAG,self.__langName)
            stream.writeTextElement(Settings.ProjectModel.NAME_TAG,self.__name)
            stream.writeTextElement(Settings.ProjectModel.PARSE_PATH_TAG,self.__parsePath)
            self.__root.toXml(stream)
            stream.writeEndElement()
            stream.writeEndDocument()
            with open(path,"bw") as ofile:
                ofile.write(xml.data())
            self.__modified = False


    def setData(
        self
        ,index
        ,value
        ,role
    ):
        block = self.__block_(index)
        if block is not None and role == Role.Properties:
            self.__push_(SetCommand(block.properties(),value,index,self))
            return True
        else:
            return False


    def setName(
        self
        ,name
    ):
        """
        Sets the name of this model's currently loaded project to the given
        name. This model must have a loaded project.

        Parameters
        ----------
        name : string
               The name.
        """
        if self.__root is None:
            raise RuntimeError("Cannot set name of no project.")
        if name != self.__name:
            self.__name = name
            self.__modified_()
            self.nameChanged.emit(name)


    def setParsePath(
        self
        ,path
    ):
        """
        Sets the relative parsing path of this model's currently loaded project
        to the given relative path. This model must have a loaded project.

        Parameters
        ----------
        path : string
               The relative path.
        """
        if self.__root is None:
            raise RuntimeError("Cannot set name of no project.")
        if path != self.__parsePath:
            self.__parsePath = path
            self.__modified_()
            self.parsePathChanged.emit(path)


    @Slot()
    def undo(
        self
    ):
        """
        Called to undo the last modification that was done to this model. If
        this model cannot undo then this does nothing.
        """
        if self.canUndo():
            self.__undoStackIndex -= 1
            self.__undoStack[self.__undoStackIndex].undo()


    def _insertRows_(
        self
        ,row
        ,blocks
        ,parent
    ):
        """
        Inserts the given blocks into the given parent index of this model at
        the given row. The given parent index and row must be valid.

        Parameters
        ----------
        row : int
              The row.
        blocks : list
                 The blocks.
        parent : QModelIndex
                 The parent index.
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


    def _moveRow_(
        self
        ,start
        ,destination
        ,parent
    ):
        """
        Moves the block at the given start row to the given destination row in
        the given parent index of this model. The given row indexes and parent
        index must all be valid.

        Parameters
        ----------
        start : int
                The start row.
        destination : int
                      The destination row.
        parent : QModelIndex
                 The parent index.
        """
        parentBlock = self.__block_(parent)
        if parentBlock is None:
            raise RuntimeError("Parent index is not a valid block.")
        volatile = False
        self.beginMoveRows(
            parent
            ,start,start
            ,parent
            ,destination + 1 if destination > start else destination
        )
        if parentBlock[start]:
            volatile = True
        parentBlock.insert(destination,parentBlock.pop(start))
        self.endMoveRows()
        self.__pushVolatileAbove_(parent)
        self.__modified_()


    def _removeRows_(
        self
        ,row
        ,count
        ,parent
    ):
        """
        Removes and returns the given count of blocks starting at the given row
        in the given parent index of this model. The given parent index, row,
        and count must all be valid.

        Parameters
        ----------
        row : int
              The row.
        count : int
                The count.
        parent : QModelIndex
                 The parent index.

        Returns
        -------
        result : list
                 Removed blocks.
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


    def _setProperties_(
        self
        ,index
        ,properties
    ):
        """
        Sets the properties of the block at the given Qt model index in this
        model to the given block properties. The given Qt model index must be
        valid.

        Parameters
        ----------
        index : QModelIndex
                The Qt model index.
        properties : dictionary
                     The block properties.
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


    def __block_(
        self
        ,index
    ):
        """
        Returns the block at the given Qt model index of this model. If the
        given Qt model index is invalid then the root block is returned. If this
        model has no project then none is returned.

        Parameters
        ----------
        index : QModelIndex
                The Qt model index.

        Returns
        -------
        result : object
                 The block or none.
        """
        if index.isValid():
            return index.internalPointer()
        else: return self.__root


    def __modified_(
        self
    ):
        """
        Marks this model as modified, emitting a signal if it was not modified
        before calling this method.
        """
        if not self.__modified:
            self.__modified = True
            self.modified.emit()


    def __push_(
        self
        ,command
    ):
        """
        Pushes the given command to this model, immediately calling its redo
        interface and then adding it to this model's undo stack. Any undone
        commands on this model's undo stack are removed.

        Parameters
        ----------
        command : AbstractCommand
                  The command.
        """
        self.__undoStack = self.__undoStack[:self.__undoStackIndex]
        self.__undoStack.append(command)
        self.__undoStackIndex += 1
        command.redo()


    def __pushVolatileAbove_(
        self
        ,index
    ):
        """
        Signals the given Qt model index of this model has changed its data if
        the given index is valid. This then recursively calls this method on the
        parent index of the given parent index if its block is volatile above.

        Parameters
        ----------
        index : QModelIndex
                The Qt model index.
        """
        if index.isValid():
            self.dataChanged.emit(index,index)
            block = self.__block_(index)
            if block is not None and block.isVolatileAbove():
                self.__pushVolatileAbove_(index.parent())


    def __pushVolatileBelow_(
        self
        ,parent
    ):
        """
        Signals the given parent index of this model has changed causing all its
        children to update their data. This then recursively calls this method
        on any children indexes whose blocks are also volatile below.

        Parameters
        ----------
        parent : QModelIndex
                 The parent index.
        """
        for row in range(self.rowCount(parent)):
            index = self.index(row,0,parent)
            self.dataChanged.emit(index,index)
            block = self.__block_(index)
            if block.isVolatileBelow():
                self.__pushVolatileBelow_(index)


Role = ProjectModel.Role
