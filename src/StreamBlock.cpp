#include "StreamBlock.h"
#include <QtCore>
#include "BlockAbstract.h"
#include "ExceptionBlockLogical.h"
#include "ExceptionBlockRead.h"
#include "ExceptionBlockWrite.h"
#include "ExceptionSystemFile.h"
#include "ExceptionSystemRun.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "ModelMetaBlock.h"
#include "gassert.h"
#define EXT ".srb"
namespace Stream {
Block* Block::_instance {nullptr};


::Block::Abstract* Block::fromDir(
    Language::Abstract* language
    ,int version
    ,const QString& path
    ,QObject* parent
) const
{
    using FileError = Exception::System::File;
    G_ASSERT(language);
    QDir dir(path);
    if (!dir.exists())
    {
        throw FileError(tr("No such directory %1.").arg(path));
    }
    if (!dir.isReadable())
    {
        throw FileError(tr("Directory %1 is not readable.").arg(path));
    }
    return read(
        language
        ,version
        ,dir
        ,::Block::Abstract::rootFileName()
        ,parent
    );
}


::Block::Abstract* Block::fromXml(
    Language::Abstract* language
    ,int version
    ,QXmlStreamReader& xml
    ,QObject* parent
) const
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


Block* Block::instance(
)
{
    if (!_instance)
    {
        _instance = new Block(QCoreApplication::instance());
    }
    return _instance;
}


QStringList Block::orphanFiles(
    const ::Block::Abstract& block
    ,const QString& path
) const
{
    using FileError = Exception::System::File;
    QDir dir(path);
    if (!dir.exists())
    {
        throw FileError(tr("Directory %1 does not exist.").arg(path));
    }
    if (!dir.isReadable())
    {
        throw FileError(tr("Directory %1 is not readable.").arg(path));
    }
    QSet<QString> registry;
    insertBlockPaths(registry,block,path);
    QStringList paths;
    insertPaths(paths,path);
    QStringList ret;
    for (const auto& rpath: paths)
    {
        if (!registry.contains(rpath))
        {
            ret.append(rpath);
        }
    }
    return ret;
}


void Block::removeOrphanFiles(
    const QStringList& filePaths
    ,const ::Block::Abstract& block
    ,const QString& path
    ,bool git
) const
{
    using FileError = Exception::System::File;
    using LogicalError = Exception::Block::Logical;
    using RunError = Exception::System::Run;
    QSet<QString> registry;
    insertBlockPaths(registry,block,path);
    for (const auto& path: filePaths)
    {
        if (registry.contains(path))
        {
            throw LogicalError(
                tr("Given path %1 is protected(NOT orpahned) block file.").arg(path)
            );
        }
        QFileInfo info(path);
        auto dir = info.dir();
        if (git)
        {
            QProcess process;
            process.setWorkingDirectory(dir.path());
            process.start("git",{"rm",info.fileName()});
            process.waitForFinished();
            if (process.exitCode())
            {
                throw RunError(
                    tr("Failed running git command: %1").arg(process.readAllStandardError())
                );
            }
        }
        else
        {
            if (!dir.remove(info.fileName()))
            {
                throw FileError(tr("Failed removing %1.").arg(path));
            }
        }
    }
}


void Block::toDir(
    ::Block::Abstract& block
    ,const QString& path
) const
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
    if (!dir.isReadable())
    {
        throw FileError(tr("Cannot read directory %1.").arg(path));
    }
    write(block,dir);
}


void Block::toXml(
    const ::Block::Abstract& block
    ,QXmlStreamWriter& xml
) const
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


Block::Block(
    QObject* parent
):
    QObject(parent)
{
}


void Block::insertBlockPaths(
    QSet<QString>& registry
    ,const ::Block::Abstract& block
    ,const QString& path
) const
{
    using LogicalError = Exception::Block::Logical;
    auto fpath = path+"/"+block.fileName()+EXT;
    if (registry.contains(fpath))
    {
        throw LogicalError(tr("Multiple children blocks with the same file path %1.").arg(fpath));
    }
    registry.insert(fpath);
    if (block.size() > 0)
    {
        for (auto child: block._children)
        {
           insertBlockPaths(registry,*child,path+"/"+block.fileName());
        }
    }
}


void Block::insertPaths(
    QStringList& paths
    ,const QString& path
) const
{
    QDir dir(path);
    if (dir.isReadable())
    {
        for (const auto& fileName: dir.entryList({QString("*")+EXT},QDir::Files))
        {
           auto cpath = path+"/"+fileName;
           paths.append(cpath);
           cpath.chop(strlen(EXT));
           insertPaths(paths,cpath);
        }
    }
}


::Block::Abstract* Block::read(
    Language::Abstract* language
    ,int version
    ,const QDir& dir
    ,const QString& fileName
    ,QObject* parent
) const
{
    using FileError = Exception::System::File;
    using ReadError = Exception::Block::Read;
    G_ASSERT(language);
    auto path = dir.absoluteFilePath(fileName+EXT);
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
                throw ReadError(
                    tr("Failed reading %1: Expected data after line %2, got EOF instead.")
                        .arg(path)
                        .arg(lineNumber)
                );
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
    if (!line.isNull())
    {
        auto cpath = dir.absoluteFilePath(fileName);
        QDir cdir(cpath);
        if (!cdir.exists())
        {
            throw FileError(tr("No such directory %1.").arg(cpath));
        }
        if (!cdir.isReadable())
        {
            throw FileError(tr("Cannot read directory %1.").arg(cpath));
        }
        while (!line.isNull())
        {
            auto child = read(language,version,cdir,line,block.get());
            child->setParent(nullptr);
            block->append(child);
            line = in.readLine();
        }
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
    ,const QDir& dir
) const
{
    using FileError = Exception::System::File;
    using LogicalError = Exception::Block::Logical;
    using WriteError = Exception::Block::Write;
    auto path = dir.absoluteFilePath(block.fileName()+EXT);
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
    if (block.size() > 0)
    {
        auto cpath = dir.absoluteFilePath(block.fileName());
        QDir cdir(cpath);
        if (!cdir.exists())
        {
            if (!cdir.mkpath("."))
            {
                throw FileError(tr("Failed creating directory %1.").arg(cpath));
            }
        }
        if (!cdir.isReadable())
        {
            throw FileError(tr("Cannot read directory %1.").arg(cpath));
        }
        QSet<QString> registry;
        out << "+children\n";
        for (auto child: block._children)
        {
            auto fileName = child->fileName();
            if (registry.contains(fileName))
            {
                throw LogicalError(
                    tr("Multiple children blocks with the same file name %1.").arg(fileName)
                );
            }
            write(*child,cdir);
            out << fileName << "\n";
        }
    }
    if (file.error() != QFileDevice::NoError)
    {
        throw WriteError(tr("Failed writing block file %1: %2").arg(path,file.errorString()));
    }
}
}
