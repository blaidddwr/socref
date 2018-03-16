#ifndef CPPQT_PARENT_H
#define CPPQT_PARENT_H
#include "cppqt_base.h"



namespace CppQt
{
   class Parent : public Base
   {
      Q_OBJECT
   public:
      enum class Access
      {
         Public
         ,Protected
         ,Private
      };
      explicit Parent() = default;
      explicit Parent(const QString& name);
      virtual QString name() const override final;
      virtual int type() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<Gui::AbstractEdit> makeEdit() override final;
      Access access() const;
      void setAccess(Access access);
      QString templateArgument() const;
      void setTemplateArgument(const QString& templateArgument);
      static const QStringList _accessNames;
   protected:
      virtual void readData(const QDomElement& data, int version) override final;
      virtual int writeVersion() const override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void copyDataFrom(const AbstractBlock* object) override final;
   private:
      void readVersion0(const QDomElement &data);
      void readVersion1(const QDomElement &data);
      constexpr static int _version {1};
      static const char* _accessTag;
      static const char* _templateArgumentTag;
      Access _access {Access::Public};
      QString _templateArgument;
   };
}



#endif
