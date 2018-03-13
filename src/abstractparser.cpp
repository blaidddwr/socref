#include "abstractparser.h"
#include <QFile>
#include <exception.h>
//






/*!
 */
AbstractParser::AbstractParser():
   _input(new QStringList),
   _index(new int(0)),
   _indent(new int(0)),
   _output(new QStringList)
{}






/*!
 *
 * @param parent  
 */
AbstractParser::AbstractParser(AbstractParser* parent)
{
   setParent(parent);
   parent->_children << this;
   if ( parent->_root ) _root = parent->_root;
   else _root = parent;
   _input = _root->_input;
   _index = _root->_index;
   _indent = _root->_indent;
   _output = _root->_output;
}






/*!
 */
AbstractParser::~AbstractParser()
{
   if ( !_root )
   {
      delete _input;
      delete _index;
      delete _output;
   }
}






/*!
 *
 * @param file  
 */
void AbstractParser::execute(QFile* file)
{
   if ( _root ) return;
   *_index = 0;
   initialize();
   read(file);
   processInput();
   processOutput();
   write(file);
}






/*!
 */
void AbstractParser::initialize()
{}






/*!
 *
 * @param child  
 */
void AbstractParser::stepIntoChild(AbstractParser* child)
{
   if ( child->parent() != this )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot step into abstract pointer that is not a child."));
      throw e;
   }
   _child = child;
}






/*!
 */
int AbstractParser::indent()
{
   return *_indent;
}






/*!
 *
 * @param indent  
 */
void AbstractParser::setIndent(int indent)
{
   if ( indent < 0 )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set indent to %1.").arg(indent));
      throw e;
   }
   *_indent = indent;
}






/*!
 *
 * @param line  
 */
void AbstractParser::add(const QString& line)
{
   QString whitespace;
   for (int i = 0; i < *_indent ;++i) whitespace.append(' ');
   *_output << whitespace.append(line);
}






/*!
 *
 * @param lines  
 */
void AbstractParser::add(const QStringList& lines)
{
   for (auto line : lines) add(line);
}






/*!
 *
 * @param count  
 */
void AbstractParser::add(int count)
{
   while ( count-- > 0 ) *_output << QString();
}






/*!
 *
 * @param file  
 */
void AbstractParser::read(QFile* file)
{
   _original = file->readAll();
   if ( file->error() )
   {
      Exception::SystemError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in file: %1").arg(file->errorString()));
      throw e;
   }
   *_input = _original.split('\n');
   _output->clear();
}






/*!
 */
void AbstractParser::processInput()
{
   while ( *_index < _input->size() )
   {
      if ( !readLine(_input->at((*_index)++)) ) return;
      if ( _child )
      {
         _child->processInput();
         _child = nullptr;
      }
   }
}






/*!
 */
void AbstractParser::processOutput()
{
   _output->clear();
   *_indent = 0;
   makeOutput();
}






/*!
 *
 * @param file  
 */
void AbstractParser::write(QFile* file)
{
   QString new_ {_output->join('\n').append('\n')};
   if ( _original != new_ )
   {
      if ( !file->resize(0) )
      {
         Exception::SystemError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Failed truncating file: %1").arg(file->errorString()));
         throw e;
      }
      if ( file->write(new_.toLocal8Bit()) == -1 )
      {
         Exception::SystemError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Failed writing to file: %1").arg(file->errorString()));
         throw e;
      }
   }
}
