#include "abstractparser.h"
#include <QFile>
#include <socutil/sut_exceptions.h>



using namespace Sut;
//






/*!
 * Constructs a new parser object that will be a root or primary parser. This means 
 * this parser will be assumed to be the root parser of a source file. 
 */
AbstractParser::AbstractParser():
   _input(new QStringList),
   _index(new int(0)),
   _indent(new int(0)),
   _output(new QStringList)
{}






/*!
 * Constructs a new parser object that is a child of another parser object. This 
 * sets up this object to use the resources of its root parser object. 
 *
 * @param parent The parent parser object for this new parser object. 
 */
AbstractParser::AbstractParser(AbstractParser* parent)
{
   // Set this object's parent as the given parent parser and add this parser to its 
   // parent's list of children. 
   setParent(parent);
   parent->_children << this;

   // Find the root parser and this object's root parser pointer. 
   if ( parent->_root ) _root = parent->_root;
   else _root = parent;

   // Set all resource pointers to point to this object's root parser resources. 
   _input = _root->_input;
   _index = _root->_index;
   _indent = _root->_indent;
   _output = _root->_output;
}






/*!
 * Deletes the dynamically allocated resources if this is a root parser. If this 
 * object is a child parser then nothing is done. 
 */
AbstractParser::~AbstractParser()
{
   // Make sure this is the root parser. 
   if ( !_root )
   {
      // Delete all resources of this root parser. 
      delete _input;
      delete _index;
      delete _output;
   }
}






/*!
 * Parses the given file using this parser as the root parser. If this is a child 
 * parser then this does nothing. 
 *
 * @param file Qt file handle to the file that is parsed. 
 */
void AbstractParser::execute(QFile* file)
{
   // Make sure this is a root parser. 
   if ( _root ) return;

   // Initialize this parser and read in the given file. 
   *_index = 0;
   initialize();
   read(file);

   // Process the input from the read in file then process the output making a 
   // temporary output. 
   processInput();
   processOutput();

   // Possibly write the processed output to the given file. 
   write(file);
}






/*!
 * This interface initializes this parser and is called before any parsing begins. 
 * The default implementation does nothing. 
 */
void AbstractParser::initialize()
{}






/*!
 * Signals this parser that the given child parser should be used for reading lines 
 * until it returns false or the last line is processed. If the given parser is not 
 * a direct child of this parser then an exception is thrown. 
 *
 * @param child Pointer to the child parser whose interface for reading lines will 
 *              be called from now on. 
 */
void AbstractParser::stepIntoChild(AbstractParser* child)
{
   // Make sure the given parser is a direct child of this parser. 
   if ( child->parent() != this )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot step into abstract pointer that is not a child."));
      throw e;
   }

   // Set this object's step into child pointer to the given parser. 
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
   // Make sure the given indent is not negative. 
   if ( indent < 0 )
   {
      Exception::OutOfRange e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set indent to %1.").arg(indent));
      throw e;
   }

   // Set the this object's root indent to the new value. 
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
   // Add white space equal to this object's root indent to the beginning of the 
   // given line and then append the line as a new output line. 
   *_output << QString(*_indent,' ').append(line);
}






/*!
 * Appends multiple lines to the output, each one being prepended with spaces equal 
 * to the indent. 
 *
 * @param lines Lines that are appended to the output. 
 */
void AbstractParser::add(const QStringList& lines)
{
   // Iterate through all strings of the given string list and add each one as 
   // output. 
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
   // Add the given number of blank lines to output. 
   while ( count-- > 0 ) *_output << QString();
}






/*!
 * Reads in the source file, splitting them into lines, and prepares the output. 
 *
 * @param file The file used as input. 
 */
void AbstractParser::read(QFile* file)
{
   // Read in the entire contents of the given file to this object's original string. 
   _original = file->readAll();

   // Make sure reading in the given file did not fail. 
   if ( file->error() )
   {
      Exception::SystemError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in file: %1").arg(file->errorString()));
      throw e;
   }

   // Split this object's original content by line and save it to this object's list 
   // of input lines to be processed. 
   *_input = _original.split('\n');
}






/*!
 * Processes the input of the source file, using the read line interface for 
 * processing. 
 */
void AbstractParser::processInput()
{
   // Iterate through all input lines using this object's root index to keep track of 
   // which line is next. 
   while ( *_index < _input->size() )
   {
      // Process the next line, returning from this function if the interface returns 
      // false. This will move parsing back to the parent parser unless this is the root 
      // parser in which case processing of input will end. 
      if ( !readLine(_input->at((*_index)++)) ) return;

      // Check if the step into child pointer was set. 
      if ( _child )
      {
         // Step into this object's step into child pointer for processing until it returns 
         // and then clear the step into child pointer. 
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
   // Clear any output lines this parser may contain and reset the root indent to 0. 
   _output->clear();
   *_indent = 0;

   // Call the make output interface responsible for building all output lines. 
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
   // Join the root parser object's output lines into a single new source string. 
   QString newSource {_output->join('\n').append('\n')};

   // Make sure the new source string is different from the original file. 
   if ( _original == newSource ) return;

   // Truncate the given file and make sure it did not fail. 
   if ( !file->resize(0) )
   {
      Exception::SystemError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Failed truncating file: %1").arg(file->errorString()));
      throw e;
   }

   // Write the new source string to the given file and make sure it did not fail. 
   if ( file->write(newSource.toLocal8Bit()) == -1 )
   {
      Exception::SystemError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Failed writing to file: %1").arg(file->errorString()));
      throw e;
   }
}
