#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class MyHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    MyHighlighter(QTextDocument *parent = 0);
    void SetText(QString text);
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;
protected:
    QString word_text;
private:
    struct HighlightingRule//语法规则结构体，包含正则表达式模式串和匹配的样式
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;//规则的集合，可以定义多个高亮规则

    QRegularExpression commentStartExpression; //注释的高亮，使用highliangBlock()匹配，下文提到
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;//关键词
    QTextCharFormat innerFormat;//内置变量
    QTextCharFormat modifierFormat;//修饰符
    QTextCharFormat precisionFormat;//精度
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif // MYHIGHLIGHTER_H



