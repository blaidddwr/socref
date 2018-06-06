#ifndef CPPQT_NAMESPACE_H
#define CPPQT_NAMESPACE_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    * This is the namespace block, defining a C++ namespace that can define any number 
    * of other C++ block types. This block is used as the root block for C++/Qt 
    * projects as the global namespace. 
    */
   class Namespace : public Base
   {
      Q_OBJECT
   public:
      class Edit;
      class View;
   public:
      virtual int type() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
      virtual QList<AbstractBlock*> realChildren() const;
   public:
      /*!
       * Constructs a new namespace block. 
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
