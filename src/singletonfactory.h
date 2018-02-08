#ifndef SINGLETONFACTORY_H
#define SINGLETONFACTORY_H
#include "exception.h"



template<class T> class SingletonFactory
{
public:
   virtual ~SingletonFactory() = default;
   static T& instance();
   static void setInstance(T* instance);
private:
   static T* _instance;
};



template<class T> T* SingletonFactory<T>::_instance {nullptr};






template<class T> T& SingletonFactory<T>::instance()
{
   if ( !_instance )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Attempted to get factory when none has been set."));
      throw e;
   }
   return *_instance;
}






template<class T> void SingletonFactory<T>::setInstance(T* instance)
{
   if ( _instance )
   {
      delete _instance;
   }
   _instance = instance;
}



#endif
