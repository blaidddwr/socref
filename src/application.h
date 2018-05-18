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
   explicit Application(int& argc, char** argv);
   virtual bool notify(QObject* receiver, QEvent* event) override final;
   static QString versionString();
   /*!
    * Major version of this program. 
    */
   constexpr static int _majorVersion {0};
   /*!
    * Minor version of this program. 
    */
   constexpr static int _minorVersion {4};
   /*!
    * Revision number of this program. 
    */
   constexpr static int _revision {6};
   /*!
    * The company key used to access this program's qt settings. 
    */
   static const char* _companyKey;
   /*!
    * The program key used to access this program's qt settings. 
    */
   static const char* _programKey;
};



#endif
