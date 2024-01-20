#ifndef COMMAND_PROJECT_SET_H
#define COMMAND_PROJECT_SET_H
#include <QHash>
#include <QVariant>
#include "CommandProjectAbstract.h"
namespace Command {
namespace Project {




/*!
 * This is a project command class. It represents a set operation. A set
 * operation sets the state of a given block in a project model.
 */
class Set:
    public Command::Project::Abstract
{
    Q_OBJECT
    QHash<QString,QVariant> _nextState;
    QHash<QString,QVariant> _previousState;
    QList<int> _location;


    /*!
     * Constructs this new instance with the given previous state, next state,
     * index, and parent.
     * 
     * The given parent must be valid. The given index must be valid and resolve
     * to a valid block contained in the given parent project model.
     *
     * @param previousState
     *        The previous state.
     *
     * @param nextState
     *        The next state.
     *
     * @param index
     *        The index.
     *
     * @param parent
     *        The parent.
     */
    public:
    Set(
        const QHash<QString,QVariant>& previousState
        ,const QHash<QString,QVariant>& nextState
        ,const QModelIndex& index
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
