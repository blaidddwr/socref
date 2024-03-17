#ifndef CONTROLLER_SPELLINGHIGHLIGHTER_H
#define CONTROLLER_SPELLINGHIGHLIGHTER_H
#include <QSyntaxHighlighter>
namespace Controller {




/*!
 * This is a controller class. It controls the highlighting of words in a Qt
 * text document by underlining any misspelled words.
 */
class SpellingHighlighter:
    public QSyntaxHighlighter
{
    Q_OBJECT


    public:
    SpellingHighlighter(
        QTextDocument* parent
    );


    public:
    virtual void highlightBlock(
        const QString& text
    ) override final;
};
}


#endif
