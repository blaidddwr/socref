#ifndef CPPQT_PARSERFACTORY_H
#define CPPQT_PARSERFACTORY_H
#include "abstractparserfactory.h"
#include "cppqt.h"



namespace CppQt
{
   class ParserFactory : public AbstractParserFactory
   {
      Q_OBJECT
   public:
      explicit ParserFactory(AbstractBlock* root);
      virtual std::unique_ptr<AbstractParser> makeParser(const QString& name, const QString& extension) const override final;
   private:
      Namespace* find(const Namespace* current, QStringList* names) const;
      Namespace* _root;
   };
}



#endif
