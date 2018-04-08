#ifndef CPPQT_SETTINGS_H
#define CPPQT_SETTINGS_H
#include <QString>
#include "singleton.h"
//



namespace CppQt
{
   class Settings : public Singleton<Settings>
   {
   public:
      Settings();
      int indentSpaces() const;
      int headerLines() const;
      int functionLines() const;
      int maxColumns() const;
      void setIndentSpaces(int indentSpaces);
      void setHeaderLines(int headerLines);
      void setFunctionLines(int functionLines);
      void setMaxColumns(int maxColumns);
   private:
      void updateSetting(const QString& key, int value);
      static const char* _indentSpacesKey;
      static const char* _headerLinesKey;
      static const char* _functionLinesKey;
      static const char* _maxColumnsKey;
      int _indentSpaces;
      int _headerLines;
      int _functionLines;
      int _maxColumns;
   };
}



#endif
