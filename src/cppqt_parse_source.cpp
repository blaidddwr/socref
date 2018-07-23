#include "cppqt_parse_source.h"
#include <QStack>
#include <QRegularExpression>
#include "cppqt_parse_function.h"
#include "cppqt_parse_variable.h"
#include "cppqt_function.h"
#include "cppqt_variable.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_access.h"
#include "cppqt_blockfactory.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 *
 * @param block  
 */
Source::Source(const Namespace* block):
   Global(block),
   _block(block),
   _headerLines(Settings::instance().headerLines()),
   _functionLines(Settings::instance().functionLines()),
   _children(block->realChildren())
{
   if ( const Class* valid = block->cast<Class>(BlockFactory::ClassType) )
   {
      _isTemplate = valid->hasAnyTemplates();
   }
   makeUsingName();
}






/*!
 *
 * @param block  
 *
 * @param name  
 */
Source::Source(const Namespace* block, const QString& name):
   Source(block)
{
   _include = QString("#include \"").append(name).append(".h\"");
}






/*!
 */
void Source::initialize()
{
   buildAll();
}






/*!
 *
 * @param line  
 */
bool Source::readLine(const QString& line)
{
   if ( QRegularExpression("\\A\\s*\\/\\*!\\z").match(line).hasMatch() )
   {
      _inComments = true;
   }
   else if ( QRegularExpression("\\A\\s*\\*\\/\\z").match(line).hasMatch() )
   {
      _inComments = false;
   }
   else if ( !_inComments )
   {
      if ( line == QString("//") ) _pastTop = true;
      if ( !_pastTop ) readTop(line);
      if ( QRegularExpression("\\A.*\\([a-zA-Z0-9_,<>:&\\* ]*\\):?[ constexp]*\\z").match(line).hasMatch() )
      {
         if ( Function* child = findDefined(line) )
         {
            child->setCutOff(line.indexOf(QRegularExpression("\\S")));
            stepIntoChild(child);
         }
         else
         {
            addUndefined(new Function(line,this));
            stepIntoChild(_undefined.back());
         }
      }
   }
   return true;
}






/*!
 */
void Source::makeOutput()
{
   add(_include);
   outputPreProcessor();
   outputMisc(true);
   outputDefinitions();
}






/*!
 *
 * @param line  
 */
void Source::readTop(const QString& line)
{
   if ( QRegularExpression("\\A#.*\\z").match(line).hasMatch() && line != _include )
   {
      addPreProcess(line);
   }
   else if ( QRegularExpression("\\A\\s*using\\s+namespace\\s+[a-zA-Z_]+[a-zA-Z0-9_]*;\\s*\\z").match(line).hasMatch()
             && line != _usingName )
   {
      addMisc(line.trimmed());
   }
}






/*!
 *
 * @param block  
 */
void Source::evaluateVariable(CppQt::Variable* block)
{
   if ( !isTemplate() && block->isStatic() && !block->isConstExpr() )
   {
      addVariable(new Variable(block,this));
   }
}






/*!
 *
 * @param block  
 */
void Source::evaluateFunction(CppQt::Function* block)
{
   if ( !isTemplate()
        && !block->isAbstract()
        && !block->isDefault()
        && block->type() != BlockFactory::SignalType
        && ( !block->hasTemplates() || block->isPrivateMethod() ) )
   {
      addDefined(new Function(block,this));
   }
}






/*!
 *
 * @param block  
 */
void Source::evaluateOther(AbstractBlock* block)
{
   Q_UNUSED(block)
}






/*!
 */
void Source::outputPreProcessor()
{
   for (auto line : _preProcess) add(line);
}






/*!
 *
 * @param addUsingName  
 */
void Source::outputMisc(bool addUsingName)
{
   if ( !_misc.isEmpty() || ( addUsingName && !_usingName.isEmpty() ) )
   {
      add(_headerLines);
      for (auto line : _misc) add(line);
      if ( addUsingName && !_usingName.isEmpty() ) add(_usingName);
   }
   add("//");
}






/*!
 */
void Source::outputDefinitions()
{
   if ( !_variables.isEmpty() )
   {
      add(_headerLines);
      for (auto variable : qAsConst(_variables))
      {
         variable->outputComments();
         variable->outputDefinition();
      }
   }
   for (auto function : _defined)
   {
      add(_functionLines);
      function->outputComments();
      function->outputDefinition();
   }
   for (auto function : _undefined)
   {
      add(_functionLines);
      function->outputComments();
      function->outputDefinition();
   }
}






/*!
 *
 * @param definition  
 */
Function* Source::findDefined(const QString& definition)
{
   for (auto function : qAsConst(_defined))
   {
      if ( function->isMatch(definition) ) return function;
   }
   return nullptr;
}






/*!
 */
const QList<AbstractBlock*>& Source::children() const
{
   return _children;
}






/*!
 *
 * @param line  
 */
void Source::addPreProcess(const QString& line)
{
   _preProcess << line;
}






/*!
 *
 * @param line  
 */
void Source::addMisc(const QString& line)
{
   _misc << line;
}






/*!
 *
 * @param parser  
 */
void Source::addVariable(Variable* parser)
{
   _variables << parser;
}






/*!
 *
 * @param defined  
 */
void Source::addDefined(Function* defined)
{
   _defined << defined;
}






/*!
 *
 * @param undefined  
 */
void Source::addUndefined(Function* undefined)
{
   _undefined << undefined;
}






/*!
 */
bool Source::isTemplate() const
{
   return _isTemplate;
}






/*!
 */
void Source::makeUsingName()
{
   QStack<Namespace*> list;
   AbstractBlock* back {_block->parent()};
   while ( back )
   {
      if ( Namespace* valid = back->cast<Namespace>(BlockFactory::NamespaceType) )
      {
         list.push(valid);
      }
      back = back->parent();
   }
   if ( list.size() > 1 )
   {
      list.pop();
      _usingName.append("using namespace ");
      while ( list.size() > 1 ) _usingName.append(list.pop()->Base::name()).append("::");
      _usingName.append(list.pop()->Base::name()).append(";");
   }
}






/*!
 */
void Source::buildAll()
{
   for (auto child : qAsConst(_children))
   {
      if ( CppQt::Variable* valid = child->cast<CppQt::Variable>(BlockFactory::VariableType) )
      {
         evaluateVariable(valid);
      }
      else if ( CppQt::Function* valid = qobject_cast<CppQt::Function*>(child) )
      {
         evaluateFunction(valid);
      }
      else evaluateOther(child);
   }
}
