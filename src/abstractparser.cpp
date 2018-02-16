#include <QFile>
#include <exception.h>
#include "abstractparser.h"






AbstractParser::AbstractParser(AbstractParser* parent)
{
   setParent(parent);
   parent->_children << this;
   if ( parent->_root ) _root = parent->_root;
   else _root = parent;
   _input = _root->_input;
   _index = _root->_index;
   _output = _root->_output;
}






AbstractParser::~AbstractParser()
{
   if ( !_root )
   {
      delete _input;
      delete _index;
      delete _output;
   }
}






void AbstractParser::execute(QFile* file)
{
   if ( _root ) return;
   _input = new QStringList;
   _index = new int;
   _output = new QStringList;
   read(file);
   processInput();
   processOutput();
   write(file);
}






void AbstractParser::addLine(const QString& line, int indent)
{
   QString whitespace;
   while ( indent-- > 0 ) whitespace.append(' ');
   *_output << whitespace.append(line);
}






void AbstractParser::read(QFile* file)
{
   _origional = file->readAll();
   if ( file->error() )
   {
      Exception::SystemError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in file: %1").arg(file->errorString()));
      throw e;
   }
   *_input = _origional.split('\n');
   _output->clear();
}






void AbstractParser::processInput()
{
   *_index = 0;
   while ( *_index < _input->size() )
   {
      if ( AbstractParser* child = readLine(_input->at(*_index)) ) child->processInput();
      else ++(*_index);
   }
}






void AbstractParser::processOutput()
{
   qDeleteAll(_children);
   _output->clear();
   makeOutput();
}






void AbstractParser::write(QFile* file)
{
   QString new_ {_output->join('\n').append('\n')};
   if ( _origional != new_ )
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
