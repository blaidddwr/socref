#ifndef STREAM_PROJECTXML_H
#define STREAM_PROJECTXML_H
#include <QString>
#include "Model.h"
namespace Stream {




/*!
 * This is a stream class. It handles all input and output of XML for project
 * model class instances.
 * 
 * Its single property is path. The path is used as the location of the XML file
 * where all loading and saving is done.
 */
class ProjectXml
{
    QString _path;


    /*!
     * Constructs this new stream with the given path.
     *
     * @param path
     *        The path.
     */
    public:
    ProjectXml(
        const QString& path
    );


    /*!
     * A write project, write block, or file system exception is thrown if any
     * error is encountered.
     *
     * @param project
     *        The project.
     */
    public:
    ProjectXml& operator<<(
        const Model::Project& project
    );


    /*!
     * Loads a project with the given parent.
     * 
     * A read project, read block, or file system exception is thrown if any
     * error is encountered.
     *
     * @param parent
     *        The parent.
     *
     * @return
     * The loaded project.
     */
    public:
    Model::Project* load(
        QObject* parent = nullptr
    ) const;
};
}


#endif
