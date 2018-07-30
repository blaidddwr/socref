#include "cppqt_parse_variable.h"
#include "cppqt_parse_base.h"
#include "cppqt_variable.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 */
void Variable::outputComments()
{
   add("/*!");
   add(makeComment(_block->description()));
   add(" */");
}






/*!
 */
void Variable::outputDeclaration()
{
   outputComments();
   QString line;
   if ( _block->isConstExpr() ) line.append("constexpr ");
   if ( _block->isStatic() ) line.append("static ");
   if ( _block->isMutable() ) line.append("mutable ");
   if ( line.isEmpty() && !_block->isMember() ) line.append("extern ");
   line.append(_block->variableType()).append(" ").append(_block->Base::name());
   outputEnd(&line
             ,_block->hasInitializer()
              && ( _block->isConstExpr() || ( _block->isMember() && !_block->isStatic() ) ));
}






/*!
 */
void Variable::outputDefinition()
{
   if ( _block->isStatic() || !_block->isMember() )
   {
      QString line;
      QString templateString {makeTemplateDeclaration(_block)};
      if ( !templateString.isEmpty() ) line.append(templateString).append(" ");
      line.append(_block->variableType()).append(" ");
      line.append(makePreScope(_block));
      line.append(_block->Base::name());
      outputEnd(&line, _block->hasInitializer() && !_block->isConstExpr() );
   }
}






/*!
 *
 * @param block  
 *
 * @param parent  
 */
Variable::Variable(CppQt::Variable* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{}






/*!
 *
 * @param line  
 */
bool Variable::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}






/*!
 *
 * @param line  
 *
 * @param withInitializer  
 */
void Variable::outputEnd(QString* line, bool withInitializer)
{
   if ( withInitializer )
   {
      const QString initialize {_block->initializer()};
      if ( (initialize.size() + line->size() + indent() + 1) > Settings::instance().maxColumns() )
      {
         add(*line);
         add("{");
         setIndent(indent() + 3);
         bool first {true};
         const QStringList parts {initialize.split(',')};
         for (auto part : parts)
         {
            QString line;
            if ( first ) first = false;
            else line.append(",");
            line.append(part);
            add(line);
         }
         setIndent(indent() - 3);
         add("};");
      }
      else
      {
         line->append(" {").append(initialize).append("};");
         add(*line);
      }
   }
   else
   {
      line->append(";");
      add(*line);
   }
}
