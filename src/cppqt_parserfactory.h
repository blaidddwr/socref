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
      explicit ParserFactory(const AbstractBlock* root);
      virtual std::unique_ptr<AbstractParser> make(const QString& name, const QString& extension) const override final;
   private:
      AbstractParser* find(const Namespace* current, const QStringList& names, const QString& name, bool isHeader, bool isCommon, int index = 0) const;
      const Namespace* _root;
   };
}



#endif
