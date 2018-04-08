#include "cppqt_parse_source.h"
#include <QStack>
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






Source::Source(CppQt::Namespace* block):
   Global(block),
   _block(block),
   _headerLines(Settings::instance().headerLines()),
   _functionLines(Settings::instance().functionLines()),
   _children(block->realChildren())
{
   if ( Class* valid = block->cast<Class>(BlockFactory::ClassType) )
   {
      _isTemplate = valid->hasAnyTemplates();
   }
   makeUsingName();
}






Source::Source(CppQt::Namespace* block, const QString& name):
   Source(block)
{
   _include = QString("#include \"").append(name).append(".h\"");
}






void Source::initialize()
{
   buildAll();
}






bool Source::readLine(const QString& line)
{
   if ( QRegExp("\\s*\\/\\*!").exactMatch(line) )
   {
      _inComments = true;
   }
   else if ( QRegExp("\\s*\\*\\/").exactMatch(line) )
   {
      _inComments = false;
   }
   else if ( !_inComments )
   {
      if ( line == QString("//") ) _pastTop = true;
      if ( !_pastTop ) readTop(line);
      if ( QRegExp(".*\\([a-zA-Z0-9_,<>:&\\* ]*\\):?[ constexp]*").exactMatch(line) )
      {
         if ( Function* child = findDefined(line) ) stepIntoChild(child);
         else
         {
            addUndefined(new Function(line,this));
            stepIntoChild(_undefined.back());
         }
      }
   }
   return true;
}






void Source::makeOutput()
{
   add(_include);
   outputPreProcesser();
   outputMisc(true);
   outputDefinitions();
}






void Source::readTop(const QString& line)
{
   if ( QRegExp("#.*").exactMatch(line) && line != _include )
   {
      addPreProcess(line);
   }
   else if ( QRegExp("\\s*using\\s+namespace\\s+[a-zA-Z_]+[a-zA-Z0-9_]*;\\s*").exactMatch(line)
             && line != _usingName )
   {
      addMisc(line.trimmed());
   }
}






void Source::evaluateVariable(CppQt::Variable* block)
{
   if ( !isTemplate() && block->isStatic() && !block->isConstExpr() )
   {
      addVariable(new Variable(block,this));
   }
}






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






void Source::evaluateOther(AbstractBlock* block)
{
   Q_UNUSED(block)
}






void Source::outputPreProcesser()
{
   for (auto line : _preProcess) add(line);
}






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






Function* Source::findDefined(const QString& definition)
{
   for (auto function : qAsConst(_defined))
   {
      if ( function->isMatch(definition) ) return function;
   }
   return nullptr;
}






const QList<AbstractBlock*>& Source::children() const
{
   return _children;
}






void Source::addPreProcess(const QString& line)
{
   _preProcess << line;
}






void Source::addMisc(const QString& line)
{
   _misc << line;
}






void Source::addVariable(Variable* parser)
{
   _variables << parser;
}






void Source::addDefined(Function* defined)
{
   _defined << defined;
}






void Source::addUndefined(Function* undefined)
{
   _undefined << undefined;
}






bool Source::isTemplate() const
{
   return _isTemplate;
}






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
