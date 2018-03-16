#ifndef DOMELEMENTREADER_H
#define DOMELEMENTREADER_H
#include <QString>
#include <QHash>
#include <QPair>



class QDomElement;



class DomElementReader
{
public:
   DomElementReader(const QDomElement& element);
   DomElementReader& set(const QString& tagName, QString* pointer, bool required = true);
   DomElementReader& set(const QString& tagName, int* pointer, bool required = true);
   DomElementReader& set(const QString& tagName, bool* pointer, bool required = true);
   DomElementReader& set(const QString& tagName, QDomElement* pointer, bool required = true);
   DomElementReader& set(const QString& tagName, QList<QDomElement>* pointer, bool required = true);
   QString attribute(const QString& name, bool required = true);
   int attributeToInt(const QString& name, bool required = true);
   void read();
   bool allRequiredFound() const;
private:
   enum class Type
   {
      String
      ,Number
      ,Boolean
      ,Element
      ,ElementList
   };
   DomElementReader& append(const QString& tagName, void* pointer, Type type, bool required);
   void clear();
   const QDomElement& _element;
   QHash<QString,int> _lookup;
   QList<Type> _type;
   QList<void*> _data;
   QList<bool> _read;
};



#endif
