#ifndef CPPQT_DESTRUCTOR_H
#define CPPQT_DESTRUCTOR_H
#include "cppqt_constructor.h"



namespace CppQt
{
   class Destructor : public Constructor
   {
      Q_OBJECT
   public:
      explicit Destructor() = default;
      virtual QString name() const override;
      virtual int type() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
   };
}



#endif
