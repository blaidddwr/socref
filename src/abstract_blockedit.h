#ifndef ABSTRACT_BLOCKEDIT_H
#define ABSTRACT_BLOCKEDIT_H
#include "gui_persistentdialog.h"
#include "abstract.h"



class QPushButton;
//



namespace Abstract
{
   /*!
    * This represents a dialog for editing the data of a block. This class provides 
    * partial construction of the dialog GUI and handling the events associated with 
    * what it creates. An implementation of this class is responsible for handling 
    * apply actions and constructing the rest of the GUI. This is a persistent dialog. 
    * The GUI of this object is not constructed in this object's constructor since it 
    * relies on an interface call as part of its construction process. Instead an 
    * initialization method must be called before this object can be used as a dialog. 
    * 
    * This class constructs OK, apply, and cancel buttons and handles their clicked 
    * events. An interface for applying changes to the block being edited is provided. 
    * Any exceptions thrown inside the apply interface are caught by this class and 
    * reported to the user, not closing its dialog if the OK button was clicked. 
    * Because this is a persistent dialog an implementation of this class must define 
    * its own unique key using the save settings method of the persistent dialog 
    * class. 
    * 
    * An implementation of this class must construct the form portion of this dialog 
    * giving user access to the data of the block being edited. This is accomplished 
    * through another interface that returns a layout of the form portion. The form 
    * portion is positioned at the top of the dialog while the buttons this class 
    * creates is at the bottom.  
    */
   class BlockEdit : public Gui::PersistentDialog
   {
      Q_OBJECT
   public:
      explicit BlockEdit(Block* block, QWidget* parent = nullptr);
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
      /*!
       * This interface is called when the user has clicked the apply or OK button and is 
       * expected to update the block it is editing with the editor dialog values. If an 
       * exception is thrown from this interface then the user is informed and the dialog 
       * is not closed. 
       */
      virtual void apply() = 0;
   protected:
      void setDisabled(bool disabled);
      template<class T> T& block();
   private slots:
      void okClicked();
      void applyClicked();
   private:
      bool tryApply();
      QLayout* setupButtons();
      /*!
       * Pointer to this object's OK button. 
       */
      QPushButton* _ok;
      /*!
       * Pointer to this object's apply button. 
       */
      QPushButton* _apply;
      /*!
       * The block that this widget edits. 
       */
      Block* _block;
   };






   /*!
    * Returns a reference to the block this widget edits, type casted to the specific 
    * block type of this widget. It is assumed the type given can be type cast 
    * successfully. 
    *
    * @tparam T Block type to return as a reference. 
    *
    * @return Reference to the block this widget edits. 
    */
   template<class T> T& BlockEdit::block()
   {
      // Type cast this object's block to the given block type and make sure it worked. 
      T* ret {qobject_cast<T*>(_block)};
      Q_CHECK_PTR(ret);
   
      // Return a reference to this object's type casted block. 
      return *ret;
   }
}



#endif
