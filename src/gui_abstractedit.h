#ifndef GUI_ABSTRACTEDIT_H
#define GUI_ABSTRACTEDIT_H
#include <memory>
#include <QMap>
#include "gui_persistentdialog.h"
#include "gui.h"
#include "global.h"



class QFormLayout;
class QPushButton;
class QCheckBox;
class QLineEdit;
//



namespace Gui
{
   /*!
    * This represents a dialog for editing the data of a block. This class provides 
    * partial construction of the dialog GUI and handling the events associated with 
    * what it creates. An implementation of this class is responsible for handling any 
    * special apply actions and constructing the rest of the GUI. This is a persistent 
    * dialog. The GUI of this object is not constructed in this object's constructor 
    * since it relies on an interface call as part of its construction process. 
    * Instead an initialization method must be called before this object can be used 
    * as a dialog. 
    * 
    * This class constructs OK, apply, and cancel buttons and handles their clicked 
    * events. An interface for applying changes to the block being edited is provided 
    * and an implementation must implement any special apply actions. Any exceptions 
    * thrown inside the apply interface are caught by this class and reported to the 
    * user, not closing its dialog if the OK button was clicked. Because this is a 
    * persistent dialog an implementation of this class must define its own unique key 
    * using the save settings method of the persistent dialog class. 
    * 
    * This class provides methods for adding edit widgets for one of the block fields 
    * of the block it edits. These methods will add a new row to a given form and 
    * return a pointer to the edit widget for a given block field. This abstract class 
    * itself handles applying any fields being edited in this fashion. Only one edit 
    * widget can be used for each unique block field. 
    * 
    * An implementation of this class must construct the form portion of this dialog 
    * giving user access to the data of the block being edited. This is accomplished 
    * through another interface that returns a layout of the form portion that an 
    * implementation must implement. The form portion is positioned at the top of the 
    * dialog while the buttons this class creates is at the bottom.  
    */
   class AbstractEdit : public PersistentDialog
   {
      Q_OBJECT
   public:
      explicit AbstractEdit(AbstractBlock* block, QWidget* parent = nullptr);
      void initialize();
   protected:
      /*!
       * This interface is called to return the layout of all GUI elements used by this 
       * dialog object's form layout. All GUI objects are added to this dialog and set as 
       * their parent. 
       *
       * @return Pointer to the layout containing all GUI elements for this dialog. 
       */
      virtual QLayout* layout() = 0;
      virtual void apply();
      /*!
       * This interface returns the title for the given field type. The title is 
       * displayed with any default form added through this base class's methods. 
       *
       * @param index  
       *
       * @return Title of the given field type. 
       */
      virtual QString fieldTitle(int index) const = 0;
   protected:
      void addCheckBoxes(QFormLayout* form, const QList<int>& fields, int columnSize, const QString& title);
      QCheckBox* addCheckBox(int index);
      QLineEdit* addLineEdit(QFormLayout* form, int index);
      TextEdit* addTextEdit(QFormLayout* form, int index);
      void setDisabled(bool disabled);
   private slots:
      void okClicked();
      void applyClicked();
      void cancelClicked();
   private:
      QLayout* setupButtons();
      void checkField(int index);
      bool tryApply();
      /*!
       * Pointer to the block that this dialog is editing. 
       */
      AbstractBlock* _block;
      /*!
       * Pointer to this object's OK button. 
       */
      QPushButton* _ok;
      /*!
       * Pointer to this object's apply button. 
       */
      QPushButton* _apply;
      /*!
       * Mapping of pointers to all edit widgets that have been added to this edit 
       * dialog. The key represents the field index the corresponding edit widget is 
       * attached to. 
       */
      QMap<int,QWidget*> _edits;
   };
}



#endif
