#ifndef COMMON_H
#define COMMON_H
#include <exception.h>
#endif
//



class AbstractBlock;
class AbstractBlockFactory;
class AbstractParser;
class AbstractParserFactory;
class AbstractProjectFactory;
class ProjectFactory;
class BlockModel;
class ScanThread;
class Project;
void showException(const QString& text, const Exception::Base& exception);



#endif
