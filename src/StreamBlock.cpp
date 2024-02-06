#include "StreamBlock.h"
#include <QtCore>
#include "BlockAbstract.h"
#include "Exceptions.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "ModelMetaBlock.h"
#define EXT ".srb"
namespace Stream {
Block* Block::_instance {nullptr};


QStringList Block::deprecatedFiles(
    const ::Block::Abstract& block
    ,const QString& path
)
{
    using FileError = Exception::System::File;
    QDir dir(path);
    if (!dir.exists())
    {
        throw FileError(tr("Directory %1 does not exist.").arg(path));
    }
    QSet<QString> filenames {idToBase64(block.id())+EXT};
    for (auto descendant: block.descendants())
    {
        filenames.insert(idToBase64(descendant->id())+EXT);
    }
    QStringList ret;
    for (const auto& filename: dir.entryList({EXT},QDir::Files))
    {
        if (!filenames.contains(filename))
        {
            ret.append(filename);
        }
    }
    return ret;
}


::Block::Abstract* Block::fromDir(
    Language::Abstract* language
    ,int version
    ,const QString& path
    ,QObject* parent
)
{
    using FileError = Exception::System::File;
    G_ASSERT(language);
    QDir dir(path);
    if (!dir.isReadable())
    {
        throw FileError(tr("Directory %1 is not readable.").arg(path));
    }
    return read(
        language
        ,version
        ,dir
        ,::Block::Abstract::rootId()
        ,parent
    );
}


::Block::Abstract* Block::fromXml(
    Language::Abstract* language
    ,int version
    ,QXmlStreamReader& xml
    ,QObject* parent
)
{
    using ReadError = Exception::Block::Read;
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
        throw ReadError(tr("Unknown block %1.").arg(blockName));
    }
    QMap<QString,QVariant> map;
    std::unique_ptr<::Block::Abstract> block(language->create(i));
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
                    throw ReadError(tr("Duplicate property element %1.").arg(name));
                }
                map.insert(name,xml.readElementText());
            }
            else
            {
                auto child = fromXml(language,version,xml,block.get());
                child->setParent(nullptr);
                block->append(child);
            }
            break;
        }
        case QXmlStreamReader::EndElement:
            if (xml.name() == blockName)
            {
                block->setParent(parent);
                try
                {
                    block->loadFromMap(map,Socref_Legacy);
                }
                catch (...)
                {
                    block->setParent(nullptr);
                    throw;
                }
                return block.release();
            }
            break;
        default:
            break;
        }
    }
    return nullptr;
}


void Block::removeFiles(
    const ::Block::Abstract& block
    ,const QString& path
    ,bool git
)
{
    using FileError = Exception::System::File;
    using RunError = Exception::System::Run;
    QDir dir(path);
    for (const auto& filename: deprecatedFiles(block,path))
    {
        if (git)
        {
            QProcess process;
            process.setWorkingDirectory(dir.absolutePath());
            process.start("git",{"rm",filename});
            process.waitForFinished();
            if (process.exitCode())
            {
                throw RunError(tr("Failed running git command: %1").arg(process.readAll()));
            }
        }
        else
        {
            if (!dir.remove(filename))
            {
                throw FileError(tr("Failed removing %1 in %2.").arg(filename,path));
            }
        }
    }
}


void Block::toDir(
    ::Block::Abstract& block
    ,const QString& path
)
{
    using FileError = Exception::System::File;
    G_ASSERT(!qobject_cast<::Block::Abstract*>(block.parent()));
    QDir dir(path);
    if (!dir.exists())
    {
        if (!dir.mkpath("."))
        {
            throw FileError(tr("Failed creating directory %1.").arg(path));
        }
    }
    block.genMissingIds();
    QHash<QString,const ::Block::Abstract*> blocks {{idToBase64(block.id()),&block}};
    for (auto descendant: block.descendants())
    {
        auto filename = idToBase64(descendant->id());
        G_ASSERT(!blocks.contains(filename));
        blocks.insert(filename,descendant);
    }
    for (auto i = blocks.begin();i != blocks.end();i++)
    {
        write(*i.value(),dir.absoluteFilePath(i.key()+EXT));
    }
}


