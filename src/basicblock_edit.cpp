#include "basicblock_edit.h"
#include <QJsonObject>
#include <QSettings>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <socutil/sut_exceptions.h>
#include "gui_textedit.h"
#include "gui_listedit.h"
#include "gui_typeselection.h"
#include "projectfactory.h"
#include "abstractblockfactory.h"



using namespace Sut;
//



/*!
 * The tag name for a form element used to define a column of this editor using a 
 * form layout. 
 */
const char* BasicBlock::Edit::_formTag {"form"};
/*!
 * The tag name for a layout element used to define a column of this editor using a 
 * vertical layout. 
 */
const char* BasicBlock::Edit::_layoutTag {"layout"};
/*!
 * The attribute name for the display string used as the title of an edit element. 
 */
const char* BasicBlock::Edit::_displayKey {"display"};
/*!
 * The tag name for a check boxes element used to define a check boxes edit widget. 
 */
const char* BasicBlock::Edit::_checkboxesTag {"checkboxes"};
/*!
 * The tag name for a checkbox element used to define a single checkbox within a 
 * check boxes element. 
 */
const char* BasicBlock::Edit::_checkboxTag {"checkbox"};
/*!
 * The tag name for a line edit element used to define a line edit widget. 
 */
const char* BasicBlock::Edit::_lineTag {"line"};
/*!
 * The tag name for a text edit element used to define a text edit widget. 
 */
const char* BasicBlock::Edit::_textTag {"text"};
/*!
 * The tag name of a list edit element used to define a list edit widget. 
 */
const char* BasicBlock::Edit::_listTag {"list"};
/*!
 * The tag name for a type selection widget used to define a type selection edit 
 * widget. 
 */
const char* BasicBlock::Edit::_typeTag {"type"};
/*!
 * The attribute name for the row size of a check boxes edit element. 
 */
const char* BasicBlock::Edit::_rowSizeKey {"rowsize"};






/*!
 * Constructs a new editor with the given basic block which it edits. 
 *
 * @param block The basic block this new editor is editing. 
 */
BasicBlock::Edit::Edit(BasicBlock* block):
   _block(block)
{
   // Make sure the given block pointer is not null. 
   if ( !block )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Invalid null pointer."));
      throw e;
   }
}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* BasicBlock::Edit::layout()
{
   // Create this editor's splitter widget. 
   _splitter = new QSplitter(this);

   // Prepare an enumeration and matching string list that matches any recognized 
   // element tag names. 
   enum {Form,Layout};
   static const QStringList list {_formTag,_layoutTag};

   // Iterate through all children nodes of this editor's basic block's edit 
   // definition XML element. 
   QDomNode node {_block->_editDefinition.firstChild()};
   while ( !node.isNull() )
   {
      // Check to see if this node is an element. 
      if ( node.isElement() )
      {
         // Determine if this element is a form or a layout, calling the appropriate method 
         // for building the sub layout and adding it to this editor's splitter as a 
         // widget. 
         QDomElement element {node.toElement()};
         switch (list.indexOf(element.tagName()))
         {
         case Form:
            _splitter->addWidget(setupForm(element));
            break;
         case Layout:
            _splitter->addWidget(setupLayout(element));
            break;
         }
      }

      // Move to the next sibling node. 
      node = node.nextSibling();
   }

   // Save the geometry settings of this persistent dialog and the state of this 
   // editor's splitter widget using a base key generated from this basic block's 
   // project type name and its element name. 
   const AbstractBlockFactory& factory {_block->factory()};
   saveSettings(QStringLiteral("basicblock.edit.")
                + ProjectFactory::instance().name(factory.type())
                + QStringLiteral(".")
                + factory.elementName(_block->type()));

   // Create a return layout, add this editor's splitter widget to it, and return the 
   // layout. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_splitter);
   return ret;
}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 */
void BasicBlock::Edit::apply()
{
   // Prepare a function for throwing a common exception if a type mismatch occurs. 
   auto throwTypeMismatch = [](const QString& key)
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Edit widget points to block field id '%1' with incorrect type.")
                   .arg(key));
      throw e;
   };

   // Iterate through all widgets of this editor. 
   for (auto i = _widgets.cbegin(); i != _widgets.cend() ;++i)
   {
      // Get the matching basic block field of the edit widget, making sure the field 
      // exists. 
      auto j {_block->_fields.find(i.key())};
      if ( j == _block->_fields.end() )
      {
         Exception::LogicError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Edit widget points to unkonwn block field id '%1'.").arg(i.key()));
         throw e;
      }

      // Figure out what type the edit widget is and set the value of the basic block 
      // field to the value of the edit widget, making sure the basic block field is of 
      // the correct type. 
      if ( QCheckBox* valid = qobject_cast<QCheckBox*>(*i) )
      {
         if ( j->type() != QVariant::Bool ) throwTypeMismatch(i.key());
         *j = valid->checkState();
      }
      else if ( QLineEdit* valid = qobject_cast<QLineEdit*>(*i) )
      {
         if ( j->type() != QVariant::String ) throwTypeMismatch(i.key());
         *j = valid->text();
      }
      else if ( Gui::TextEdit* valid = qobject_cast<Gui::TextEdit*>(*i) )
      {
         if ( j->type() != QVariant::String ) throwTypeMismatch(i.key());
         *j = valid->toPlainText();
      }
      else if ( Gui::ListEdit* valid = qobject_cast<Gui::ListEdit*>(*i) )
      {
         if ( j->type() != QVariant::StringList ) throwTypeMismatch(i.key());
         *j = valid->value();
      }
      else if ( Gui::TypeSelection* valid = qobject_cast<Gui::TypeSelection*>(*i) )
      {
         if ( j->type() != QVariant::String ) throwTypeMismatch(i.key());
         *j = valid->value();
      }

      // Else the edit widget is of an unknown type so throw an exception. 
      else
      {
         Exception::LogicError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Edit widget of unknown type encountered."));
         throw e;
      }
   }

   // Notify the block system that this editor's basic block has been modified. 
   _block->notifyModified();
   _block->notifyNameModified();
   _block->notifyBodyModified();
}






