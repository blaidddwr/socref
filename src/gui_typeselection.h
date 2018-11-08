#ifndef GUI_TYPESELECTION_H
#define GUI_TYPESELECTION_H
#include <QPushButton>
//



namespace Gui
{
   /*!
    * This is the type selection widget. This expanded push button widget provides a 
    * menu with all available variable types parsed from a given JSON object. Objects 
    * within the given object are recursively transformed into sub menus. This is only 
    * used for editors of blocks of a project type that has strongly typed variables 
    * such as C++. 
    */
   class TypeSelection : public QPushButton
   {
      Q_OBJECT
   public:
      explicit TypeSelection(const QJsonObject& object, QWidget* parent = nullptr);
      QString value() const;
      void setValue(QString value);
   private slots:
      void selectionTriggered(int index);
   private:
      QMenu* setupMenu(const QJsonObject& object, const QString& key);
      void addType(QMenu* menu, QString value);
      /*!
       * List of all possible types the menu of this widget allows the user to select. 
       */
      QStringList _types;
      /*!
       * The current type value for this selection widget. 
       */
      QString _value;
   };
}



#endif
