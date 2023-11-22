#ifndef WIDGET_BLOCK_ABSTRACT_H
#define WIDGET_BLOCK_ABSTRACT_H
#include <QWidget>
#include "Block.h"
namespace Widget {
namespace Block {




/*!
 * This is the abstract block widget class. It extends the Qt object class. It
 * exposes an abstract block implementation to the user.
 */
class Abstract:
    public QWidget
{
    Q_OBJECT
    ::Block::Abstract* _block;
    public:
    using QWidget::QWidget;


    /*!
     * Constructs this new widget with the given block implementation and
     * parent. The given block implementation must be valid and cannot be
     * destroyed during the life of this widget instance.
     *
     * @param block
     *        The block implementation.
     *
     * @param parent
     *        The parent.
     */
    public:
    Abstract(
        ::Block::Abstract* block
        ,QWidget* parent = nullptr
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's block.
     */
    public:
    ::Block::Abstract* block(
    ) const;


    /*!
     * Applies any modifications in this widget made by the user to its block
     * implementation.
     */
    public:
    virtual void applyModifications(
    ) = 0;


    /*!
     * Returns true if this widget has modifications not applied to its block
     * implementation or false otherwise.
     */
    public:
    virtual bool hasModifications(
    ) const = 0;


    /*!
     * Called when this widget's block's destroyed signal is emitted.
     */
    private slots:
    void onBlockDestroyed(
        QObject* object
    );
};
}
}


#endif
