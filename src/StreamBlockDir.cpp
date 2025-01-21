#include "StreamBlockDir.h"
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
using QApp = QCoreApplication;


BlockDir::BlockDir(
    const QString& path
    ,Language::Abstract* language
    ,int version
):
    _language(language)
    ,_dir(path)
    ,_version(version)
{
    using FileError = Exception::System::File;
    if (!_dir.exists())
    {
        if (!_dir.mkpath("."))
        {
            throw FileError(QApp::tr("Failed creating directory %1.").arg(_dir.path()));
        }
    }
    if (!_dir.isReadable())
    {
        throw FileError(QApp::tr("The directory %1 is not readable.").arg(path));
    }
}


BlockDir& BlockDir::operator<<(
    const ::Block::Abstract& block
)
{
    G_ASSERT(!qobject_cast<::Block::Abstract*>(block.parent()));
    write(block,_dir);
    return *this;
}


::Block::Abstract* BlockDir::load(
    QObject* parent
) const
{
    G_ASSERT(_language);
    G_ASSERT(_version >= 0);
    return read(
        _dir
        ,::Block::Abstract::rootFileName()
        ,parent
    );
}


QStringList BlockDir::orphanFiles(
    const ::Block::Abstract& block
) const
{
    _registry.clear();
    _paths.clear();
    insertBlockPaths(block,_dir);
    insertPaths(_dir.absolutePath());
    QStringList ret;
    for (const auto& rpath: _paths)
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
    ,const ::Block::Abstract& block
    ,bool git
)
{
    using FileError = Exception::System::File;
    using LogicalError = Exception::Block::Logical;
    using RunError = Exception::System::Run;
    _registry.clear();
    insertBlockPaths(block,_dir);
    for (const auto& path: paths)
    {
        if (_registry.contains(path))
        {
            throw LogicalError(
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
                throw RunError(
                    QApp::tr("Failed running git command: %1").arg(process.readAllStandardError())
                );
            }
        }
        else
        {
            if (!dir.remove(info.fileName()))
            {
                throw FileError(QApp::tr("Failed removing %1.").arg(path));
            }
        }
    }
}


void BlockDir::insertBlockPaths(
    const ::Block::Abstract& block
    ,const QDir& dir
) const
{
    using LogicalError = Exception::Block::Logical;
    auto fpath = dir.absoluteFilePath(block.fileName()+EXT);
    if (_registry.contains(fpath))
    {
        throw LogicalError(
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
        for (const auto& fileName: dir.entryList({QString("*")+EXT},QDir::Files))
        {
            auto cpath = dir.absoluteFilePath(fileName);
            _paths.append(cpath);
            cpath.chop(strlen(EXT));
            insertPaths(cpath);
        }
    }
}


::Block::Abstract* BlockDir::read(
    const QDir& dir
    ,const QString& fileName
    ,QObject* parent
) const
{
    using FileError = Exception::System::File;
    using ReadError = Exception::Block::Read;
    G_ASSERT(_language);
    G_ASSERT(_version >= 0);
    auto path = dir.absoluteFilePath(fileName+EXT);
    QFileInfo info(path);
    if (!info.isFile())
    {
        throw FileError(QApp::tr("Given path %1 is not a file.").arg(path));
    }
    if (!info.isReadable())
    {
        throw FileError(QApp::tr("Given file %1 is not readablee.").arg(path));
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileError(QApp::tr("Failed opening %1: %2").arg(path,file.errorString()));
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
        throw ReadError(QApp::tr("Unknown block %1").arg(blockName));
    }
    std::unique_ptr<::Block::Abstract> block(_language->create(i));
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
                throw ReadError(QApp::tr("Duplicate property element %1.").arg(name));
            }
            QString data = in.readLine();
            if (data.isNull())
            {
                throw ReadError(
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
            throw FileError(QApp::tr("No such directory %1.").arg(cpath));
        }
        if (!cdir.isReadable())
        {
            throw FileError(QApp::tr("Cannot read directory %1.").arg(cpath));
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
    const ::Block::Abstract& block
    ,const QDir& dir
)
{
    using FileError = Exception::System::File;
    using LogicalError = Exception::Block::Logical;
    using WriteError = Exception::Block::Write;
    auto path = dir.absoluteFilePath(block.fileName()+EXT);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        throw FileError(QApp::tr("Failed opening %1: %2").arg(path,file.errorString()));
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
                throw FileError(QApp::tr("Failed creating directory %1.").arg(cpath));
            }
        }
        if (!cdir.isReadable())
        {
            throw FileError(QApp::tr("Cannot read directory %1.").arg(cpath));
        }
        QSet<QString> registry;
        out << "+children\n";
        for (auto child: block._children)
        {
            auto fileName = child->fileName();
            if (registry.contains(fileName))
            {
                throw LogicalError(
                    QApp::tr("Multiple children blocks with the same file name %1.").arg(fileName)
                );
            }
            write(*child,cdir);
            out << fileName << "\n";
        }
    }
    if (file.error() != QFileDevice::NoError)
    {
        throw WriteError(QApp::tr("Failed writing block file %1: %2").arg(path,file.errorString()));
    }
}
}
