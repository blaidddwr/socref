#include "glsl_parse_factory.h"
#include <QStack>
#include "glsl_parse_shader.h"
#include "glsl_factory.h"
#include "glsl_namespace.h"



using namespace GLSL::Parse;
//






/*!
 * Implements _AbstractParserFactory_ interface. 
 *
 * @param name See interface docs. 
 *
 * @param extension See interface docs. 
 *
 * @return See interface docs. 
 */
Soc::Ut::QPtr<AbstractParser> Factory::make(const QString& name, const QString& extension) const
{
   Q_UNUSED(extension)

   // Initialize a static list of all possible sub extensions for the different 
   // shader types. 
   static const QStringList exts {"v","tc","te","g","f","c"};

   // Split the given name into its actual name and the sub extension, making sure 
   // there is only one period separating the two. 
   QStringList subextnames {name.split('.')};
   Q_ASSERT(subextnames.size() == 2);

   // Figure out the shader type of this file based off of its sub extension, making 
   // sure the sub extension string is recognized. 
   const int i {exts.indexOf(subextnames.at(1))};
   Q_ASSERT(i >= 0);
   const GLSL::Shader::Type type {static_cast<GLSL::Shader::Type>(i)};

   // Separate the list of namespace names and shader name in the files given name. 
   QStringList names {subextnames.at(0).split('_')};

   // Create a reverse stack of pointers to the namespace name strings that will be 
   // worked on by the recursive find function. 
   QStack<const QString*> namespaces;
   for (int i = names.size() - 1; i >= 0 ;--i)
   {
      namespaces.push(&names.at(i));
   }

   // Call the recursive find method that attempts to find a matching shader, 
   // returning its result. 
   return find(_root,&namespaces,type);
}






/*!
 * Constructs a new GLSL parser factory with the given root block. 
 *
 * @param root The root block used by this parser factory. 
 */
Factory::Factory(const AbstractBlock* root):
   _root(qobject_cast<const Namespace*>(root))
{
   // Make sure the given root pointer is not null. 
   Q_CHECK_PTR(root);
}






/*!
 * Recursively attempts to find a matching namespace that is a child of the given 
 * namespace and matches the top string pointed to on the given stack. This 
 * continues recursively calls itself until a shader that matches is found and a 
 * parser returned or no match is found and a null pointer is returned. 
 *
 * @param base The current namespace that is being searched for a child namespace 
 *             or shader. 
 *
 * @param names The stack of names still to be resolved. The very last name being 
 *              the name of the shader that is being searched. These are pointers 
 *              to strings so the strings must not be destroyed while this function 
 *              is being called. 
 *
 * @param type The shader type that should be matched with its name. 
 *
 * @return Pointer to new shader parser if a match is found or a null pointer if no 
 *         match is found. 
 */
AbstractParser* Factory::find(const Namespace* base, QStack<const QString*>* names, GLSL::Shader::Type type)
{
   // If there is only one name left in the given stack then call the find shader 
   // function. 
   if ( names->size() == 1 ) return findShader(base,*names->pop(),type);

   // Else there is more namespace names to resolve. 
   else if ( !names->isEmpty() )
   {
      // Get the next name from the given stack. 
      const QString& name {*names->pop()};

      // Search the given base namespace block's children, recursively calling this 
      // function if a namespace that matches the next name is found. 
      for (auto child: base->list())
      {
         if ( const Namespace* valid = child->cast<const Namespace>(GLSL::Factory::NamespaceType) )
         {
            if ( valid->baseName() == name ) return find(valid,names,type);
         }
      }
   }

   // No namespace name match is found so return a null pointer. 
   return nullptr;
}






/*!
 * Attempts to find a matching shader with the given name and type as a child in 
 * the given namespace. This returns a pointer to a new shader parser if a match is 
 * found or a null pointer if no match is found. 
 *
 * @param base The namespace whose children are searched for a matching shader 
 *             block. 
 *
 * @param name The base name of the shader that should be matched. 
 *
 * @param type The type of the shader that should be matched. 
 *
 * @return Pointer to new shader parser if a match is found or a null pointer if no 
 *         match is found. 
 */
AbstractParser* Factory::findShader(const Namespace* base, const QString& name, GLSL::Shader::Type type)
{
   // Search the given base namespace block's children, returning a new shader parser 
   // of a matched shader if it is found. 
   for (auto child: base->list())
   {
      if ( const GLSL::Shader* valid = child->cast<const GLSL::Shader>(GLSL::Factory::ShaderType) )
      {
         if ( valid->baseName() == name && valid->shaderType() == type ) return new Shader(valid);
      }
   }

   // No shader match is found so return a null pointer. 
   return nullptr;
}
