#ifndef CPPQT_VARIABLE_H
#define CPPQT_VARIABLE_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    */
   class Variable : public Base
   {
      Q_OBJECT
   public:
      /*!
       */
      explicit Variable() = default;
      explicit Variable(const QString& name);
      explicit Variable(const QString& type, const QString& name);
      virtual int type() const override;
      virtual QString name() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
      bool isConstExpr() const;
      void setConstExpr(bool isConstExpr);
      bool isStatic() const;
      void setStatic(bool isStatic);
      bool isMutable() const;
      void setMutable(bool isMutable);
      void unnamed_function();
      QString variableType() const;
      void setVariableType(const QString& type);
      bool hasInitializer() const;
      QString initializer() const;
      void setInitializer(const QString& initializer);
      bool isClassMember() const;
      bool isFunctionArgument() const;
   protected:
      virtual void readData(const QDomElement& element, int version) override;
      virtual int writeVersion() const override;
      virtual QDomElement writeData(QDomDocument& document) const override;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual void copyDataFrom(const AbstractBlock* other) override;
   private:
      QString attributes() const;
      void checkTypeSyntax(const QString& type);
      void readVersion0(const QDomElement& element);
      void readVersion1(const QDomElement& element);
      /*!
       */
      constexpr static int _version {1};
      /*!
       */
      static const char* _constExprTag;
      /*!
       */
      static const char* _staticTag;
      /*!
       */
      static const char* _mutableTag;
      /*!
       */
      static const char* _typeTag;
      /*!
       */
      static const char* _initializerTag;
      /*!
       */
      bool _constExpr {false};
      /*!
       */
      bool _static {false};
      /*!
       */
      bool _mutable {false};
      /*!
       */
      QString _type;
      /*!
       */
      QString _initializer;
   };
}



#endif
