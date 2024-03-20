#include "ControllerSpellingHighlighter.h"
#include <QtCore>
#include <hunspell/hunspell.hxx>
#include "SettingsSpelling.h"
namespace Controller {


SpellingHighlighter::SpellingHighlighter(
    QTextDocument* parent
):
    QSyntaxHighlighter(parent)
{
    using Settings = Settings::Spelling;
    connect(
        Settings::instance()
        ,&Settings::dictionaryChanged
        ,this
        ,&SpellingHighlighter::rehighlight
    );
}


void SpellingHighlighter::highlightBlock(
    const QString& text
)
{
    static const QRegularExpression regExp("\\w+");
    QTextCharFormat format;
    format.setFontUnderline(true);
    format.setUnderlineColor(Qt::red);
    format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    auto settings = Settings::Spelling::instance();
    if (settings->hasDictionary())
    {
        Hunspell hunspell(
            settings->absoluteAffPath().toLocal8Bit().data()
            ,settings->absoluteDicPath().toLocal8Bit().data()
        );
        auto i = regExp.globalMatch(text);
        while (i.hasNext())
        {
            auto match = i.next();
            if (!hunspell.spell(match.captured().toLocal8Bit().toStdString()))
            {
                setFormat(match.capturedStart(),match.capturedLength(),format);
            }
        }
    }
}
}
