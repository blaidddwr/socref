#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H
#include <QObject>



class QFile;
//



/*!
 * This represents a parser object which takes in a single source code file to 
 * parse and outputs the results, overwriting the file if the results are 
 * different from the original contents. It can contain child parser objects 
 * which can be used for parsing segments of the source file allowing for 
 * specializing of parsing within segments of the file. Children parser objects 
 * will write lines to its root parser object. This allows for parsing the same 
 * file but using different parser object types when different parts of the file 
 * are reached. This class is designed to only have implementations executed 
 * once and then destroyed. 
 */
class AbstractParser : public QObject
{
   Q_OBJECT
public:
   explicit AbstractParser();
   explicit AbstractParser(AbstractParser* parent);
   virtual ~AbstractParser();
   void execute(QFile* file);
protected:
   virtual void initialize();
   /*!
    * This interface is called for each line of the input source file, reading in 
    * all lines of the file until the end is reached or this returns false. If 
    * false is returned and this is a child parser then this interface keeps 
    * getting called on this object's parent parser under the same conditions. A 
    * different function is used to signal this parser should start feeding input 
    * lines to a child parser this parser contains. 
    *
    * @param line The line from the source file that is processed. 
    *
    * @return True signals this interface should continue being called on lines 
    *         else false signals this parser is finished processing input. 
    */
   virtual bool readLine(const QString& line) = 0;
   /*!
    * This interface creates the output lines for the source file, using the add 
    * and indent functions to do so. 
    */
   virtual void makeOutput() = 0;
   void stepIntoChild(AbstractParser* child);
   int indent();
   void setIndent(int indent);
   void add(const QString& line);
   void add(const QStringList& lines);
   void add(int count);
private:
   void read(QFile* file);
   void processInput();
   void processOutput();
   void write(QFile* file);
   /*!
    * The original content of the file that is used to compare the output once 
    * parsing is complete. 
    */
   QString _original;
   /*!
    * Pointer to the input lines of the parsed source file. All parsing object's 
    * with the same root share this as a single resource where the root parser is 
    * responsible for allocating and destroying it. 
    */
   QStringList* _input {nullptr};
   /*!
    * This stores the index pointing to the next input line to be processed. All 
    * parsing object's with the same root share this as a single resource where the 
    * root parser is responsible for allocating and destroying it. 
    */
   int* _index {nullptr};
   /*!
    * Pointer to the indent that is used for prepending spaces to all output lines 
    * when they are added. All parsing object's with the same root share this as a 
    * single resource where the root parser is responsible for allocating and 
    * destroying it. 
    */
   int* _indent {nullptr};
   /*!
    * Pointer to the output lines of the parsed file. All parsing object's with the 
    * same root share this as a single resource where the root parser is 
    * responsible for allocating and destroying it. 
    */
   QStringList* _output {nullptr};
   /*!
    * Pointer to this parser object's root parser object. If this parser is the 
    * root then this is null. 
    */
   AbstractParser* _root {nullptr};
   /*!
    * A list of pointers to all of this parser object's children. 
    */
   QList<AbstractParser*> _children;
   /*!
    * Special pointer signal variable set by the step into child function. It is 
    * used to signal the process input function that it should step into the 
    * pointed to child parser object's process input function. 
    */
   AbstractParser* _child {nullptr};
};



#endif
