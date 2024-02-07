#include "ModelProject.h"
#include <QtCore>
#include <QtGui>
#include "CommandProjectInsert.h"
#include "CommandProjectMove.h"
#include "CommandProjectRemove.h"
#include "CommandProjectSet.h"
#include "Exceptions.h"
#include "LanguageAbstract.h"
#include "ModelMetaBlock.h"
#include "BlockAbstract.h"
#include "FactoryLanguage.h"
#define CONFIG_FILE "project.xml"
namespace Model {
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


Block::Abstract* Project::beginSet(
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


int Project::blockIndex(
    const QModelIndex& index
) const
{
    return block(index)->meta()->index();
}


bool Project::canMove(
    const QModelIndex& parent
    ,int from
    ,int to
) const
{
    if (
        (parent.isValid() && parent.model() != this)
        || from == to
        || from < 0
        || from >= rowCount(parent)
        || to < 0
        || to >= rowCount(parent)
    )
    {
        return false;
    }
    else
    {
        return true;
    }
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
            if (pushCommand(new Command::Project::Remove(index.row(),index.parent(),this)))
            {
                _copied.append(copy.release());
                ret++;
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
    QModelIndex index = _setIndex;
    _undoStack.push_back(
        new Command::Project::Set(_previousState,block(index)->state(),_setIndex,this)
    );
    emit dataChanged(index,index,{Qt::DisplayRole});
    setModified(true);
    _setIndex = QPersistentModelIndex();
    _previousState.clear();
    return true;
}


QVariant Project::headerData(
    int section
    ,Qt::Orientation orientation
    ,int role
) const
{
    if (
        section == 0
        && orientation == Qt::Horizontal
        && role == Qt::DisplayRole
    )
    {
        return _language->meta()->label();
    }
    else
    {
        return QVariant();
    }
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
    return pushCommand(new Command::Project::Insert(_language->create(blockIndex),row,parent,this));
}


Language::Abstract* Project::language(
) const
{
    G_ASSERT(_language);
    return _language;
}


bool Project::modified(
) const
{
    return _modified;
}


bool Project::move(
    const QModelIndex& parent
    ,int from
    ,int to
)
{
    if (!canMove(parent,from,to))
    {
        return false;
    }
    return pushCommand(new Command::Project::Move(from,to,parent,this));
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
            if (pushCommand(new Command::Project::Insert(copy.release(),row,parent,this)))
            {
                ret++;
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
    if (_redoStack.back()->redo())
    {
        _undoStack.push_back(_redoStack.back());
        _redoStack.pop_back();
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
            if (pushCommand(new Command::Project::Remove(index.row(),index.parent(),this)))
            {
                ret++;
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


bool Project::undo(
)
{
    if (_undoStack.isEmpty())
    {
        return false;
    }
    if (_undoStack.back()->undo())
    {
        _redoStack.push_back(_undoStack.back());
        _undoStack.pop_back();
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


void Project::onBlockDisplayIconChanged(
    Block::Abstract* block
)
{
    G_ASSERT(block);
    if (auto p = qobject_cast<Block::Abstract*>(block->parent()))
    {
        auto index = createIndex(p->indexOf(block),0,p);
        emit dataChanged(index,index,{Qt::DecorationRole});
    }
}


void Project::onBlockDisplayTextChanged(
    Block::Abstract* block
)
{
    G_ASSERT(block);
    if (auto p = qobject_cast<Block::Abstract*>(block->parent()))
    {
        auto index = createIndex(p->indexOf(block),0,p);
        emit dataChanged(index,index,{Qt::DisplayRole});
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


bool Project::pushCommand(
    Command::Project::Abstract* command
)
{
    G_ASSERT(command);
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


void Project::setModified(
    bool value
)
{
    if (_modified != value)
    {
        _modified = value;
        emit modifiedChanged(value);
    }
}
}
