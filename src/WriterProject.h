#ifndef WRITER_PROJECT_H
#define WRITER_PROJECT_H
#include <QObject>
#include "Model.h"
namespace Writer {




/*!
 * This is a writer class. It writes a given project to a directory at a given
 * directory path using the multi block file system.
 */
class Project:
    public QObject
{
    Q_OBJECT
    QString _path;
    bool _open {false};


    /*!
     * Constructs this new instance with the given directory path and parent.
     *
     * @param path
     *        The directory path.
     *
     * @param parent
     *        The parent.
     */
    public:
    Project(
        const QString& path
        ,QObject* parent = nullptr
    );


    /*!
     * A file system or write project exception is thrown if any error is
     * encountered.
     */
    public:
    Writer::Project& operator<<(
        const Model::Project& project
    );


    /*!
     * Opens this instance's directory path for writing a project and its
     * blocks.
     * 
     * A file system exception is thrown if any error is encountered.
     */
    public:
    void open(
    );


    /*!
     * Writes the project configuration file from the given project to this
     * instance's directory path.
     * 
     * A file system or write project exception is thrown if any error is
     * encountered.
     *
     * @param project
     *        The project.
     */
    private:
    void writeConfig(
        const Model::Project& project
    );
};
}


#endif
