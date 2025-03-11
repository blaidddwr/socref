#ifndef MODEL_CODE_H
#define MODEL_CODE_H
#include <QAbstractListModel>
namespace Model {




/*!
 * This models the code property of a block. Its list is the list of keys for a
 * code property. It also provides a text property which displays the code lines
 * of the currently indexed key.
 * 
 * @property code The code of a block which this class models.
 * 
 * @property currentIndex The currently indexed key.
 * 
 * @property text The code lines of the currently indexed key.
 */
class Code:
    public QAbstractListModel
{
    Q_OBJECT
    QMap<QString,QStringList> _code;
    QString _text;
    QStringList _keys;


    /*!
     * Setter for the code property.
     */
    public:
    void setCode(
        const QMap<QString,QStringList>& value
    );


    /*!
     * Setter for the current index property.
     */
    public:
    void setCurrentIndex(
        const QModelIndex& index
    );


    /*!
     * Notifier for the text property.
     */
    signals:
    void textChanged(
        const QString& value
    );


    /*!
     * Getter for the text property.
     */
    public:
    const QString& text(
    ) const;


    /*!
     * Setter for the text property.
     */
    private:
    void setText(
        const QString& value
    );


    public:
    Code(
        QObject* parent = nullptr
    );


    public:
    virtual QVariant data(
        const QModelIndex& index
        ,int role = Qt::DisplayRole
    ) const override final;


    public:
    virtual int rowCount(
        const QModelIndex& parent = QModelIndex()
    ) const override final;
};
}


#endif
