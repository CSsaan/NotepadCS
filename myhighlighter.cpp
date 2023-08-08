#include "myhighlighter.h"
#include <QDebug>
#include <QRegularExpressionMatchIterator>

MyHighlighter::MyHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    //设定关键词的高亮样式
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns; //关键词集合
    keywordPatterns << "void\\b" << "bool\\b" << "int\\b"<< "float\\b" << "bvec2\\b"
                    << "bvec3\\b" << "bvec4\\b" << "ivec2\\b" << "ivec3\\b"
                    << "vec3\\b" << "vec4\\b" << "vec2\\b" << "ivec3\\b"
                    << "ivec4\\b" << "mat2\\b" << "mat2x2" << "mat3\\b" << "mat3x3\\b"
                    << "mat2x3\\b" << "mat2x4\\b" << "mat3x2\\b" << "mat3x4\\b"
                    << "mat4x2\\b" << "mat4x3\\b" << "sampler1D\\b" << "sampler2D\\b"
                    << "mat4x4\\b" << "sampler3D\\b" << "samplerCube\\b"
                    << "sampler1DShadow\\b" << "sampler2DShadow\\b" << "\\signed\\b"
                    << "static\\b" << "\\struct\\b" << "typedef\\b" << "\\typename\\b"
                    << "union\\b" << "unsigned\\b" << "void\\b" << "volatile\\b" << "bool\\b";
    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    //设定GLSL内置变量的高亮样式
    innerFormat.setForeground(Qt::darkCyan);
    innerFormat.setFontWeight(QFont::Bold);
    QStringList innerPatterns; //关键词集合
    innerPatterns << "gl_Color\\b" << "gl_SecondaryColor\\b" << "gl_TexCoord\\b"
                  << "gl_FogFragCoord\\b" << "gl_FragCoord\\b"<< "gl_FrontFacing\\b"
                  << "gl_PointCoord\\b" << "gl_FragData\\b" << "gl_FragColor\\b"
                  << "gl_FragDepth\\b";
    foreach (const QString &pattern, innerPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = innerFormat;
        highlightingRules.append(rule);
    }
    //设定GLSL内置修饰符
    modifierFormat.setForeground(Qt::darkCyan);
    modifierFormat.setFontWeight(QFont::Bold);
    QStringList modifierPatterns; //关键词集合
    modifierPatterns << "const\\b" << "attribute\\b" << "uniform\\b"
                  << "varying\\b" << "invariant\\b"<< "in\\b"
                  << "out\\b" << "inout\\b";
    foreach (const QString &pattern, modifierPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = modifierFormat;
        highlightingRules.append(rule);
    }
    //设定GLSL精度饰符
    precisionFormat.setForeground(Qt::red);
    precisionFormat.setFontWeight(QFont::Bold);
    QStringList precisionPatterns; //关键词集合
    precisionPatterns << "lowp\\b" << "mediump\\b" << "highp\\b";
    foreach (const QString &pattern, precisionPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = precisionFormat;
        highlightingRules.append(rule);
    }
    //添加Qt的类到高亮规则中
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);
    //字符串
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    //函数
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::darkYellow);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    //单行注释
    singleLineCommentFormat.setForeground(Qt::darkGray);
    singleLineCommentFormat.setFontWeight(QFont::Light);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
    //多行注释的匹配正则表达式
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
    //多行注释，只设定了样式，具体匹配在highlightBlock中设置
    multiLineCommentFormat.setForeground(Qt::darkGray);
    multiLineCommentFormat.setFontWeight(QFont::Light);
}


void MyHighlighter::highlightBlock(const QString &text)
{
    //文本采用高亮规则
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    //以下是多行注释的匹配
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

void MyHighlighter::SetText(QString text)
{
    word_text=text;
}




