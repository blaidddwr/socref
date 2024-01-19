#ifndef COMMAND_PROJECT_MOVE_H
#define COMMAND_PROJECT_MOVE_H
#include "CommandProjectAbstract.h"
namespace Command {
namespace Project {




/*!
 * This is a project command class. It represents a move operation. A move
 * operation moves a child block from a given index/row to another given
 * index/row in a project model.
 */
class Move:
    public Command::Project::Abstract
{
    Q_OBJECT
    QList<int> _parent;
    QString _description;
    int _from;
    int _to;


    /*!
     * Constructs this new instance with the given from row, to row, parent
     * index, and parent.
     * 
     * The given from and to rows must make a valid move operation within the
     * given parent index.
     *
     * @param from
     *        The from row.
     *
     * @param to
     *        The to row.
     *
     * @param parentIndex
     *        The parent index.
     *
     * @param parent
     *        The parent.
     */
    public:
    Move(
        int from
        ,int to
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


    /*!
     * Moves the child block at the given from index to the given to index in
     * this instance's parent block.
     *
     * @param from
     *        The from index.
     *
     * @param to
     *        The to index.
     *
     * @return
     * True on success or false otherwise.
     */
    private:
    bool move(
        int from
        ,int to
    );
};
}
}


#endif
