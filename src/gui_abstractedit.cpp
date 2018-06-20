#include "gui_abstractedit.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <exception.h>
#include "gui_blockview.h"
#include "gui_textedit.h"
#include "gui_listedit.h"
#include "abstractblock.h"
#include "common.h"



using namespace Gui;
//






/*!
 * Constructs a new abstract edit object with the given block and an optional 
 * parent. The new abstract edit is not initialized and not ready to be used as a 
 * dialog until that method is called. 
 *
 * @param block Pointer to the block this dialog is editing. 
 *
 * @param parent Optional parent for this new abstract edit dialog. 
 */
AbstractEdit::AbstractEdit(AbstractBlock* block, QWidget* parent):
   PersistentDialog(nullptr,parent),
   _block(block)
{}






/*!
 * Constructs and initializes the GUI for this abstract edit dialog, making it 
 * ready to be used as a GUI dialog window. 
 */
void AbstractEdit::initialize()
{
   // Setup this object's buttons saving the returned layout to _buttons_ and then 
   // create a new vertical box layout _main_. 
   QLayout* buttons {setupButtons()};
   QVBoxLayout* main {new QVBoxLayout};

   // Add the implementation's layout to _main_ by calling the layout interface, then 
   // add the layout _buttons_, and then set this dialog's layout to _main_. 
   main->addLayout(layout());
   main->addLayout(buttons);
   setLayout(main);
}






/*!
 * This interface is called when the user has clicked the apply or OK buttons and 
 * expected to update the block an implementation is editing. If an exception is 
 * thrown from this interface then the user is informed and the dialog is not 
 * closed. The default implementation applies all edit widgets added to this object 
 * during construction. 
 */
void AbstractEdit::apply()
{
   // Iterate through all of this object's edit widgets. For each one apply the value 
   // the edit widget contains to the corresponding field of the block this dialog is 
   // editing. 
   for (auto i = _edits.cbegin(); i != _edits.cend() ;++i)
   {
      switch (_block->fieldType(i.key()))
      {
      case AbstractBlock::Field::Boolean:
         if ( QCheckBox* valid = qobject_cast<QCheckBox*>(*i) )
         {
            _block->setField(i.key(),valid->isChecked());
         }
         break;
      case AbstractBlock::Field::String:
         if ( QLineEdit* valid = qobject_cast<QLineEdit*>(*i) )
         {
            _block->setField(i.key(),valid->text());
         }
         else if ( TextEdit* valid = qobject_cast<TextEdit*>(*i) )
         {
            _block->setField(i.key(),valid->toPlainText());
         }
         else if ( QComboBox* valid = qobject_cast<QComboBox*>(*i) )
         {
            _block->setField(i.key(),valid->currentText());
         }
         break;
      case AbstractBlock::Field::StringList:
         if ( ListEdit* valid = qobject_cast<ListEdit*>(*i) )
         {
            _block->setField(i.key(),valid->value());
         }
         break;
      }
   }
}






/*!
 * Adds a new grid of checkbox edit widgets for the given fields to the given form 
 * layout using the given title and column size. If anything is wrong with any of 
 * the given field indexes then an exception is thrown. 
 *
 * @param form Pointer to the form layout which has the grid of checkbox widgets 
 *             added to it. 
 *
 * @param fields List of field indexes which have checkbox edit widgets added to 
 *               them. 
 *
 * @param columnSize The column size for the checkbox grid layout. 
 *
 * @param title Title used when adding the grid layout to the given form layout. 
 */
void AbstractEdit::addCheckBoxes(QFormLayout* form, const QList<int>& fields, int columnSize, const QString& title)
{
   // Create a new grid layout _layout_ and initialize the row and column indexes. 
   QGridLayout* layout {new QGridLayout};
   int row {0};
   int column {0};

   // Iterate through all given fields, adding a new checkbox for each one to 
   // _layout_ and increment the column index. If column index reaches the given size 
   // then reset to 0 and increment the row index. 
   for (auto field: fields)
   {
      layout->addWidget(addCheckBox(field),column++,row);
      if ( column >= columnSize )
      {
         column = 0;
         ++row;
      }
   }

   // Add _layout_ to the given form layout using the given title. 
   form->addRow(new QLabel(title,this),layout);
}






