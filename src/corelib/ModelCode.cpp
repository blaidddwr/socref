#include "ModelCode.h"
namespace Model {


void Code::setCode(
    const QMap<QString,QStringList>& value
)
{
    beginResetModel();
    _code = value;
    _keys = value.keys();
    setText("");
    endResetModel();
}


void Code::setCurrentIndex(
    const QModelIndex& index
)
{
    Q_ASSERT(index.model() == this);
    Q_ASSERT(index.row() >= 0);
    Q_ASSERT(index.row() < _keys.size());
    setText(_code.value(_keys.at(index.row())).join("\n"));
}


const QString& Code::text(
) const
{
    return _text;
}


void Code::setText(
    const QString& value
)
{
    if (_text != value)
    {
        _text = value;
        emit textChanged(value);
    }
}


Code::Code(
    QObject* parent
):
    QAbstractListModel(parent)
{
}


QVariant Code::data(
    const QModelIndex& index
    ,int role
) const
{
    Q_ASSERT(index.isValid());
    switch (role)
    {
    case Qt::DisplayRole:
        return _keys.at(index.row());
    default:
        return {};
    }
}


int Code::rowCount(
    const QModelIndex& parent
) const
{
    Q_ASSERT(!parent.isValid());
    return _keys.size();
}
}
