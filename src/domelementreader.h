#ifndef DOMELEMENTREADER_H
#define DOMELEMENTREADER_H
#include <QString>
#include <QHash>
#include <QPair>



class QDomNode;
class QDomElement;



class DomElementReader
{
public:
   DomElementReader(const QDomNode& node);
   DomElementReader& set(const QString& tagName, QString* pointer, bool onlyOnce = true, bool required = true);
   DomElementReader& set(const QString& tagName, int* pointer, bool onlyOnce = true, bool required = true);
   DomElementReader& set(const QString& tagName, QDomElement* pointer, bool onlyOnce = true, bool required = true);
   void read();
   bool allRequiredFound() const;
private:
   enum class Type
   {
      String
      ,Number
      ,Element
   };
   DomElementReader& append(const QString& tagName, void* pointer, Type type, bool onlyOnce, bool required);
   const QDomNode& _node;
   QHash<QString,int> _lookup;
   QList<Type> _type;
   QList<void*> _data;
   QList<bool> _read;
   QList<bool> _onlyOnce;
};



#endif
