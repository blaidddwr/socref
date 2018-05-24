#ifndef CPPQT_NAMESPACE_H
#define CPPQT_NAMESPACE_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    */
   class Namespace : public Base
   {
      Q_OBJECT
   public:
      virtual int type() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
      virtual QList<AbstractBlock*> realChildren() const;
   public:
      /*!
       */
      explicit Namespace() = default;
      explicit Namespace(const QString& name);
      const Namespace* root() const;
      Namespace* root();
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
   };
}



#endif
