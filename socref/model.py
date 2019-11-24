"""
Contains all model classes used by this core application.
"""
import traceback
import enum
from PySide2 import QtCore as qtc
from . import exception
from . import util
from . import abstract
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








@util.Singleton
class Parser(qtc.QObject):
    """
    This is the singleton parser model class. It starts execution of an abstract parser until
    completion. It is designed to work on its own thread separate of the main GUI thread.
    """


    #######################
    # PUBLIC - Initialize #
    #######################


    def __init__(self):
        """
        Initializes a new parser.
        """
        qtc.QObject.__init__(self)


    ##################
    # PUBLIC - Slots #
    ##################


    @qtc.Slot(abstract.Parser)
    def start(self, parser):
        """
        Called to start execution of the given abstract parser, returning when execution is
        complete.

        parser : The abstract parser that is executed.
        """
        try:
            #
            # Execute the given parser by calling its parse method.
            #
            parser.parse()
        except:
            #
            # If any exception occurs then catch it and report it to standard error because qt
            # threads ignore python exceptions.
            #
            traceback.print_exc()








class Project(qtc.QAbstractItemModel):
    """
    This is the project model class. It implements the qt abstract item model class. It provides
    additional methods above and beyond the basic model methods for added functionality. It has
    built in support for undo and redo of any modification done to it.

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
        #
        # Initialize the currently loaded project's name, root block, and language name to none.
        #
        self.__name = None
        self.__root = None
        self.__lang_name = None
        #
        # Initialize the empty undo stack and its current index.
        #
        self.__undo_stack = []
        self.__undo_stack_index = 0
        #
        # Initialize the currently loaded project's modification state to false.
        #
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


    def new(self, lang_name):
        """
        Creates a new project for this model with the given language name, closing any currently
        loaded project. The given language name must be valid.

        lang_name : The name of the language used to create a new project.
        """
        #
        # If this model has a project loaded then close it.
        #
        if self.__root is not None : self.close()
        #
        # Signal this model is being reset.
        #
        self.beginResetModel()
        try:
            #
            # Set the new default project name, language name, and newly created root block.
            #
            self.__name = "New Project"
            self.__lang_name = lang_name
            self.__root = abstract.Factory().create_root(self.__lang_name)
        except:
            #
            # If any exception occurs while creating the new project then reset this model to being
            # empty and raise it again.
            #
            self.__name = None
            self.__lang_name = None
            self.__root = None
            raise
        finally:
            #
            # Finish this model reset.
            #
            self.endResetModel()


    def load(self, path):
        """
        Loads a project to this model from the given file path, closing any currently loaded
        project.

        path : The file path of the project that is loaded into this model.
        """
        #
        # Read in the XML from the file with the given path.
        #
        xml = None
        with open(path,"br") as ifile : xml = ifile.read()
        #
        # Create a new qt XML stream reader from the loaded XML.
        #
        stream = qtc.QXmlStreamReader(xml)
        #
        # Read to the next start element and make sure it is the correct project tag.
        #
        stream.readNextStartElement()
        if not stream.isStartElement() or stream.name() != self.__PROJECT_TAG :
            raise exception.LoadError("Invalid XML project tag.")
        #
        # Read to the next start element and read in the language name, making sure it is the
        # correct language tag.
        #
        stream.readNextStartElement()
        if stream.name() != self.__LANG_TAG :
            raise exception.LoadError("Invalid/missing XML language tag.")
        lang_name = stream.readElementText()
        #
        # Read to the next start element and read in the project name, making sure it is the correct
        # name tag.
        #
        stream.readNextStartElement()
        if stream.name() != self.__NAME_TAG :
            raise exception.LoadError("Invalid/missing XML name tag.")
        name = stream.readElementText()
        #
        # Create a new project for this model using the read in language name and then set the
        # project name to the read in name.
        #
        self.new(lang_name)
        self.__name = name
        try:
            #
            # Read to the next start element and make sure it is the correct bock type tag.
            #
            stream.readNextStartElement()
            if stream.name() != self.__root._TYPE_ :
                raise exception.LoadError("Invalid/missing XML root block tag.")
            #
            # Read in all block data from the root block XML element.
            #
            self.__root.set_from_xml(stream)
        except:
            #
            # If any exception occurs then close the partially loaded project and raise it again.
            #
            self.close()
            raise


    def save(self, path):
        """
        Saves this model's currently loaded project to the file with the given path. If this model
        has no project then this does nothing.

        path : The path to the save file of this model's project.
        """
        #
        # Make sure this model has a project.
        #
        if self.__root is not None:
            #
            # Create a new qt byte array the XML will be written to.
            #
            xml = qtc.QByteArray()
            #
            # Create a new qt XML writer stream attached to the byte array.
            #
            stream = qtc.QXmlStreamWriter(xml)
            #
            # Initialize the stream.
            #
            stream.setAutoFormatting(True)
            #
            # Write the start document and start project tag.
            #
            stream.writeStartDocument()
            stream.writeStartElement(self.__PROJECT_TAG)
            #
            # Write the language name and project name text elements.
            #
            stream.writeTextElement(self.__LANG_TAG,self.__lang_name)
            stream.writeTextElement(self.__NAME_TAG,self.__name)
            #
            # Write the block data element.
            #
            self.__root.to_xml(stream)
            #
            # Write the end project tag and end document.
            #
            stream.writeEndElement()
            stream.writeEndDocument()
            #
            # Open the output file at the given path and save the XML document, setting this model's
            # modified state to false.
            #
            with open(path,"bw") as ofile : ofile.write(xml.data())
            self.__modified = False


    def close(self):
        """
        Closes any currently loaded project of this model. If this model has no loaded project then
        this does nothing.
        """
        #
        # Make sure this model has a project.
        #
        if self.__root is not None:
            #
            # Signal this model is being reset.
            #
            self.beginResetModel()
            #
            # Set the project name, language name, and root block to none.
            #
            self.__name = None
            self.__lang_name = None
            self.__root = None
            #
            # Set this model's modified state to false.
            #
            self.__modified = False
            #
            # Reset this model's undo stack and stack index.
            #
            self.__undo_stack.clear()
            self.__undo_stack_index = 0
            #
            # Finish this model reset.
            #
            self.endResetModel()


    def lang_name(self):
        """
        Getter method.

        return : The language name of this model's currently loaded project or none if it has no
                 project.
        """
        return self.__lang_name


    def name(self):
        """
        Getter method.

        return : The name of this model's currently loaded project or none if it has no project.
        """
        return self.__name


    def set_name(self, name):
        """
        Sets the name of this model's currently loaded project to the one given. This model must
        have a loaded project.

        name : The new name for this model's project.
        """
        #
        # Make sure this model has a project.
        #
        if self.__root is None : raise RuntimeError("Cannot set name of no project.")
        #
        # Make sure the given name is different from the current one.
        #
        if name != self.__name :
            #
            # Update the name, signal the project is modified and the name is modified.
            #
            self.__name = name
            self.__modified_()
            self.name_changed.emit(name)


    def is_modified(self):
        """
        Getter method.

        return : True if this model's currently loaded project has unsaved changes or false
                 otherwise.
        """
        return self.__modified


    def can_undo(self):
        """
        Getter method.

        return : True if this model has a modification that can be undone or false otherwise.
        """
        return self.__undo_stack_index > 0


    def can_redo(self):
        """
        Getter method.

        return : True if this model has a modification that can be redone or false otherwise.
        """
        return self.__undo_stack_index < len(self.__undo_stack)


    def headerData(self, section, orientation, role):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        section : See qt docs.

        orientation : See qt docs.

        role : See qt docs.

        return : See qt docs.
        """
        #
        # If the given orientation is horizontal, the section is 0, and the role is display then
        # return the model's language name.
        #
        if orientation == qtc.Qt.Horizontal and section == 0 and role == qtc.Qt.DisplayRole :
            return self.__lang_name
        #
        # Else return the default header data.
        #
        else: return qtc.QAbstractItemModel.headerData(self,section,orientation,role)


    def index(self, row, column, parent):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        row : See qt docs.

        column : See qt docs.

        parent : See qt docs.

        return : See qt docs.
        """
        #
        # Make sure the given row and column are valid.
        #
        if row < 0 or column != 0 : return qtc.QModelIndex()
        #
        # Get the parent block, making sure it worked and the row is valid.
        #
        parent_block = self.__block_(parent)
        if parent_block is None or row >= len(parent_block) : return qtc.QModelIndex()
        #
        # Return a new index with its internal pointer set to the child block of the given parent at
        # the given row.
        #
        return self.createIndex(row,column,parent_block[row])


    def parent(self, child):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        child : See qt docs.

        return : See qt docs.
        """
        #
        # Get the child block and make sure it worked.
        #
        child_block = self.__block_(child)
        if child_block is None : return qtc.QModelIndex()
        #
        # Get the parent block, making sure it works and it also has a parent block.
        #
        parent_block = child_block.parent()
        if parent_block is None or parent_block.parent() is None : return qtc.QModelIndex()
        #
        # Return the given index's parent using its index method to get its row.
        #
        return self.createIndex(parent_block.index(),0,parent_block)


    def rowCount(self, index):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        index : See qt docs.

        return : See qt docs.
        """
        #
        # Get the block of the given index, making sure it worked.
        #
        block = self.__block_(index)
        if block is None : return 0
        #
        # Return the block's total number of children.
        #
        else: return len(block)


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
        #
        # Get the block of the given index, making sure it worked.
        #
        block = self.__block_(index)
        if block is not None :
            #
            # Return the appropriate data based off the given role.
            #
            if role == qtc.Qt.DisplayRole : return block.display_name()
            elif role == qtc.Qt.DecorationRole : return block.icon()
            elif role == Role.BUILD_LIST : return block.build_list()
            elif role == Role.VIEW : return block.display_view()
            elif role == Role.EDIT_DEFS : return block.edit_definitions()
            elif role == Role.PROPERTIES : return block.properties()
            elif role == Role.BLOCK_TYPE : return block._TYPE_


    def setData(self, index, value, role):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        index : See qt docs.

        value : See qt docs.

        role : See qt docs.

        return : See qt docs.
        """
        #
        # Get the block of the given index, making sure it worked and the given role is properties.
        #
        block = self.__block_(index)
        if block is not None and role == Role.PROPERTIES :
            #
            # Push a new set command to this model's undo stack and then return true.
            #
            self.__push_(Set(block.properties(),value,index,self))
            return True
        #
        # Else return false.
        #
        else: return False


    def insertRows(self, row, block_types, parent):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        row : See qt docs.

        block_types : A list of block types, where each one represents a new block that is created
                      and inserted with the given type name.

        parent : See qt docs.

        return : See qt docs.
        """
        #
        # Get the parent block of the given parent index, making sure it worked and the given row is
        # valid for it.
        #
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or row > len(parent_block) : return False
        #
        # Build a list of new blocks from the list of block types, setting their default properties.
        #
        blocks = []
        for block_type in block_types :
            block = abstract.Factory().create(self.__lang_name,block_type)
            block.set_default_properties()
            blocks.append(block)
        #
        # Push a new insert command to this model's undo stack with the list of new blocks and then
        # return true.
        #
        self.__push_(Insert(row,blocks,parent,self))
        return True


    def removeRows(self, row, count, parent):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        row : See qt docs.

        count : See qt docs.

        parent : See qt docs.

        return : List of blocks that were successfully removed from this model.
        """
        #
        # Get the parent block of the given parent index, making sure it worked and the given row
        # and count are both valid.
        #
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or count < 0 or (row + count) > len(parent_block) :
            return False
        #
        # Push a new remove command to this model's undo stack and then return true.
        #
        self.__push_(Remove(row,count,parent,self))
        return True


    def insertRow(self, row, block_type, parent):
        """
        Implements the PySide2.QtCore.QAbstractItemModel interface.

        row : See qt docs.

        block_type : The block type that is created and inserted into this model.

        parent : See qt docs.

        return : See qt docs.
        """
        return self.insertRows(row,(block_type,),parent)


    def move_row(self, change, index):
        """
        Moves the given index's row by the amount given.

        change : The row change for the index that is moved. Negative is up and positive is down.

        index : The qt model index that is moved.

        return : True if the move was successful or false otherwise.
        """
        #
        # Make sure the given change and index are both valid.
        #
        if not change or not index.isValid() : return False
        #
        # Get the block of the given index, making sure it worked.
        #
        block = self.__block_(index.parent())
        if block is None : return False
        #
        # Figure out the new row for the given index and make sure it is valid.
        #
        to_row = index.row() + change
        if to_row < 0 or to_row >= len(block) : return False
        #
        # Push a new move command to this model's undo stack and then return true.
        #
        self.__push_(Move(change,index,self))
        return True


    def copy_to_xml(self, indexes):
        """
        Copies the given indexes to XML.

        indexes : List of qt model indexes whose blocks are copied to XML.

        return : A tuple of two items. The first item is copied blocks at the given indexes in the
                 form of XML. The XML format is copy with each index an element in the root XML copy
                 tag. The second item is a set of all block types that were copied.
        """
        #
        # Initialize the block types set and XML byte array.
        #
        block_types = set()
        xml = qtc.QByteArray()
        #
        # Create a new qt XML writer stream attached to the byte array.
        #
        stream = qtc.QXmlStreamWriter(xml)
        #
        # Write the start document and start copy element.
        #
        stream.setAutoFormatting(True)
        #
        # Write the language name text element.
        #
        stream.writeStartDocument()
        stream.writeStartElement(self.__COPY_TAG)
        #
        # Iterate through all given indexes.
        #
        stream.writeTextElement(self.__LANG_TAG,self.__lang_name)
        #
        # Get the block of the index, writing its XML element and adding its type name to the set if
        # it is valid.
        #
        for index in indexes :
            #
            # Write the end copy element and end document.
            #
            block = self.__block_(index)
            if block is not None :
                block.to_xml(stream)
                block_types.add(block._TYPE_)
        #
        # Return the XML byte array and block types set.
        #
        stream.writeEndElement()
        stream.writeEndDocument()
        #
        return xml,block_types


    def insert_from_xml(self, row, xml, parent):
        """
        Inserts blocks into this model at the given parent index and row loaded from the given XML.

        row : The row in the given parent index where loaded blocks are inserted.

        xml : XML byte array containing the loaded blocks. This must conform to the XML copy
              standard produced by the copy to XML method of this class.

        parent : The parent index where the loaded blocks are inserted.

        return : Total number of blocks that were inserted into this model.
        """
        #
        # Get the parent block of the given parent index, making sure it worked and the given row is
        # valid.
        #
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or row > len(parent_block) : return 0
        #
        # Create a new qt XML reader stream attached to the given XML byte array.
        #
        stream = qtc.QXmlStreamReader(xml)
        #
        # Read the next start element and make sure it is the copy tag.
        #
        stream.readNextStartElement()
        if not stream.isStartElement() or stream.name() != self.__COPY_TAG : return 0
        #
        # Read the next start element, making sure it is the language tag and the language is the
        # same as this model's currently loaded project's language.
        #
        stream.readNextStartElement()
        if stream.name() != self.__LANG_TAG : return 0
        lang_name = stream.readElementText()
        if lang_name != self.__lang_name : return 0
        #
        # Initialize an empty list of loaded blocks and continue reading the XML stream until the
        # end.
        #
        blocks = []
        while not stream.atEnd() :
            #
            # Get the next XML node and check to see if it is a start element.
            #
            stream.readNext()
            if stream.isStartElement() :
                #
                # Load the block from its XML tag.
                #
                name = stream.name()
                block = abstract.Factory().create(lang_name,name)
                block.set_from_xml(stream)
                #
                # If the given parent block can have the loaded block as a child then append it to
                # the loaded block list.
                #
                if name in parent_block.build_list() : blocks.append(block)
        #
        # Push a new insert command to this model's undo stack using the loaded block list and then
        # return the number of blocks inserted.
        #
        self.__push_(Insert(row,blocks,parent,self))
        return len(blocks)


    def parser(self):
        """
        Builds a new abstract parser that can be used to parse the source code of this model's
        project. If this model has no project that this does nothing.

        return : The newly built abstract parser or none if this model has no project.
        """
        #
        # Make sure this model has a project.
        #
        if self.__root is not None :
            #
            # Create a new abstract parser from this model's project's root block, making sure it is
            # a valid abstract parser.
            #
            ret = self.__root.parser()
            if not isinstance(ret,abstract.Parser) :
                raise RuntimeError("Generated parser is not an abstract parser.")
            #
            # Return the created parser.
            #
            return ret


    ####################
    # PUBLIC - Signals #
    ####################


    #
    # Signals this model's project has been modified with unsaved changes.
    #
    modified = qtc.Signal()
    #
    # Signals this model's project name has changed.
    #
    name_changed = qtc.Signal(str)


    ##################
    # PUBLIC - Slots #
    ##################


    @qtc.Slot()
    def undo(self):
        """
        Called to undo the last modification that was done to this model. If this model cannot undo
        then this does nothing.
        """
        #
        # Make sure this model can undo a modification.
        #
        if self.can_undo() :
            #
            # Undo the last command done on this model's stack and decrement its undo stack index.
            #
            self.__undo_stack_index -= 1
            self.__undo_stack[self.__undo_stack_index].undo()


    @qtc.Slot()
    def redo(self):
        """
        Called to redo the last undone modification of this model. If this model cannot redo then
        this does nothing.
        """
        #
        # Make sure this model can redo a modification.
        #
        if self.can_redo() :
            #
            # Redo the last command undone on this model's stack and increment its undo stack index.
            #
            self.__undo_stack[self.__undo_stack_index].redo()
            self.__undo_stack_index += 1


    #######################
    # PROTECTED - Methods #
    #######################


    def _insert_rows_(self, row, blocks, parent):
        """
        Inserts the given blocks into the given parent index at the given row. The given parent
        index and row must be valid.

        row : The row where the given blocks are inserted into the given parent.

        blocks : List of blocks that are inserted into this model.

        parent : The parent index where the given blocks are inserted into as children.
        """
        #
        # Get the parent block of the given parent index, making sure it works and the given row is
        # valid.
        #
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or row > len(parent_block) :
            raise RuntimeError("Parent index is not a valid block.")
        #
        # Initialize the volatile above indicator and signal this model is inserting new rows.
        #
        volatile = False
        self.beginInsertRows(parent,row,row + len(blocks) - 1)
        #
        # Insert the entire list of given blocks into the parent block starting at the given row,
        # setting the volatile above indicator to true if any one of the inserted blocks are
        # volatile above.
        #
        for block in blocks :
            parent_block.insert(row,block)
            if block.is_volatile_above() : volatile = True
            row += 1
        #
        # Finish the row insertion.
        #
        self.endInsertRows()
        #
        # If any of the blocks were volatile above then push updates accordingly.
        #
        if volatile : self.__push_volatile_above_(parent)
        #
        # Signal this model has been modified.
        #
        self.__modified_()


    def _remove_rows_(self, row, count, parent):
        """
        Removes the given count of blocks starting at the given row in the given parent index. The
        given parent index, row, and count must all be valid.

        row : The starting row where blocks are removed.

        count : The total number of blocks that are removed.

        parent : The parent index where child blocks are removed.

        return : List of removed blocks.
        """
        #
        # Get the parent block of the given parent index, making sure it works and the given row and
        # count are both valid.
        #
        parent_block = self.__block_(parent)
        if parent_block is None or row < 0 or count < 0 or (row + count) > len(parent_block) :
            raise RuntimeError("Parent index is not a valid block.")
        #
        # Initialize the volatile above indicator, the empty list of removed blocks, and then signal
        # this model is removing rows.
        #
        volatile = False
        blocks = []
        self.beginRemoveRows(parent,row,row + count - 1)
        #
        # Remove the given count number of blocks starting at the given row in the given parent,
        # adding them to the removed block list and setting the volatile above indicator to true if
        # any removed block is volatile above.
        #
        while count :
            blocks.append(parent_block.pop(row))
            if blocks[-1].is_volatile_above() : volatile = True
            count -= 1
        #
        # Finish the row removal.
        #
        self.endRemoveRows()
        #
        # If any of the blocks were volatile above then push updates accordingly.
        #
        self.__push_volatile_above_(parent)
        #
        # Signal this model has been modified and return the list of removed blocks.
        #
        self.__modified_()
        return blocks


    def _move_row_(self, from_row, to_row, parent):
        """
        Moves the block at the given from row to the given to row in the given parent index. The
        given rows and parent index must all be valid.

        from_row : The row of the block that is moved.

        to_row : The new row of the moved block.

        parent : The parent index where a child block is moved.
        """
        #
        # Get the parent block of the given parent index, making sure it worked.
        #
        parent_block = self.__block_(parent)
        if parent_block is None : raise RuntimeError("Parent index is not a valid block.")
        #
        # Initialize the volatile above indicator and signal this model is moving rows.
        #
        volatile = False
        self.beginMoveRows(parent
                           ,from_row,from_row
                           ,parent
                           ,to_row + 1 if to_row > from_row else to_row)
        #
        # Move the block at the given from row to the given to row, setting the volatile above
        # indicator to true if it is volatile above.
        #
        block = parent_block.pop(from_row)
        if block.is_volatile_above() : volatile = True
        parent_block.insert(to_row,block)
        #
        # Finish the move rows.
        #
        self.endMoveRows()
        #
        # If the moved block was volatile above then push updates accordingly.
        #
        self.__push_volatile_above_(parent)
        #
        # Signal this model has been modified.
        #
        self.__modified_()


    def _set_props_(self, index, props):
        """
        Sets the properties of the block at the given index in this model. The given index must be
        valid.

        index : The qt model index of the updated block.

        props : The new properties dictionary of the block at the given index.
        """
        #
        # Get the block at the given index, making sure it worked.
        #
        block = self.__block_(index)
        if block is None: raise RuntimeError("Index is not a valid block.")
        #
        # Set the block's properties to the new one given.
        #
        block.set_properties(props)
        #
        # Signal that this model's data has changed.
        #
        self.dataChanged.emit(index,index)
        #
        # If the block is volatile above or below then push updates above or below accordingly.
        #
        if block.is_volatile_above() : self.__push_volatile_above_(index.parent())
        if block.is_volatile_below() : self.__push_volatile_below_(index)
        #
        # Signal that this model has been modified.
        #
        self.__modified_()


    #####################
    # PRIVATE - Methods #
    #####################


    def __modified_(self):
        """
        Marks this model as modified, emitting a signal if it was not modified before calling this
        method.
        """
        if not self.__modified :
            self.__modified = True
            self.modified.emit()


    def __push_(self, command):
        """
        Pushes the given command to this model, immediately calling its redo interface and then
        adding it to this model's undo stack.

        command : The command that is immediately executed and added to this model's undo stack. Any
                  commands that have been undone on the stack are erased.
        """
        #
        # Erase any commands on this model's undo stack that have been undone.
        #
        self.__undo_stack = self.__undo_stack[:self.__undo_stack_index]
        #
        # Add the given command to the undo stack and call its redo interface.
        #
        self.__undo_stack.append(command)
        self.__undo_stack_index += 1
        command.redo()


    def __push_volatile_above_(self, index):
        """
        Signals the given index of this model has changed its data if it is valid, recursively
        calling itself on the given index's parent index if its block is also volatile above.

        index : The index of this model whose data has changed due to volatile children.
        """
        #
        # Make sure the given index is valid.
        #
        if index.isValid() :
            #
            # Signal this model's data has changed.
            #
            self.dataChanged.emit(index,index)
            #
            # If the block at the given index is also volatile above then recursively call this
            # method on its parent index.
            #
            block = self.__block_(index)
            if block is not None and block.is_volatile_above() :
                self.__push_volatile_above_(index.parent())


    def __push_volatile_below_(self, parent):
        """
        Signals the given parent index of this model has changed its children's data if it is valid,
        recursively calling itself on any child that is also volatile below.

        parent : The parent index of this model whose children's data has changed due to the
                 volatile below parent.
        """
        #
        # Iterate through every row of the given parent index.
        #
        for row in range(self.rowCount(parent)) :
            #
            # Get the index of the child and signal this model's data has changed.
            #
            index = self.index(row,0,parent)
            self.dataChanged.emit(index,index)
            #
            # If the child is also volatile below then recursively call this method on the child
            # index.
            #
            block = self.__block_(index)
            if block.is_volatile_below() : self.__push_volatile_below_(index)


    def __block_(self, index):
        """
        Getter method.

        index : The index whose block is returned.

        return : The block of the given index. If the given index is invalid the root block is
                 returned, which is none if this model has no project.
        """
        if index.isValid() : return index.internalPointer()
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
    # The root copy tag used for internal copy XML byte arrays.
    #
    __COPY_TAG = "pysoref_copy"
    #
    # The language tag used for the language name text element of XML project files.
    #
    __LANG_TAG = "language"
