#include "WidgetDialogProject.h"
#include <QtWidgets>
#include "ModelProject.h"
#include "gassert.h"
#define SETTINGS_KEY "widget.dialog.project"
namespace Widget {
namespace Dialog {


Project::Project(
    Model::Project* model
    ,QWidget* parent
    ,Qt::WindowFlags flags
):
    QDialog(parent,flags)
    ,_model(model)
{
    G_ASSERT(_model);
    connect(_model,&QObject::destroyed,this,&Project::onModelDestroyed);
    auto layout = new QVBoxLayout;
    layout->addLayout(formLayout());
    layout->addLayout(buttonsLayout());
    setLayout(layout);
    restoreGS();
}


void Project::closeEvent(
    QCloseEvent* event
)
{
    saveGS();
    event->accept();
}


void Project::hideEvent(
    QHideEvent* event
)
{
    saveGS();
    event->accept();
}


void Project::apply(
)
{
    G_ASSERT(_model);
    _model->setName(nameLineEdit()->text());
    _model->setRelativeCodePath(codePathLineEdit()->text());
}


void Project::codePathBrowse(
)
{
    G_ASSERT(_model);
    auto path = QFileDialog::getExistingDirectory(this,tr("Select Relative Code Path"));
    if (!path.isNull())
    {
        codePathLineEdit()->setText(QDir(_model->directoryPath()).relativeFilePath(path));
    }
}


void Project::onModelDestroyed(
    QObject* object
)
{
    if (_model == object)
    {
        _model = nullptr;
    }
}


QPushButton* Project::applyButton(
)
{
    if (!_applyButton)
    {
        _applyButton = new QPushButton(tr("Apply"));
        connect(_applyButton,&QPushButton::clicked,this,&Project::apply);
    }
    return _applyButton;
}


QHBoxLayout* Project::buttonsLayout(
)
{
    if (!_buttonsLayout)
    {
        _buttonsLayout = new QHBoxLayout;
        _buttonsLayout->addStretch();
        _buttonsLayout->addWidget(okButton());
        _buttonsLayout->addWidget(applyButton());
        _buttonsLayout->addWidget(cancelButton());
    }
    return _buttonsLayout;
}


QPushButton* Project::cancelButton(
)
{
    if (!_cancelButton)
    {
        _cancelButton = new QPushButton(tr("Cancel"));
        connect(_cancelButton,&QPushButton::clicked,this,&QDialog::reject);
    }
    return _cancelButton;
}


QPushButton* Project::codePathBrowseButton(
)
{
    if (!_codePathBrowseButton)
    {
        _codePathBrowseButton = new QPushButton(tr("Browse"));
        connect(_codePathBrowseButton,&QPushButton::clicked,this,&Project::codePathBrowse);
    }
    return _codePathBrowseButton;
}


QHBoxLayout* Project::codePathLayout(
)
{
    if (!_codePathLayout)
    {
        _codePathLayout = new QHBoxLayout;
        _codePathLayout->addWidget(codePathLineEdit());
        _codePathLayout->addWidget(codePathBrowseButton());
    }
    return _codePathLayout;
}


QLineEdit* Project::codePathLineEdit(
)
{
    if (!_codePathLine)
    {
        G_ASSERT(_model);
        _codePathLine = new QLineEdit;
        _codePathLine->setReadOnly(true);
        _codePathLine->setText(_model->relativeCodePath());
    }
    return _codePathLine;
}


QFormLayout* Project::formLayout(
)
{
    if (!_formLayout)
    {
        _formLayout = new QFormLayout;
        _formLayout->addRow(tr("Name:"),nameLineEdit());
        _formLayout->addRow(tr("Relative Code Path:"),codePathLayout());
    }
    return _formLayout;
}


QLineEdit* Project::nameLineEdit(
)
{
    if (!_nameEdit)
    {
        G_ASSERT(_model);
        _nameEdit = new QLineEdit;
        _nameEdit->setText(_model->name());
    }
    return _nameEdit;
}


QPushButton* Project::okButton(
)
{
    if (!_okButton)
    {
        _okButton = new QPushButton(tr("OK"));
        connect(_okButton,&QPushButton::clicked,this,&Project::apply);
        connect(_okButton,&QPushButton::clicked,this,&QDialog::accept);
    }
    return _okButton;
}


void Project::restoreGS(
)
{
    QSettings settings;
    restoreGeometry(settings.value(SETTINGS_KEY).toByteArray());
}


void Project::saveGS(
) const
{
    QSettings settings;
    settings.setValue(SETTINGS_KEY,saveGeometry());
}
}
}
