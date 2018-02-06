#include "cppqt_template.h"
#include "cppqt_view_template.h"
#include "cppqt_edit_template.h"
#include "cppqt_blockfactory.h"



using namespace std;
using namespace Gui;
using namespace CppQt;






Template::Template(const QString& name):
   Variable(name)
{}






Template::Template(const QString& type, const QString& name):
   Variable(type,name)
{}






unique_ptr<AbstractBlock> Template::makeCopy() const
{
   unique_ptr<Template> ret {new Template};
   ret->copyDataFrom(*this);
   return ret;
}






int Template::type() const
{
   return BlockFactory::TemplateType;
}






QIcon Template::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/template.svg");
   return ret;
}






QList<int> Template::buildList() const
{
   return QList<int>();
}






unique_ptr<QWidget> Template::makeView() const
{
   return unique_ptr<QWidget>(new View::Template(this));
}






unique_ptr<AbstractEdit> Template::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Template(this));
}
