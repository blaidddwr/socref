#ifndef CPPQT_TYPE_GUI_COMBOBOX_H
#define CPPQT_TYPE_GUI_COMBOBOX_H
#include <memory>
#include <QWidget>
#include "cppqt_classes.h"



class QComboBox;
class QSpinBox;



namespace CppQt
{
   namespace Type
   {
      namespace Gui
      {
         class ComboBox : public QWidget
         {
            Q_OBJECT
         public:
            explicit ComboBox(const QList<AbstractType*>& list, int depth = -1, QWidget* parent = nullptr);
            std::unique_ptr<AbstractType> makeCurrentType() const;
            ComboBox& setList(const QList<AbstractType*>& list, int depth = -1);
         private slots:
            void depthChanged(int depth);
         private:
            void update();
            void createSpinner();
            QList<AbstractType*> _list;
            int _depth;
            QComboBox* _combo;
            QSpinBox* _spinner;
         };
      }
   }
}



#endif
