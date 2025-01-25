#ifndef WIDGET_BLOCK_ABSTRACT_H
#define WIDGET_BLOCK_ABSTRACT_H
#include <QPersistentModelIndex>
#include <QWidget>
#include "Block.h"
#include "Model.h"
namespace Widget {
namespace Block {




/*!
 * This is an abstract widget class. It provides an editable view of a block
 * contained in a project model.
 * 
 * Its single property is modified. The modified flag is true if a block widget
 * has modified edit widgets whose data has not been applied to its block or
 * false otherwise.
 * 
 * In order to correctly modify its block through the block's project model, a
 * block widget is provided the model and index of its block. When applying a
 * block widget's data to its block, its model and index must be valid.
 */
class Abstract:
    public QWidget
{
    Q_OBJECT
    const ::Block::Abstract* _block;
    Model::Project* _model {nullptr};
    QPersistentModelIndex _index;
    bool _modified {false};


    /*!
     * Constructs this new widget with the given block and parent.
     * 
     * The given block must be valid and cannot be destroyed during the life of
     * this widget instance.
     *
     * @param block
     *        The block.
     *
     * @param parent
     *        The parent.
     */
    public:
    Abstract(
        const ::Block::Abstract* block
        ,QWidget* parent = nullptr
    );


    /*!
     * Signals this widget's modified property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void modifiedChanged(
        bool value
    );


    /*!
     * Called to Apply any modifications from this widget to its block.
     * 
     * This widget must have a valid model, index, and block.
     *
     * @return
     * True on success or false otherwise.
     */
    public slots:
    virtual bool apply(
    ) = 0;


    /*!
     * Getter method.
     *
     * @return
     * This widget's modified property.
     */
    public:
    bool modified(
    ) const;


    /*!
     * Sets this widget's index to the given index.
     * 
     * The given index must point to this widget's block in its project model.
     *
     * @param index
     *        The index.
     */
    public:
    void setIndex(
        const QPersistentModelIndex& index
    );


    /*!
     * Sets this widget's model to the given model.
     * 
     * The given model must be the project model which contains this widget's
     * block.
     *
     * @param model
     *        The model.
     */
    public:
    void setModel(
        Model::Project* model
    );


    /*!
     * Sets this widget's modified property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setModified(
        bool value
    );


    /*!
     * Called to set this widget's modified property to true.
     */
    protected slots:
    void touch(
    );


    /*!
     * Calls this widget's model's abort set method, signaling no modifications
     * were applied to this widget's block.
     * 
     * This can only be called after calling this widget's begin set method.
     */
    protected:
    void abortSet(
    );


    /*!
     * Calls this widget's model's begin set method, signaling modifications are
     * about to be applied to this widget's block.
     * 
     * This must be called before this widget can safely apply its modifications
     * to its block. This widget's block must be able to be Qt object cast to
     * the given type T. This widget must have a valid model, index, and block.
     *
     * @return
     * This widget's block Qt object cast as type T.
     */
    protected:
    template<class T> T* beginSet(
    );


    /*!
     * Returns this widget's block, Qt object cast to the given type T.
     * 
     * This widget's block must be able to be cast to the given type T.
     */
    protected:
    template<class T> const T* block(
    );


    /*!
     * Calls this widget's model's abort set method, signaling modifications
     * were successfully applied to this widget's block.
     * 
     * This can only be called after calling this widget's begin set method.
     */
    protected:
    void finishSet(
    );


    /*!
     * Called when this widget's block's destroyed signal is emitted.
     */
    private slots:
    void onBlockDestroyed(
        QObject* object
    );


    /*!
     * Called when this widget's model's destroyed signal is emitted.
     */
    private slots:
    void onModelDestroyed(
        QObject* object
    );
};
}
}


#endif
