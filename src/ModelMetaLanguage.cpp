#include "ModelMetaLanguage.h"
namespace Model {
namespace Meta {


Language::Language(
    const QString& name
    ,const QString& label
    ,QObject* parent
):
    QObject(parent)
    ,_label(label)
    ,_name(name)
{
}


const QString& Language::label(
) const
{
    return _label;
}


const QString& Language::name(
) const
{
    return _name;
}
}
}
