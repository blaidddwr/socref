#ifndef CPPQT_GUI_TYPESELECTION_H
#define CPPQT_GUI_TYPESELECTION_H
#include <QPushButton>
#include "cppqt.h"
#include "global.h"
//



namespace CppQt
{
   namespace Gui
   {
      /*!
       * This is the type selection widget. This expanded push button widget provides a 
       * menu with all available C++ types parsed from a given block. Any type lists are 
       * grabbed from any parent block above the block given and added to the menu of 
       * this widget. Type lists are recursively transformed into sub menus. 
       */
      class TypeSelection : public QPushButton
      {
         Q_OBJECT
      public:
         explicit TypeSelection(AbstractBlock* block, QWidget* parent = nullptr);
         QString value() const;
         void setValue(const QString& value);
      private slots:
         void selectionTriggered(int index);
      private:
         QMenu* buildRootMenu(AbstractBlock* block);
         QMenu* buildMenu(TypeList* block);
         void addType(QMenu* menu, Type* type);
         /*!
          * List of all possible types the menu of this widget allows the user to select. 
          */
         QStringList _types;
         /*!
          * The current C++ type value for this selection widget. 
          */
         QString _value;
      };
   }
}



#endif
