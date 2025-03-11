#ifndef GLOBAL_H
#define GLOBAL_H
#define CODE_LEGACY 0

class AbstractBlock;
class AbstractBlockWidget;
class AbstractLanguage;
class AbstractParser;
class AbstractRouter;


/*!
 * This enumerates all versions of this application, excluding revisions.
 */
enum Version
{
    Socref_Legacy = 0
    ,Socref_1_0 = 1
    ,Socref_Current = Socref_1_0
};


#endif
