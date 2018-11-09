#include "basicblock_edit.h"
#include <QJsonObject>
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



using namespace Sut;
//






/*!
 *
 * @param block  
 */
BasicBlock::Edit::Edit(BasicBlock* block):
   _block(block)
{
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
   QHBoxLayout* ret {new QHBoxLayout};
   enum {Form,Layout};
   static const QStringList list {"form","layout"};
   QDomNode node {_block->_editDefinition.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement element {node.toElement()};
         switch (list.indexOf(element.tagName()))
         {
         case Form:
            ret->addLayout(setupForm(element));
            break;
         case Layout:
            ret->addLayout(setupLayout(element));
            break;
         }
      }
      node = node.nextSibling();
   }
   return ret;
}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 */
void BasicBlock::Edit::apply()
{
   auto throwTypeMismatch = [](const QString& key)
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Edit widget points to block field id '%1' with incorrect type.")
                   .arg(key));
      throw e;
   };
   for (auto i = _widgets.cbegin(); i != _widgets.cend() ;++i)
   {
      auto j {_block->_fields.find(i.key())};
      if ( j == _block->_fields.end() )
      {
         Exception::LogicError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Edit widget points to unkonwn block field id '%1'.").arg(i.key()));
         throw e;
      }
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
      else
      {
         Exception::LogicError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Edit widget of unknown type encountered."));
         throw e;
      }
   }
}






/*!
 *
 * @param element  
 */
QString BasicBlock::Edit::checkId(const QDomElement& element)
{
   const QString id {element.attribute("id")};
   if ( id.isEmpty() )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML edit element does not contain valid id attribute."));
      throw e;
   }
   return id;
}






/*!
 *
 * @param element  
 */
QLayout* BasicBlock::Edit::setupForm(const QDomElement& element)
{
   QFormLayout* ret {new QFormLayout};
   addWidgets(ret,element);
   return ret;
}






/*!
 *
 * @param element  
 */
QLayout* BasicBlock::Edit::setupLayout(const QDomElement& element)
{
   QVBoxLayout* ret {new QVBoxLayout};
   addWidgets(ret,element);
   return ret;
}






/*!
 *
 * @param layout  
 *
 * @param element  
 */
void BasicBlock::Edit::addWidgets(QLayout* layout, const QDomElement& element)
{
   enum {CheckBoxes,LineEdit,TextEdit,ListEdit,TypeEdit};
   static const QStringList list {"checkboxes","line","text","list","type"};
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
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
      node = node.nextSibling();
   }
}






/*!
 *
 * @param layout  
 *
 * @param element  
 */
void BasicBlock::Edit::addCheckBoxes(QLayout* layout, const QDomElement& element)
{
   bool ok;
   int rowSize {element.attribute("rowsize","1").toInt(&ok)};
   if ( !ok )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML checkboxes edit element does not contain valid rowsize attribute."));
      throw e;
   }
   if ( rowSize < 0 )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML checkboxes edit element contains negative rowsize."));
      throw e;
   }
   QGridLayout* grid {new QGridLayout};
   int row {0};
   int column {0};
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement child {node.toElement()};
         if ( child.tagName() == QStringLiteral("checkbox") )
         {
            grid->addWidget(setupCheckBox(checkId(child),child.attribute("display")),row++,column);
            if ( row >= rowSize )
            {
               row = 0;
               ++column;
            }
         }
      }
      node = node.nextSibling();
   }
   add(layout,element,grid);
}






/*!
 *
 * @param layout  
 *
 * @param element  
 */
void BasicBlock::Edit::addLineEdit(QLayout* layout, const QDomElement& element)
{
   QString id {checkId(element)};
   QVariant field {check(id,QVariant::String)};
   QLineEdit* edit {new QLineEdit(field.toString())};
   add(layout,element,edit);
}






/*!
 *
 * @param layout  
 *
 * @param element  
 */
void BasicBlock::Edit::addTextEdit(QLayout* layout, const QDomElement& element)
{
   QString id {checkId(element)};
   QVariant field {check(id,QVariant::String)};
   Gui::TextEdit* edit {new Gui::TextEdit(_block)};
   edit->setPlainText(field.toString());
   add(layout,element,edit);
}






/*!
 *
 * @param layout  
 *
 * @param element  
 */
void BasicBlock::Edit::addListEdit(QLayout* layout, const QDomElement& element)
{
   QString id {checkId(element)};
   QVariant field {check(id,QVariant::StringList)};
   Gui::ListEdit* edit {new Gui::ListEdit(_block)};
   edit->setValue(field.toStringList());
   add(layout,element,edit);
}






/*!
 *
 * @param layout  
 *
 * @param element  
 */
void BasicBlock::Edit::addTypeEdit(QLayout* layout, const QDomElement& element)
{
   QString id {checkId(element)};
   QVariant field {check(id,QVariant::String)};
   Gui::TypeSelection* edit {new Gui::TypeSelection(_block->typeList())};
   edit->setValue(field.toString());
   add(layout,element,edit);
}






/*!
 *
 * @param id  
 *
 * @param title  
 */
QCheckBox* BasicBlock::Edit::setupCheckBox(const QString& id, const QString& title)
{
   QVariant field {check(id,QVariant::Bool)};
   QCheckBox* ret {new QCheckBox(title)};
   ret->setChecked(field.toBool());
   _widgets.insert(id,ret);
   return ret;
}






/*!
 *
 * @param layout  
 *
 * @param element  
 *
 * @param item  
 */
void BasicBlock::Edit::add(QLayout* layout, const QDomElement& element, QWidget* item)
{
   if ( QFormLayout* valid = qobject_cast<QFormLayout*>(layout) )
   {
      valid->addRow(new QLabel(element.attribute("display"),this),item);
   }
   else if ( QVBoxLayout* valid = qobject_cast<QVBoxLayout*>(layout) )
   {
      valid->addWidget(new QLabel(element.attribute("display"),this));
      valid->addWidget(item);
   }
   else
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Given layout is of unsupported type."));
      throw e;
   }
}






/*!
 *
 * @param layout  
 *
 * @param element  
 *
 * @param item  
 */
void BasicBlock::Edit::add(QLayout* layout, const QDomElement& element, QLayout* item)
{
   if ( QFormLayout* valid = qobject_cast<QFormLayout*>(layout) )
   {
      valid->addRow(new QLabel(element.attribute("display"),this),item);
   }
   else if ( QVBoxLayout* valid = qobject_cast<QVBoxLayout*>(layout) )
   {
      valid->addWidget(new QLabel(element.attribute("display"),this));
      valid->addLayout(item);
   }
   else
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Given layout is of unsupported type."));
      throw e;
   }
}






/*!
 *
 * @param id  
 *
 * @param type  
 */
QVariant BasicBlock::Edit::check(const QString& id, quint32 type)
{
   if ( _widgets.contains(id) )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML edit element overrides already existing edit widget with id '%1'.")
                   .arg(id));
      throw e;
   }
   auto i {qAsConst(_block->_fields).find(id)};
   if ( i == _block->_fields.cend() )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML edit element id '%1' does not match known block field.").arg(id));
      throw e;
   }
   if ( i->type() != type )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("XML edit element points to block incorrect block field type with id '%1'.")
                   .arg(id));
      throw e;
   }
   return *i;
}