/*!
 * Implements _QWidget_ interface. This implementation uses this interface to save 
 * the state of its splitter widget and then calls its parent interface. 
 *
 * @param event See Qt docs. 
 */
void BasicBlock::Edit::closeEvent(QCloseEvent* event)
{
   // Save the state of this editor's splitter with its state key. 
   QSettings settings;
   settings.setValue(_stateKey,_splitter->saveState());

   // Call this object's parent interface. 
   Gui::AbstractEdit::closeEvent(event);
}






/*!
 * Returns the value of the id attribute from the given XML element. If the id 
 * attribute does not exist or is empty then an exception is thrown. 
 *
 * @param element XML element whose id attribute value is extracted and return. 
 *
 * @return Value of the id attribute of the given XML element. 
 */
QString BasicBlock::Edit::extractId(const QDomElement& element)
{
   // Get the id attribute value from the given XML element and make sure it is not 
   // empty or not set. 
   const QString id {element.attribute(_idKey)};
   if ( id.isEmpty() )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML edit element does not contain valid id attribute."));
      throw e;
   }

   // Return the id attribute value. 
   return id;
}






/*!
 * Sets the base key used by this dialog to save its geometry and state and 
 * restores its geometry and state. Additional strings are appended to this base 
 * string to the geometry and state. 
 *
 * @param baseKey The base key which is copied to generate this object's geometry 
 *                and state keys. 
 */
void BasicBlock::Edit::saveSettings(const QString& baseKey)
{
   // Set the geometry and state keys for this dialog using the given base key. 
   QString geometryKey {baseKey + QStringLiteral(".geometry")};
   _stateKey = baseKey + QStringLiteral(".state");

   // Restore the state of this object's splitter using the state key. 
   QSettings settings;
   _splitter->restoreState(settings.value(_stateKey).toByteArray());

   // Call the abstract edit save settings method using the geometry key. 
   Gui::AbstractEdit::saveSettings(geometryKey);
}






/*!
 * Creates and initializes a form layout row of this new editor's form using the 
 * given XML element to define all edit widgets it contains. 
 *
 * @param element The XML element that defines the newly created form layout. 
 *
 * @return A widget whose layout is set to a form layout row created with the 
 *         provided XML element definition. 
 */
