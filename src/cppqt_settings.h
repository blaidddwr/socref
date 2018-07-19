#ifndef CPPQT_SETTINGS_H
#define CPPQT_SETTINGS_H
#include <QString>
#include "singleton.h"
//



namespace CppQt
{
   /*!
    * This is the singleton settings class that holds all global settings for C++/Qt 
    * projects. This class uses qt settings behind the scenes to make its setting 
    * persistent. As mentioned, it is also a single global class since it holds global 
    * settings. 
    */
   class Settings : public Singleton<Settings>
   {
   public:
      Settings();
      int indentSpaces() const;
      int headerLines() const;
      int functionLines() const;
      int maxColumns() const;
      void setIndentSpaces(int value);
      void setHeaderLines(int value);
      void setFunctionLines(int value);
      void setMaxColumns(int value);
   private:
      void updateSetting(const QString& key, int value);
      /*!
       * The settings key used to save the indent spaces setting. 
       */
      static const char* _indentSpacesKey;
      /*!
       * The settings key used to save the header lines setting. 
       */
      static const char* _headerLinesKey;
      /*!
       * The settings key used to save the function lines setting. 
       */
      static const char* _functionLinesKey;
      /*!
       * The settings key used to save the max columns setting. 
       */
      static const char* _maxColumnsKey;
      /*!
       * The number of spaces used per indent in parsed code. 
       */
      int _indentSpaces;
      /*!
       * The number of lines preceding the header of parsed code files. 
       */
      int _headerLines;
      /*!
       * The number of blank lines preceding functions in parsed code. 
       */
      int _functionLines;
      /*!
       * The max columns global setting used to determine how long lines will be in 
       * parsed code. 
       */
      int _maxColumns;
   };
}



#endif
