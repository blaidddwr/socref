#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>

#include "cppqt_type_gui_combobox.h"
#include "cppqt_abstracttype.h"



using namespace std;
using namespace CppQt;
using namespace CppQt::Type::Gui;






ComboBox::ComboBox(const QList<AbstractType*>& list, int depth, QWidget* parent):
   QWidget(parent),
   _list(list),
   _depth(depth)
{
   QHBoxLayout* layout {new QHBoxLayout};
   layout->addWidget(_combo = new QComboBox);
   layout->addWidget(_spinner);
   setLayout(layout);
   update();
}






unique_ptr<AbstractType> ComboBox::makeCurrentType() const
{
   if ( _combo->currentIndex() == -1 || _combo->currentIndex() >= _list.size() )
   {
      return nullptr;
   }
   return _list.at(_combo->currentIndex())->makeCopy();
}






ComboBox& ComboBox::setList(const QList<AbstractType*>& list, int depth)
{
   _list = list;
   _depth = depth;
   update();
   return *this;
}






void ComboBox::depthChanged(int depth)
{
   _depth = depth;
   update();
}






void ComboBox::update()
{
   sort(_list.begin(),_list.end(),[this](AbstractType* first, AbstractType* last) ->bool { return first->scopedNameWithDepth(_depth) < last->scopedNameWithDepth(_depth); });
   while ( _list.size() > _combo->count() )
   {
      _combo->removeItem(_combo->count() - 1);
   }
   for (int i = 0; i < _list.size() ;++i)
   {
      _combo->setItemText(i,_list.at(i)->scopedNameWithDepth(_depth));
   }
   _combo->setCurrentIndex(-1);
}






void ComboBox::createSpinner()
{
   _spinner = new QSpinBox;
   _spinner->setMinimum(-1);
   _spinner->setMaximum(INT_MAX);
   _spinner->setValue(_depth);
   connect(_spinner,QOverload<int>::of(&QSpinBox::valueChanged),this,&ComboBox::depthChanged);
}
