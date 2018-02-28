#include <QFormLayout>
#include <QLabel>
#include "cppqt_common.h"
#include "abstractblock.h"
#include "cppqt_template.h"
#include "cppqt_blockfactory.h"






bool CppQt::isValidTemplateArgument(const QString& argument)
{
   bool ret {true};
   QStringList arguments {argument.split(',')};
   for (auto arg : arguments)
   {
      if ( QRegExp("\\s*((((::)?[a-zA-Z_]+[a-z-A-Z0-9_]*)+(<(.*)>)?(\\s*\\*(\\s*const)?)*\\s*&?)|([0-9]+(\\.[0-9]+)?))\\s*").exactMatch(arg) )
      {
         if ( arg.contains("<") )
         {
            int begin {arg.indexOf('<')};
            int end {arg.lastIndexOf('>')};
            if ( !isValidTemplateArgument(arg.mid(begin+1,end-begin-1)) )
            {
               ret = false;
               break;
            }
         }
      }
      else
      {
         ret = false;
         break;
      }
   }
   return ret;
}






QString CppQt::getTemplateName(const AbstractBlock* block)
{
   QString ret;
   const QList<Template*> list {block->makeChildListOfType<Template>(BlockFactory::TemplateType)};
   if ( !list.isEmpty() )
   {
      ret.append("template<");
      bool first {true};
      for (auto template_ : list)
      {
         if ( first ) first = false;
         else ret.append(", ");
         ret.append(template_->name());
      }
      ret.append("> ");
   }
   return ret;
}
