#ifndef GUI_DICTIONARYDIALOG_H
#define GUI_DICTIONARYDIALOG_H
#include "gui_persistentdialog.h"
#include "global.h"



class QListView;
//



namespace Gui
{
   /*!
    * This is the project dictionary dialog. This displays the list of all custom 
    * dictionary words contained in the project it is given. This also allows the user 
    * to remove words in the list, but does not allow the user to add new words. 
    * Adding new words is done through the spell checking dialog interface. 
    */
   class DictionaryDialog : public PersistentDialog
   {
      Q_OBJECT
   public:
      DictionaryDialog(Project* project);
   private slots:
      void removeTriggered();
   private:
      void setupGui();
      QLayout* setupButtons();
      /*!
       * The List view widget of this dictionary dialog that displays the list of custom 
       * dictionary words. 
       */
      QListView* _view;
      /*!
       * The custom dictionary model of this dialog that it displays and edits. 
       */
      DictionaryModel* _model;
   };
}



#endif
