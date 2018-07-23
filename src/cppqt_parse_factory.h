#ifndef CPPQT_PARSE_FACTORY_H
#define CPPQT_PARSE_FACTORY_H
#include "abstractparserfactory.h"
#include "cppqt.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       */
      class Factory : public AbstractParserFactory
      {
         Q_OBJECT
      public:
         virtual std::unique_ptr<AbstractParser> make(const QString& name, const QString& extension) const override final;
      public:
         explicit Factory(const AbstractBlock* root);
      private:
         AbstractParser* find(const Namespace* current, const QStringList& names, const QString& name, bool isHeader, bool isCommon, int index = 0) const;
         /*!
          */
         const Namespace* _root;
      };
   }
}



#endif
