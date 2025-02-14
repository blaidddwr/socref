#ifndef CONTROLLER_CODE_H
#define CONTROLLER_CODE_H
#include <QObject>
#include "ControllerCode.h"
#include "Model.h"
#include "Shape.h"
namespace Controller {




/*!
 * This is a controller class. It controls the parsing and building of all
 * source code files for an entire project. Each source code file is treated as
 * a route, with each route having information contained by the route shape
 * class.
 * 
 * @property project This property holds the project of a code controller. This
 * must be initialized to a valid project and the project cannot be destroyed or
 * modified during the lifetime of a code controller.
 * 
 * @property size This property holds the total number of routes contained in a
 * code controller for parsing and building.
 */
class Code:
    public QObject
{
    Q_OBJECT
    Model::Project* _project;
    QString _error;
    QList<Shape::Route> _routes;


    /*!
     * Constructor.
     * 
     * @exception Exception::LogicalRoute Thrown when there is a logical routing
     * error.
     *
     * @param project
     *        The initial object of the project property.
     */
    public:
    Code(
        Model::Project* project
        ,QObject* parent = nullptr
    );


    /*!
     * Clears all code lines in all blocks of this method's code controller's
     * project.
     */
    public:
    void clear(
    );


    /*!
     * Parses a source code file.
     * 
     * @exception Exception::FileSystem Thrown when a file system error occurs.
     *
     * @param index
     *        The route index of the source code file this will parse. This must
     *        be a valid index.
     */
    public:
    void parse(
        int index
    );


    /*!
     * Getter for size property.
     */
    public:
    int size(
    ) const;


    /*!
     * Called when the project property's destroyed signal is emitted.
     */
    private slots:
    void onProjectDestroyed(
        QObject* object
    );


    /*!
     * Called when the project property's modified changed signal is emitted.
     */
    private slots:
    void onProjectModified(
        bool value
    );


    /*!
     * Recursively clears the code lines a given block and all of its descendant
     * blocks.
     *
     * @param block
     *        The block.
     */
    private:
    static void clear(
        AbstractBlock* block
    );


    /*!
     * Recursively parses the given lines of code with the given parser. This
     * handles all delegate to children returns by a parser by recursively
     * calling itself again with the child parser as the given parser.
     *
     * @param parser
     *        The parser. This must be a valid parser.
     *
     * @param lines
     *        The lines of code.
     *
     * @param where
     *        The next line of code that requires parsing.
     *
     * @return
     * The next line of code that requires parsing after this method is
     * finished.
     */
    private:
    static int parse(
        AbstractParser* parser
        ,const QStringList& lines
        ,int where
    );
};
}


#endif
