#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>



class Application : public QApplication
{
   Q_OBJECT
public:
   explicit Application(int& argc, char** argv);
   virtual bool notify(QObject* receiver, QEvent* event) override final;
   static const char* _companyKey;
   static const char* _programKey;
};



#endif
