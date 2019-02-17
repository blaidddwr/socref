#ifndef GLSL_NAMESPACE_H
#define GLSL_NAMESPACE_H
#include "glsl_base.h"



/*!
 * Contains the GLSL project type.
 */
namespace GLSL
{
   /*!
    * This is the namespace block. While GLSL has no concept of a namespace this
    * block is used for file organization. Each namespace is prepended to the file
    * name of a shader and separated by underscores.
    */
   class Namespace : public Base
   {
      Q_OBJECT
   public:
      /*!
       * Constructs a new namespace block.
       */
      Q_INVOKABLE explicit Namespace() = default;
   };
}

#endif
