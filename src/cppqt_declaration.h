#ifndef CPPQT_DECLARATION_H
#define CPPQT_DECLARATION_H
#include "abstractblock.h"



namespace CppQt
{
   class Declaration : public AbstractBlock
   {
      Q_OBJECT
   public:
      enum class Type
      {
         Using
         ,Friend
      };
      static bool isValidUsingString(const QString& text);
      static bool isValidFriendString(const QString& text);
      static bool isValidDeclare(const QString& type, const QString& field);
      Declaration() = default;
      Declaration(Type type);
      Declaration(Type type, const QString& field);
      virtual int type() const override final;
      virtual const AbstractBlockFactory& factory() const override final;
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
      const QStringList& declareTypes() const;
      QString declareTypeName() const;
      Declaration::Type declareType() const;
      void setDeclareType(Type type);
      void setDeclareType(const QString& type);
      QString field() const;
      void setField(const QString& field);
   protected:
      virtual void readData(const QDomElement& element, int version) override final;
      virtual int writeVersion() const override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void copyDataFrom(const AbstractBlock* other) override final;
      constexpr static int _version {0};
      static const QStringList _typeNames;
      static const char* _typeTag;
      static const char* _fieldTag;
      Type _type {Type::Using};
      QString _field;
   };
}



#endif
