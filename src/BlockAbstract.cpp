#include "BlockAbstract.h"
#include <QtCore>
#include "Exception.h"
#include "ExceptionBlockLogical.h"
#include "ExceptionBlockRead.h"
#include "ExceptionSystemFile.h"
#include "LanguageAbstract.h"
#include "Global.h"
#include "ModelMetaBlock.h"
namespace Block {


Abstract::Abstract(
    Model::Meta::Block* meta
    ,QObject* parent
):
    QObject(parent)
    ,_meta(meta)
{
    G_ASSERT(meta);
    connect(meta,&QObject::destroyed,this,&Abstract::onMetaDestroyed);
}


void Abstract::append(
    Block::Abstract* block
)
{
    G_ASSERT(block);
    G_ASSERT(meta()->allowList().contains(block->meta()->index()));
    G_ASSERT(!_children.contains(block));
    G_ASSERT(!qobject_cast<Abstract*>(block->parent()));
    block->setParent(this);
    connect(block,&QObject::destroyed,this,&Abstract::onChildDestroyed);
    _children.append(block);
    block->addEvent();
}


QList<Block::Abstract*> Abstract::descendants(
) const
{
    QList<Block::Abstract*> ret = _children;
    for (auto child: _children)
    {
        ret += child->descendants();
    }
    return ret;
}


Block::Abstract* Abstract::fromDir(
    Language::Abstract* language
    ,int version
    ,const QString& path
    ,QObject* parent
)
{
    G_ASSERT(language);
    QDir dir(path);
    if (!dir.isReadable())
    {
        throw Exception::System::File(tr("Directory %1 is not readable.").arg(path));
    }
    return read(language,version,dir.absoluteFilePath("ROOT.srb"),parent);
}


Block::Abstract* Abstract::fromXml(
    Language::Abstract* language
    ,int version
    ,QXmlStreamReader& xml
    ,QObject* parent
)
{
    G_ASSERT(language);
    auto blockName = xml.name().toString();
    int i = -1;
    if (version == Socref_Legacy)
    {
        i = language->indexFromName(blockName.toLower());
    }
    else
    {
        i = language->indexFromName(blockName);
    }
    if (i == -1)
    {
        throw Exception::Block::Read(tr("Unknown block %1.").arg(blockName));
    }
    QMap<QString,QVariant> map;
    std::unique_ptr<Abstract> block(language->create(i));
    while (!xml.atEnd())
    {
        xml.readNext();
        switch (xml.tokenType())
        {
        case QXmlStreamReader::StartElement:
        {
            auto name = xml.name().toString();
            bool isProp = false;
            if (version == Socref_Legacy)
            {
                if (name.first(4) == "__p_")
                {
                    name = name.mid(4);
                    isProp = true;
                }
            }
            else
            {
                if (name.at(0) == '_')
                {
                    name = name.mid(1);
                    isProp = true;
                }
            }
            if (isProp)
            {
                if (map.contains(name))
                {
                    throw Exception::Block::Read(tr("Duplicate property element %1.").arg(name));
                }
                map.insert(name,xml.readElementText());
            }
            else
            {
                block->append(fromXml(language,version,xml));
            }
            break;
        }
        case QXmlStreamReader::EndElement:
            if (xml.name() == blockName)
            {
                block->loadFromMap(map,Socref_Legacy);
                block->setParent(parent);
                return block.release();
            }
            break;
        default:
            break;
        }
    }
    return nullptr;
}


Block::Abstract* Abstract::get(
    int index
) const
{
    G_ASSERT(index >= 0);
    G_ASSERT(index < _children.size());
    return _children.at(index);
}


int Abstract::indexOf(
    Block::Abstract* block
)
{
    return _children.indexOf(block);
}


void Abstract::insert(
    int index
    ,Block::Abstract* block
)
{
    G_ASSERT(block);
    G_ASSERT(index >= 0);
    G_ASSERT(index <= _children.size());
    G_ASSERT(meta()->allowList().contains(block->meta()->index()));
    G_ASSERT(!_children.contains(block));
    G_ASSERT(!qobject_cast<Abstract*>(block->parent()));
    block->setParent(this);
    connect(block,&QObject::destroyed,this,&Abstract::onChildDestroyed);
    _children.insert(index,block);
    block->addEvent();
}


Model::Meta::Block* Abstract::meta(
) const
{
    G_ASSERT(_meta);
    return _meta;
}


void Abstract::move(
    int from
    ,int to
)
{
    G_ASSERT(from >= 0);
    G_ASSERT(from < _children.size());
    G_ASSERT(to >= 0);
    G_ASSERT(to <= _children.size());
    _children.move(from,to);
}


int Abstract::size(
) const
{
    return _children.size();
}


Block::Abstract* Abstract::take(
    int index
)
{
    G_ASSERT(index >= 0);
    G_ASSERT(index < _children.size());
    _children.at(index)->removeEvent();
    auto ret = _children.takeAt(index);
    disconnect(ret,&QObject::destroyed,this,&Abstract::onChildDestroyed);
    ret->setParent(nullptr);
    return ret;
}


void Abstract::toDir(
    const QString& path
) const
{
    G_ASSERT(!qobject_cast<Abstract*>(parent()));
    G_ASSERT(scope() == "ROOT");
    QHash<QString,const Abstract*> blocks;
    for (auto descendant: descendants())
    {
        static const QRegularExpression nonPrintable("[\t\r\n\a\e\f]|(ROOT)");
        auto scope = descendant->scope();
        G_ASSERT(!scope.contains(nonPrintable));
        auto path = scope+".srb";
        if (blocks.contains(path))
        {
            throw Exception::Block::Logical(
                tr("Conflicting scope of %1 with two or more blocks.").arg(descendant->scope())
            );
        }
        blocks.insert(path,descendant);
    }
    QFileInfo dirInfo(path);
    QDir dir(path);
    if (!dir.exists())
    {
        if (!dirInfo.dir().mkdir(dirInfo.fileName()))
        {
            throw Exception::System::File(tr("Failed making directory %1.").arg(path));
        }
    }
    if (!dirInfo.isWritable())
    {
        throw Exception::System::File(tr("Given directory %1 is not writable.").arg(path));
    }
    for (const auto& info: dir.entryInfoList({"*.srb"}))
    {
        if (!info.isWritable())
        {
            throw Exception::System::File(
                tr("Block file %1 is not writable.").arg(info.fileName())
            );
        }
        if (!blocks.contains(info.fileName()))
        {
            if (!dir.remove(info.fileName()))
            {
                throw Exception::System::File(
                    tr("Failed removing trash block file %1.").arg(info.fileName())
                );
            }
        }
    }
    write(dir.absoluteFilePath("ROOT.srb"));
    for (auto i = blocks.begin();i != blocks.end();i++)
    {
        i.value()->write(dir.absoluteFilePath(i.key()));
    }
}


void Abstract::toXml(
    QXmlStreamWriter& xml
) const
{
    xml.writeStartElement(meta()->name());
    auto map = saveToMap();
    for (auto i = map.begin();i != map.end();i++)
    {
        xml.writeTextElement("_"+i.key(),i.value().toString());
    }
    for (auto child: _children)
    {
        child->toXml(xml);
    }
    xml.writeEndElement();
}


void Abstract::addEvent(
)
{
}


void Abstract::removeEvent(
)
{
}


void Abstract::onChildDestroyed(
    QObject* object
)
{
    auto index = _children.indexOf(object);
    if (index != -1)
    {
        _children.removeAt(index);
    }
}


void Abstract::onMetaDestroyed(
    QObject* object
)
{
    if (_meta == object)
    {
        _meta = nullptr;
    }
}


Block::Abstract* Abstract::read(
    Language::Abstract* language
    ,int version
    ,const QString& path
    ,QObject* parent
)
{
    G_ASSERT(language);
    QFileInfo info(path);
    if (!info.isFile())
    {
        throw Exception::System::File(tr("Given path %1 is not a file.").arg(path));
    }
    if (!info.isReadable())
    {
        throw Exception::System::File(tr("Given file %1 is not readablee.").arg(path));
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw Exception::System::File(tr("Failed opening %1: %2").arg(path,file.errorString()));
    }
    QTextStream in(&file);
    auto blockName = in.readLine();
    if (version == Socref_Legacy)
    {
        blockName = blockName.toLower();
    }
    auto i = language->indexFromName(blockName);
    if (i == -1)
    {
        throw Exception::Block::Read(tr("Unknown block %1").arg(blockName));
    }
    std::unique_ptr<Abstract> block(language->create(i));
    QMap<QString,QVariant> map;
    int lineNumber = 1;
    QString line = in.readLine();
    auto dir = info.dir();
    while (!line.isNull())
    {
        if (line.front() == ':')
        {
            QString name = line.mid(1);
            if (map.contains(name))
            {
                throw Exception::Block::Read(tr("Duplicate property element %1.").arg(name));
            }
            QString data = in.readLine();
            if (data.isNull())
            {
                static const QString msg = tr(
                    "Failed reading %1: Expected data after line %2, got EOF instead."
                );
                throw Exception::Block::Read(msg.arg(path).arg(lineNumber));
            }
            data.replace("\\\\","\\");
            data.replace("\\n","\n");
            map.insert(name,data);
        }
        else if (line.front() == '+')
        {
            line = in.readLine();
            break;
        }
        line = in.readLine();
        lineNumber++;
    }
    while (!line.isNull())
    {
        QString path = dir.absoluteFilePath(line+".srb");
        block->append(read(language,version,path));
        line = in.readLine();
    }
    block->loadFromMap(map,version);
    block->setParent(parent);
    return block.release();
}


void Abstract::write(
    const QString& path
) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        throw Exception::System::File(tr("Failed opening %1: %2").arg(path,file.errorString()));
    }
    QTextStream out(&file);
    out << meta()->name() << "\n";
    auto map = saveToMap();
    for (auto i = map.begin();i != map.end();i++)
    {
        auto data = i.value().toString();
        data.replace("\\","\\\\");
        data.replace("\n","\\n");
        out << ":"+i.key() << "\n" << data << "\n";
    }
    out << "+CHILDREN+\n";
    for (auto child: _children)
    {
        static const QRegularExpression nonPrintable("[\t\r\n\a\e\f]|(ROOT)");
        auto scope = child->scope();
        G_ASSERT(!scope.contains(nonPrintable));
        out << child->scope() << "\n";
    }
    if (file.error() != QFileDevice::NoError)
    {
        throw Exception::System::File(
            tr("Failed writing block file %1: %2").arg(path,file.errorString())
        );
    }
}
}
