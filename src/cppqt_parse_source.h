#ifndef CPPQT_PARSE_SOURCE_H
#define CPPQT_PARSE_SOURCE_H
#include "cppqt_parse_global.h"
#include "cppqt_parse.h"
#include "global.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       */
      class Source : public Global
      {
         Q_OBJECT
      public:
         explicit Source(const Namespace* block);
         explicit Source(const Namespace* block, const QString& name);
      protected:
         virtual void initialize() override final;
         virtual bool readLine(const QString& line) override final;
         virtual void makeOutput() override;
         virtual void readTop(const QString& line);
         virtual void evaluateVariable(CppQt::Variable* block);
         virtual void evaluateFunction(CppQt::Function* block);
         virtual void evaluateOther(AbstractBlock* block);
      protected:
         void outputPreProcessor();
         void outputMisc(bool addUsingName = false);
         void outputDefinitions();
         Function* findDefined(const QString& definition);
         const QList<AbstractBlock*>& children() const;
         void addPreProcess(const QString& line);
         void addMisc(const QString& line);
         void addVariable(Variable* parser);
         void addDefined(Function* defined);
         void addUndefined(Function* undefined);
         bool isTemplate() const;
      private:
         void makeUsingName();
         void buildAll();
         /*!
          */
         const Namespace* _block;
         /*!
          */
         int _headerLines;
         /*!
          */
         int _functionLines;
         /*!
          */
         QList<AbstractBlock*> _children;
         /*!
          */
         QStringList _preProcess;
         /*!
          */
         QStringList _misc;
         /*!
          */
         QList<Variable*> _variables;
         /*!
          */
         QList<Function*> _defined;
         /*!
          */
         QList<Function*> _undefined;
         /*!
          */
         bool _pastTop {false};
         /*!
          */
         bool _isTemplate {false};
         /*!
          */
         bool _inComments {false};
         /*!
          */
         QString _usingName;
         /*!
          */
         QString _include;
      };
   }
}



#endif
