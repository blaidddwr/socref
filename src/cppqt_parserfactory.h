#ifndef CPPQT_PARSERFACTORY_H
#define CPPQT_PARSERFACTORY_H
#include "abstractparserfactory.h"
#include "singleton.h"



namespace CppQt
{
   class ParserFactory : public AbstractParserFactory, public Singleton<ParserFactory>
   {
   public:
      virtual std::unique_ptr<AbstractParser> makeParser(const QString& name, const QString& extension) const override final;
   };
}



#endif
