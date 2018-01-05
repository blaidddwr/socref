#ifndef SINGLETON_H
#define SINGLETON_H



template<class T>
class Singleton
{
public:
   virtual ~Singleton() = default;
   static T& instance();
private:
   static T* _instance;
};



template<class T> T* Singleton<T>::_instance {nullptr};






template<class T> T& Singleton<T>::instance()
{
   if ( !_instance )
   {
      _instance = new T();
   }
   return *_instance;
}



#endif
