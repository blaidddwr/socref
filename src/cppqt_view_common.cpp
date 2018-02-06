#include "cppqt_view_common.h"
#include "cppqt_template.h"
#include "cppqt_blockfactory.h"






namespace CppQt
{
   namespace View
   {
      QString displayTemplatesText(AbstractBlock* block)
      {
         QString ret;
         const QList<Template*> list {block->makeChildListOfType<Template>(BlockFactory::TemplateType)};
         if ( !list.isEmpty() )
         {
            ret.append("<h2>Templates</h2>");
            for (auto template_ : list)
            {
               ret.append("<p>").append(template_->variableType()).append(" <b>").append(template_->Base::name()).append("</b> : ");
               ret.append(template_->description()).append("</p>");
            }
         }
         return ret;
      }
   }
}
