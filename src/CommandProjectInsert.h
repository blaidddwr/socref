#ifndef COMMAND_PROJECT_INSERT_H
#define COMMAND_PROJECT_INSERT_H
#include "Block.h"
#include "CommandProjectAbstract.h"
namespace Command {
namespace Project {




/*!
 * This is a project command class. It represents an insert operation. An insert
 * operation inserts a given block into a project model.
 */
class Insert:
    public Command::Project::Abstract
{
    Q_OBJECT
    Block::Abstract* _block;
    QList<int> _parent;
    QString _blockScope;
    QString _parentScope;
    int _row;


    /*!
     * Constructs this new instance with the given block, row, parent index, and
     * parent.
     * 
     * The given block and parent must be valid. The given row must be a valid
     * insertion point for the given parent index.
     * 
     * This new instance takes ownership of the given block.
     *
     * @param block
     *        The block.
     *
     * @param row
     *        The row.
     *
     * @param parentIndex
     *        The parent index.
     *
     * @param parent
     *        The parent.
     */
    public:
    Insert(
        Block::Abstract* block
        ,int row
        ,const QModelIndex& parentIndex
        ,Model::Project* parent
    );


    public:
    virtual QString description(
    ) const override final;


    public:
    virtual bool redo(
    ) override final;


    public:
    virtual bool undo(
    ) override final;
};
}
}


#endif
