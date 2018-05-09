#ifndef SINGLETON_H
#define SINGLETON_H
//



/*!
 * This template can be inherited by any class that wants to be a singleton class. 
 * Any class that uses this template to be a singleton class must have a default 
 * constructor. A singleton class is a class that has only one global instance for 
 * the entire program. 
 *
 * @tparam T This must be the class type that inherits this class. 
 */
template<class T> class Singleton
{
public:
   static T& instance();
private:
   /*!
    * Pointer to the global single instance of the singleton class that inherits from 
    * this template class. 
    */
   static T* _instance;
};



/*!
 * Pointer to the global single instance of the singleton class that inherits from 
 * this template class. 
 */
template<class T> T* Singleton<T>::_instance {nullptr};






/*!
 * Returns the single global instance of the singleton class that inherits from 
 * this template class. 
 *
 * @return Reference to singleton class instance. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the global pointer is null then make a new instance of the singleton class 
 *    with its default constructor and set the global pointer to this new instance. 
 *
 * 2. Return a reference to the global singleton class instance. 
 */
template<class T> T& Singleton<T>::instance()
{
   // 1
   if ( !_instance )
   {
      _instance = new T();
   }

   // 2
   return *_instance;
}



#endif
