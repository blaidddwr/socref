#ifndef COMMAND_PROJECT_INSERT_H
#define COMMAND_PROJECT_INSERT_H
#include "Block.h"
#include "CommandProjectAbstract.h"
namespace Command {
namespace Project {




/*!
 * This is a project command class. It represents an insert operation. An insert
 * operation inserts a given block into a project model.
 * 
 * This class can also act as a base class for other operations that utilize
 * inserting and removing a block, such as a remove operation. A protected
 * constructor and methods are provided to facilitate the use of this class as a
 * base project command parent class.
 */
class Insert:
    public Abstract
{
    Q_OBJECT
    Block::Abstract* _block {nullptr};
    QList<int> _parent;
    QString _description;
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
    ) override;


    public:
    virtual bool undo(
    ) override;


    /*!
     * Constructs this new instance with the given row, parent index, and
     * parent.
     * 
     * The given parent must be valid. The given row must be a valid
     * insertion/removal point for the given parent index.
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
    protected:
    Insert(
        int row
        ,const QModelIndex& parentIndex
        ,Model::Project* parent
    );


    /*!
     * Inserts this instance's block into its parent project model. If this
     * instance does not have ownership over a block then this fails.
     *
     * @return
     * True on success or false otherwise.
     */
    protected:
    bool insert(
    );


    /*!
     * Removes the child block from this instance's parent project model, taking
     * ownership of the removed block. If this instance currently has a block
     * then this fails.
     *
     * @return
     * True on success or false otherwise.
     */
    protected:
    bool remove(
    );


    /*!
     * Sets this instance's description property to the given value.
     *
     * @param value
     *        The value.
     */
    protected:
    void setDescription(
        const QString& value
    );
};
}
}


#endif
