#ifndef CPPQT_FUNCTION_EDIT_H
#define CPPQT_FUNCTION_EDIT_H
#include "cppqt_function.h"
#include "cppqt_gui.h"
#include "gui_abstractedit.h"



class QSplitter;
//



namespace CppQt
{
   /*!
    * This is the edit class for the function block. Because this class is inherited 
    * by any other function type block for this project type it provides additional 
    * interfaces to handle inherited classes. Most notably it replaces the layout 
    * interface with its own left layout interface. The layout of this edit class is 
    * also different as a result of redefining the layout interface. 
    * 
    * Any edit class that inherits from this will always have a list edit widget on 
    * the right side of the dialog. The left side is defined by the new interface left 
    * layout. Both sides are separated by a qt splitter whose state is persistent. 
    * 
    * The persistence of the qt splitter is made unique by this class overriding the 
    * save settings method from the persistent dialog class. This allows the 
    * persistence to be unique for all inherited classes and tying that persistence in 
    * with its base persistent dialog class. 
    */
   class Function::Edit : public ::Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(Function* block);
   protected:
      virtual QLayout* leftLayout();
      virtual QLayout* layout() override final;
      virtual void apply() override;
      virtual QString fieldTitle(int index) const override final;
      virtual void closeEvent(QCloseEvent* event) override final;
   protected:
      void saveSettings(const char* baseKey);
      void addReturnEdit(QFormLayout* form);
   private:
      QWidget* rightLayout();
      /*!
       * The function block that this edit dialog is editing. 
       */
      Function* _block;
      /*!
       * This object's the operations list edit widget for this editing the operations 
       * field. 
       */
      ::Gui::ListEdit* _operationsEdit;
      /*!
       * This object's type edit widget for editing the type field. 
       */
      Gui::TypeSelection* _returnEdit {nullptr};
      /*!
       * The qt splitter of this edit dialog that splits the operations edit list from 
       * the rest of the edit GUI elements. 
       */
      QSplitter* _splitter;
      /*!
       * The geometry key for this edit dialog used to keep its dialog geometry 
       * persistent. 
       */
      QByteArray _geometryKey;
      /*!
       * The state key for this edit dialog used to keep its qt splitter persistent. 
       */
      QByteArray _stateKey;
   };
}



#endif
