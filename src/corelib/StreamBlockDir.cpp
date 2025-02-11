#include "StreamBlockDir.h"
#include <QtCore>
#include "AbstractBlock.h"
#include "AbstractLanguage.h"
#include "Exception.h"
#include "Global.h"
#include "ModelMetaBlock.h"
#define EXT ".srb"
namespace Stream {
using QApp = QCoreApplication;


BlockDir::BlockDir(
    const QString& path
    ,AbstractLanguage* language
    ,int version
):
    _language(language)
    ,_dir(path)
    ,_version(version)
{
    using FileSystem = Exception::FileSystem;
    if (!_dir.exists())
    {
        if (!_dir.mkpath("."))
        {
            throw FileSystem(QApp::tr("Failed creating directory %1.").arg(_dir.path()));
        }
    }
    if (!_dir.isReadable())
    {
        throw FileSystem(QApp::tr("The directory %1 is not readable.").arg(path));
    }
}


BlockDir& BlockDir::operator<<(
    const AbstractBlock& block
)
{
    Q_ASSERT(!qobject_cast<AbstractBlock*>(block.parent()));
    write(block,_dir);
    return *this;
}


AbstractBlock* BlockDir::load(
    QObject* parent
) const
{
    Q_ASSERT(_language);
    Q_ASSERT(_version >= 0);
    return read(
        _dir
        ,AbstractBlock::rootFileName()
        ,parent
        );
}


QStringList BlockDir::orphanFiles(
    const AbstractBlock& block
) const
{
    _registry.clear();
    _paths.clear();
    insertBlockPaths(block,_dir);
    insertPaths(_dir.absolutePath());
    QStringList ret;
    for (const auto& rpath: std::as_const(_paths))
    {
        if (!_registry.contains(rpath))
        {
            ret.append(rpath);
        }
    }
    return ret;
}


void BlockDir::removeOrphanFiles(
    const QStringList& paths
    ,const AbstractBlock& block
    ,bool git
)
{
    using FileSystem = Exception::FileSystem;
    using LogicalBlock = Exception::LogicalBlock;
    using RunSystem = Exception::RunSystem;
    _registry.clear();
    insertBlockPaths(block,_dir);
    for (const auto& path: paths)
    {
        if (_registry.contains(path))
        {
            throw LogicalBlock(
                QApp::tr("Given path %1 is protected(NOT orpahned) block file.").arg(path)
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
                throw RunSystem(
                    QApp::tr("Failed running git command: %1").arg(process.readAllStandardError())
                    );
            }
        }
        else
        {
            if (!dir.remove(info.fileName()))
            {
                throw FileSystem(QApp::tr("Failed removing %1.").arg(path));
            }
        }
    }
}


void BlockDir::insertBlockPaths(
    const AbstractBlock& block
    ,const QDir& dir
) const
{
    using LogicalBlock = Exception::LogicalBlock;
    auto fpath = dir.absoluteFilePath(block.fileName()+EXT);
    if (_registry.contains(fpath))
    {
        throw LogicalBlock(
            QApp::tr("Multiple children blocks with the same file path %1.").arg(fpath)
            );
    }
    _registry.insert(fpath);
    if (block.size() > 0)
    {
        for (auto child: block._children)
        {
            insertBlockPaths(*child,dir.absoluteFilePath(block.fileName()));
        }
    }
}


void BlockDir::insertPaths(
    const QDir& dir
) const
{
    if (dir.isReadable())
    {
        auto const list = dir.entryList({QString("*")+EXT},QDir::Files);
        for (const auto& fileName: list)
        {
            auto cpath = dir.absoluteFilePath(fileName);
            _paths.append(cpath);
            cpath.chop(strlen(EXT));
            insertPaths(cpath);
        }
    }
}


AbstractBlock* BlockDir::read(
    const QDir& dir
    ,const QString& fileName
    ,QObject* parent
) const
{
    using FileSystem = Exception::FileSystem;
    using ReadBlock = Exception::ReadBlock;
    Q_ASSERT(_language);
    Q_ASSERT(_version >= 0);
    auto path = dir.absoluteFilePath(fileName+EXT);
    QFileInfo info(path);
    if (!info.isFile())
    {
        throw FileSystem(QApp::tr("Given path %1 is not a file.").arg(path));
    }
    if (!info.isReadable())
    {
        throw FileSystem(QApp::tr("Given file %1 is not readablee.").arg(path));
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileSystem(QApp::tr("Failed opening %1: %2").arg(path,file.errorString()));
    }
    QTextStream in(&file);
    auto blockName = in.readLine();
    if (_version == Socref_Legacy)
    {
        blockName = blockName.toLower();
    }
    auto i = _language->indexFromName(blockName);
    if (i == -1)
    {
        throw ReadBlock(QApp::tr("Unknown block %1").arg(blockName));
    }
    std::unique_ptr<AbstractBlock> block(_language->createBlock(i));
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
                throw ReadBlock(QApp::tr("Duplicate property element %1.").arg(name));
            }
            QString data = in.readLine();
            if (data.isNull())
            {
                throw ReadBlock(
                    QApp::tr("Failed reading %1: Expected data after line %2, got EOF instead.")
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
            throw FileSystem(QApp::tr("No such directory %1.").arg(cpath));
        }
        if (!cdir.isReadable())
        {
            throw FileSystem(QApp::tr("Cannot read directory %1.").arg(cpath));
        }
        while (!line.isNull())
        {
            auto child = read(cdir,line,block.get());
            child->setParent(nullptr);
            block->append(child);
            line = in.readLine();
        }
    }
    block->setParent(parent);
    try
    {
        block->loadFromMap(map,_version);
    }
    catch (...)
    {
        block->setParent(nullptr);
        throw;
    }
    return block.release();
}


void BlockDir::write(
    const AbstractBlock& block
    ,const QDir& dir
)
{
    using FileSystem = Exception::FileSystem;
    using LogicalBlock = Exception::LogicalBlock;
    using WriteBlock = Exception::WriteBlock;
    auto path = dir.absoluteFilePath(block.fileName()+EXT);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        throw FileSystem(QApp::tr("Failed opening %1: %2").arg(path,file.errorString()));
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
                throw FileSystem(QApp::tr("Failed creating directory %1.").arg(cpath));
            }
        }
        if (!cdir.isReadable())
        {
            throw FileSystem(QApp::tr("Cannot read directory %1.").arg(cpath));
        }
        QSet<QString> registry;
        out << "+children\n";
        for (auto child: block._children)
        {
            auto fileName = child->fileName();
            if (registry.contains(fileName))
            {
                throw LogicalBlock(
                    QApp::tr("Multiple children blocks with the same file name %1.").arg(fileName)
                    );
            }
            write(*child,cdir);
            out << fileName << "\n";
        }
    }
    if (file.error() != QFileDevice::NoError)
    {
        throw WriteBlock(QApp::tr("Failed writing block file %1: %2").arg(path,file.errorString()));
    }
}
}
