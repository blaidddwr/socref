#include "WriterBlock.h"
#include <QtCore>
#include "BlockAbstract.h"
#include "Exception.h"
#include "ExceptionBlockLogical.h"
#include "ExceptionBlockWrite.h"
#include "ExceptionSystemFile.h"
#include "ModelMetaBlock.h"
namespace Writer {


Block::Block(
    const QString& path
    ,QObject* parent
):
    QObject(parent)
    ,_path(path)
{
}


Writer::Block& Block::operator<<(
    const ::Block::Abstract& block
)
{
    using LogicalError = Exception::Block::Logical;
    G_ASSERT(_open);
    QDir dir(_path);
    QHash<QString,const ::Block::Abstract*> blocks {{block.scope(),&block}};
    for (auto descendant: block.descendants())
    {
        static const QRegularExpression nonPrintable("[\t\r\n\a\e\f]|(ROOT)");
        auto scope = descendant->scope();
        G_ASSERT(!scope.contains(nonPrintable));
        if (blocks.contains(scope))
        {
            throw LogicalError(tr("Conflicting scope of %1 with two or more blocks.").arg(scope));
        }
        blocks.insert(scope,descendant);
    }
    for (auto i = blocks.begin();i != blocks.end();i++)
    {
        writeFile(*i.value(),dir.absoluteFilePath(i.key()+".srb"));
    }
    return *this;
}


void Block::open(
)
{
    using FileError = Exception::System::File;
    QFileInfo dirInfo(_path);
    QDir dir(_path);
    if (!dir.exists())
    {
        if (!dirInfo.dir().mkpath(dirInfo.fileName()))
        {
            throw FileError(tr("Failed making directory %1.").arg(_path));
        }
    }
    if (!dirInfo.isWritable())
    {
        throw FileError(tr("Given directory %1 is not writable.").arg(_path));
    }
    _scopes.clear();
    _open = true;
}


void Block::writeFile(
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
    out << "+CHILDREN+\n";
    for (auto child: block._children)
    {
        static const QRegularExpression nonPrintable("[\t\r\n\a\e\f]|(ROOT)");
        auto scope = child->scope();
        G_ASSERT(!scope.contains(nonPrintable));
        out << child->scope() << "\n";
        *this << *child;
    }
    if (file.error() != QFileDevice::NoError)
    {
        throw WriteError(tr("Failed writing block file %1: %2").arg(path,file.errorString()));
    }
}
}