QWidget* BasicBlock::Edit::setupForm(const QDomElement& element)
{
   // Create a new form layout and add all widgets defined by the given XML element. 
   QFormLayout* column {new QFormLayout};
   addWidgets(column,element);

   // Create a return widget, set its layout to the new form layout, and return it. 
   QWidget* ret {new QWidget(this)};
   ret->setLayout(column);
   return ret;
}






/*!
 * Creates and initializes a vertical layout row of this new editor's form using 
 * the given XML element to define all edit widgets it contains. 
 *
 * @param element The XML element that defines the newly created vertical layout. 
 *
 * @return A widget whose layout is set to a vertical layout row created with the 
 *         provided XML element definition. 
 */
QWidget* BasicBlock::Edit::setupLayout(const QDomElement& element)
{
   // Create a new vertical layout and add all widgets defined by the given XML 
   // element. 
   QVBoxLayout* column {new QVBoxLayout};
   addWidgets(column,element);

   // Create a return widget, set its layout to the new form layout, and return it. 
   QWidget* ret {new QWidget(this)};
   ret->setLayout(column);
   return ret;
}






/*!
 * Adds all edit widgets to the given layout using the given XML element which 
 * defines all edit widgets to add. The given layout can be a form or vertical 
 * layout. 
 *
 * @param layout A form or vertical layout that has all created edit widgets added 
 *               to it. 
 *
 * @param element The XML element that defines all edit widgets to create and add 
 *                to the given layout. 
 */
void BasicBlock::Edit::addWidgets(QLayout* layout, const QDomElement& element)
{
   // Prepare an enumeration and matching string list that matches any recognized 
   // edit element tag names. 
   enum {CheckBoxes,LineEdit,TextEdit,ListEdit,TypeEdit};
   static const QStringList list {_checkboxesTag,_lineTag,_textTag,_listTag,_typeTag};

   // Iterate through all children nodes of the given element. 
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      // Check to see if the node is an element. 
      if ( node.isElement() )
      {
         // Determine which edit element this is by its tag name and call the appropriate 
         // method for adding a new edit widget. 
         QDomElement child {node.toElement()};
         switch (list.indexOf(child.tagName()))
         {
         case CheckBoxes:
            addCheckBoxes(layout,child);
            break;
         case LineEdit:
            addLineEdit(layout,child);
            break;
         case TextEdit:
            addTextEdit(layout,child);
            break;
         case ListEdit:
            addListEdit(layout,child);
            break;
         case TypeEdit:
            addTypeEdit(layout,child);
            break;
         }
      }

      // Move to the next sibling node. 
      node = node.nextSibling();
   }
}






/*!
 * Adds a new check boxes layout of check box edit widgets to the given layout 
 * using the given XML element to define the new check box edit widgets. This also 
 * adds the new edit widget to this editor's list of edit widgets. 
 *
 * @param layout The layout that has the new layout of edit widgets added to it. 
 *
 * @param element The XML edit element that defines the new layout of edit widgets 
 *                and children elements that define each check box edit widget that 
 *                is created. 
 */
void BasicBlock::Edit::addCheckBoxes(QLayout* layout, const QDomElement& element)
{
   // Get the row size attribute from the given XML element, or its default if it 
   // does not exist, and make sure reading it as an integer worked. 
   bool ok;
   int rowSize {element.attribute(_rowSizeKey,"1").toInt(&ok)};
   if ( !ok )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML checkboxes edit element does not contain valid rowsize attribute."));
      throw e;
   }

   // Make sure the row size is not a negative integer. 
   if ( rowSize < 0 )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML checkboxes edit element contains negative rowsize."));
      throw e;
   }

   // Create the grid layout for the new check boxes edit widget and initialize the 
   // row and column indexes. 
   QGridLayout* grid {new QGridLayout};
   int row {0};
   int column {0};

   // Iterate through all child nodes of the given element. 
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      // Check to see if the node is an element. 
      if ( node.isElement() )
      {
         // Check to see if the element's tag name matches the check box tag. 
         QDomElement child {node.toElement()};
         if ( child.tagName() == _checkboxTag )
         {
            // Setup a new check box edit widget and then add it to the grid layout using the 
            // current row and column indexes. 
            grid->addWidget(setupCheckBox(extractId(child),child.attribute(_displayKey))
                            ,row++
                            ,column);

            // If the row index is above the row size then reset it and increment the column 
            // index. 
            if ( row >= rowSize )
            {
               row = 0;
               ++column;
            }
         }
      }
      node = node.nextSibling();
   }

   // Add the new grid layout with all the check box edit widgets to the given 
   // layout. 
   add(layout,element,grid);
}






