#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QString>

#define MARK_EXCEPTION(e) e.setFile(__FILE__);e.setLine(__LINE__);\
   e.setFunction(__PRETTY_FUNCTION__);



//@@
namespace Exception {

//@@
enum class Icon
{
   Warning
   ,Critical
};



//@@
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
   void show(const QString& text, Icon icon, bool debug = false) const noexcept;
private:
   const char* _file;
   int _line;
   const char* _function;
   QString _title;
   QString _details;
};



//@@
inline const char* Base::what() const noexcept { return "Base"; }

//@@
inline void Base::setFile(const char* file) noexcept { _file = file; }

//@@
inline void Base::setLine(int line) noexcept { _line = line; }

//@@
inline void Base::setFunction(const char* function) noexcept { _function = function; }

//@@
inline void Base::setTitle(const QString& title) noexcept { _title = title; }

//@@
inline void Base::setDetails(const QString& details) noexcept { _details = details; }

//@@
inline const char* Base::getFile() const noexcept { return _file; }

//@@
inline int Base::getLine() const noexcept { return _line; }

//@@
inline const char* Base::getFunction() const noexcept { return _function; }

//@@
inline QString Base::getTitle() const noexcept { return _title; }

//@@
inline QString Base::getDetails() const noexcept { return _details; }



//@@
class OpenError : public Base
{
   virtual const char* what() const noexcept override final { return "OpenError"; }
};



//@@
class ReadError : public Base
{
   virtual const char* what() const noexcept override final { return "ReadError"; }
};



//@@
class WriteError : public Base
{
   virtual const char* what() const noexcept override final { return "WriteError"; }
};



//@@
class SystemError : public Base
{
   virtual const char* what() const noexcept override final { return "SystemError"; }
};



//@@
class InvalidArgument : public Base
{
   virtual const char* what() const noexcept override final { return "InvalidArgument"; }
};



//@@
class InvalidUse : public Base
{
   virtual const char* what() const noexcept override final { return "InvalidUse"; }
};



//@@
class OutOfRange : public Base
{
   virtual const char* what() const noexcept override final { return "OutOfRange"; }
};



//@@
class DomainError : public Base
{
   virtual const char* what() const noexcept override final { return "DomainError"; }
};



//@@
class RangeError : public Base
{
   virtual const char* what() const noexcept override final { return "RangeError"; }
};

} // namespace Exception



#endif
