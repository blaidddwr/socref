#ifndef COMMAND_PROJECT_REMOVE_H
#define COMMAND_PROJECT_REMOVE_H
#include "CommandProjectInsert.h"
namespace Command {
namespace Project {




/*!
 * This is a project command class. It represents a remove operation. A remove
 * operation removes a given block from a project model.
 */
class Remove:
    public Insert
{
    Q_OBJECT


    /*!
     * Constructs this new instance with the given row, parent index, and
     * parent.
     * 
     * The given parent must be valid. The given row must be a valid removal
     * point for the given parent index.
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
    Remove(
        int row
        ,const QModelIndex& parentIndex
        ,Model::Project* parent
    );


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
