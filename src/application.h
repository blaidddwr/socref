#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
//



/*!
 * This is the main application class for this program. It implements the notify 
 * interface to be able to handle exceptions and holds the keys for qt settings. 
 */
class Application : public QApplication
{
   Q_OBJECT
public:
   Application(int& argc, char** argv);
   virtual bool notify(QObject* receiver, QEvent* event) override final;
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
