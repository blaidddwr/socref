#ifndef BASICBLOCK_EDIT_H
#define BASICBLOCK_EDIT_H
#include "basicblock.h"
#include "gui_abstractedit.h"



class QLayout;
class QSplitter;
class QCheckBox;
//



/*!
 * This is the basic block editor that implements the abstract edit GUI class. Like 
 * the basic block it edits this class reads in its configuration from an XML 
 * element supplied to it from its basic block. The XML definition provides all 
 * information about the layout of the form and what fields edit widgets control. 
 * This class cannot be implemented by another more specialized class because all 
 * data is designed to be on the basic block layer and this editor must be aware of 
 * all data in the basic block it edits. 
 * 
 * The layout of this editor is defined by the XML element between different layout 
 * widgets contained within a qt splitter. Each layout added to the splitter can 
 * either be a form layout or a vertical layout. The state of the splitter is 
 * persistent along with the editor's dialog geometry. 
 */
class BasicBlock::Edit : public Gui::AbstractEdit
{
   Q_OBJECT
public:
   explicit Edit(BasicBlock* block);
protected:
   virtual QLayout* layout() override final;
   virtual void apply();
   virtual void closeEvent(QCloseEvent* event) override final;
private:
   static QString extractId(const QDomElement& element);
   /*!
    * The tag name for a form element used to define a column of this editor using a 
    * form layout. 
    */
   static const char* _formTag;
   /*!
    * The tag name for a layout element used to define a column of this editor using a 
    * vertical layout. 
    */
   static const char* _layoutTag;
   /*!
    * The attribute name for the id used to reference what data field an edit element 
    * edits. 
    */
   static const char* _idKey;
   /*!
    * The attribute name for the display string used as the title of an edit element. 
    */
   static const char* _displayKey;
   /*!
    * The tag name for a check boxes element used to define a check boxes edit widget. 
    */
   static const char* _checkboxesTag;
   /*!
    * The tag name for a checkbox element used to define a single checkbox within a 
    * check boxes element. 
    */
   static const char* _checkboxTag;
   /*!
    * The tag name for a line edit element used to define a line edit widget. 
    */
   static const char* _lineTag;
   /*!
    * The tag name for a text edit element used to define a text edit widget. 
    */
   static const char* _textTag;
   /*!
    * The tag name of a list edit element used to define a list edit widget. 
    */
   static const char* _listTag;
   /*!
    * The tag name for a type selection widget used to define a type selection edit 
    * widget. 
    */
   static const char* _typeTag;
   /*!
    * The attribute name for the row size of a check boxes edit element. 
    */
   static const char* _rowSizeKey;
private:
   void saveSettings(const QString& baseKey);
   QWidget* setupForm(const QDomElement& element);
   QWidget* setupLayout(const QDomElement& element);
   void addWidgets(QLayout* layout, const QDomElement& element);
   void addCheckBoxes(QLayout* layout, const QDomElement& element);
   void addLineEdit(QLayout* layout, const QDomElement& element);
   void addTextEdit(QLayout* layout, const QDomElement& element);
   void addListEdit(QLayout* layout, const QDomElement& element);
   void addTypeEdit(QLayout* layout, const QDomElement& element);
   QCheckBox* setupCheckBox(const QString& id, const QString& title);
   void add(QLayout* layout, const QDomElement& element, QWidget* item);
   void add(QLayout* layout, const QDomElement& element, QLayout* item);
   QVariant fieldValue(const QString& id, quint32 type);
   /*!
    * The basic block that this editor is editing. 
    */
   BasicBlock* _block;
   /*!
    * The qt splitter of this edit dialog that splits each single layout column 
    * between one another. 
    */
   QSplitter* _splitter;
   /*!
    * The list of edit widgets this editor contains. 
    */
   QMap<QString,QWidget*> _widgets;
   /*!
    * The state key for this edit dialog used to keep its qt splitter persistent. 
    */
   QString _stateKey;
};



#endif
