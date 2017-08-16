#ifndef SINGLETON_H
#define SINGLETON_H



// @@ Singleton<T>
template<class T>
class Singleton
{
public:
   static T& getInstance();
private:
   static T* _instance;
};



// @@ Singleton<T>::_instance
template<class T> T* Singleton<T>::_instance {nullptr};






// @@ Singleton<T>::getInstance()
template<class T>
T& Singleton<T>::getInstance()
{
   // make sure there is an instance to return
   if ( !_instance )
   {
      _instance = new T();
   }

   // return reference to object
   return *_instance;
}



#endif
