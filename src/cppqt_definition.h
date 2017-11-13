#ifndef CPPQT_DEFINITION_H
#define CPPQT_DEFINITION_H
#include "abstractblock.h"
#include "cppqt_type.h"



namespace CppQt
{
   class Definition : public AbstractBlock
   {
      Q_OBJECT
   public:
      explicit Definition(const AbstractBlockFactory& factory, int type):
         AbstractBlock(factory,type),
         _type("unnamed_type_definition")
         {}
      virtual QString getName() const override final { return _type.fullName(); }
      virtual void readData(QXmlStreamReader& xml) override final { xml >> _type; }
      virtual void writeData(QXmlStreamWriter& xml) const override final { xml << _type; }
      virtual AbstractBlock* makeCopy() const override final;
      void setType(const Type& type);
   private:
      Type _type;
   };
}



#endif
