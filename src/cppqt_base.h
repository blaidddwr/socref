#ifndef CPPQT_BASE_H
#define CPPQT_BASE_H
#include "abstractblock.h"
//



namespace CppQt
{
   /*!
    */
   class Base : public AbstractBlock
   {
      Q_OBJECT
   public:
      /*!
       */
      explicit Base() = default;
      explicit Base(const QString& name);
      virtual const AbstractBlockFactory& factory() const override final;
      virtual QString name() const override;
      QString description() const;
      void setName(const QString& name);
      void setDescription(const QString& description);
   protected:
      virtual void readData(const QDomElement& element, int version) override;
      virtual int writeVersion() const override;
      virtual QDomElement writeData(QDomDocument& document) const override;
      virtual void copyDataFrom(const AbstractBlock* other) override;
   private:
      void readVersion0(const QDomElement& element);
      void readVersion1(const QDomElement& element);
      /*!
       */
      constexpr static int _version {1};
      /*!
       */
      static const char* _nameTag;
      /*!
       */
      static const char* _descriptionTag;
      /*!
       */
      QString _name;
      /*!
       */
      QString _description;
   };
}



#endif
