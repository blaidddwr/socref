#include "abstractparser.h"
#include <QFile>
#include <exception.h>
//






/*!
 * This constructs a new parser object that will be a root or primary parser. This 
 * means this parser will be assumed to be the root parser of a source file. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Allocate new resources required for this new parser object. 
 */
AbstractParser::AbstractParser():
   _input(new QStringList),
   _index(new int(0)),
   _indent(new int(0)),
   _output(new QStringList)
{}






/*!
 * This constructs a new parser object that is a child of another parser object. 
 * This sets up this object to use the resources of its root parser object. 
 *
 * @param parent The parent parser object for this new parser object. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set this object's parent as the given parent parser, add this parser to its 
 *    parent's list of children, and set this object's root parser pointer. 
 *
 * 2. Set all resource pointers to point to this object's root parser resources. 
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
 * This deletes the dynamically allocated resources if this parser if it is the 
 * root. If this is a child parser then this does nothing. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this is a root parser then delete all allocated resources else do nothing. 
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
 * This parses the given file using this parser as the root parser. If this is a 
 * child parser then this does nothing. 
 *
 * @param file Qt file handle to the file that is parsed. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this is not a root parser then do nothing and exit, else go to the next 
 *    step. 
 *
 * 2. Set the read line index to the beginning, call the initialize interface, and 
 *    read in all lines of the source file. 
 *
 * 3. Process the input lines. 
 *
 * 4. Process the output lines. 
 *
 * 5. Write the output to the source file if the output is different from the 
 *    original. 
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
 * This interface initializes this parser. The default implementation does nothing. 
 */
void AbstractParser::initialize()
{}






/*!
 * Signals this parser that the given child parser should be used for calling the 
 * interface for reading lines until it returns false or the last line is 
 * processed. If the given parser is not a direct child of this parser then an 
 * exception is thrown. 
 *
 * @param child Pointer to the child parser whose interface for reading lines will 
 *              be called from now on. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given parser object's parent is not this parser then an exception is 
 *    thrown, else go to the next step. 
 *
 * 2. Set the internal child pointer signal variable to the given parser. 
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
 * Returns the indent, in spaces, that is prepended to each line added to the 
 * output. 
 *
 * @return Number of spaces prepended to each added line of output. 
 */
int AbstractParser::indent() const
{
   return *_indent;
}






/*!
 * Sets the indent, in spaces, that is prepended to each line added to output. 
 *
 * @param indent The new indent value in spaces. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given indent is less than 0 then throw an exception, else go to the 
 *    next step. 
 *
 * 2. Set the root parser object's indent to the new value given. 
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
 * Adds the given line as a new line to output, appending it to the end of any 
 * existing lines. The line is prepended with a number of spaces equal to the 
 * indent. 
 *
 * @param line The line that is prepended with the indent and appended to the 
 *             output. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Prepend the given line with spaces equal to the indent of the root parser. 
 *
 * 2. Append the given line to the list of output lines of the root parser. 
 */
void AbstractParser::add(const QString& line)
{
   QString whitespace;
   for (int i = 0; i < *_indent ;++i) whitespace.append(' ');
   *_output << whitespace.append(line);
}






/*!
 * Appends multiple lines to the output, each one being prepended with spaces equal 
 * to the indent. 
 *
 * @param lines Lines that are appended to the output. 
 */
void AbstractParser::add(const QStringList& lines)
{
   for (auto line : lines) add(line);
}






/*!
 * Appends the given number of blank lines to the output. If the given number is 
 * less than 1 then nothing is added to the output. 
 *
 * @param count The number of blank lines added to the output. 
 */
void AbstractParser::add(int count)
{
   while ( count-- > 0 ) *_output << QString();
}






/*!
 * Reads in the source file, splitting them into lines, and prepares the output. 
 *
 * @param file The file used as input. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Read all contents of the given file, storing it as this object's original 
 *    contents. 
 *
 * 2. If reading in the file failed and caused an error then throw an exception, 
 *    else go to the next step. 
 *
 * 3. Split the original content by line and save it to this object's list of input 
 *    lines to be processed. 
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
}






/*!
 * Processes the input of the source file, using the read line interface for 
 * processing. 
 *
 *
 * Steps of Operation: 
 *
 * 1. While the root parser object's index is not at the end of the input line list 
 *    do the proceeding steps. 
 *
 * 2. Call the read line interface with the next input line to process. If the 
 *    interface returns false then break the while loop and exit this function. 
 *
 * 3. If the read line interface calls the step into child function then call the 
 *    child's process input function and then set the child pointer signal back to 
 *    null. 
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
 * Processes the output by calling the make output interface. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Clear the root parser object's output string list and set its indent to 0. 
 *
 * 2. Call the make output interface. 
 */
void AbstractParser::processOutput()
{
   _output->clear();
   *_indent = 0;
   makeOutput();
}






/*!
 * Write the processed output to the given file if, after joining the output lines, 
 * it is different from the original. If the output is the same as the original 
 * then this does nothing. 
 *
 * @param file The file that is truncated and written to if the output is different 
 *             from the original. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Join the output lines into one giant string. If the output is different from 
 *    the root parser object's original file content then go to the next step, else 
 *    do nothing and exist the function. 
 *
 * 2. Truncate the given file to 0 and write the root parser object's output to it. 
 *    If any error occurred for either operation then throw an exception. 
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
