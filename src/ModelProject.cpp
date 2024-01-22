#include "ModelProject.h"
#include <QtCore>
#include <QtGui>
#include "CommandProjectInsert.h"
#include "CommandProjectRemove.h"
#include "Exception.h"
#include "ExceptionBase.h"
#include "ExceptionProjectRead.h"
#include "ExceptionSystemFile.h"
#include "LanguageAbstract.h"
#include "BlockAbstract.h"
#include "FactoryLanguage.h"
#include "Global.h"
#define CONFIG_FILE "project.xml"
namespace Model {


Project::Project(
    int languageIndex
    ,QObject* parent
):
    QAbstractItemModel(parent)
    ,_language(Factory::Language::instance()->get(languageIndex))
{
    G_ASSERT(_language);
    connect(_language,&QObject::destroyed,this,&Project::onLanguageDestroyed);
    _root = _language->create(_language->rootIndex(),this);
    G_ASSERT(_root);
}


Project::Project(
    const QString& directoryPath
    ,QObject* parent
):
    QAbstractItemModel(parent)
    ,_directoryPath(directoryPath)
{
    readDir(directoryPath);
}


QString Project::absoluteParsePath(
) const
{
    return QDir(_directoryPath).absoluteFilePath(_relativeParsePath);
}


Block::Abstract* Project::block(
    const QModelIndex& index
) const
{
    if (!index.isValid())
    {
        return _root;
    }
    else
    {
        G_ASSERT(index.model() == this);
        return reinterpret_cast<Block::Abstract*>(index.internalPointer());
    }
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
    G_ASSERT(index.isValid());
    auto block = reinterpret_cast<Block::Abstract*>(index.internalPointer());
    G_ASSERT(block);
    switch (role)
    {
    case Qt::DisplayRole:
        return block->displayText();
    case Qt::DecorationRole:
        return block->displayIcon();
    default:
        return QVariant();
    }
}


const QString& Project::directoryPath(
) const
{
    return _directoryPath;
}


Model::Project* Project::import(
    const QString& path
    ,QObject* parent
)
{
    std::unique_ptr<Project> ret(new Project);
    ret->readXml(path);
    ret->setParent(parent);
    return ret.release();
}


QModelIndex Project::index(
    int row
    ,int column
    ,const QModelIndex& parent
) const
{
    G_ASSERT(row >= 0);
    G_ASSERT(column == 0);
    auto realParent = _root;
    if (parent.isValid())
    {
        realParent = reinterpret_cast<Block::Abstract*>(parent.internalPointer());
    }
    G_ASSERT(realParent);
    G_ASSERT(row < realParent->size());
    return createIndex(row,column,realParent->get(row));
}


bool Project::insert(
    int blockIndex
    ,int row
    ,const QModelIndex& parent
)
{
    G_ASSERT(blockIndex >= 0);
    G_ASSERT(blockIndex < _language->size());
    G_ASSERT(row >= 0);
    G_ASSERT(row <= block(parent)->size());
    auto command = new Command::Project::Insert(_language->create(blockIndex),row,parent,this);
    if (command->redo())
    {
        _undoStack.push_front(command);
        return true;
    }
    else
    {
        delete command;
        return false;
    }
}


Language::Abstract* Project::language(
) const
{
    G_ASSERT(_language);
    return _language;
}


const QString& Project::name(
) const
{
    return _name;
}


QModelIndex Project::parent(
    const QModelIndex& index
) const
{
    G_ASSERT(index.isValid());
    auto b = reinterpret_cast<Block::Abstract*>(index.internalPointer());
    G_ASSERT(b);
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
    G_ASSERT(row < gp->size());
    return createIndex(row,0,p);
}


const QString& Project::relativeParsePath(
) const
{
    return _relativeParsePath;
}


bool Project::remove(
    const QModelIndex& index
)
{
    G_ASSERT(index.isValid());
    G_ASSERT(index.model() == this);
    auto command = new Command::Project::Remove(index.row(),index.parent(),this);
    if (command->redo())
    {
        _undoStack.push_front(command);
        return true;
    }
    else
    {
        delete command;
        return false;
    }
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


void Project::setRelativeParsePath(
    const QString& value
)
{
    if (_relativeParsePath != value)
    {
        _relativeParsePath = value;
        emit relativeParsePathChanged(value);
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


void Project::readDir(
    const QString& path
)
{
    QDir dir(path);
    if (!dir.exists())
    {
        throw Exception::System::File(tr("No such directory at path %1.").arg(path));
    }
    if (!dir.isReadable())
    {
        throw Exception::System::File(tr("The directory at %1 is not readable.").arg(path));
    }
    try
    {
        readDirConfig(dir.absoluteFilePath(CONFIG_FILE));
        _root = Block::Abstract::fromDir(_language,Socref_1_0,dir.absolutePath(),this);
    }
    catch (Exception::Base& e)
    {
        throw Exception::Project::Read(tr("Failed reading %1: %2").arg(path,e.message()));
    }
}


void Project::readDirConfig(
    const QString& path
)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw Exception::System::File(tr("Failed opening %1: %2").arg(path,file.errorString()));
    }
    QXmlStreamReader xml(&file);
    while(!xml.atEnd())
    {
        xml.readNext();
        if (xml.isStartElement())
        {
            auto name = xml.name().toString();
            if (name == "language")
            {
                auto langName = xml.readElementText();
                auto factory = Factory::Language::instance();
                auto i = factory->indexFromName(langName);
                if (i == -1)
                {
                    throw Exception::Project::Read(tr("Unknown language %1.").arg(langName));
                }
                _language = factory->get(i);
                G_ASSERT(_language);
                connect(_language,&QObject::destroyed,this,&Project::onLanguageDestroyed);
            }
            else if (name == "name")
            {
                _name = xml.readElementText();
            }
            else if (name == "relativeParsePath")
            {
                _relativeParsePath = xml.readElementText();
            }
        }
    }
    if (!_language)
    {
        throw Exception::Project::Read(tr("Language not set in project config file."));
    }
}


void Project::readXml(
    const QString& path
)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw Exception::System::File(tr("Failed opening %1: %2").arg(path,file.errorString()));
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
                    readXmlLegacy(xml);
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


void Project::readXmlLegacy(
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
                    throw Exception::Project::Read(tr("Unknown language %1.").arg(langName));
                }
                _language = factory->get(i);
                G_ASSERT(_language);
                connect(_language,&QObject::destroyed,this,&Project::onLanguageDestroyed);
            }
            else if (name == "name")
            {
                _name = xml.readElementText();
            }
            else if (name == "parse_path")
            {
                _relativeParsePath = xml.readElementText();
            }
            else if (!_root)
            {
                if (!_language)
                {
                    throw Exception::Project::Read(
                        tr("Language not set before first block element.")
                    );
                }
                _root = Block::Abstract::fromXml(_language,Socref_Legacy,xml,this);
            }
        }
    }
}
}
