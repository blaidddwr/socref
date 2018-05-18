#ifndef CPPQT_FUNCTION_H
#define CPPQT_FUNCTION_H
#include "cppqt_variable.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    */
   class Function : public Variable
   {
      Q_OBJECT
   public:
      /*!
       */
      explicit Function() = default;
      explicit Function(const QString& name);
      explicit Function(const QString& returnType, const QString& name);
      virtual int type() const override;
      virtual QString name() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
      bool isVoidReturn() const;
      bool isMethod() const;
      bool isPrivateMethod() const;
      bool hasAnyTemplates() const;
      bool hasTemplates() const;
      QList<Template*> templates() const;
      QList<Variable*> arguments() const;
      QString returnType() const;
      QString returnDescription() const;
      QStringList operations() const;
      bool isDefault() const;
      bool isExplicit() const;
      bool isVirtual() const;
      bool isConst() const;
      bool isNoExcept() const;
      bool isOverride() const;
      bool isFinal() const;
      bool isAbstract() const;
      void setReturnType(const QString& type);
      void setReturnDescription(const QString& description);
      void setOperations(const QStringList& operations);
      void setDefault(bool isDefault);
      void setExplicit(bool isExplicit);
      void setVirtual(bool isVirtual);
      void setConstExpr(bool isConstExpr);
      void setStatic(bool isStatic);
      void setConst(bool isConst);
      void setNoExcept(bool isNoExcept);
      void setOverride(bool isOverride);
      void setFinal(bool isFinal);
      void setAbstract(bool isAbstract);
   protected:
      virtual void readData(const QDomElement& element, int version) override;
      virtual int writeVersion() const override;
      virtual QDomElement writeData(QDomDocument& document) const override;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual void copyDataFrom(const AbstractBlock* other) override;
      virtual bool childAdded(AbstractBlock* child) override final;
      virtual bool childRemoved(AbstractBlock* child) override final;
      QString fullName(bool hasReturn, const QString& name) const;
   private:
      QString attributes() const;
      void readVersion0(const QDomElement& element);
      void readVersion1(const QDomElement& element);
      /*!
       */
      constexpr static int _version {1};
      /*!
       */
      static const char* _returnDescriptionTag;
      /*!
       */
      static const char* _defaultTag;
      /*!
       */
      static const char* _explicitTag;
      /*!
       */
      static const char* _virtualTag;
      /*!
       */
      static const char* _constTag;
      /*!
       */
      static const char* _noExceptTag;
      /*!
       */
      static const char* _overrideTag;
      /*!
       */
      static const char* _finalTag;
      /*!
       */
      static const char* _abstractTag;
      /*!
       */
      static const char* _operationTag;
      /*!
       */
      QString _returnDescription;
      /*!
       */
      QStringList _operations;
      /*!
       */
      bool _default {false};
      /*!
       */
      bool _explicit {false};
      /*!
       */
      bool _virtual;
      /*!
       */
      bool _const {false};
      /*!
       */
      bool _noExcept {false};
      /*!
       */
      bool _override {false};
      /*!
       */
      bool _final {false};
      /*!
       */
      bool _abstract {false};
   };
}



#endif
