#ifndef GLSL_SETTINGS_H
#define GLSL_SETTINGS_H
#include <QString>
#include <socutil/soc_ut_singleton.h>
//



namespace GLSL
{
   /*!
    * This is the singleton settings class that holds all global settings for GLSL 
    * projects. This class uses qt settings behind the scenes to make its setting 
    * persistent. As mentioned, it is also a single global class since it holds global 
    * settings. 
    */
   class Settings : public Soc::Ut::Singleton<Settings>
   {
   public:
      Settings();
      int variableLines() const;
      int structLines() const;
      int functionLines() const;
      int indentSpaces() const;
      int maxColumns() const;
      void setVariableLines(int value);
      void setStructLines(int value);
      void setFunctionLines(int value);
      void setIndentSpaces(int value);
      void setMaxColumns(int value);
   private:
      /*!
       * The settings key used to save the variable lines setting. 
       */
      static const char* _variableLinesKey;
      /*!
       * The settings key used to save the structure lines setting. 
       */
      static const char* _structLinesKey;
      /*!
       * The settings key used to save the function lines setting. 
       */
      static const char* _functionLinesKey;
      /*!
       * The settings key used to save the indent spaces setting. 
       */
      static const char* _indentSpacesKey;
      /*!
       * The settings key used to save the max columns setting. 
       */
      static const char* _maxColumnsKey;
   private:
      void updateSetting(const QString& key, int value);
      /*!
       * The number of blank lines preceding variables in parsed code. 
       */
      int _variableLines;
      /*!
       * The number of blank lines preceding structures in parsed code. 
       */
      int _structLines;
      /*!
       * The number of blank lines preceding functions in parsed code. 
       */
      int _functionLines;
      /*!
       * The number of spaces used per indent in parsed code. 
       */
      int _indentSpaces;
      /*!
       * The max columns global setting used to determine how long lines will be in 
       * parsed code. 
       */
      int _maxColumns;
   };
}



#endif
