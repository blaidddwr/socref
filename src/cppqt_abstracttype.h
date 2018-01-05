#ifndef CPPQT_ABSTRACTTYPE_H
#define CPPQT_ABSTRACTTYPE_H
#include <memory>

#include <QList>



class QXmlStreamReader;
class QXmlStreamWriter;



namespace CppQt
{
   class AbstractType
   {
   public:
      AbstractType() = default;
      AbstractType(const QString& name):
         _name(name)
         {}
      virtual ~AbstractType() = default;
      virtual bool isConcrete() const = 0;
      virtual bool isTemplate() const = 0;
      virtual bool isClass() const = 0;
      virtual bool isEquivalent(const AbstractType* type) const = 0;
      virtual const QList<AbstractType*> contains() const = 0;
      virtual std::unique_ptr<AbstractType> makeCopy() const = 0;
      virtual QString fullName(const QList<QString> scope = QList<QString>()) const = 0;
      virtual AbstractType* read(QXmlStreamReader& xml) = 0;
      virtual int type() const = 0;
      QString scopedName(const QList<QString> scope = QList<QString>()) const;
      const QList<QString> scope() const { return _scope; }
      AbstractType* clearScope();
      AbstractType* prependScope(const QString& scope);
      QString name() const { return _name; }
      AbstractType* setName(const QString& name);
      AbstractType* write(const QString& elementName, QXmlStreamWriter& xml);
      bool operator!=(const AbstractType& type) { return _scope != type._scope || !isEquivalent(&type); }
      bool operator==(const AbstractType& type) { return _scope == type._scope && isEquivalent(&type); }
   protected:
      virtual void writeData(QXmlStreamWriter& xml) const = 0;
   private:
      int _type {-1};
      QList<QString> _scope;
      QString _name;
   };
}



#endif
