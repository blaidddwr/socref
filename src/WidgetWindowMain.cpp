#include "WidgetWindowMain.h"
#include <QtWidgets>
#include "FactoryLanguage.h"
#include "LanguageAbstract.h"
#include "ModelMetaLanguage.h"
namespace Widget {
namespace Window {


Main::Main(
    QWidget* parent
    ,Qt::WindowFlags flags
):
    QMainWindow(parent,flags)
{
    auto menu = menuBar();
    menu->addMenu(fileMenu());
    menu->addMenu(editMenu());
    menu->addMenu(helpMenu());
    addToolBar(fileToolBar());
    addToolBar(editToolBar());
    statusBar();
}


void Main::about(
)
{
    //TODO
}


void Main::build(
)
{
    //TODO
}


void Main::close(
)
{
    //TODO
}


void Main::make(
)
{
    //TODO
}


void Main::new_(
    int index
)
{
    Q_UNUSED(index);
    //TODO
}


void Main::open(
)
{
    //TODO
}


void Main::parse(
)
{
    //TODO
}


void Main::properties(
)
{
    //TODO
}


void Main::save(
)
{
    //TODO
}


void Main::saveAs(
)
{
    //TODO
}


QAction* Main::aboutAction(
)
{
    if (!_aboutAction)
    {
        _aboutAction = new QAction(QIcon::fromTheme("help-about"),tr("About"),this);
        _aboutAction->setStatusTip(tr("Get more information about this application."));
        connect(_aboutAction,&QAction::triggered,this,&Main::about);
    }
    return _aboutAction;
}


QAction* Main::aboutQtAction(
)
{
    if (!_aboutQtAction)
    {
        _aboutQtAction = new QAction(QIcon::fromTheme("help-about"),tr("About Qt"),this);
        _aboutQtAction->setStatusTip(tr("Get more information about the Qt framework."));
        connect(_aboutQtAction,&QAction::triggered,this,[this](){ QMessageBox::aboutQt(this); });
    }
    return _aboutQtAction;
}


QAction* Main::buildAction(
)
{
    if (!_buildAction)
    {
        _buildAction = new QAction(tr("Build"),this);
        _buildAction->setStatusTip(
            tr(
                "Build the source code of this window's project, using the remembered"
                " implementation code from parsing."
            )
        );
        _buildAction->setShortcut(Qt::CTRL|Qt::Key_B);
        connect(_buildAction,&QAction::triggered,this,&Main::build);
    }
    return _buildAction;
}


QMenu* Main::editMenu(
)
{
    if (!_editMenu)
    {
        _editMenu = new QMenu(tr("Edit"),this);
        //TODO
    }
    return _editMenu;
}


QToolBar* Main::editToolBar(
)
{
    if (!_editToolBar)
    {
        _editToolBar = new QToolBar(tr("Edit"),this);
        //TODO
    }
    return _editToolBar;
}


QAction* Main::exitAction(
)
{
    if (!_exitAction)
    {
        _exitAction = new QAction(QIcon::fromTheme("application-exit"),tr("Exit"),this);
        _exitAction->setStatusTip(tr("Exit this window, closing it and any open project."));
        _exitAction->setShortcut(QKeySequence::Quit);
        connect(_exitAction,&QAction::triggered,this,&QWidget::close);
    }
    return _exitAction;
}


QMenu* Main::fileMenu(
)
{
    if (!_fileMenu)
    {
        _fileMenu = new QMenu(tr("File"),this);
        _fileMenu->addMenu(newMenu());
        _fileMenu->addAction(openAction());
        _fileMenu->addAction(saveAction());
        _fileMenu->addAction(saveAsAction());
        _fileMenu->addAction(closeAction());
        _fileMenu->addSeparator();
        _fileMenu->addAction(propertiesAction());
        _fileMenu->addSeparator();
        _fileMenu->addAction(parseAction());
        _fileMenu->addAction(buildAction());
        _fileMenu->addAction(makeAction());
        _fileMenu->addSeparator();
        _fileMenu->addAction(exitAction());
    }
    return _fileMenu;
}


QToolBar* Main::fileToolBar(
)
{
    if (!_fileToolBar)
    {
        _fileToolBar = new QToolBar(tr("File"),this);
        _fileToolBar->addAction(openAction());
        _fileToolBar->addAction(saveAction());
        _fileToolBar->addAction(saveAsAction());
        _fileToolBar->addAction(makeAction());
    }
    return _fileToolBar;
}


QAction* Main::closeAction(
)
{
    if (!_closeAction)
    {
        _closeAction = new QAction(QIcon::fromTheme("document-close"),tr("Close"),this);
        _closeAction->setStatusTip(tr("Close this window's current project."));
        _closeAction->setShortcut(QKeySequence::Close);
        connect(_closeAction,&QAction::triggered,this,&Main::close);
    }
    return _closeAction;
}


QMenu* Main::helpMenu(
)
{
    if (!_helpMenu)
    {
        _helpMenu = new QMenu(tr("Help"),this);
        _helpMenu->addAction(aboutAction());
        _helpMenu->addAction(aboutQtAction());
    }
    return _helpMenu;
}


QAction* Main::makeAction(
)
{
    if (!_makeAction)
    {
        _makeAction = new QAction(QIcon::fromTheme("view-refresh"),tr("Make"),this);
        _makeAction->setStatusTip(
            tr("Parse and then build the source code of this window's project.")
        );
        _makeAction->setShortcut(Qt::CTRL|Qt::Key_M);
        connect(_makeAction,&QAction::triggered,this,&Main::make);
    }
    return _makeAction;
}


QMenu* Main::newMenu(
)
{
    if (!_newMenu)
    {
        _newMenu = new QMenu(tr("New"),this);
        _newMenu->setIcon(QIcon::fromTheme("document-new"));
        auto factory = Factory::Language::instance();
        for (int i = 0;i < factory->size();i++)
        {
            if (!factory->isHidden(i))
            {
                auto label = factory->get(i)->meta()->label();
                auto action = new QAction(label,this);
                action->setStatusTip(tr("Create a new %1 project in this window.").arg(label));
                connect(action,&QAction::triggered,this,[this,i](){ new_(i); });
                _newMenu->addAction(action);
            }
        }
    }
    return _newMenu;
}


QAction* Main::openAction(
)
{
    if (!_openAction)
    {
        _openAction = new QAction(QIcon::fromTheme("document-open"),tr("Open"),this);
        _openAction->setStatusTip(tr("Open an existing project in this window."));
        _openAction->setShortcut(QKeySequence::Open);
        connect(_openAction,&QAction::triggered,this,&Main::open);
    }
    return _openAction;
}


QAction* Main::parseAction(
)
{
    if (!_parseAction)
    {
        _parseAction = new QAction(tr("Parse"),this);
        _parseAction->setStatusTip(
            tr(
                "Parse the source code of this window's project, remembering the implementation"
                " code found."
            )
        );
        _parseAction->setShortcut(Qt::CTRL|Qt::Key_P);
        connect(_parseAction,&QAction::triggered,this,&Main::parse);
    }
    return _parseAction;
}


QAction* Main::propertiesAction(
)
{
    if (!_propertiesAction)
    {
        _propertiesAction = new QAction(tr("Properties"),this);
        _propertiesAction->setStatusTip(tr("Edit the properties of this window's project."));
        connect(_propertiesAction,&QAction::triggered,this,&Main::properties);
    }
    return _propertiesAction;
}


QAction* Main::saveAction(
)
{
    if (!_saveAction)
    {
        _saveAction = new QAction(QIcon::fromTheme("document-save"),tr("Save"),this);
        _saveAction->setStatusTip(tr("Save this window's project."));
        _saveAction->setShortcut(QKeySequence::Save);
        connect(_saveAction,&QAction::triggered,this,&Main::save);
    }
    return _saveAction;
}


QAction* Main::saveAsAction(
)
{
    if (!_saveAsAction)
    {
        _saveAsAction = new QAction(QIcon::fromTheme("document-save-as"),tr("Save As"),this);
        _saveAsAction->setStatusTip(tr("Save this window's project to a new directory path."));
        _saveAsAction->setShortcut(QKeySequence::SaveAs);
        connect(_saveAsAction,&QAction::triggered,this,&Main::saveAs);
    }
    return _saveAsAction;
}
}
}
