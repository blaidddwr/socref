#include "basicblock_edit.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include "gui_textedit.h"
#include "gui_listedit.h"



//






/*!
 *
 * @param block  
 */
BasicBlock::Edit::Edit(BasicBlock* block)
{}






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
{}






/*!
 *
 * @param element  
 */
QString BasicBlock::Edit::checkId(const QDomElement& element)
{
   const QString id {element.attribute("id")};
   if ( id.isEmpty() )
   {
      ;//ERR
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
   static const QStringList list {"checkboxes","line","text","list","menu"};
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
      ;//ERR
   }
   if ( rowSize < 0 )
   {
      ;//ERR
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
{}






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
      ;//ERR
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
      ;//ERR
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
      ;//ERR
   }
   auto i {qAsConst(_block->_fields).find(id)};
   if ( i == _block->_fields.cend() )
   {
      ;//ERR
   }
   if ( i->type() != type )
   {
      ;//ERR
   }
   return *i;
}
