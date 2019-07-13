#include "scanner.h"
#include <QFile>
#include <QRegularExpression>
#include <socutil/ReadError>
#include <socutil/WriteError>
#include "abstract_parser.h"
using Soc::Ut::ReadError;
using Soc::Ut::WriteError;






/*!
 * Deletes the header expression if it exists.
 */
Scanner::~Scanner()
{
   delete _headerExpr;
}






/*!
 * Parses the source file with the given path using this scanner's parser
 * elements, overwriting any contents the file may contain if it is different
 * from the output of the parser elements. If this scanner has no parser
 * elements then this does nothing.
 *
 * @param path Path of the source file that is parsed and possibly overwritten.
 */
void Scanner::parse(const QString& path)
{
   // Make sure this scanner has parser elements.
   if ( _parsers.isEmpty() ) return;

   try
   {
      // Open the source file with the given path and make sure it works.
      QFile file(path);
      if ( !file.open(QFile::ReadWrite) )
      {
         throw ReadError(qUtf8Printable(tr("Failed opening source file %1").arg(path))
                         ,qUtf8Printable(file.errorString()));
      }

      // Read the entire contents of the opened source file and make sure it worked.
      _input = file.readAll();
      if ( file.error() != QFile::NoError )
      {
         throw ReadError(qUtf8Printable(tr("Failed reading source file %1").arg(path))
                         ,qUtf8Printable(file.errorString()));
      }

      // Parse the input for any input parser elements and then create the output using
      // all parser elements.
      parseInput();
      QString output {createOutput()};

      // Check to see if the output is different from the original contents of the open
      // source file.
      if ( output != _input )
      {
         // Truncate the open source file to zero and make sure it worked.
         if ( !file.resize(0) )
         {
            throw WriteError(qUtf8Printable(tr("Failed truncating source file %1").arg(path))
                             ,qUtf8Printable(file.errorString()));
         }

         // Write the output to the open source file and make sure all bytes were written.
         QByteArray bytes {output.toLocal8Bit()};
         if ( file.write(bytes) != bytes.size() )
         {
            throw WriteError(qUtf8Printable(tr("Failed writing source file %1").arg(path))
                             ,qUtf8Printable(file.errorString()));
         }
      }

      // Reset all parser elements of this scanner in case this parses another source
      // file.
      for (auto parser: _parsers) parser->reset();
   }

   // Catch any exception thrown to reset all parser elements of this scanner in case
   // it is used again and throw the exception again.
   catch(...)
   {
      for (auto parser: _parsers) parser->reset();
      throw;
   }
}






/*!
 * Adds a new parser element to this scanner, taking ownership of it. Parser
 * element output is added in the same order the parser element's are added
 * using this method.
 *
 * @param parser Parser element added to this scanner which also takes ownership
 *               of it.
 */
void Scanner::addParser(Soc::Ut::QPtr<Abstract::Parser>&& parser)
{
   // Take ownership of the given parser and add it to this scanner's list.
   Abstract::Parser* parserPtr {parser.release(this)};
   _parsers << parserPtr;

   // If the given parser requires input then add it to this scanner's list for
   // input.
   if ( parserPtr->needsInput() ) _inputParsers << parserPtr;
}






/*!
 * Parses the input of the original contents of the source file this scanner is
 * parsing, finding any input parser that still requires input by matching its
 * given header expression.
 */
void Scanner::parseInput()
{
   // Make sure this scanner has parser elements requiring input.
   if ( _inputParsers.isEmpty() ) return;

   // Initialize the current parser pointer taking input to null and create the input
   // lines for parsing from the source file being parsed.
   Abstract::Parser* parser {nullptr};
   QStringList lines {_input.split('\n')};

   // Iterate through all input lines.
   for (auto line: lines)
   {
      // Check to see if there is an active parser element parsing the input lines.
      if ( parser )
      {
         // Parse the line with the current parser element taking input, resetting the
         // active parser pointer to null if it returns false.
         if ( !parser->input(line) ) parser = nullptr;
      }

      // Else there is no active parser element taking input.
      else
      {
         // Try to find a parser element of this scanner whose header expression matches
         // the line.
         if ( (parser = findParser(line)) )
         {
            // Provide the matched header expression line as the first line of input to the
            // parser element that was found.
            if ( !parser->input(line) ) parser = nullptr;
         }
      }
   }
}






/*!
 * Creates and returns the output for the source file this scanner is parsing.
 *
 * @return The output for the source file this scanner is parsing.
 */
QString Scanner::createOutput()
{
   // Construct all lines of output using this scanner's list of parser elements,
   // adding them in order that the parser elements were added.
   QStringList lines;
   for (auto parser: _parsers) lines << parser->output();

   // Combine and return all output lines of output.
   return lines.join('\n');
}






/*!
 * Attempts to find an parser element of this scanner that requires input and
 * whose header expression matches the given line, returning its pointer if one
 * is found.
 *
 * @param line The line used to find a match with any parser element's header
 *             expression that still requires input.
 *
 * @return Pointer to matched parser element requiring input or null if no match
 *         is found.
 */
Abstract::Parser* Scanner::findParser(const QString& line)
{
   // Check to see if the header expression is not initialized.
   if ( !_headerExpr )
   {
      // Construct the header expression consisting of all this scanner's input parser's
      // header expressions separated into groupings of the main header expression.
      QString expression;
      bool first {true};
      for (auto parser: _inputParsers)
      {
         if ( first ) first = false;
         else expression += QStringLiteral("|");
         expression += QStringLiteral("(") + parser->headerExpression() + QStringLiteral(")");
      }

      // Create a new regular expression and set its pointer to this scanner's header
      // expression.
      _headerExpr = new QRegularExpression(expression);
   }

   // Check to see if there is an input parser match for the given line.
   auto match {_headerExpr->match(line)};
   if ( match.hasMatch() )
   {
      // Get the index of the input parser that was matched, making sure it is valid.
      int index {match.lastCapturedIndex() - 1};
      Q_ASSERT(index >= 0);
      Q_ASSERT(index < _inputParsers.size());

      // Return a pointer to the input parser that was matched.
      return _inputParsers.at(index);
   }

   // No match was found to return null.
   return nullptr;
}

