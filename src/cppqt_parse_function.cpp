#include "cppqt_parse_function.h"
#include <QStack>
#include <QRegularExpression>
#include <exception.h>
#include "cppqt_parse_common.h"
#include "cppqt_function.h"
#include "cppqt_blockfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_template.h"
#include "cppqt_operator.h"
#include "cppqt_constructor.h"
#include "cppqt_destructor.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;






Function::Function(CppQt::Function* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{}






Function::Function(const QString& definition, AbstractParser* parent):
   Base(parent)
{
   _cutOff = definition.indexOf(QRegularExpression("\\S"));
   _definition = definition.mid(_cutOff);
}






void Function::outputComments()
{
   if ( _block )
   {
      add("/*!");
      add(makeComment(_block->description()));
      add(makeTemplateComments(_block));
      outputArgumentComments();
      if ( _block->type() != BlockFactory::SignalType )
      {
         if ( _block->type() != BlockFactory::SlotType ) outputReturnDescriptionComment();
      }
      add(" */");
   }
   else add("/*! !!! UNKNOWN FUNCTION !!! */");
}






void Function::outputDeclaration()
{
   if ( _block )
   {
      if ( _block->isAbstract()
           || _block->type() == BlockFactory::SignalType
           || _block->isDefault() )
      {
         outputComments();
      }
      QString line {getTemplates(_block,true)};
      if ( !line.isEmpty() ) line.append(" ");
      if ( Constructor* valid = _block->cast<Constructor>(BlockFactory::ConstructorType) )
      {
         if ( valid->isExplicit() ) line.append("explicit ");
      }
      else
      {
         if ( _block->isVirtual() ) line.append("virtual ");
         if ( _block->isConstExpr() ) line.append("constexpr ");
         if ( _block->isStatic() ) line.append("static ");
      }
      line.append(getReturnValue()).append(getName()).append(getArguments(true));
      if ( _block->isConst() ) line.append(" const");
      if ( _block->isNoExcept() ) line.append(" noexcept");
      if ( _block->isOverride() ) line.append(" override");
      if ( _block->isFinal() ) line.append(" final");
      if ( _block->isAbstract() ) line.append(" = 0");
      if ( _block->isDefault() ) line.append(" = default");
      line.append(";");
      add(line);
   }
}






void Function::outputDefinition()
{
   if ( _block )
   {
      QString line;
      QString templateString {getTemplateDeclaration(_block)};
      if ( !templateString.isEmpty() ) line.append(templateString).append(" ");
      line.append(getReturnValue())
          .append(getScope(!templateString.isEmpty()))
          .append(getName())
          .append(getArguments(false));
      if ( _block->isConst() ) line.append(" const");
      if ( _block->isNoExcept() ) line.append(" noexcept");
      if ( !_initializers.isEmpty() ) line.append(":");
      add(line);
   }
   else add(_definition);
   for (auto line : _initializers) add(line);
   if ( _code.isEmpty() ) add("{}");
   else
   {
      add("{");
      for (auto line : _code) add(line);
      add("}");
   }
}






bool Function::isMatch(const QString& line)
{
   if ( hasCode() ) return false;
   QString regular {"\\A.*[: ]"};
   regular.append(getName(true)).append("\\(\\s*");
   for (auto argument : _block->arguments())
   {
      regular.append(argument->variableType().replace("*","\\*")).append(".*");
   }
   regular.append("\\):?");
   if ( _block->isConst() ) regular.append("\\s+const");
   if ( _block->isNoExcept() ) regular.append("\\s+noexcept");
   regular.append("\\s*\\z");
   return QRegularExpression(regular).match(line).hasMatch();
}






bool Function::hasCode() const
{
   return !_initializers.isEmpty() || !_code.isEmpty();
}






void Function::setCutOff(int cutOff)
{
   _cutOff = cutOff;
}






bool Function::readLine(const QString& line)
{
   if ( _level == 0 && QRegularExpression("\\A\\s*\\{\\}\\s*\\z").match(line).hasMatch() )
   {
      return false;
   }
   if ( QRegularExpression("\\A\\s*\\{\\s*\\z").match(line).hasMatch() )
   {
      ++_level;
      if ( _level == 1 ) return true;
   }
   else if ( QRegularExpression("\\A\\s*\\};?\\s*\\z").match(line).hasMatch() )
   {
      --_level;
      if ( _level == 0 ) return false;
   }
   if ( _level == 0) _initializers << line.mid(_cutOff);
   else
   {
      if ( _block && QRegularExpression("\\A\\s*// .*\\z").match(line).hasMatch() )
      {
         if ( !_edgePast )
         {
            _edgePast = true;
            processInlineComment(line);
         }
      }
      else
      {
         _edgePast = false;
         _code << line.mid(_cutOff);
      }
   }
   return true;
}







void Function::processInlineComment(const QString& line)
{
   int spacing {-_cutOff};
   for (auto ch: line)
   {
      if ( !ch.isSpace() ) break;
      else ++spacing;
   }
   if ( spacing < 0 ) spacing = 0;
   if ( _nextOperation < _block->operations().size() )
   {
      insertInlineComment(_nextOperation++,spacing);
   }
   else
   {
      QString blank(spacing,' ');
      _code << blank.append("// ");
   }
}






void Function::insertInlineComment(int index, int spacing)
{
   const int max {Settings::instance().maxColumns()};
   QStringList words {_block->operations().at(index).split(QRegularExpression("\\s+"))};
   while ( !words.isEmpty() )
   {
      int total {words.first().size() + 1};
      QString line(spacing,' ');
      line.append("// ").append(words.takeFirst().append(" "));
      while ( !words.isEmpty() && (total + words.first().size() + 1) <= max )
      {
         total += words.first().size() + 1;
         line.append(words.takeFirst().append(" "));
      }
      _code << line;
   }
}






void Function::outputArgumentComments()
{
   const QList<Variable*> arguments {_block->arguments()};
   for (auto argument : arguments)
   {
      add(" *");
      QString base {"@param "};
      base.append(argument->Base::name()).append(" ");
      int justified {base.size()};
      base.append(argument->description());
      add(makeComment(base,justified));
   }
}






void Function::outputReturnDescriptionComment()
{
   const QString returnDescription {_block->returnDescription()};
   if ( !returnDescription.isEmpty() )
   {
      add(" *");
      QString base {"@return "};
      int justified {base.size()};
      base.append(returnDescription);
      add(makeComment(base,justified));
   }
}






QString Function::getReturnValue()
{
   QString ret;
   if ( _block->type() == BlockFactory::SlotType || _block->type() == BlockFactory::SignalType )
   {
      ret = QString("void ");
   }
   else if ( _block->type() != BlockFactory::ConstructorType
             && _block->type() != BlockFactory::DestructorType )
   {
      ret.append(_block->returnType()).append(" ");
   }
   return ret;
}






QString Function::getScope(bool hasTemplates)
{
   QString ret;
   if ( _block->isMethod() ) ret.append(getClassScope(_block));
   else if ( !hasTemplates ) ret.append(getNamespace(_block));
   return ret;
}






QString Function::getName(bool isRegExp)
{
   QString ret;
   if ( const Operator* valid = qobject_cast<const Operator*>(_block) )
   {
      ret.append("operator");
      if ( isRegExp )
      {
         for (auto ch : valid->operation()) ret.append("\\").append(ch);
      }
      else ret.append(valid->operation());
   }
   else if ( const Destructor* valid = qobject_cast<const Destructor*>(_block) )
   {
      ret.append("~").append(valid->className());
   }
   else if ( const Constructor* valid = qobject_cast<const Constructor*>(_block) )
   {
      ret.append(valid->className());
   }
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
      if ( withInitializers && argument->hasInitializer() )
      {
         ret.append(" = ").append(argument->initializer());
      }
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
      else if ( Class* block = back->cast<Class>(BlockFactory::ClassType) )
      {
         return block->hasAnyTemplates();
      }
      back = back->parent();
   }
   return false;
}
