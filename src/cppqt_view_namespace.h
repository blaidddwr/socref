#ifndef CPPQT_VIEW_NAMESPACE_H
#define CPPQT_VIEW_NAMESPACE_H
#include <QLabel>



class AbstractBlock;



namespace CppQt {

namespace View {

class Namespace : public QLabel
{
public:
   Namespace(AbstractBlock* block);
};

}

}



#endif