/*!
 * Adds a new line edit widget to the given layout using the given XML element to 
 * define the new edit widget. This also adds the new edit widget to this editor's 
 * list of edit widgets. 
 *
 * @param layout The layout that has the new edit widget added to it. 
 *
 * @param element The XML edit element that defines the new edit widget that is 
 *                created. 
 */
void BasicBlock::Edit::addLineEdit(QLayout* layout, const QDomElement& element)
{
   // Get the id attribute from the given XML element and then get the basic block 
   // field value with the id. 
   QString id {extractId(element)};
   QVariant field {fieldValue(id,QVariant::String)};

   // Create and initialize the new line edit widget. 
   QLineEdit* edit {new QLineEdit(field.toString())};

   // Add the new widget to this editor's list of edit widgets and to the given 
   // layout. 
   _widgets.insert(id,edit);
   add(layout,element,edit);
}






/*!
 * Adds a new text edit widget to the given layout using the given XML element to 
 * define the new edit widget. This also adds the new edit widget to this editor's 
 * list of edit widgets. 
 *
 * @param layout The layout that has the new edit widget added to it. 
 *
 * @param element The XML edit element that defines the new edit widget that is 
 *                created. 
 */
void BasicBlock::Edit::addTextEdit(QLayout* layout, const QDomElement& element)
{
   // Get the id attribute from the given XML element and then get the basic block 
   // field value with the id. 
   QString id {extractId(element)};
   QVariant field {fieldValue(id,QVariant::String)};

   // Create and initialize the new text edit widget. 
   Gui::TextEdit* edit {new Gui::TextEdit(_block)};
   edit->setPlainText(field.toString());

   // Add the new widget to this editor's list of edit widgets and to the given 
   // layout. 
   _widgets.insert(id,edit);
   add(layout,element,edit);
}






/*!
 * Adds a new list edit widget to the given layout using the given XML element to 
 * define the new edit widget. This also adds the new edit widget to this editor's 
 * list of edit widgets. 
 *
 * @param layout The layout that has the new edit widget added to it. 
 *
 * @param element The XML edit element that defines the new edit widget that is 
 *                created. 
 */
void BasicBlock::Edit::addListEdit(QLayout* layout, const QDomElement& element)
{
   // Get the id attribute from the given XML element and then get the basic block 
   // field value with the id. 
   QString id {extractId(element)};
   QVariant field {fieldValue(id,QVariant::StringList)};

   // Create and initialize the new list edit widget. 
   Gui::ListEdit* edit {new Gui::ListEdit(_block)};
   edit->setValue(field.toStringList());

   // Add the new widget to this editor's list of edit widgets and to the given 
   // layout. 
   _widgets.insert(id,edit);
   add(layout,element,edit);
}






/*!
 * Adds a new type selection edit widget to the given layout using the given XML 
 * element to define the new edit widget. This also adds the new edit widget to 
 * this editor's list of edit widgets. 
 *
 * @param layout The layout that has the new edit widget added to it. 
 *
 * @param element The XML edit element that defines the new edit widget that is 
 *                created. 
 */
void BasicBlock::Edit::addTypeEdit(QLayout* layout, const QDomElement& element)
{
   // Get the id attribute from the given XML element and then get the basic block 
   // field value with the id. 
   QString id {extractId(element)};
   QVariant field {fieldValue(id,QVariant::String)};

   // Create and initialize the new type selection edit widget. 
   Gui::TypeSelection* edit {new Gui::TypeSelection(_block->typeList())};
   edit->setValue(field.toString());

   // Add the new widget to this editor's list of edit widgets and to the given 
   // layout. 
   _widgets.insert(id,edit);
   add(layout,element,edit);
}






/*!
 * Creates and initializes a new check box widget with the given id and title, 
 * returning its pointer. This also adds it to this editor's list of edit widgets. 
 *
 * @param id The basic block field id the new check box edit widget is attached to. 
 *
 * @param title The display title of the new check box edit widget. 
 *
 * @return Pointer to the new check box edit widget. 
 */
