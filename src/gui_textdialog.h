#ifndef GUI_TEXTDIALOG_H
#define GUI_TEXTDIALOG_H
#include "gui_persistentdialog.h"
#include "gui.h"
#include "global.h"
#include "abstract.h"
//



namespace Gui
{
   /*!
    * This is a text dialog containing the the text edit class. This allows the user 
    * to edit text in its own dialog window thereby expanding it as large as 
    * preferred. This is a persistent dialog so it remembers its geometry. Because 
    * this dialog uses the text edit widget it comes with all its spell checking 
    * functionality but does not allow opening a text edit dialog since that is 
    * exactly what this class represents. Like the text edit widget this requires 
    * either a block pointer or a pointer to the custom dictionary for custom spell 
    * checking words. 
    */
   class TextDialog : public PersistentDialog
   {
      Q_OBJECT
   public:
      explicit TextDialog(Abstract::Block* block, QWidget* parent = nullptr);
      explicit TextDialog(DictionaryModel* dictionary, QWidget* parent = nullptr);
      QString text() const;
      void setText(const QString& text);
   private:
      void setupGui();
      QLayout* setupButtons();
      /*!
       * Pointer to the text edit widget of this dialog. 
       */
      TextEdit* _edit;
   };
}



#endif
