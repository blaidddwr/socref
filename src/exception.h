#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QString>

#define MARK_EXCEPTION(e) e.setFile(__FILE__);e.setLine(__LINE__);\
   e.setFunction(__PRETTY_FUNCTION__);



//@@
namespace Exception {

//@@
class Base
{
public:
   virtual ~Base() = default;
   //@@
   virtual const char* what() const noexcept { return "Base"; }
   //@@
   void setFile(const char* file) noexcept { _file = file; }
   //@@
   void setLine(int line) noexcept { _line = line; }
   //@@
   void setFunction(const char* function) noexcept { _function = function; }
   //@@
   void setTitle(const QString& title) noexcept { _title = title; }
   //@@
   void setDetails(const QString& details) noexcept { _details = details; }
   //@@
   const char* getFile() const noexcept { return _file; }
   //@@
   int getLine() const noexcept { return _line; }
   //@@
   const char* getFunction() const noexcept { return _function; }
   //@@
   QString getTitle() const noexcept { return _title; }
   //@@
   QString getDetails() const noexcept { return _details; }
private:
   const char* _file;
   int _line;
   const char* _function;
   QString _title;
   QString _details;
};



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
