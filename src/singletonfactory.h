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
   // make sure there is an instance to return
   if ( !_instance )
   {
      Exception::NullReturn e;
      MARK_EXCEPTION(e);
      e.setTitle(QObject::tr("Base Factory"));
      e.setDetails(QObject::tr("Attempted to get abstract factory when none has been set!"));
      throw e;
   }

   // return reference to object
   return *_instance;
}






// @@ SingletonFactory<T>::setInstance(T*)
template<class T>
void SingletonFactory<T>::setInstance(T* instance)
{
   // if there already is an instance delete it
   if ( _instance )
   {
      delete _instance;
   }

   // set new instance pointer
   _instance = instance;
}



#endif
