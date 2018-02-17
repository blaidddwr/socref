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



using namespace CppQt::Parse;






Function::Function(int indent, CppQt::Function* block):
   _block(block),
   _header(buildHeader()),
   _baseIndent(indent)
{}






Function::Function(int indent, const QString& header):
   _header(header),
   _baseIndent(indent)
{}






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






void Function::makeOutput()
{
   if ( _block )
   {
      outputComments();
   }
   else
   {
      outputComment("!!! UNKNOWN FUNCTION !!!");
   }
   addLine(_header,_baseIndent);
   addLine("{",_baseIndent);
   for (auto line : _code) addLine(line);
   addLine("}",_baseIndent);
}






QString Function::buildHeader()
{
   QString ret {templateName(_block)};
   if ( _block->type() != BlockFactory::ConstructorType && _block->type() != BlockFactory::DestructorType ) ret.append(_block->returnType()).append(" ");
   appendScope(&ret);
   if ( Operator* block = qobject_cast<Operator*>(_block) ) ret.append("operator").append(block->operation());
   else ret.append(_block->Base::name());
   appendArguments(&ret);
   if ( _block->isConst() ) ret.append(" const");
   if ( _block->isNoExcept() ) ret.append(" noexcept");
   if ( !_initializers.isEmpty() ) ret.append(":");
   return ret;
}






void Function::appendScope(QString* line)
{
   if ( _block->isMethod() )
   {
      appendClass(line);
   }
   else if ( !_block->hasTemplates() )
   {
      appendNamespace(line);
   }
}







void Function::appendClass(QString* line)
{
   QStack<Class*> classes;
   AbstractBlock* parent {_block->parent()};
   while ( parent )
   {
      if ( parent->type() == BlockFactory::NamespaceType ) break;
      else if ( Class* block = qobject_cast<Class*>(parent) ) classes.push(block);
      parent = parent->parent();
   }
   if ( classes.isEmpty() )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Found no parents of type class when at least should be present."));
      throw e;
   }
   appendClassTemplate(line,classes.top(),true);
   while ( !classes.isEmpty() )
   {
      line->append(classes.top()->Base::name());
      appendClassTemplate(line,classes.pop());
      line->append("::");
   }
}






void Function::appendClassTemplate(QString* line, Class* block, bool definition)
{
   if ( block->hasTemplates() )
   {
      const QList<Template*> list {block->templates()};
      bool first {true};
      if ( definition ) line->append("template");
      line->append("<");
      for (auto item : list)
      {
         if ( first ) first = false;
         else line->append(",");
         if ( definition ) line->append(item->name());
         else line->append(item->Base::name());
      }
      line->append(">");
      if ( definition ) line->append(" ");
   }
}






void Function::appendNamespace(QString* line)
{
   Namespace* parent {qobject_cast<Namespace*>(_block->parent())};
   if ( !parent )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Parent of function is type '%1' when it should be namespace.").arg(BlockFactory::instance().name(parent->type())));
      throw e;
   }
   line->append(parent->Base::name()).append("::");
}






void Function::appendArguments(QString* line)
{
   line->append("(");
   const QList<Variable*> arguments {_block->arguments()};
   bool first {true};
   for (auto argument : arguments)
   {
      if ( first ) first = false;
      else line->append(", ");
      line->append(argument->name());
   }
   line->append(")");
}






void Function::outputComments()
{
   outputComment(_block->description());
   outputArgumentComments();
   outputReturnDescriptionComment();
   outputOperationComments();
}






void Function::outputArgumentComments()
{
   const QList<Variable*> arguments {_block->arguments()};
   for (auto argument : arguments)
   {
      addLine("/// ");
      QString base {"@param "};
      int indent {base.size()};
      outputComment(base.append(argument->Base::name()).append(" ").append(argument->description()),indent);
   }
}






void Function::outputReturnDescriptionComment()
{
   const QString returnDescription {_block->returnDescription()};
   if ( !returnDescription.isEmpty() )
   {
      addLine("///",_baseIndent);
      QString base {"@return "};
      int indent {base.size()};
      outputComment(base.append(returnDescription),indent);
   }
}






void Function::outputOperationComments()
{
   const QStringList operations {_block->operations()};
   if ( !operations.isEmpty() )
   {
      addLine("///",_baseIndent);
      addLine("///",_baseIndent);
      outputComment("Steps of Operation:");
      for (int i = 0; i < operations.size() ;++i)
      {
         addLine("///",_baseIndent);
         QString base {QString::number(i + 1).append(". ")};
         int indent {base.size()};
         outputComment(base.append(operations.at(i)),indent);
      }
   }
}






/// blah blah
///
/// @param text big long text that goes
///             really far like this.
///
/// @param indent another long winded
///               thing that does this
///               far.
///
/// @return NOTHING, you get NOTHING sir
///         now good day SIR.
///
///
/// Steps of Operation:
///
/// 1. blah blah blah
///    blah
///
/// 2. Do this other thing
///    haha
void Function::outputComment(const QString& text, int indent)
{
   bool first {true};
   QStringList words {text.split(QRegExp("\\s"),QString::SkipEmptyParts)};
   while ( !words.isEmpty() )
   {
      QString line {QString("/// ").append(words.takeFirst().append(" "))};
      if ( !first )
      {
         for (int i = 0; i < indent ;++i) line.append(" ");
      }
      int total {_baseIndent + 4 + words.first().size()};
      if ( first ) first = false;
      else total += indent;
      while ( !words.isEmpty() && (total + words.first().size() + 1) <= 100 )
      {
         total += words.first().size() + 1;
         line.append(words.takeFirst()).append(" ");
      }
      addLine(line,_baseIndent);
   }
}
