#ifndef CPPQT_TYPE_EDIT_H
#define CPPQT_TYPE_EDIT_H
#include "cppqt_type.h"
#include "gui_abstractedit.h"
//



namespace CppQt
{
   /*!
    * This is the edit class for the type block. 
    */
   class Type::Edit : public Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(Type* block);
   protected:
      virtual QLayout* layout() override final;
      virtual QString fieldTitle(int index) const override final;
   private slots:
      void textChanged(const QString& text);
   };
}



#endif
