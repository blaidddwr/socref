#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QObject>

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
struct OpenError : public Base
{
   OpenError() { setTitle(QObject::tr("Open Error")); }
   virtual const char* what() const noexcept override final { return "OpenError"; }
};



//@@
struct ReadError : public Base
{
   ReadError() { setTitle(QObject::tr("Read Error")); }
   virtual const char* what() const noexcept override final { return "ReadError"; }
};



//@@
struct WriteError : public Base
{
   WriteError() { setTitle(QObject::tr("Write Error")); }
   virtual const char* what() const noexcept override final { return "WriteError"; }
};



//@@
struct SystemError : public Base
{
   SystemError() { setTitle(QObject::tr("System Error")); }
   virtual const char* what() const noexcept override final { return "SystemError"; }
};



//@@
struct InvalidArgument : public Base
{
   InvalidArgument() { setTitle(QObject::tr("Invalid Argument")); }
   virtual const char* what() const noexcept override final { return "InvalidArgument"; }
};



//@@
struct InvalidUse : public Base
{
   InvalidUse() { setTitle(QObject::tr("Invalid Use")); }
   virtual const char* what() const noexcept override final { return "InvalidUse"; }
};



//@@
struct OutOfRange : public Base
{
   OutOfRange() { setTitle(QObject::tr("Out Of Range")); }
   virtual const char* what() const noexcept override final { return "OutOfRange"; }
};



//@@
struct DomainError : public Base
{
   DomainError() { setTitle(QObject::tr("Domain Error")); }
   virtual const char* what() const noexcept override final { return "DomainError"; }
};



//@@
struct RangeError : public Base
{
   RangeError() { setTitle(QObject::tr("Range Error")); }
   virtual const char* what() const noexcept override final { return "RangeError"; }
};

} // namespace Exception



#endif
