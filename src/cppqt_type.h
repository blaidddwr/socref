#ifndef CPPQT_TYPE_H
#define CPPQT_TYPE_H
#include <abstractblock.h>



namespace CppQt
{
   class Type : public AbstractBlock
   {
      Q_OBJECT
   public:
      static bool isValidTypeString(const QString& input);
      static bool isValidTemplateArgument(const QString& argument);
      explicit Type() = default;
      explicit Type(const QString& type);
      virtual int type() const override final;
      virtual const AbstractBlockFactory& factory() const override final;
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
      QString cppType();
      void setType(const QString& type);
   protected:
      virtual void readData(const QDomElement& element, int version) override final;
      virtual int writeVersion() const override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void copyDataFrom(const AbstractBlock* other) override final;
   private:
      constexpr static int _version {0};
      static const char* _typeTag;
      QString _type;
   };
}



#endif
