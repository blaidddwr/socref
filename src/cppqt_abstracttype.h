#ifndef CPPQT_ABSTRACTTYPE_H
#define CPPQT_ABSTRACTTYPE_H
#include <QObject>



class AbstractType : public QObject
{
   Q_OBJECT
public:
   AbstractType(QObject* parent):
      QObject(parent)
      {}
   virtual bool isConcrete() const = 0;
   virtual bool isClass() const = 0;
   virtual bool isEquivalent(const AbstractType* type) const = 0;
   virtual QList<const AbstractType*> contains() const = 0;
   virtual void load(const QString& text) = 0;
   virtual QString save() const = 0;
   QString name(const QList<QString> scope) const;
   const QList<QString> scope() const { return _scope; }
   void setScope(const QList<QString> scope);
protected:
   virtual QString rawName() const = 0;
private:
   QList<QString> _scope;
};



#endif
