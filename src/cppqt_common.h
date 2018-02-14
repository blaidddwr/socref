#ifndef CPPQT_COMMON_H
#define CPPQT_COMMON_H
#include <QString>
#include "global.h"
#include "cppqt.h"



class QFormLayout;



namespace CppQt
{
   bool isValidTemplateArgument(const QString& argument);
   QString templateName(const AbstractBlock* block);
   void addTitle(QFormLayout* layout, const QString& title);
}



#endif
