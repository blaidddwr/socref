#include "abstractparser.h"
#include <QFile>
#include <exception.h>
//






/*!
 * This constructs a new parser object that will be a root or primary parser. This 
 * means this parser will be assumed to be the root parser of a source file. 
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
 */
AbstractParser::AbstractParser(AbstractParser* parent)
{
   // Set this object's parent as the given parent parser, add this parser to its 
   // parent's list of children, and set this object's root parser pointer. 
   setParent(parent);
   parent->_children << this;
   if ( parent->_root ) _root = parent->_root;
   else _root = parent;

   // Set all resource pointers to point to this object's root parser resources. 
   _input = _root->_input;
   _index = _root->_index;
   _indent = _root->_indent;
   _output = _root->_output;
}






/*!
 * This deletes the dynamically allocated resources if this parser if it is the 
 * root. If this is a child parser then this does nothing. 
 */
AbstractParser::~AbstractParser()
{
   // If this is a root parser then delete all allocated resources else do nothing. 
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
 */
void AbstractParser::execute(QFile* file)
{
   // If this is not a root parser then return. 
   if ( _root ) return;

   // Set the read line index to the beginning, call the initialize interface, and 
   // read in all lines of the source file. 
   *_index = 0;
   initialize();
   read(file);

   // Process the input lines, output lines, and then write the output to the source 
   // file. 
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
 */
void AbstractParser::stepIntoChild(AbstractParser* child)
{
   // If the given parser object's parent is not this parser then throw an exception, 
   // else set the internal child pointer signal variable to the given parser. 
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
 */
void AbstractParser::setIndent(int indent)
{
   // If the given indent is less than 0 then throw an exception, else set the root 
   // parser object's indent to the new value given. 
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
 */
void AbstractParser::add(const QString& line)
{
   // Prepend the given line with spaces equal to the indent of the root parser. 
   QString whitespace;
   for (int i = 0; i < *_indent ;++i) whitespace.append(' ');

   // Append the given line to the list of output lines of the root parser. 
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
 */
void AbstractParser::read(QFile* file)
{
   // Read all contents of the given file, storing it as this object's original 
   // contents. If reading in the file failed then throw an exception. 
   _original = file->readAll();
   if ( file->error() )
   {
      Exception::SystemError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in file: %1").arg(file->errorString()));
      throw e;
   }

   // Split the original content by line and save it to this object's list of input 
   // lines to be processed. 
   *_input = _original.split('\n');
}






/*!
 * Processes the input of the source file, using the read line interface for 
 * processing. 
 */
void AbstractParser::processInput()
{
   // While the root parser object's index is not at the end of the input line list 
   // continue. Call the read line interface with the next input line to process. If 
   // the interface returns false then return. If the step into child function was 
   // called and set the child pointer signal then call the child's process input 
   // function and then set the child pointer signal back to null. 
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
 */
void AbstractParser::processOutput()
{
   // Clear the root parser object's output string list, set its indent to 0, and 
   // call the make output interface. 
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
 */
void AbstractParser::write(QFile* file)
{
   // Join the root parser object's output lines into the string _newSource_. 
   QString newSource {_output->join('\n').append('\n')};

   // If _newSource_ is different from the root parser object's original file content 
   // then continue. Truncate the given file to 0 size and then write out the 
   // contents of _newSource_ to the given file. If any error occurs in truncating or 
   // writing to the given file then throw an exception. 
   if ( _original != newSource )
   {
      if ( !file->resize(0) )
      {
         Exception::SystemError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Failed truncating file: %1").arg(file->errorString()));
         throw e;
      }
      if ( file->write(newSource.toLocal8Bit()) == -1 )
      {
         Exception::SystemError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Failed writing to file: %1").arg(file->errorString()));
         throw e;
      }
   }
}