QCheckBox* BasicBlock::Edit::setupCheckBox(const QString& id, const QString& title)
{
   // Get the basic block field value with the given id. 
   QVariant field {fieldValue(id,QVariant::Bool)};

   // Create and initialize the new check box edit widget. 
   QCheckBox* ret {new QCheckBox(title)};
   ret->setChecked(field.toBool());

   // Add the new check box to this editor's list of edit widgets and then return its 
   // pointer. 
   _widgets.insert(id,ret);
   return ret;
}






/*!
 * Adds the given widget to the given layout using the given XML element to derive 
 * its display title. The given layout must be a vertical or form layout. 
 *
 * @param layout The layout that has the given widget added to it. 
 *
 * @param element The XML element whose display attribute is used as the display 
 *                title. 
 *
 * @param item The widget that is added to the given with a display title extracted 
 *             from the given XML element. 
 */
void BasicBlock::Edit::add(QLayout* layout, const QDomElement& element, QWidget* item)
{
   // If the given layout is a form then use its methods to add the given item to it 
   // with a title derived from the given XML element. 
   if ( QFormLayout* valid = qobject_cast<QFormLayout*>(layout) )
   {
      valid->addRow(new QLabel(element.attribute(_displayKey),this),item);
   }

   // Else if the given layout is a vertical box then use its methods to add the 
   // given item to it with a title derived from the given XML element. 
   else if ( QVBoxLayout* valid = qobject_cast<QVBoxLayout*>(layout) )
   {
      valid->addWidget(new QLabel(element.attribute(_displayKey),this));
      valid->addWidget(item);
   }

   // Else this is an unsupported layout type so throw an exception. 
   else
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Given layout is of unsupported type."));
      throw e;
   }
}






/*!
 * Adds the given item to the given layout using the given XML element to derive 
 * its display title. The given layout must be a vertical or form layout. 
 *
 * @param layout The layout that has the given item added to it. 
 *
 * @param element The XML element whose display attribute is used as the display 
 *                title. 
 *
 * @param item The item that is added to the given with a display title extracted 
 *             from the given XML element. 
 */
void BasicBlock::Edit::add(QLayout* layout, const QDomElement& element, QLayout* item)
{
   // If the given layout is a form then use its methods to add the given item to it 
   // with a title derived from the given XML element. 
   if ( QFormLayout* valid = qobject_cast<QFormLayout*>(layout) )
   {
      valid->addRow(new QLabel(element.attribute(_displayKey),this),item);
   }

   // Else if the given layout is a vertical box then use its methods to add the 
   // given item to it with a title derived from the given XML element. 
   else if ( QVBoxLayout* valid = qobject_cast<QVBoxLayout*>(layout) )
   {
      valid->addWidget(new QLabel(element.attribute(_displayKey),this));
      valid->addLayout(item);
   }

   // Else this is an unsupported layout type so throw an exception. 
   else
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Given layout is of unsupported type."));
      throw e;
   }
}






/*!
 * Return the value of this editor's basic block's field with the given id, 
 * checking its type matches with the given type. This assumes this is being called 
 * to add a new edit widget so additional checks are done. If this editor already 
 * has an edit widget with the given id, no field exists with the given id, or the 
 * field type does not match the given type then an exception is thrown. 
 *
 * @param id The basic block field id whose value is returned. 
 *
 * @param type The basic block field type that is checked with the field type 
 *             matched with the given id. 
 *
 * @return Basic block field value with the given id. 
 */
QVariant BasicBlock::Edit::fieldValue(const QString& id, quint32 type)
{
   // Make sure this editor's list of edit widgets does not already contain the given 
   // id. 
   if ( _widgets.contains(id) )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML edit element overrides already existing edit widget with id '%1'.")
                   .arg(id));
      throw e;
   }

   // Get this editor's basic block's field with the given id, making sure it exists. 
   auto i {qAsConst(_block->_fields).find(id)};
   if ( i == _block->_fields.cend() )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML edit element id '%1' does not match known block field.").arg(id));
      throw e;
   }

   // Make sure the field type matches the given type. 
   if ( i->type() != type )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML edit element points to block incorrect block field type with id '%1'.")
                   .arg(id));
      throw e;
   }

   // Return the basic block field value. 
   return *i;
}
