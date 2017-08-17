#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QString>

#define MARK_EXCEPTION(e) e.setFile(__FILE__);e.setLine(__LINE__);\
   e.setFunction(__PRETTY_FUNCTION__);



// @@ Exception
namespace Exception {

// @@ Base
class Base
{
public:
   virtual ~Base() = default;
   virtual const char* what() const noexcept;
   void setFile(const char* file) noexcept;
   void setLine(int line) noexcept;
   void setFunction(const char* function) noexcept;
   void setTitle(const QString& title) noexcept;
   void setDetails(const QString& details) noexcept;
   const char* getFile() const noexcept;
   int getLine() const noexcept;
   const char* getFunction() const noexcept;
   QString getTitle() const noexcept;
   QString getDetails() const noexcept;
private:
   const char* _file;
   int _line;
   const char* _function;
   QString _title;
   QString _details;
};



// @@ NullReturn
class NullReturn : public Base
{
   virtual const char* what() const noexcept override final { return "NullReturn"; }
};



// @@ ReadError
class ReadError : public Base
{
   virtual const char* what() const noexcept override final { return "ReadError"; }
};






// @@ Exception::Base::what()
inline const char* Base::what() const noexcept
{
   return "Base";
}






// @@ Exception::Base::setFile(const char*)
inline void Base::setFile(const char* file) noexcept
{
   _file = file;
}






// @@ Exception::Base::setLine(int)
inline void Base::setLine(int line) noexcept
{
   _line = line;
}






// @@ Exception::Base::setFunction(const char*)
inline void Base::setFunction(const char* function) noexcept
{
   _function = function;
}






// @@ Exception::Base::setTitle(const QString&)
inline void Base::setTitle(const QString& title) noexcept
{
   _title = title;
}






// @@ Exception::Base::setDetails(const QString&)
inline void Base::setDetails(const QString& details) noexcept
{
   _details = details;
}






// @@ Exception::Base::getFile()
inline const char* Base::getFile() const noexcept
{
   return _file;
}






// @@ Exception::Base::getLine()
inline int Base::getLine() const noexcept
{
   return _line;
}






// @@ Exception::Base::getFunction()
inline const char* Base::getFunction() const noexcept
{
   return _function;
}






// @@ Exception::Base::getTitle()
inline QString Base::getTitle() const noexcept
{
   return _title;
}






// @@ Exception::Base::getDetails()
inline QString Base::getDetails() const noexcept
{
   return _details;
}

} // namespace Exception



#endif
