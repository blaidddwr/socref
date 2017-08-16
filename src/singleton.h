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
   if ( !_instance )
   {
      _instance = new T();
   }
   return *_instance;
}



#endif
