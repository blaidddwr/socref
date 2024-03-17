#include "ControllerSpellingHighlighter.h"
#include <QtCore>
#include <hunspell/hunspell.hxx>
namespace Controller {


SpellingHighlighter::SpellingHighlighter(
    QTextDocument* parent
):
    QSyntaxHighlighter(parent)
{
}


void SpellingHighlighter::highlightBlock(
    const QString& text
)
{
    static const QRegularExpression regExp("\\w+");
    static const char* affPath = "/usr/share/hunspell/en_US.aff";
    static const char* dicPath = "/usr/share/hunspell/en_US.dic";
    QTextCharFormat format;
    format.setFontUnderline(true);
    format.setUnderlineColor(Qt::red);
    format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    Hunspell hunspell(affPath,dicPath);
    auto i = regExp.globalMatch(text);
    while (i.hasNext())
    {
        auto match = i.next();
        if (!hunspell.spell(match.captured().toStdString()))
        {
            setFormat(match.capturedStart(),match.capturedLength(),format);
        }
    }
}
}