/*!
 * Adds a new checkbox edit widget to the block field with the given index, 
 * returning its pointer. If this edit dialog already contains an edit widget for 
 * this field, the index is out of range, or the field type is not a boolean then 
 * an exception is thrown. 
 *
 * @param index Index of the field that has an edit widget attached to it. 
 *
 * @return Pointer to the new checkbox edit widget attached to the block field with 
 *         the given index. 
 */
QCheckBox* AbstractEdit::addCheckBox(int index)
{
   // Check the given field index can be added. If the field type for the given index 
   // is not a boolean then throw an exception. 
   checkField(index);
   if ( _block->fieldType(index) != AbstractBlock::Field::Boolean )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set checkbox widget for field %1 when it is not a boolean type.")
                   .arg(index));
      throw e;
   }

   // Create a new checkbox edit widget with the field title of the given index, 
   // setting its value to the current value of the block, inserting it into this 
   // object's list edit widgets, and returning its pointer. 
   QCheckBox* ret {new QCheckBox(fieldTitle(index),this)};
   ret->setChecked(_block->field(index).toBool());
   _edits.insert(index,ret);
   return ret;
}






/*!
 * Adds a new line edit widget to the block field with the given index, returning 
 * its pointer. If this edit dialog already contains an edit widget for this field, 
 * the index is out of range, or the field type is not a string then an exception 
 * is thrown. 
 *
 * @param form Pointer to the form layout which has the new edit widget added to 
 *             it. 
 *
 * @param index Index of the field that has an edit widget attached to it. 
 *
 * @return Pointer to the new line edit widget attached to the block field with the 
 *         given index. 
 */
QLineEdit* AbstractEdit::addLineEdit(QFormLayout* form, int index)
{
   // Check the given field index can be added. If the field type for the given index 
   // is not a string then throw an exception. 
   checkField(index);
   if ( _block->fieldType(index) != AbstractBlock::Field::String )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set line edit widget for field %1 when it is not a string type.")
                   .arg(index));
      throw e;
   }

   // Create a new line edit widget with the field title of the given index, setting 
   // its value to the current value of the block, inserting it into this object's 
   // list edit widgets, and returning its pointer. 
   QLineEdit* ret {new QLineEdit(this)};
   ret->setText(_block->field(index).toString());
   _edits.insert(index,ret);
   form->addRow(new QLabel(fieldTitle(index),this),ret);
   return ret;
}






/*!
 * Adds a new text edit widget to the block field with the given index, returning 
 * its pointer. If this edit dialog already contains an edit widget for this field, 
 * the index is out of range, or the field type is not a string then an exception 
 * is thrown. 
 *
 * @param form Pointer to the form layout which has the new edit widget added to 
 *             it. 
 *
 * @param index Index of the field that has an edit widget attached to it. 
 *
 * @return Pointer to the new text edit widget attached to the block field with the 
 *         given index. 
 */
TextEdit* AbstractEdit::addTextEdit(QFormLayout* form, int index)
{
   // Check the given field index can be added. If the field type for the given index 
   // is not a string then throw an exception. 
   checkField(index);
   if ( _block->fieldType(index) != AbstractBlock::Field::String )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set text edit widget for field %1 when it is not a string type.")
                   .arg(index));
      throw e;
   }

   // Create a new text edit widget with the field title of the given index, setting 
   // its value to the current value of the block, inserting it into this object's 
   // list edit widgets, and returning its pointer. 
   TextEdit* ret {new TextEdit(this)};
   ret->setPlainText(_block->field(index).toString());
   _edits.insert(index,ret);
   form->addRow(new QLabel(fieldTitle(index),this),ret);
   return ret;
}






/*!
 * Adds a new list edit widget to the block field with the given index, returning 
 * its pointer. If this edit dialog already contains an edit widget for this field, 
 * the index is out of range, or the field type is not a string list then an 
 * exception is thrown. 
 *
 * @param form Pointer to the form layout which has the new edit widget added to 
 *             it. 
 *
 * @param index Index of the field that has an edit widget attached to it. 
 *
 * @return Pointer to the new list edit widget attached to the block field with the 
 *         given index. 
 */
