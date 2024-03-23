#include "WidgetDialogSpellingSettings.h"
#include <QtWidgets>
#include "SettingsSpelling.h"
#define SETTINGS_KEY "widget.dialog.spellingsettings"
namespace Widget {
namespace Dialog {


SpellingSettings::SpellingSettings(
    QWidget* parent
    ,Qt::WindowFlags flags
):
    QDialog(parent,flags)
{
    auto layout = new QVBoxLayout;
    layout->addLayout(formLayout());
    layout->addLayout(buttonsLayout());
    setLayout(layout);
    restoreGS();
}


void SpellingSettings::closeEvent(
    QCloseEvent* event
)
{
    saveGS();
    event->accept();
}


void SpellingSettings::hideEvent(
    QHideEvent* event
)
{
    saveGS();
    event->accept();
}


void SpellingSettings::apply(
)
{
    auto settings = Settings::Spelling::instance();
    settings->setPath(pathLineEdit()->text());
    auto dcb = dictionaryComboBox();
    if (dcb->currentIndex() > 0)
    {
        settings->setDictionary(dcb->currentText());
    }
    else
    {
        settings->setDictionary(QString());
    }
}


void SpellingSettings::pathBrowse(
)
{
    auto path = QFileDialog::getExistingDirectory(this,tr("Select Hunspell Dictionary Path"));
    if (!path.isNull())
    {
        pathLineEdit()->setText(QFileInfo(path).absoluteFilePath());
        updateDictionaries();
    }
}


QPushButton* SpellingSettings::applyButton(
)
{
    if (!_applyButton)
    {
        _applyButton = new QPushButton(tr("Apply"));
        connect(_applyButton,&QPushButton::clicked,this,&SpellingSettings::apply);
    }
    return _applyButton;
}


QHBoxLayout* SpellingSettings::buttonsLayout(
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


QPushButton* SpellingSettings::cancelButton(
)
{
    if (!_cancelButton)
    {
        _cancelButton = new QPushButton(tr("Cancel"));
        connect(_cancelButton,&QPushButton::clicked,this,&SpellingSettings::reject);
    }
    return _cancelButton;
}


QComboBox* SpellingSettings::dictionaryComboBox(
)
{
    if (!_dictionaryComboBox)
    {
        _dictionaryComboBox = new QComboBox;
        updateDictionaries();
    }
    return _dictionaryComboBox;
}


QFormLayout* SpellingSettings::formLayout(
)
{
    if (!_formLayout)
    {
        _formLayout = new QFormLayout;
        _formLayout->addRow(tr("Dictionary Path:"),pathLayout());
        _formLayout->addRow(tr("Dictionary:"),dictionaryComboBox());
    }
    return _formLayout;
}


QPushButton* SpellingSettings::okButton(
)
{
    if (!_okButton)
    {
        _okButton = new QPushButton(tr("OK"));
        connect(_okButton,&QPushButton::clicked,this,&SpellingSettings::apply);
        connect(_okButton,&QPushButton::clicked,this,&SpellingSettings::accept);
    }
    return _okButton;
}


QPushButton* SpellingSettings::pathBrowseButton(
)
{
    if (!_pathBrowseButton)
    {
        _pathBrowseButton = new QPushButton(tr("Browse"));
        connect(_pathBrowseButton,&QPushButton::clicked,this,&SpellingSettings::pathBrowse);
    }
    return _pathBrowseButton;
}


QHBoxLayout* SpellingSettings::pathLayout(
)
{
    if (!_pathLayout)
    {
        _pathLayout = new QHBoxLayout;
        _pathLayout->addWidget(pathLineEdit());
        _pathLayout->addWidget(pathBrowseButton());
    }
    return _pathLayout;
}


QLineEdit* SpellingSettings::pathLineEdit(
)
{
    if (!_pathLineEdit)
    {
        _pathLineEdit = new QLineEdit;
        _pathLineEdit->setReadOnly(true);
        _pathLineEdit->setText(Settings::Spelling::instance()->path());
    }
    return _pathLineEdit;
}


void SpellingSettings::restoreGS(
)
{
    QSettings settings;
    restoreGeometry(settings.value(SETTINGS_KEY).toByteArray());
}


void SpellingSettings::saveGS(
) const
{
    QSettings settings;
    settings.setValue(SETTINGS_KEY,saveGeometry());
}


void SpellingSettings::updateDictionaries(
)
{
    auto dcb = dictionaryComboBox();
    dcb->clear();
    dcb->addItem(tr("None"));
    dcb->addItems(Settings::Spelling::dictionaries(pathLineEdit()->text()));
    auto settings = Settings::Spelling::instance();
    if (settings->hasDictionary())
    {
        int index = -1;
        for (int i = 1;i < dcb->count();i++)
        {
            if (dcb->itemText(i) == settings->dictionary())
            {
                index = i;
                break;
            }
        }
        if (index != -1)
        {
            dcb->setCurrentIndex(index);
        }
    }
}
}
}
