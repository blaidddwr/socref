#include "ModelProject.h"
#include <QtCore>
#include <QtGui>
#include "ExceptionBase.h"
#include "ExceptionProjectRead.h"
#include "ExceptionSystemFile.h"
#include "LanguageAbstract.h"
#include "BlockAbstract.h"
#include "FactoryLanguage.h"
#include "Global.h"
namespace Model {


Project::Project(
    const QString& path
    ,QObject* parent
):
    QAbstractItemModel(parent)
{
    read(path);
}


int Project::columnCount(
    const QModelIndex& parent
) const
{
    Q_UNUSED(parent);
    return 1;
}


QVariant Project::data(
    const QModelIndex& index
    ,int role
) const
{
    Q_ASSERT(index.isValid());
    auto b = reinterpret_cast<Block::Abstract*>(index.internalPointer());
    Q_ASSERT(b);
    switch (role)
    {
    case Qt::DisplayRole:
        return b->displayText();
    case Qt::DecorationRole:
        return b->displayIcon();
    default:
        return QVariant();
    }
}


const QString& Project::directoryPath(
)
{
    return _directoryPath;
}


QModelIndex Project::index(
    int row
    ,int column
    ,const QModelIndex& parent
) const
{
    Q_ASSERT(row >= 0);
    Q_ASSERT(column == 0);
    auto p = _root;
    if (parent.isValid())
    {
        p = reinterpret_cast<Block::Abstract*>(parent.internalPointer());
    }
    Q_ASSERT(p);
    Q_ASSERT(row < p->size());
    return createIndex(row,column,p->get(row));
}


Language::Abstract* Project::language(
)
{
    Q_ASSERT(_language);
    return _language;
}


const QString& Project::name(
)
{
    return _name;
}


QModelIndex Project::parent(
    const QModelIndex& index
) const
{
    Q_ASSERT(index.isValid());
    auto b = reinterpret_cast<Block::Abstract*>(index.internalPointer());
    Q_ASSERT(b);
    auto p = qobject_cast<Block::Abstract*>(b->parent());
    if (!p)
    {
        return QModelIndex();
    }
    auto gp = qobject_cast<Block::Abstract*>(p->parent());
    if (!gp)
    {
        return QModelIndex();
    }
    auto row = gp->indexOf(p);
    if (row < 0)
    {
        return QModelIndex();
    }
    Q_ASSERT(row < gp->size());
    return createIndex(row,0,p);
}


const QString& Project::parsePath(
)
{
    return _parsePath;
}


int Project::rowCount(
    const QModelIndex& parent
) const
{
    auto p = _root;
    if (parent.isValid())
    {
        p = reinterpret_cast<Block::Abstract*>(parent.internalPointer());
    }
    if (!p)
    {
        return 0;
    }
    return p->size();
}


void Project::setDirectoryPath(
    const QString& value
)
{
    if (_directoryPath != value)
    {
        _directoryPath = value;
        emit directoryPathChanged(value);
    }
}


void Project::setName(
    const QString& value
)
{
    if (_name != value)
    {
        _name = value;
        emit nameChanged(value);
    }
}


void Project::setParsePath(
    const QString& value
)
{
    if (_parsePath != value)
    {
        _parsePath = value;
        emit parsePathChanged(value);
    }
}


void Project::onLanguageDestroyed(
    QObject* object
)
{
    if (_language == object)
    {
        _language = nullptr;
    }
}


void Project::read(
    const QString& path
)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw Exception::System::File(tr("Failed opening %1").arg(path));
    }
    QXmlStreamReader xml(&file);
    try
    {
        while (!xml.atEnd())
        {
            xml.readNext();
            if (xml.isStartElement())
            {
                auto name = xml.name().toString();
                if (name == "srp_project")
                {
                    readLegacy(xml);
                    break;
                }
            }
        }
        if (xml.hasError())
        {
            throw xml.errorString();
        }
    }
    catch (Exception::Base& e)
    {
        throw Exception::Project::Read(
            tr("Failed reading %1 on line %2: %3").arg(path).arg(xml.lineNumber()).arg(e.message())
        );
    }
}


void Project::readLegacy(
    QXmlStreamReader& xml
)
{
    while(!xml.atEnd())
    {
        xml.readNext();
        if (xml.isStartElement())
        {
            auto name = xml.name().toString();
            if (name == "language")
            {
                auto langName = xml.readElementText().toLower();
                langName.replace("+","p").replace("/","");
                auto factory = Factory::Language::instance();
                auto i = factory->indexFromName(langName);
                if (i == -1)
                {
                    throw Exception::Project::Read(tr("Unknown language %1").arg(langName));
                }
                _language = factory->get(i);
                Q_ASSERT(_language);
                connect(_language,&QObject::destroyed,this,&Project::onLanguageDestroyed);
            }
            else if (name == "name")
            {
                _name = xml.readElementText();
            }
            else if (name == "parse_path")
            {
                _parsePath = xml.readElementText();
            }
            else if (!_root)
            {
                if (!_language)
                {
                    throw Exception::Project::Read(
                        tr("Language not set before first block element")
                    );
                }
                _root = Block::Abstract::fromXml(_language,Socref_Legacy,xml,this);
            }
        }
    }
}
}
