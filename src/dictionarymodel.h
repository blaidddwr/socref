#ifndef DICTIONARYMODEL_H
#define DICTIONARYMODEL_H
#include <QAbstractListModel>
#include <QDomElement>



/*!
 * This is a dictionary list model. This stores a list of unique words sorted
 * alphabetically and provides a method for testing if it contains a given word.
 * This is intended to be used to store a custom set of spell checker words on a
 * per project bases. Copies of words cannot be stored in this model. The data
 * of this model can be written or saved using XML elements.
 */
class DictionaryModel : public QAbstractListModel
{
   Q_OBJECT
public:
   virtual int rowCount(const QModelIndex& parent) const override final;
   virtual QVariant data(const QModelIndex& index, int role) const override final;
public:
   explicit DictionaryModel(QObject* parent = nullptr);
public:
   bool hasWord(const QString& word) const;
   QDomElement write(QDomDocument& document) const;
   bool addWord(const QString& word);
   bool removeWord(const QModelIndex& index);
   void read(const QDomElement& element);
signals:
   /*!
    * Signals that this dictionary model has been modified.
    */
   void modified();
private:
   /*!
    * The list of words this dictionary model contains.
    */
   QStringList _list;
};

#endif
