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
       */
      class Factory : public AbstractParserFactory
      {
         Q_OBJECT
      public:
         virtual Sut::QPtr<AbstractParser> make(const QString& name, const QString& extension) const override final;
      public:
         explicit Factory(const AbstractBlock* root);
      private:
         static AbstractParser* find(const Namespace* base, QStack<const QString*>* names, Shader::Type type);
         static AbstractParser* findShader(const Namespace* base, const QString& name, Shader::Type type);
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
