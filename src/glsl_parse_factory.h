#ifndef GLSL_PARSE_FACTORY_H
#define GLSL_PARSE_FACTORY_H
#include "abstractparserfactory.h"
#include "glsl.h"
#include "glsl_shader.h"
//



namespace GLSL
{
   namespace Parse
   {
      /*!
       * This is the parser factory for the GLSL project type. This recognizes only one 
       * type of source file, that being a shader source file that defines a single GLSL 
       * shader. Sub extensions are also used to identify the shader type. All shader 
       * types defined as of OpenGL version 4.3 are recognized. 
       */
      class Factory : public AbstractParserFactory
      {
         Q_OBJECT
      public:
         virtual Soc::Ut::QPtr<AbstractParser> make(const QString& name, const QString& extension) const override final;
      public:
         explicit Factory(const AbstractBlock* root);
      private:
         static AbstractParser* find(const Namespace* base, QStack<const QString*>* names, GLSL::Shader::Type type);
         static AbstractParser* findShader(const Namespace* base, const QString& name, GLSL::Shader::Type type);
      private:
         /*!
          * Pointer to the root block used by this parser factory and all parser objects it 
          * makes. 
          */
         const Namespace* _root;
      };
   }
}



#endif
