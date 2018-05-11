#ifndef GUI_ABSTRACTEDIT_H
#define GUI_ABSTRACTEDIT_H
#include <memory>
#include "gui_persistentdialog.h"
#include "gui.h"



class QPushButton;
//



namespace Gui
{
   /*!
    * This represents a dialog for editing the data of a block. This class provides 
    * partial construction of the dialog GUI, specifically providing OK/cancel/apply 
    * buttons. An implementation of this class is responsible for handling any apply 
    * action and constructing the rest of the GUI and returning its layout. Any 
    * exception thrown when the apply interface is called this class catches and 
    * reports to the user. The buttons this class provides are placed on the bottom of 
    * the GUI and the layout an implementation returns is placed on the top. This is a 
    * persistent dialog so implementations of this class can choose to have their 
    * geometry saved by calling the save settings method from that class. The GUI of 
    * this object is not constructed in this object's constructor since it relies on 
    * an interface call as part of its construction process. Instead an initialization 
    * method must be called before this object can be used as a dialog. This dialog 
    * will also call on its block view to update the view of the block it is editing 
    * if updated. Because of this a pointer to the block view of the project the block 
    * being edited belongs to must be passed to this class. 
    */
   class AbstractEdit : public PersistentDialog
   {
      Q_OBJECT
   public:
      explicit AbstractEdit(QWidget* parent = nullptr);
      void initialize(BlockView* view);
   protected:
      /*!
       * This interface is called to return the layout of all GUI elements used by an 
       * implementation to edit its block type. All GUI objects are added to this dialog 
       * and set as their parent. 
       *
       * @return Pointer to the layout containing all GUI elements used by an 
       *         implementation. 
       */
      virtual QLayout* layout() = 0;
      /*!
       * This interface is called when the user has clicked the apply or OK buttons and 
       * expected to update the block an implementation is editing. If an exception is 
       * thrown from this interface then the user is informed and the dialog is not 
       * closed. 
       */
      virtual void apply() = 0;
      void setDisabled(bool disabled);
   private slots:
      void okClicked();
      void applyClicked();
      void cancelClicked();
   private:
      bool tryApply();
      QLayout* setupButtons();
      void setupOkButton();
      void setupApplyButton();
      QPushButton* setupCancelButton();
      /*!
       * Pointer to the block view of the project the block being edited belongs to. 
       */
      BlockView* _view;
      /*!
       * Pointer to this object's OK button. 
       */
      QPushButton* _ok;
      /*!
       * Pointer to this object's apply button. 
       */
      QPushButton* _apply;
   };
}



#endif
