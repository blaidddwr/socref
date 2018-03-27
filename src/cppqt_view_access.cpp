#include "cppqt_view_access.h"
#include <exception.h>
#include "cppqt_access.h"
#include "cppqt_blockfactory.h"



using namespace CppQt::View;






Access::Access(const CppQt::Access* block, QWidget* parent):
   QLabel(parent),
   _block(block)
{
   connect(_block,&CppQt::Access::bodyChanged,this,&Access::bodyChanged);
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);
   setMargin(8);
   bodyChanged();
}






void Access::bodyChanged()
{
   int functions {0};
   int variables {0};
   int signals_ {0};
   int slots_ {0};
   for (auto child : _block->list())
   {
      switch (child->type())
      {
      case BlockFactory::FunctionType:
         functions++;
         break;
      case BlockFactory::VariableType:
         variables++;
         break;
      case BlockFactory::SignalType:
         signals_++;
         break;
      case BlockFactory::SlotType:
         slots_++;
         break;
      }
   }
   QString text;
   text.append(QString::number(functions)).append(" functions<br/>");
   text.append(QString::number(variables)).append(" variables<br/>");
   text.append(QString::number(signals_)).append(" signals<br/>");
   text.append(QString::number(slots_)).append(" slots<br/>");
   setText(text);
}
