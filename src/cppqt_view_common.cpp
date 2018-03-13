#include "cppqt_view_common.h"
#include "cppqt_template.h"
#include "cppqt_blockfactory.h"






QString CppQt::View::displayTemplatesText(const AbstractBlock* block)
{
   QString ret;
   const QList<Template*> list {block->makeListOfType<Template>(BlockFactory::TemplateType)};
   if ( !list.isEmpty() )
   {
      ret.append("<h3>Templates</h3>");
      for (auto template_ : list)
      {
         ret
               .append("<p>")
               .append(template_->variableType())
               .append(" <b>")
               .append(template_->Base::name())
               .append("</b> : ")
               .append(template_->description())
               .append("</p>");
      }
   }
   return ret;
}
