#include "WidgetDialogOrphanFiles.h"
#include <QtWidgets>
#include "Exceptions.h"
#include "ModelProject.h"
#include "StreamProject.h"
namespace Widget {
namespace Dialog {


OrphanFiles::OrphanFiles(
    const Model::Project* projectModel
    ,QWidget* parent
    ,Qt::WindowFlags flags
):
    QDialog(parent,flags)
    ,_projectModel(projectModel)
{
    G_ASSERT(_projectModel);
    connect(_projectModel,&QObject::destroyed,this,&OrphanFiles::onProjectModelDestroyed);
    auto layout = new QVBoxLayout;
    layout->addWidget(listView());
    layout->addLayout(buttonsLayout());
    setLayout(layout);
    refresh();
}


void OrphanFiles::gitRemoveAll(
)
{
    remove(true);
}


void OrphanFiles::onProjectModelDestroyed(
    QObject* object
)
{
    if (_projectModel == object)
    {
        _projectModel = nullptr;
    }
}


void OrphanFiles::refresh(
)
{
    _model->setStringList(Stream::Project::orphanFiles(*_projectModel));
}


void OrphanFiles::removeAll(
)
{
    remove(false);
}


QHBoxLayout* OrphanFiles::buttonsLayout(
)
{
    if (!_buttonsLayout)
    {
        _buttonsLayout = new QHBoxLayout;
        _buttonsLayout->addStretch();
        _buttonsLayout->addWidget(gitRemoveAllButton());
        _buttonsLayout->addWidget(removeAllButton());
        _buttonsLayout->addWidget(refreshButton());
        _buttonsLayout->addWidget(closeButton());
    }
    return _buttonsLayout;
}


QPushButton* OrphanFiles::closeButton(
)
{
    if (!_closeButton)
    {
        _closeButton = new QPushButton(tr("Close"));
        connect(_closeButton,&QPushButton::clicked,this,&QWidget::close);
    }
    return _closeButton;
}


QPushButton* OrphanFiles::gitRemoveAllButton(
)
{
    if (!_gitRemoveAllButton)
    {
        _gitRemoveAllButton = new QPushButton(tr("Git Remove All"));
        connect(_gitRemoveAllButton,&QPushButton::clicked,this,&OrphanFiles::gitRemoveAll);
    }
    return _gitRemoveAllButton;
}


QListView* OrphanFiles::listView(
)
{
    if (!_listView)
    {
        _listView = new QListView;
        _listView->setModel(model());
    }
    return _listView;
}


QStringListModel* OrphanFiles::model(
)
{
    if (!_model)
    {
        _model = new QStringListModel(this);
    }
    return _model;
}


QPushButton* OrphanFiles::refreshButton(
)
{
    if (!_refreshButton)
    {
        _refreshButton = new QPushButton(tr("Refresh"));
        connect(_refreshButton,&QPushButton::clicked,this,&OrphanFiles::refresh);
    }
    return _refreshButton;
}


void OrphanFiles::remove(
    bool git
)
{
    G_ASSERT(_projectModel);
    auto paths = _model->stringList();
    if (!paths.isEmpty())
    {
        try
        {
            Stream::Project::removeOrphanFiles(paths,*_projectModel,git);
        }
        catch (Exception::Project::Logical& e)
        {
            QMessageBox::warning(this,tr("Logical Project Error"),e.message());
        }
        catch (Exception::Block::Logical& e)
        {
            QMessageBox::warning(this,tr("Logical Block Error"),e.message());
        }
        catch (Exception::System::File& e)
        {
            QMessageBox::warning(this,tr("File System Error"),e.message());
        }
        catch (Exception::System::Run& e)
        {
            QMessageBox::warning(this,tr("Run System Error"),e.message());
        }
        refresh();
    }
}


QPushButton* OrphanFiles::removeAllButton(
)
{
    if (!_removeAllButton)
    {
        _removeAllButton = new QPushButton(tr("Remove All"));
        connect(_removeAllButton,&QPushButton::clicked,this,&OrphanFiles::removeAll);
    }
    return _removeAllButton;
}
}
}
