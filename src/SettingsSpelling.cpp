#include "SettingsSpelling.h"
#include <QtCore>
#include "gassert.h"
#define AFF_EXT ".aff"
#define DIC_EXT ".dic"
#define DICTIONARY_KEY "settings.spelling.dictionary"
#define PATH_KEY "settings.spelling.path"
namespace Settings {
Spelling* Spelling::_instance {nullptr};


QString Spelling::absoluteAffPath(
) const
{
    if (!_dictionary.isNull())
    {
        return QDir(_path).absoluteFilePath(_dictionary+AFF_EXT);
    }
    else
    {
        return QString();
    }
}


QString Spelling::absoluteDicPath(
) const
{
    if (!_dictionary.isNull())
    {
        return QDir(_path).absoluteFilePath(_dictionary+DIC_EXT);
    }
    else
    {
        return QString();
    }
}


QStringList Spelling::dictionaries(
    const QDir& dir
)
{
    QStringList ret;
    if (
        !dir.exists()
        || !dir.isReadable()
    )
    {
        return ret;
    }
    for (const auto& fileName: dir.entryList({QString("*")+AFF_EXT},QDir::Files))
    {
        auto dictionary = fileName.chopped(strlen(AFF_EXT));
        if (isReadable(dir,dictionary))
        {
            ret.append(dictionary);
        }
    }
    return ret;
}


const QString& Spelling::dictionary(
) const
{
    return _dictionary;
}


bool Spelling::hasDictionary(
) const
{
    return !_dictionary.isNull();
}


Spelling* Spelling::instance(
)
{
    if (!_instance)
    {
        _instance = new Spelling(QCoreApplication::instance());
    }
    return _instance;
}


const QString& Spelling::path(
) const
{
    return _path;
}


void Spelling::setDictionary(
    const QString& value
)
{
    QDir dir(_path);
    G_ASSERT(
        value.isNull()
        || isReadable(dir,value)
    );
    if (_dictionary != value)
    {
        QSettings().setValue(DICTIONARY_KEY,value);
        _dictionary = value;
        emit dictionaryChanged(value);
    }
}


void Spelling::setPath(
    const QString& value
)
{
    QDir dir(value);
    G_ASSERT(dir.exists());
    G_ASSERT(dir.isReadable());
    if (_path != value)
    {
        setDictionary(QString());
        QSettings().setValue(PATH_KEY,value);
        _path = value;
        emit pathChanged(value);
    }
}


Spelling::Spelling(
    QObject* parent
):
    QObject(parent)
{
    QSettings settings;
    _path = settings.value(PATH_KEY,"/usr/share/hunspell").toString();
    auto dictionary = settings.value(DICTIONARY_KEY,"en_US").toString();
    QDir dir(_path);
    if (dir.exists() && dir.isReadable() && isReadable(dir,dictionary))
    {
        _dictionary = dictionary;
    }
}


bool Spelling::isReadable(
    const QDir& dir
    ,const QString& dictionary
)
{
    return (
        QFileInfo(dir.absoluteFilePath(dictionary+AFF_EXT)).isReadable()
        && QFileInfo(dir.absoluteFilePath(dictionary+DIC_EXT)).isReadable()
    );
}
}
