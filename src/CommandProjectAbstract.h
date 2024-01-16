#ifndef COMMAND_PROJECT_ABSTRACT_H
#define COMMAND_PROJECT_ABSTRACT_H
#include <QObject>
#include "Model.h"
namespace Command {
namespace Project {




/*!
 * This is an abstract class. It is a project command which makes a change to
 * its parent project model by calling its redo interface, or undoing its change
 * by calling its undo interface. It provides the ability for a project model
 * class to implement undo or redo operations given to it.
 * 
 * Its single property is description. The description provides a user readable
 * description of a project command.
 */
class Abstract:
    public QObject
{
    Q_OBJECT


    /*!
     * Constructs this new instance with the given parent. The given parent must
     * be valid.
     *
     * @param parent
     *        The parent.
     */
    public:
    Abstract(
        Model::Project* parent
    );


    /*!
     * Getter method.
     *
     * @return
     * This instance's description property.
     */
    public:
    virtual QString description(
    ) const = 0;


    /*!
     * Getter method.
     *
     * @return
     * This instance's parent project model.
     */
    public:
    Model::Project& project(
    ) const;


    /*!
     * Modifies this instance's parent project model with its project command.
     */
    public:
    virtual bool redo(
    ) = 0;


    /*!
     * Reverts the modification done on this instance's parent project model by
     * its project command.
     */
    public:
    virtual bool undo(
    ) = 0;


    /*!
     * Converts the given index to a list of row indexes. The given index must
     * be a valid index generated from this instance's parent project model.
     *
     * @param index
     *        The index.
     *
     * @return
     * The list of row indexes.
     */
    protected:
    QList<int> convertIndexToList(
        const QModelIndex& index
    );


    /*!
     * Converts the given list of row indexes to a valid Qt model index. The
     * given list of row indexes must be able to generate a valid Qt model index
     * from this instance's parent project model.
     *
     * @param list
     *        The list of row indexes.
     *
     * @return
     * The index.
     */
    protected:
    QModelIndex convertListToIndex(
        const QList<int>& list
    );
};
}
}


#endif
