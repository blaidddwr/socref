#ifndef CPPQT_PARENT_H
#define CPPQT_PARENT_H
#include "abstractblock.h"



namespace CppQt
{
   class Parent : public AbstractBlock
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
      explicit Parent(const QString& className);
      virtual QString name() const override final;
      virtual int type() const override final;
      virtual const AbstractBlockFactory& factory() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
      const QStringList& accessNames() const;
      QString accessName() const;
      Access access() const;
      void setAccess(Access access);
      void setAccess(const QString& accessName);
      QString className() const;
      void setClassName(const QString& templateArgument);
   protected:
      virtual void readData(const QDomElement& data, int version) override final;
      virtual int writeVersion() const override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void copyDataFrom(const AbstractBlock* object) override final;
   private:
      void readVersion0(const QDomElement &data);
      void readVersion1(const QDomElement &data);
      void readVersion2(const QDomElement &data);
      constexpr static int _version {2};
      static const QStringList _accessNames;
      static const char* _accessTag;
      static const char* _templateArgumentTag;
      static const char* _nameTag;
      static const char* _classTag;
      Access _access {Access::Public};
      QString _class;
   };
}



#endif