ListEdit* AbstractEdit::addListEdit(QFormLayout* form, int index)
{
   // Check the given field index can be added. If the field type for the given index 
   // is not a string list then throw an exception. 
   checkField(index);
   if ( _block->fieldType(index) != AbstractBlock::Field::StringList )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set list edit widget for field %1 when it is not a string list type.")
                   .arg(index));
      throw e;
   }

   // Create a new list edit widget with the field title of the given index, setting 
   // its value to the current value of the block, inserting it into this object's 
   // list edit widgets, and returning its pointer. 
   ListEdit* ret {new ListEdit(this)};
   ret->setValue(_block->field(index).toStringList());
   _edits.insert(index,ret);
   form->addRow(new QLabel(fieldTitle(index),this),ret);
   return ret;
}






/*!
 *
 * @param form Pointer to the form layout which has the new edit widget added to 
 *             it. 
 *
 * @param index Index of the field that has an edit widget attached to it. 
 *
 * @param options  
 */
QComboBox* AbstractEdit::addComboEdit(QFormLayout* form, int index, const QStringList& options)
{
   checkField(index);
   if ( _block->fieldType(index) != AbstractBlock::Field::String )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set combo box widget for field %1 when it is not a string type.")
                   .arg(index));
      throw e;
   }
   QComboBox* ret {new QComboBox(this)};
   for (auto option: options) ret->addItem(option);
   ret->setCurrentIndex(ret->findText(_block->field(index).toString()));
   _edits.insert(index,ret);
   form->addRow(new QLabel(fieldTitle(index),this),ret);
   return ret;
}






/*!
 * Sets the state of the apply and OK buttons of this dialog to being disabled or 
 * enabled. 
 *
 * @param disabled True to disable apply and OK buttons or false to enable them. 
 */
void AbstractEdit::setDisabled(bool disabled)
{
   // Set the disabled state of this object's apply and OK buttons based off the 
   // given state. 
   _ok->setDisabled(disabled);
   _apply->setDisabled(disabled);
}






/*!
 * Called when this object's OK button is clicked. This tries to apply changes to 
 * the implementation's block and if successful closes this dialog. 
 */
void AbstractEdit::okClicked()
{
   if ( tryApply() ) done(QDialog::Accepted);
}






/*!
 * Called when this object's apply button is clicked. This tries to apply changes 
 * to the implementation's block and if successful updates the block's view. 
 */
void AbstractEdit::applyClicked()
{
   tryApply();
}






/*!
 * Constructs the three buttons this abstract edit object provides to its 
 * implementation, returning the horizontal layout containing the setup buttons. 
 *
 * @return Horizontal layout containing this object's three buttons. 
 */
QLayout* AbstractEdit::setupButtons()
{
   // Create a new push button, setting it to this object's OK button and connecting 
   // its clicked signal. 
   _ok = new QPushButton(tr("&Ok"));
   connect(_ok,&QPushButton::clicked,this,&AbstractEdit::okClicked);

   // Create a new push button, setting it to this object's apply button and 
   // connecting its clicked signal. 
   _apply = new QPushButton(tr("&Apply"));
   connect(_apply,&QPushButton::clicked,this,&AbstractEdit::applyClicked);

   // Create a new push button _cancel_, connecting its clicked signal. 
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(cancel,&QPushButton::clicked,[this]{ done(QDialog::Rejected); });

   // Create a new horizontal layout _ret_, adding the OK button, apply button, 
   // stretch, and _cancel_ button in that order. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_ok);
   ret->addWidget(_apply);
   ret->addStretch();
   ret->addWidget(cancel);

   // Return _ret_. 
   return ret;
}






/*!
 * Checks the given field index and makes sure it is valid and this dialog does not 
 * already contain an edit widget for it, throwing an exception if either check 
 * fails. 
 *
 * @param index  
 */
void AbstractEdit::checkField(int index)
{
   if ( _edits.contains(index) )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Edit dialog already contains field index %1.").arg(index));
      throw e;
   }
   if ( index < 0 || index >= _block->fieldSize() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Field index %1 is out of range (%2 total).")
                   .arg(index)
                   .arg(_block->fieldSize()));
      throw e;
   }
}






/*!
 * Calls this object's apply interface, returning true if it was successful or 
 * false if it caught an exception. 
 *
 * @return True is the implementation applied changes successfully or false if an 
 *         exception was thrown. 
 */
bool AbstractEdit::tryApply()
{
   // Call this object's apply interface and return true on success. 
   try
   {
      apply();
      return true;
   }

   // If an exception is caught then report the error to the user and return false on 
   // failure. 
   catch (Exception::Base e)
   {
      showException(tr("An error occured while attempting to save changes to this block."),e);
      return false;
   }
}
