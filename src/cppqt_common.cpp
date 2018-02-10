#include <QFormLayout>
#include <QLabel>
#include "cppqt_common.h"
#include "abstractblock.h"
#include "cppqt_template.h"
#include "cppqt_blockfactory.h"






QString CppQt::templateName(const AbstractBlock* block)
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






void CppQt::addTitle(QFormLayout* layout, const QString& title)
{
   QLabel* label {new QLabel(QString("<h3>").append(title).append("</h3>"))};
   label->setStyleSheet("margin-top:16px;");
   layout->addRow(label);
}
