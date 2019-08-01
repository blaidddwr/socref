#ifndef CPPQT_TEMPLATE_H
#define CPPQT_TEMPLATE_H
#include "cppqt_base.h"



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    * This is the template block. This represents template arguments for classes or
    * functions.
    */
   class Template : public Base
   {
      Q_OBJECT
   public:
      static QString displayTemplates(const Base& block);
   public:
      /*!
       * Constructs a new template block.
       */
      Q_INVOKABLE explicit Template() = default;
   public:
      QString templateType() const;
   };
}

#endif
