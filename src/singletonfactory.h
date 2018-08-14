#ifndef SINGLETONFACTORY_H
#define SINGLETONFACTORY_H
#include <QObject>
#include <socutil/sut_exceptions.h>



using namespace Sut;
//



/*!
 * This template can be inherited by any class that wants to be a singleton 
 * factory. A singleton class is a class that has only one global instance for the 
 * entire program. The global instance can be set to a new instance at anytime, 
 * deleting any existing instance. The is most commonly used with abstract 
 * factories that have a single global instance in a program. 
 *
 * @tparam T This must be the class type that inherits this class. 
 */
template<class T> class SingletonFactory
{
public:
   static T& instance();
   static void setInstance(T* instance);
private:
   /*!
    * Pointer to the global single instance of the singleton factory that inherits 
    * from this template class. 
    */
   static T* _instance;
};



/*!
 * Pointer to the global single instance of the singleton factory that inherits 
 * from this template class. 
 */
template<class T> T* SingletonFactory<T>::_instance {nullptr};






/*!
 * Returns the single global instance of the singleton factory that inherits from 
 * this template class. If an instance has never been set then an exception is 
 * thrown. 
 *
 * @return Reference to singleton factory instance. 
 */
template<class T> T& SingletonFactory<T>::instance()
{
   // Make sure a global instance has been set. 
   if ( !_instance )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Attempted to get factory when none has been set."));
      throw e;
   }

   // Return a reference to the global instance. 
   return *_instance;
}






/*!
 * Sets a new instance of the singleton factory that inherits this template class 
 * with the given pointer. If an instance already exists the old one is deleted and 
 * replaced by the new one given. 
 *
 * @param instance Pointer to the new global instance for the singleton factory 
 *                 inherited from this template class. 
 */
template<class T> void SingletonFactory<T>::setInstance(T* instance)
{
   // Delete any previously set global instance and then set the new instance given. 
   delete _instance;
   _instance = instance;
}



#endif
