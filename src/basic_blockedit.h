#ifndef BASIC_BLOCKEDIT_H
#define BASIC_BLOCKEDIT_H
#include "basic_block.h"
#include "abstract_blockedit.h"
class QLayout;
class QSplitter;
class QCheckBox;



/*!
 * This contains all classes that implement the block and parsing system with a
 * basic foundation that can be used by specific project types.
 */
namespace Basic
{
   /*!
    * This is the basic block editor that implements the abstract edit GUI class.
    * Like the basic block it edits this class reads in its configuration from an
    * XML element supplied to it from its basic block. The XML definition provides
    * all information about the layout of the form and what fields edit widgets
    * control. This class cannot be implemented by another more specialized class
    * because all data is designed to be on the basic block layer and this editor
    * must be aware of all data in the basic block it edits.
    *
    * The layout of this editor is defined by the XML element between different
    * layout widgets contained within a qt splitter. Each layout added to the
    * splitter can either be a form layout or a vertical layout. The state of the
    * splitter is persistent along with the editor's dialog geometry.
    */
   class BlockEdit : public Abstract::BlockEdit
   {
      Q_OBJECT
   public:
      explicit BlockEdit(Block* block);
   protected:
      virtual QLayout* layout() override final;
      virtual void apply() override final;
      virtual void closeEvent(QCloseEvent* event) override final;
   private:
      static QString extractId(const QDomElement& element);
   private:
      /*!
       * The tag name for a form element used to define a column of this editor using
       * a form layout.
       */
      static const QString _formTag;
      /*!
       * The tag name for a layout element used to define a column of this editor
       * using a vertical layout.
       */
      static const QString _layoutTag;
      /*!
       * The attribute name for the display string used as the title of an edit
       * element.
       */
      static const QString _displayKey;
      /*!
       * The tag name for a check boxes element used to define a check boxes edit
       * widget.
       */
      static const QString _checkboxesTag;
      /*!
       * The tag name for a checkbox element used to define a single checkbox within a
       * check boxes element.
       */
      static const QString _checkboxTag;
      /*!
       * The tag name for a combo box element used to define a combo box widget.
       */
      static const QString _comboTag;
      /*!
       * The tag name for an option element nested within combo elements used to
       * define a value of a combo box widget.
       */
      static const QString _optionTag;
      /*!
       * The tag name for a line edit element used to define a line edit widget.
       */
      static const QString _lineTag;
      /*!
       * The tag name for a text edit element used to define a text edit widget.
       */
      static const QString _textTag;
      /*!
       * The tag name of a list edit element used to define a list edit widget.
       */
      static const QString _listTag;
      /*!
       * The attribute name for the row size of a check boxes edit element.
       */
      static const QString _rowSizeKey;
      /*!
       * The attribute name for the value of an option element nested within a combo
       * element.
       */
      static const QString _valueKey;
      /*!
       * The attribute name for the option of disabling spell checking on a text edit
       * widget if the attribute is set to no.
       */
      static const QString _spellKey;
      /*!
       * The attribute name for the option of disabling the popup dialog on a text
       * edit widget if the attribute is set to no.
       */
      static const QString _dialogKey;
   private:
      void saveSettings(const QString& baseKey);
      QWidget* setupForm(const QDomElement& element);
      QWidget* setupLayout(const QDomElement& element);
      void addWidgets(QLayout* layout, const QDomElement& element);
      void addCheckBoxes(QLayout* layout, const QDomElement& element);
      void addComboBox(QLayout* layout, const QDomElement& element);
      void addLineEdit(QLayout* layout, const QDomElement& element);
      void addTextEdit(QLayout* layout, const QDomElement& element);
      void addListEdit(QLayout* layout, const QDomElement& element);
      QCheckBox* setupCheckBox(const QString& id, const QString& title);
      void add(QLayout* layout, const QDomElement& element, QWidget* item);
      void add(QLayout* layout, const QDomElement& element, QLayout* item);
      QVariant fieldValue(const QString& id, quint32 type);
   private:
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
}

#endif
