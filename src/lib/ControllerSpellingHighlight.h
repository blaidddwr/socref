#ifndef CONTROLLER_SPELLINGHIGHLIGHT_H
#define CONTROLLER_SPELLINGHIGHLIGHT_H
#include <QSyntaxHighlighter>
namespace Controller {




/*!
 * This is a controller class. It controls the highlighting of words in a Qt
 * text document by underlining any misspelled words.
 */
class SpellingHighlight:
    public QSyntaxHighlighter
{
    Q_OBJECT


    public:
    SpellingHighlight(
        QTextDocument* parent
    );


    public:
    virtual void highlightBlock(
        const QString& text
    ) override final;
};
}


#endif
