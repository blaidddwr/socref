#ifndef CPPQT_ACCESS_H
#define CPPQT_ACCESS_H
#include "abstractblock.h"



namespace CppQt
{
   class Access : public AbstractBlock
   {
      Q_OBJECT
   public:
      enum class Type
      {
         Public
         ,Signals
         ,PublicSlots
         ,Protected
         ,ProtectedSlots
         ,Private
         ,PrivateSlots
      };
      Access() = default;
      Access(Type type);
      virtual QString name() const override final;
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override final;
      virtual int type() const override final;
      virtual const AbstractBlockFactory& factory() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<Gui::AbstractEdit> makeEdit() override final;
      Type accessType();
      void setAccessType(Type type);
      bool hasSignals() const;
      bool hasSlots() const;
      bool hasSignalsOrSlots() const;
      bool hasFunctionsOrVariables() const;
      bool hasVirtual() const;
      bool hasAbstract() const;
      static bool isSlot(Type type);
      static bool isNormal(Type type);
   signals:
      void bodyChanged();
   protected slots:
      virtual void childAdded(AbstractBlock* child) override final;
      virtual void childRemoved(AbstractBlock* child) override final;
   protected:
      virtual void readData(const QDomElement& data) override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
   private:
      static const char* _typeTag;
      Type _type;
   };
}



#endif
