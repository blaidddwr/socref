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
      explicit Function() = default;
      explicit Function(const QString& name);
      explicit Function(const QString& returnType, const QString& name);
      virtual QString name() const override;
      virtual int type() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
      bool isVoidReturn() const;
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
      bool isPrivateMethod() const;
      bool hasAnyTemplates() const;
      bool hasTemplates() const;
      QStringList operations() const;
      void setOperations(const QStringList& operations);
      QList<Variable*> arguments() const;
      QList<Template*> templates() const;
   signals:
      void bodyChanged();
   protected slots:
      virtual void childNameChanged(AbstractBlock* child) override final;
      virtual void childAdded(AbstractBlock* child) override final;
      virtual void childRemoved(AbstractBlock* child) override final;
   protected:
      virtual void readData(const QDomElement& data, int version) override;
      virtual int writeVersion() const override;
      virtual QDomElement writeData(QDomDocument& document) const override;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual void copyDataFrom(const AbstractBlock* object) override;
      QString fullName(bool hasReturn, const QString& name) const;
   private:
      QString attributes() const;
      void readVersion0(const QDomElement& data);
      void readVersion1(const QDomElement& data);
      constexpr static int _version {1};
      static const char* _returnDescriptionTag;
      static const char* _virtualTag;
      static const char* _constTag;
      static const char* _noExceptTag;
      static const char* _overrideTag;
      static const char* _finalTag;
      static const char* _abstractTag;
      static const char* _operationTag;
      bool _virtual {false};
      bool _const {false};
      bool _noExcept {false};
      bool _override {false};
      bool _final {false};
      bool _abstract {false};
      QString _returnDescription;
      QStringList _operations;
   };
}



#endif