void Block::toXml(
    const ::Block::Abstract& block
    ,QXmlStreamWriter& xml
)
{
    using WriteError = Exception::Block::Write;
    xml.writeStartElement(block.meta()->name());
    auto map = block.saveToMap();
    for (auto i = map.begin();i != map.end();i++)
    {
        xml.writeTextElement("_"+i.key(),i.value().toString());
    }
    for (auto child: block._children)
    {
        toXml(*child,xml);
    }
    xml.writeEndElement();
    if (xml.hasError())
    {
        throw WriteError(xml.device()->errorString());
    }
}


quint32 Block::base64ToId(
    const QString& value
)
{
    auto data = QByteArray::fromBase64(value.toUtf8().replace("!","/"));
    G_ASSERT(data.size() == sizeof(quint32));
    return *reinterpret_cast<quint32*>(data.data());
}


QString Block::idToBase64(
    quint32 id
)
{
    auto ret = QByteArray(reinterpret_cast<char*>(&id),sizeof(quint32));
    ret = ret.toBase64(QByteArray::Base64Encoding|QByteArray::OmitTrailingEquals);
    return ret.replace("/","!");
}


::Block::Abstract* Block::read(
    Language::Abstract* language
    ,int version
    ,const QDir& dir
    ,quint32 id
    ,QObject* parent
)
{
    using FileError = Exception::System::File;
    using ReadError = Exception::Block::Read;
    G_ASSERT(language);
    auto path = dir.absoluteFilePath(idToBase64(id)+EXT);
    QFileInfo info(path);
    if (!info.isFile())
    {
        throw FileError(tr("Given path %1 is not a file.").arg(path));
    }
    if (!info.isReadable())
    {
        throw FileError(tr("Given file %1 is not readablee.").arg(path));
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileError(tr("Failed opening %1: %2").arg(path,file.errorString()));
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
        throw ReadError(tr("Unknown block %1").arg(blockName));
    }
    std::unique_ptr<::Block::Abstract> block(language->create(i));
    block->_id = id;
    QMap<QString,QVariant> map;
    int lineNumber = 1;
    QString line = in.readLine();
    while (!line.isNull())
    {
        if (line.front() == ':')
        {
            QString name = line.mid(1);
            if (map.contains(name))
            {
                throw ReadError(tr("Duplicate property element %1.").arg(name));
            }
            QString data = in.readLine();
            if (data.isNull())
            {
                static const QString msg = tr(
                    "Failed reading %1: Expected data after line %2, got EOF instead."
                );
                throw ReadError(msg.arg(path).arg(lineNumber));
            }
            lineNumber++;
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
        auto child = read(language,version,dir,base64ToId(line),block.get());
        child->setParent(nullptr);
        block->append(child);
        line = in.readLine();
    }
    block->setParent(parent);
    try
    {
        block->loadFromMap(map,version);
    }
    catch (...)
    {
        block->setParent(nullptr);
        throw;
    }
    return block.release();
}


void Block::write(
    const ::Block::Abstract& block
    ,const QString& path
)
{
    using FileError = Exception::System::File;
    using WriteError = Exception::Block::Write;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        throw FileError(tr("Failed opening %1: %2").arg(path,file.errorString()));
    }
    QTextStream out(&file);
    out << block.meta()->name() << "\n";
    auto map = block.saveToMap();
    for (auto i = map.begin();i != map.end();i++)
    {
        auto data = i.value().toString();
        data.replace("\\","\\\\");
        data.replace("\n","\\n");
        out << ":"+i.key() << "\n" << data << "\n";
    }
    out << "+children\n";
    for (auto child: block._children)
    {
        out << idToBase64(child->id()) << "\n";
    }
    if (file.error() != QFileDevice::NoError)
    {
        throw WriteError(tr("Failed writing block file %1: %2").arg(path,file.errorString()));
    }
}
}
