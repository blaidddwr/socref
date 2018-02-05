#ifndef CPPQT_FUNCTION_H
#define CPPQT_FUNCTION_H
#include "cppqt_variable.h"
#include "cppqt.h"



namespace CppQt
{
   class Function : public Variable
   {
      Q_OBJECT
   public:
      Function() = default;
      Function(const QString& name);
      Function(const QString& returnType, const QString& name);
      virtual QString name() const override final;
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override final;
      virtual int type() const override final;
      virtual QString elementName() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      QString returnType() const;
      void setReturnType(const QString& type);
      QString returnDescription() const;
      void setReturnDescription(const QString& description);
      bool isVirtual() const;
      void setVirtual(bool isVirtual);
      void setConstExpr(bool isConstExpr);
      void setStatic(bool isStatic);
      bool isConst() const;
      void setConst(bool isConst);
      bool isNoExcept() const;
      void setNoExcept(bool isNoExcept);
      bool isOverride() const;
      void setOverride(bool isOverride);
      bool isFinal() const;
      void setFinal(bool isFinal);
      bool isAbstract() const;
      void setAbstract(bool isAbstract);
      bool isMethod() const;
      bool hasTemplates() const;
      QStringList operations() const;
      void setOperations(const QStringList& operations);
      QList<Variable*> arguments() const;
      QList<Template*> templates() const;
   signals:
      void bodyChanged();
   protected:
      virtual void readData(const QDomElement& data) override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
   private slots:
      virtual void childNameChanged(AbstractBlock* child) override final;
      virtual void childAdded(AbstractBlock* child) override final;
      virtual void childRemoved(AbstractBlock* child) override final;
   private:
      using Variable::variableType;
      using Variable::setVariableType;
      static const char* _typeTag;
      static const char* _returnDescriptionTag;
      static const char* _virtualTag;
      static const char* _constTag;
      static const char* _noExceptTag;
      static const char* _overrideTag;
      static const char* _finalTag;
      static const char* _abstractTag;
      static const char* _operationTag;
      QString _returnDescription;
      bool _virtual {false};
      bool _const {false};
      bool _noExcept {false};
      bool _override {false};
      bool _final {false};
      bool _abstract {false};
      QStringList _operations;
   };
}



#endif
