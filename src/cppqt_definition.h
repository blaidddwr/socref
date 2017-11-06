#ifndef CPPQT_DEFINITION_H
#define CPPQT_DEFINITION_H
#include "abstractblock.h"
#include "cppqt_type.h"



//@@
namespace CppQt
{
   //@@
   class Definition : public AbstractBlock
   {
      Q_OBJECT
   public:
      explicit Definition(const AbstractBlockFactory& factory, int type);
      virtual QString getName() const override final;
      virtual void readData(QXmlStreamReader& xml) override final;
      virtual void writeData(QXmlStreamWriter& xml) const override final;
      virtual AbstractBlock* makeCopy() const override final;
      void setType(const Type& type);
   private:
      Type _type;
   };

   //@@
   inline Definition::Definition(const AbstractBlockFactory& factory, int type):
      AbstractBlock(factory,type), _type("unnamed_type_definition") {}

   //@@
   inline QString Definition::getName() const { return _type.getName(); }

   //@@
   inline void Definition::readData(QXmlStreamReader& xml) { _type.readData(xml); }

   //@@
   inline void Definition::writeData(QXmlStreamWriter& xml) const { _type.writeData(xml); }
}



#endif
