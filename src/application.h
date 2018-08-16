#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
//



/*!
 * This is the main application class for this program. It implements the notify 
 * interface to be able to handle exceptions. It also contains the version of the 
 * program, along with the keys used to access this program's qt settings. 
 */
class Application : public QApplication
{
   Q_OBJECT
public:
   virtual bool notify(QObject* receiver, QEvent* event) override final;
public:
   static QString versionString();
   explicit Application(int& argc, char** argv);
   /*!
    * Major version of this program. 
    */
   constexpr static int _majorVersion {MAJOR_VERSION};
   /*!
    * Minor version of this program. 
    */
   constexpr static int _minorVersion {MINOR_VERSION};
   /*!
    * Revision number of this program. 
    */
   constexpr static int _revision {REVISION};
   /*!
    * The organization key used to access this program's qt settings. 
    */
   static const char* _organizationKey;
   /*!
    * The application key used to access this program's qt settings. 
    */
   static const char* _applicationKey;
};



#endif
