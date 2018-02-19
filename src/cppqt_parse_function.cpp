#include <QStack>
#include <exception.h>
#include "cppqt_parse_function.h"
#include "cppqt_function.h"
#include "cppqt_common.h"
#include "cppqt_blockfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_template.h"
#include "cppqt_operator.h"
#include "cppqt_constructor.h"
#include "cppqt_destructor.h"
#include "cppqt_parse_common.h"



using namespace CppQt::Parse;






Function::Function(CppQt::Function* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{}






Function::Function(const QString& definition, AbstractParser* parent):
   Base(parent),
   _definition(definition)
{}






void Function::outputComments()
{
   if ( _block )
   {
      addLine("///");
      addLines(makeComment(_block->description()));
      if ( _block->type() != BlockFactory::SignalType )
      {
         if ( _block->type() != BlockFactory::ConstructorType || _block->type() != BlockFactory::DestructorType || _block->type() != BlockFactory::SlotType ) addLines(makeTemplateComments(_block));
         outputArgumentComments();
         if ( _block->type() != BlockFactory::SlotType ) outputReturnDescriptionComment();
         if ( !_block->isAbstract() ) outputOperationComments();
      }
      addLine("///");
   }
   else addLine("/// !!! UNKNOWN FUNCTION !!!");
}






void Function::outputDeclaration()
{
   if ( _block )
   {
      if ( _block->isAbstract() || _block->type() == BlockFactory::SignalType ) outputComments();
      QString line {templateName(_block)};
      if ( _block->isVirtual() ) line.append("virtual ");
      line.append(getReturnValue()).append(getName()).append(getArguments(true));
      if ( _block->isConst() ) line.append(" const");
      if ( _block->isNoExcept() ) line.append(" noexcept");
      if ( _block->isOverride() ) line.append(" override");
      if ( _block->isFinal() ) line.append(" final");
      if ( _block->isAbstract() ) line.append(" = 0");
      line.append(";");
      addLine(line);
   }
}






void Function::outputDefinition()
{
   if ( _block && _block->type() != BlockFactory::SignalType )
   {
      QString line;
      QString templateString {getTemplateDeclaration(_block)};
      if ( !templateString.isEmpty() ) line.append(templateString).append(" ");
      line.append(getReturnValue()).append(getScope(!templateString.isEmpty())).append(getName()).append(getArguments(false));
      if ( _block->isConst() ) line.append(" const");
      if ( _block->isNoExcept() ) line.append(" noexcept");
      if ( !_initializers.isEmpty() ) line.append(":");
      addLine(line);
   }
   else addLine(_definition);
   for (auto line : _initializers) addLine(line);
   if ( _code.isEmpty() ) addLine("{}");
   else
   {
      addLine("{");
      for (auto line : _code) addLine(line);
      addLine("}");
   }
}






bool Function::isMatch(const QString& line)
{
   if ( hasCode() ) return false;
   QString regular {".*"};
   regular.append(_block->Base::name()).append("\\(\\s*");
   for (auto argument : _block->arguments()) regular.append(argument->variableType()).append(".*");
   regular.append("\\):?\\s*");
   return QRegExp(regular).exactMatch(line);
}






bool Function::hasCode() const
{
   return !_initializers.isEmpty() || !_code.isEmpty();
}






bool Function::readLine(const QString& line)
{
   QString trim {line.trimmed()};
   if ( _level == 0 && trim == QString("{}") ) return false;
   if ( trim == QString("{") )
   {
      ++_level;
      if ( _level == 1 ) return true;
   }
   else if ( trim == QString("}") )
   {
      --_level;
      if ( _level == 0 ) return false;
   }
   if ( _level == 0) _initializers << line;
   else _code << line;
   return true;
}






void Function::outputArgumentComments()
{
   const QList<Variable*> arguments {_block->arguments()};
   for (auto argument : arguments)
   {
      addLine("/// ");
      QString base {"@param "};
      base.append(argument->Base::name()).append(" ");
      int justified {base.size()};
      base.append(argument->description());
      addLines(makeComment(base,justified));
   }
}






void Function::outputReturnDescriptionComment()
{
   const QString returnDescription {_block->returnDescription()};
   if ( !returnDescription.isEmpty() )
   {
      addLine("///");
      QString base {"@return "};
      int justified {base.size()};
      base.append(returnDescription);
      addLines(makeComment(base,justified));
   }
}






void Function::outputOperationComments()
{
   const QStringList operations {_block->operations()};
   if ( !operations.isEmpty() )
   {
      addLine("///");
      addLine("///");
      addLines(makeComment("Steps of Operation:"));
      for (int i = 0; i < operations.size() ;++i)
      {
         addLine("///");
         QString base {QString::number(i + 1).append(". ")};
         int justified {base.size()};
         base.append(operations.at(i));
         addLines(makeComment(base,justified));
      }
   }
}






QString Function::getReturnValue()
{
   QString ret;
   if ( _block->type() == BlockFactory::SlotType || _block->type() == BlockFactory::SignalType ) ret = QString("void ");
   else if ( _block->type() != BlockFactory::ConstructorType && _block->type() != BlockFactory::DestructorType ) ret.append(_block->returnType()).append(" ");
   return ret;
}






QString Function::getScope(bool hasTemplates)
{
   QString ret;
   if ( _block->isMethod() ) ret.append(getClassScope(_block));
   else if ( !hasTemplates ) ret.append(getNamespace(_block));
   return ret;
}






QString Function::getName()
{
   QString ret;
   if ( const Operator* operator_ = qobject_cast<const Operator*>(_block) ) ret.append("operator").append(operator_->operation());
   else if ( const Destructor* destructor = qobject_cast<const Destructor*>(_block) ) ret.append("~").append(destructor->className());
   else if ( const Constructor* constructor = qobject_cast<const Constructor*>(_block) ) ret.append(constructor->className());
   else ret.append(_block->Base::name());
   return ret;
}






QString Function::getArguments(bool withInitializers)
{
   QString ret;
   ret.append("(");
   bool first {true};
   for (auto argument : _block->arguments())
   {
      if ( first ) first = false;
      else ret.append(", ");
      ret.append(argument->variableType()).append(" ").append(argument->Base::name());
      if ( withInitializers && argument->hasInitializer() ) ret.append(" = ").append(argument->initializer());
   }
   ret.append(")");
   return ret;
}






bool Function::hasAnyTemplates()
{
   AbstractBlock* back {_block->parent()};
   while ( back )
   {
      if ( back->type() == BlockFactory::NamespaceType ) break;
      else if ( Class* block = back->cast<Class>(BlockFactory::ClassType) ) return block->hasAnyTemplates();
      back = back->parent();
   }
   return false;
}
