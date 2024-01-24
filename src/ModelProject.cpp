#include "ModelProject.h"
#include <QtCore>
#include <QtGui>
#include "CommandProjectInsert.h"
#include "CommandProjectMove.h"
#include "CommandProjectRemove.h"
#include "CommandProjectSet.h"
#include "Exception.h"
#include "ExceptionBase.h"
#include "ExceptionProjectLogical.h"
#include "ExceptionProjectRead.h"
#include "ExceptionProjectWrite.h"
#include "ExceptionSystemFile.h"
#include "LanguageAbstract.h"
#include "ModelMetaBlock.h"
#include "BlockAbstract.h"
#include "FactoryLanguage.h"
#include "Global.h"
#define CONFIG_FILE "project.xml"
namespace Model {
using FileError = Exception::System::File;
using LogicalError = Exception::Project::Logical;
using ReadError = Exception::Project::Read;
using WriteError = Exception::Project::Write;
QList<Block::Abstract*> Project::_copied {};


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
    ,_directoryPath(QFileInfo(directoryPath).absoluteFilePath())
{
    readDir(directoryPath);
}


bool Project::abortSet(
)
{
    if (
        !_setIndex.isValid()
    )
    {
        return false;
    }
    _setIndex = QPersistentModelIndex();
    _previousState.clear();
    return true;
}


QString Project::absoluteParsePath(
) const
{
    return QDir(_directoryPath).absoluteFilePath(_relativeParsePath);
}


int Project::canPaste(
    const QModelIndex& parent
) const
{
    int ret = 0;
    auto pb = block(parent);
    for (auto block: _copied)
    {
        if (pb->meta()->allowList().contains(block->meta()->index()))
        {
            ret++;
        }
    }
    return ret;
}


bool Project::canRedo(
) const
{
    return !_redoStack.isEmpty();
}


bool Project::canUndo(
) const
{
    return !_undoStack.isEmpty();
}


int Project::columnCount(
    const QModelIndex& parent
) const
{
    Q_UNUSED(parent);
    return 1;
}


int Project::copy(
    const QModelIndexList& indexes
) const
{
    int ret = 0;
    qDeleteAll(_copied);
    _copied.clear();
    for (const auto& index: indexes)
    {
        if (
            index.isValid()
            && index.model() == this
        )
        {
            _copied.append(block(index)->copy());
            ret++;
        }
    }
    return ret;
}


int Project::cut(
    const QModelIndexList& indexes
)
{
    int ret = 0;
    qDeleteAll(_copied);
    _copied.clear();
    QList<QPersistentModelIndex> pi;
    for (const auto& index: indexes)
    {
        pi.append(index);
    }
    for (const auto& index: pi)
    {
        if (
            index.isValid()
            && index.model() == this
        )
        {
            std::unique_ptr<Block::Abstract> copy(block(index)->copy());
            auto command = new Command::Project::Remove(index.row(),index.parent(),this);
            if (command->redo())
            {
                _copied.append(copy.release());
                _undoStack.push_front(command);
                ret++;
            }
            else
            {
                delete command;
            }
        }
    }
    return ret;
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


bool Project::finishSet(
)
{
    if (
        !_setIndex.isValid()
    )
    {
        return false;
    }
    _undoStack.push_back(
        new Command::Project::Set(_previousState,block(_setIndex)->state(),_setIndex,this)
    );
    _setIndex = QPersistentModelIndex();
    _previousState.clear();
    return true;
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
    if (
        blockIndex < 0
        || blockIndex >= _language->size()
        || row < 0
        || row > block(parent)->size()
    )
    {
        return false;
    }
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


bool Project::move(
    const QModelIndex& parent
    ,int from
    ,int to
)
{
    if (
        from == to
        || from < 0
        || from >= rowCount(parent)
        || to < 0
        || to >= rowCount(parent)
    )
    {
        return false;
    }
    auto command = new Command::Project::Move(from,to,parent,this);
    if (command->redo())
    {
        _undoStack.push_back(command);
        return true;
    }
    else
    {
        delete command;
        return false;
    }
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


int Project::paste(
    const QModelIndex& parent
    ,int row
)
{
    if (
        _copied.isEmpty()
        || row < 0
        || row > rowCount(parent)
    )
    {
        return 0;
    }
    int ret = 0;
    for (auto b: _copied)
    {
        std::unique_ptr<Block::Abstract> copy(b->copy());
        if (block(parent)->meta()->allowList().contains(copy->meta()->index()))
        {
            auto command = new Command::Project::Insert(copy.release(),row,parent,this);
            if (command->redo())
            {
                _undoStack.push_back(command);
                ret++;
            }
            else
            {
                delete command;
            }
        }
    }
    return ret;
}


bool Project::redo(
)
{
    if (_redoStack.isEmpty())
    {
        return false;
    }
    if (_redoStack.front()->redo())
    {
        _undoStack.push_front(_redoStack.front());
        _redoStack.pop_front();
        return true;
    }
    else
    {
        return false;
    }
}


const QString& Project::relativeParsePath(
) const
{
    return _relativeParsePath;
}


int Project::remove(
    const QModelIndexList& indexes
)
{
    int ret = 0;
    QList<QPersistentModelIndex> pi;
    for (const auto& index: indexes)
    {
        pi.append(index);
    }
    for (const auto& index: pi)
    {
        if (
            index.isValid()
            && index.model() == this
        )
        {
            auto command = new Command::Project::Remove(index.row(),index.parent(),this);
            if (command->redo())
            {
                _undoStack.push_front(command);
                ret++;
            }
            else
            {
                delete command;
            }
        }
    }
    return ret;
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


void Project::save(
)
{
    if (_directoryPath.isNull())
    {
        throw LogicalError(tr("Cannot save new project without directory path."));
    }
    writeDir(_directoryPath);
}


void Project::saveToDir(
    const QString& path
)
{
    writeDir(path);
    setDirectoryPath(QFileInfo(path).absoluteFilePath());
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


Block::Abstract* Project::startSet(
    const QModelIndex& index
)
{
    if (
        index.model() != this
        || !index.isValid()
        || _setIndex.isValid()
    )
    {
        return nullptr;
    }
    _setIndex = index;
    auto ret = block(index);
    _previousState = ret->state();
    return ret;
}


bool Project::undo(
)
{
    if (_undoStack.isEmpty())
    {
        return false;
    }
    if (_undoStack.front()->undo())
    {
        _redoStack.push_front(_undoStack.front());
        _undoStack.pop_front();
        return true;
    }
    else
    {
        return false;
    }
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
        throw FileError(tr("No such directory at path %1.").arg(path));
    }
    if (!dir.isReadable())
    {
        throw FileError(tr("The directory at %1 is not readable.").arg(path));
    }
    try
    {
        readDirConfig(dir.absoluteFilePath(CONFIG_FILE));
        _root = Block::Abstract::fromDir(_language,Socref_1_0,dir.absolutePath(),this);
    }
    catch (Exception::Base& e)
    {
        throw ReadError(tr("Failed reading %1: %2").arg(path,e.message()));
    }
}


void Project::readDirConfig(
    const QString& path
)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileError(tr("Failed opening %1: %2").arg(path,file.errorString()));
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
                    throw ReadError(tr("Unknown language %1.").arg(langName));
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
        throw ReadError(tr("Language not set in project config file."));
    }
}


void Project::readXml(
    const QString& path
)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileError(tr("Failed opening %1: %2").arg(path,file.errorString()));
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
            throw ReadError(xml.errorString());
        }
    }
    catch (Exception::Base& e)
    {
        throw ReadError(
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
                    throw ReadError(tr("Unknown language %1.").arg(langName));
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
                    throw ReadError(tr("Language not set before first block element."));
                }
                _root = Block::Abstract::fromXml(_language,Socref_Legacy,xml,this);
            }
        }
    }
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


void Project::writeDir(
    const QString& path
)
{
    QDir dir(path);
    if (!dir.exists())
    {
        if (!dir.mkpath("."))
        {
            throw FileError(tr("Failed creating directory %1.").arg(path));
        }
    }
    if (!dir.isReadable())
    {
        throw FileError(tr("The directory at %1 is not readable.").arg(path));
    }
    try
    {
        writeDirConfig(dir.absoluteFilePath(CONFIG_FILE));
        _root->toDir(path);
    }
    catch (Exception::Base& e)
    {
        throw WriteError(tr("Failed writing %1: %2").arg(path,e.message()));
    }
}


void Project::writeDirConfig(
    const QString& path
)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        throw FileError(tr("Failed opening %1: %2").arg(path,file.errorString()));
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("socref");
    xml.writeTextElement("name",_name);
    xml.writeTextElement("language",_language->meta()->name());
    xml.writeTextElement("relativeParsePath",_relativeParsePath);
    xml.writeEndElement();
    xml.writeEndDocument();
    if (file.error() != QFileDevice::NoError)
    {
        throw WriteError(tr("Failed writing project file %1: %2").arg(path,file.errorString()));
    }
}
}
