#include "WidgetDialogAbout.h"
#include <QtWidgets>
#define SETTINGS_KEY "widget.dialog.about"
namespace Widget {
namespace Dialog {


About::About(
    QWidget* parent
    ,Qt::WindowFlags flags
):
    QDialog(parent,flags)
{
    auto layout = new QVBoxLayout;
    layout->addWidget(bodyLabel());
    layout->addLayout(buttonsLayout());
    setLayout(layout);
    restoreGS();
}


void About::closeEvent(
    QCloseEvent* event
)
{
    saveGS();
    event->accept();
}


void About::hideEvent(
    QHideEvent* event
)
{
    saveGS();
    event->accept();
}


QLabel* About::bodyLabel(
)
{
    if (!_bodyLabel)
    {
        _bodyLabel = new QLabel;
        _bodyLabel->setTextFormat(Qt::RichText);
        _bodyLabel->setText("<h1>TODO</h1>");
    }
    return _bodyLabel;
}


QHBoxLayout* About::buttonsLayout(
)
{
    if (!_buttonsLayout)
    {
        _buttonsLayout = new QHBoxLayout;
        _buttonsLayout->addStretch();
        _buttonsLayout->addWidget(okButton());
    }
    return _buttonsLayout;
}


QPushButton* About::okButton(
)
{
    if (!_okButton)
    {
        _okButton = new QPushButton(tr("OK"));
        connect(_okButton,&QPushButton::clicked,this,&About::accept);
    }
    return _okButton;
}


void About::restoreGS(
)
{
    QSettings settings;
    restoreGeometry(settings.value(SETTINGS_KEY).toByteArray());
}


void About::saveGS(
) const
{
    QSettings settings;
    settings.setValue(SETTINGS_KEY,saveGeometry());
}
}
}
