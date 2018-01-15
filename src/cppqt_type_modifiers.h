#ifndef CPPQT_TYPE_MODIFIERS_H
#define CPPQT_TYPE_MODIFIERS_H
#include <QList>



namespace CppQt
{
   namespace Type
   {
      class Modifiers
      {
      public:
         enum
         {
            NotConst = 0
            ,Const
            ,ConstExpr
            ,Total
         };
         Modifiers() = default;
         Modifiers(const QString& text) { fromString(text); }
         Modifiers(int base, bool isStatic, const QList<int>& pointers):
            _base(base),
            _isStatic(isStatic),
            _pointers(pointers)
            {}
         int base() const { return _base; }
         Modifiers& setBase(int base);
         bool isStatic() const { return _isStatic; }
         Modifiers& setStatic(bool isStatic);
         const QList<int> pointers() const { return _pointers; }
         Modifiers& setPointers(const QList<int>& pointers);
         bool isNull() const { return _base == NotConst && _isStatic == false && _pointers.isEmpty(); }
         QString toString() const;
         Modifiers& fromString(const QString& text);
      private:
         int _base {NotConst};
         bool _isStatic {false};
         QList<int> _pointers;
      };
   }
}



#endif
