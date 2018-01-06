#ifndef CPPQT_DEFINITION_H
#define CPPQT_DEFINITION_H
#include <memory>

#include "abstractblock.h"
#include "cppqt_abstractdefinition.h"
#include "cppqt_type_concrete.h"



namespace CppQt
{
   class Definition : public AbstractBlock, public AbstractDefinition
   {
   public:
      virtual QString name() const override final { return _type->fullName(); }
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override final;
      virtual int type() const override final;
      virtual const AbstractBlockFactory& factory() const override final;
      Definition& setType(std::unique_ptr<AbstractType>&& type);
   private:
      virtual AbstractBlock* readData(QXmlStreamReader& xml) override final;
      virtual const AbstractBlock* writeData(QXmlStreamWriter& xml) const override final;
      virtual AbstractType* rawType() override final { return _type.get(); }
      std::unique_ptr<AbstractType> _type;
   };
}



#endif
