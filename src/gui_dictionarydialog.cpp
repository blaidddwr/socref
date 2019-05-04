#include "gui_dictionarydialog.h"
#include <QVBoxLayout>
#include <QListView>
#include <QPushButton>
#include <QAction>
#include "project.h"
#include "dictionarymodel.h"



namespace Gui
{






/*!
 * Constructs a new dictionary dialog with the given project.
 *
 * @param project Pointer to the project whose dictionary model is used for this
 *                new dictionary dialog.
 */
DictionaryDialog::DictionaryDialog(Project* project)
   :
   PersistentDialog("gui.dictionarydialog.geometry")
{
   // Make sure the given project pointer is not null.
   Q_ASSERT(project);

   // Get the dictionary model of the given project.
   _model = project->dictionary();

   // Create and initialize the GUI of this new dialog and set its window title.
   setupGui();
   setWindowTitle(tr("Dictionary"));
}






/*!
 * Called when the remove button or shortcut is triggered, removing the
 * currently selected dictionary model index if any is selected.
 */
void DictionaryDialog::removeTriggered()
{
   // Make sure this object's view has an index selected.
   if ( _view->selectionModel()->selectedIndexes().isEmpty() ) return;

   // Remove the word at the currently selected index from this object's model.
   _model->removeWord(_view->selectionModel()->selectedIndexes().first());
}






/*!
 * Creates and initializes the GUI for this new dialog.
 */
void DictionaryDialog::setupGui()
{
   // Create the list view for this dialog and set its model to the dictionary.
   _view = new QListView(this);
   _view->setModel(_model);

   // Create and initialize the remove shortcut action, adding it to this object's
   // list view and connecting its triggered signal.
   QAction* remove {new QAction(this)};
   remove->setShortcutContext(Qt::WidgetShortcut);
   remove->setShortcut(Qt::CTRL + Qt::Key_Delete);
   connect(remove,&QAction::triggered,this,&DictionaryDialog::removeTriggered);
   _view->addAction(remove);

   // Create a new vertical layout, adding this object's list view and then its
   // buttons.
   QVBoxLayout* ret {new QVBoxLayout};
   ret->addWidget(_view);
   ret->addLayout(setupButtons());

   // Set the layout for this new dialog.
   setLayout(ret);
}






/*!
 * Creates and initializes the buttons for this new dialog, returning the button
 * layout.
 *
 * @return Layout of the buttons for this new dialog.
 */
QLayout* DictionaryDialog::setupButtons()
{
   // Create the remove button, connecting its clicked signal.
   QPushButton* removeButton {new QPushButton(tr("&Remove"))};
   connect(removeButton,&QPushButton::clicked,this,&DictionaryDialog::removeTriggered);

   // Create the close button, connecting its clicked signal.
   QPushButton* close {new QPushButton(tr("&Close"))};
   connect(close,&QPushButton::clicked,[this]{ done(QDialog::Rejected); });

   // Create a new horizontal layout, adding the remove button and then the close
   // button.
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(removeButton);
   ret->addWidget(close);

   // Return the button layout.
   return ret;
}

}
