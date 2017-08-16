#ifndef SINGLETONFACTORY_H
#define SINGLETONFACTORY_H
#include "exception.h"



// @@ SingletonFactory<T>
template<class T>
class SingletonFactory
{
public:
   static T& getInstance();
   static void setInstance(T* instance);
private:
   static T* _instance;
};



// @@ SingletonFactory<T>::_instance
template<class T> T* SingletonFactory<T>::_instance {nullptr};






// @@ SingletonFactory<T>::getInstance()
template<class T>
T& SingletonFactory<T>::getInstance()
{
   if ( !_instance )
   {
      Exception::NullReturn e;
      MARK_EXCEPTION(e);
      e.setTitle(QObject::tr("Null Return"));
      e.setDetails(QObject::tr("Attempted to get factory when none has been set!"));
      throw e;
   }
   return *_instance;
}






// @@ SingletonFactory<T>::setInstance(T*)
template<class T>
void SingletonFactory<T>::setInstance(T* instance)
{
   if ( _instance )
   {
      delete _instance;
   }
   _instance = instance;
}



#endif
