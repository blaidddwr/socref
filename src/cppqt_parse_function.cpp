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






Function::Function(CppQt::Function* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{
   _definition = functionDefinition(_block,true,true,false,false);
}






Function::Function(const QString& definition, AbstractParser* parent):
   Base(parent),
   _definition(definition)
{}






bool Function::isMatch(QString line)
{
   if ( line.at(line.size() - 1) == QChar(':') ) line.chop(1);
   return line == _definition;
}






bool Function::hasCode() const
{
   return !_initializers.isEmpty() || !_code.isEmpty();
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
   if ( !_initializers.isEmpty() )
   {
      addLine(_definition + ":");
      for (auto line : _initializers) addLine(line);
   }
   else addLine(_definition);
   if ( _code.isEmpty() ) addLine("{}");
   else
   {
      addLine("{");
      for (auto line : _code) addLine(line);
      addLine("}");
   }
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






void Function::outputComments()
{
   outputComment(_block->description());
   outputArgumentComments();
   outputReturnDescriptionComment();
   outputOperationComments();
   addLine("///");
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
      addLine("///");
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
      addLine("///");
      addLine("///");
      outputComment("Steps of Operation:");
      for (int i = 0; i < operations.size() ;++i)
      {
         addLine("///");
         QString base {QString::number(i + 1).append(". ")};
         int indent {base.size()};
         outputComment(base.append(operations.at(i)),indent);
      }
   }
}
