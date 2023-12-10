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
    ASSERT(meta);
    connect(meta,&QObject::destroyed,this,&Abstract::onMetaDestroyed);
}


void Abstract::append(
    Block::Abstract* block
)
{
    ASSERT(block);
    ASSERT(meta()->allowList().contains(block->meta()->index()));
    ASSERT(!_children.contains(block));
    ASSERT(!qobject_cast<Abstract*>(block->parent()));
    block->setParent(this);
    _children.append(block);
    block->addEvent();
}


Block::Abstract* Abstract::fromDir(
    Language::Abstract* language
    ,int version
    ,const QString& path
    ,QObject* parent
)
{
    ASSERT(language);
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
    ASSERT(language);
    auto blockName = xml.name().toString();
    if (version == Socref_Legacy)
    {
        blockName = blockName.toLower();
    }
    auto i = language->indexFromName(blockName);
    if (i == -1)
    {
        throw Exception::Block::Read(tr("Unknown block %1").arg(blockName));
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
                    throw Exception::Block::Read(tr("Duplicate property element %1").arg(name));
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
    ASSERT(index >= 0);
    ASSERT(index < _children.size());
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
    ASSERT(block);
    ASSERT(meta()->allowList().contains(block->meta()->index()));
    ASSERT(!_children.contains(block));
    ASSERT(!qobject_cast<Abstract*>(block->parent()));
    block->setParent(this);
    _children.insert(index,block);
    block->addEvent();
}


Model::Meta::Block* Abstract::meta(
) const
{
    ASSERT(_meta);
    return _meta;
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
    ASSERT(index >= 0);
    ASSERT(index < _children.size());
    _children.at(index)->removeEvent();
    auto ret = _children.takeAt(index);
    ret->setParent(nullptr);
    return ret;
}


void Abstract::toDir(
    const QString& path
) const
{
    ASSERT(qobject_cast<Abstract*>(parent()));
    ASSERT(scope() == "ROOT");
    QHash<QString,const Abstract*> blocks;
    std::function<void(const Abstract*)> getDescendants;
    getDescendants = [getDescendants,&blocks](const Abstract* block) {
        for (auto child: block->_children)
        {
            static const QRegularExpression nonPrintable("[\t\r\n\a\e\f]|(ROOT)");
            auto scope = child->scope();
            ASSERT(!scope.contains(nonPrintable));
            auto childPath = scope+".srb";
            if (blocks.contains(childPath))
            {
                throw Exception::Block::Logical(
                    tr("Conflicting scope of %1 with two or more blocks.").arg(child->scope())
                );
            }
            blocks.insert(childPath,child);
            getDescendants(child);
        }
    };
    getDescendants(this);
    QFileInfo dirInfo(path);
    if (!dirInfo.isDir())
    {
        throw Exception::System::File(tr("Given path %1 is not a directory.").arg(path));
    }
    if (!dirInfo.isWritable())
    {
        throw Exception::System::File(tr("Given directory %1 is not writable.").arg(path));
    }
    QDir dir(path);
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
    ASSERT(language);
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
            QString name = line.remove(1,1);
            if (map.contains(name))
            {
                throw Exception::Block::Read(tr("Duplicate property element %1").arg(name));
            }
            QString data = in.readLine();
            if (data.isNull())
            {
                static const QString msg = tr(
                    "Failed reading %1: Expected data after line %2, got EOF instead."
                );
                throw Exception::Block::Read(msg.arg(path).arg(lineNumber));
            }
            data = data.replace("\\\\","\\");
            data = data.replace("\\n","\n");
            map.insert(name,data);
        }
        else if (line.front() == '+')
        {
            QString path = dir.absoluteFilePath(line.remove(1,1)+".srb");
            block->append(read(language,version,path));
        }
        line = in.readLine();
        lineNumber++;
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
        data = data.replace("\\","\\\\");
        data = data.replace("\n","\\n");
        out << ":"+i.key() << "\n" << data << "\n";
    }
    out << "+CHILDREN+\n";
    for (auto child: _children)
    {
        static const QRegularExpression nonPrintable("[\t\r\n\a\e\f]|(ROOT)");
        auto scope = child->scope();
        ASSERT(!scope.contains(nonPrintable));
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
