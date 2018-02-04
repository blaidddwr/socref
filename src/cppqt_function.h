#ifndef CPPQT_FUNCTION_H
#define CPPQT_FUNCTION_H
#include "cppqt_base.h"
#include "cppqt.h"



namespace CppQt
{
   class Function : public Base
   {
      Q_OBJECT
   public:
      Function() = default;
      Function(const QString& name);
      Function(const QString& returnType, const QString& name);
      virtual QString name() const override final;
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override;
      virtual int type() const override final;
      virtual QString elementName() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      QString returnType() const;
      void setReturnType(const QString& type);
      QString returnDescription() const;
      void setReturnDescription(const QString& description);
      QString preFlags() const;
      void setPreFlags(const QString& flags);
      QString postFlags() const;
      void setPostFlags(const QString& flags);
      QStringList operations() const;
      void setOperations(const QStringList& operations);
      QList<Variable*> arguments() const;
      QList<Template*> templates() const;
      static const char* _preFlagRegExp;
      static const char* _postFlagRegExp;
   signals:
      void bodyChanged();
   private slots:
      virtual void childNameChanged(AbstractBlock* child) override final;
      virtual void childAdded(AbstractBlock* child) override final;
      virtual void childRemoved(AbstractBlock* child) override final;
   private:
      virtual void readData(const QDomElement& data) override;
      virtual QDomElement writeData(QDomDocument& document) const override;
      static const char* _typeTag;
      static const char* _descriptionTag;
      static const char* _preFlagTag;
      static const char* _postFlagTag;
      static const char* _operationTag;
      QString _returnType;
      QString _returnDescription;
      QString _preFlags;
      QString _postFlags;
      QStringList _operations;
   };
}



#endif
