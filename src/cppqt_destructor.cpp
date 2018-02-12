#include "cppqt_destructor.h"
#include "cppqt_view_destructor.h"
#include "cppqt_edit_destructor.h"
#include "cppqt_blockfactory.h"



using namespace std;
using namespace Gui;
using namespace CppQt;






QString Destructor::name() const
{
   return QString("~").append(Constructor::name());
}






int Destructor::type() const
{
   return BlockFactory::DestructorType;
}






QIcon Destructor::icon() const
{
   return Function::icon();
}






QList<int> Destructor::buildList() const
{
   return QList<int>();
}






std::unique_ptr<QWidget> Destructor::makeView() const
{
   return unique_ptr<QWidget>(new View::Destructor(this));
}






std::unique_ptr<::Gui::AbstractEdit> Destructor::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Destructor(this));
}






std::unique_ptr<AbstractBlock> Destructor::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Destructor);
}
