#include "cppqt_constructor.h"
#include <exception.h>
#include "cppqt_view_constructor.h"
#include "cppqt_edit_constructor.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;






QString Constructor::name() const
{
   return fullName(false,className());
}






int Constructor::type() const
{
   return BlockFactory::ConstructorType;
}






QIcon Constructor::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/constructor.svg");
   return ret;
}






QList<int> Constructor::buildList() const
{
   return QList<int>{BlockFactory::VariableType};
}






std::unique_ptr<QWidget> Constructor::makeView() const
{
   return unique_ptr<QWidget>(new View::Constructor(this));
}






std::unique_ptr<AbstractEdit> Constructor::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Constructor(this));
}






QString Constructor::className() const
{
   AbstractBlock* access {parent()};
   if ( !access || !access->parent() )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Parent or grandparent is nullptr."));
      throw e;
   }
   if ( Base* base = qobject_cast<Base*>(access->parent()) )
   {
      return base->Base::name();
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Grandparent does not contain CppQt::Base class."));
      throw e;
   }
}






void Constructor::classNameChanged()
{
   notifyOfNameChange();
}






std::unique_ptr<AbstractBlock> Constructor::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Constructor);
}
