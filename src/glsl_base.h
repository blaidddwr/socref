#ifndef GLSL_BASE_H
#define GLSL_BASE_H
#include "basic_block.h"



/*!
 * Contains the GLSL project type.
 */
namespace GLSL
{
   /*!
    * This is the base block class used by many other concrete block types within
    * the GLSL project type. This base class provides methods for getting the base
    * name and description of a block which almost all block types share.
    */
   class Base : public Basic::Block
   {
      Q_OBJECT
   public:
      QString baseName() const;
      QString description() const;
   };
}

#endif
