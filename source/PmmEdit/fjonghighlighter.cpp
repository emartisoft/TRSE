#include "fjonghighlighter.h"

#include "source/Compiler/token.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/syntax.h"

#include <QDebug>


FjongHighlighter::FjongHighlighter(CIniFile ini, int type, QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    m_colors = ini;
    keywordFormat.setForeground(m_colors.getColor("keywordcolor"));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    for (Token t: Syntax::s.reservedWordsFjong) {
        //qDebug() << QString::number(i) << TokenType::types[i].toLower();
        QString s = "\\b" + t.m_value.toLower() + "\\b";
        keywordPatterns<<s;
    }

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    if (type==0)
    {
        builtinFunctionFormat.setForeground(m_colors.getColor("builtinfunctioncolor"));
        builtinFunctionFormat.setFontWeight(QFont::Bold);
        keywordPatterns.clear();
        for (QString k: Syntax::s.builtinFunctionsFjong.keys()) {
            QString name = k;
            QString s = "\\b" + name.toLower() + "\\b";
            keywordPatterns<<s;
        }

        foreach (const QString &pattern, keywordPatterns) {
            rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
            rule.format = builtinFunctionFormat;
            highlightingRules.append(rule);
        }

        /* CONSTANTS */

        SymbolTable::Initialize();

        constantsFormat.setForeground(m_colors.getColor("constantscolor"));
        constantsFormat.setFontWeight(QFont::Normal);
        keywordPatterns.clear();

        for (QString k: SymbolTable::m_constants.keys()) {
            //qDebug() << QString::number(i) << TokenType::types[i].toLower();
            QString s = "\\b" + k.toLower() + "\\b";
            keywordPatterns<<s;
        }
    }
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
        rule.format = constantsFormat;
        highlightingRules.append(rule);
    }


    numberFormat.setFontWeight(QFont::Normal);
    numberFormat.setForeground(m_colors.getColor("numberscolor"));
//    rule.pattern = QRegularExpression("#[0-9a-f#\\\$%]+",QRegularExpression::CaseInsensitiveOption);
//    rule.pattern = QRegularExpression("(\\b\\$[0-9a-f]+)||(\\b[0-9]+)",QRegularExpression::CaseInsensitiveOption);
//    rule.pattern = QRegularExpression("(?!(\\\^))(\\\$)?\\b[0-9a-f]+\\b",QRegularExpression::CaseInsensitiveOption);
    rule.pattern = QRegularExpression("((\\\$)\\b([0-9a-f]+)\\b)|(\\b([0-9]+)\\b)",QRegularExpression::CaseInsensitiveOption);
    rule.format = numberFormat;
    highlightingRules.append(rule);


    addressFormat.setFontWeight(QFont::Normal);
    addressFormat.setForeground(m_colors.getColor("addresscolor"));
//    rule.pattern = QRegularExpression("(?!#)(\\\$[0-9a-f]+)|\\b[0-9]+\\b",QRegularExpression::CaseInsensitiveOption);
//    rule.pattern = QRegularExpression("(\\b\\$[0-9a-f]+\\b)|(\\b[0-9]+\\b)",QRegularExpression::CaseInsensitiveOption);
    rule.pattern = QRegularExpression("\\\^(\\\$)?\\b([0-9a-f]+)\\b",QRegularExpression::CaseInsensitiveOption);
    rule.format = addressFormat;
    highlightingRules.append(rule);



    symbolsFormat.setFontWeight(QFont::Normal);
    symbolsFormat.setForeground(m_colors.getColor("symbolscolor"));
    rule.pattern = QRegularExpression("[\\\+\\\-:=\\\/\\\*\\\(\\\)\\\<\\\>\\\[\\\]]",QRegularExpression::CaseInsensitiveOption);
    rule.format = symbolsFormat;
    highlightingRules.append(rule);



    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::green);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b",QRegularExpression::CaseInsensitiveOption);
    rule.format = classFormat;
    highlightingRules.append(rule);




    quotationFormat.setForeground(m_colors.getColor("quotationcolor"));
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

/*    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(200,255, 100));
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);
*/
    singleLineCommentFormat.setForeground(m_colors.getColor("commentcolor"));
    rule.pattern = QRegularExpression("--[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);


    multiLineCommentFormat.setForeground(m_colors.getColor("commentcolor"));

    commentStartExpression = QRegularExpression("--\\[\\[");
//    commentEndExpression = QRegularExpression("\\*/");
    commentEndExpression = QRegularExpression("\\]\\]--");


}

void FjongHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);


    while (startIndex >= 0) {
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

void FjongHighlighter::Save(QString fn)
{
    m_colors.setVec("keywordcolor", QVector3D(128,200,255));
    m_colors.setVec("builtinfunctioncolor",QVector3D(255,200,50));
    m_colors.setVec("constantscolor", QVector3D(50,200,100));
    m_colors.setVec("addresscolor", QVector3D(255,150,170));
    m_colors.setVec("numberscolor", QVector3D(180,140,255));
    m_colors.setVec("symbolscolor", QVector3D(120,140,160));
    m_colors.setVec("quotationcolor", QVector3D(255,128,40));
    m_colors.setVec("commentcolor", QVector3D(128,128,128));
    m_colors.Save(fn);
}

void FjongHighlighter::HighlightText(QString &text)
{
    QString otext = text;
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
//            qDebug() << match.captured();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            //setFormat(match.capturedStart(), match.capturedLength(), QColor(255,0,0,1));
 //             qDebug() << match.captured()
/*            if (match.capturedLength()>5) {
                text.insert(match.capturedStart(),"<font color=\"#FF0000\">");
                text.insert(match.capturedEnd(),"</font>");
            }*/
        }

    }
//    text = otext;
}

