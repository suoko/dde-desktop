#include "elidelabel.h"

ElidedLabel::ElidedLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    this->setMinimumWidth(0);
    setTextFormat(Qt::PlainText);
    setWordWrap(true);
    setAlignment(Qt::AlignCenter);
}

ElidedLabel::ElidedLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f), m_fullText(text)
{
    this->setMinimumWidth(0);
    setTextFormat(Qt::PlainText);
    setWordWrap(true);
    setAlignment(Qt::AlignCenter);
}

void ElidedLabel::setFullText(const QString &text)
{
    m_fullText = text;
    this->setText(text);
    elideText();
}

void ElidedLabel::setTextLimitShrink(const QString &text, int width)
{
    this->setMinimumWidth(qMin(this->fontMetrics().width(text), width));
    setFullText(text);
}

void ElidedLabel::setTextLimitExpand(const QString &text)
{
    this->setMaximumWidth(this->fontMetrics().width(text));
    setFullText(text);
}

QString ElidedLabel::fullText() const
{
    return m_fullText;
}

QString ElidedLabel::getSimpWrapText(){
    return m_simpleWrapText;
}

QString ElidedLabel::getFullWrapText(){
    return m_fullWrapText;
}

QStringList ElidedLabel::getTexts(){
    return m_texts;
}

void ElidedLabel::resizeEvent(QResizeEvent *event)
{
    elideText();
    QLabel::resizeEvent(event);
}

void ElidedLabel::elideText()
{
    QStringList texts;
    QString fullWrapText;
    QString simpleWrapText;
    QFontMetrics fm = this->fontMetrics();
    int fmWidth = width() - 20;
    int i = 0, start = 0;

    if (fm.width(m_fullText) < fmWidth){
        texts.append(m_fullText);
    }else{
        for(i = 0; i < m_fullText.length(); i++){
            QString unitString = m_fullText.mid(start, i-start);
            if (fm.width(unitString) < fmWidth){
                continue;
            }else{
                QString retString = m_fullText.mid(start, i - start - 1);
                texts.append(retString);
                start = i -1;
            }
        }
        QString lastString = m_fullText.mid(start, m_fullText.length());
        texts.append(lastString);
    }
    fullWrapText = texts.join("\n");
    if (texts.length() >= 2){
        QString elidedText = fm.elidedText(	QStringList(texts.mid(1)).join("\n"), Qt::ElideRight, fmWidth);
        QStringList simpleList;
        simpleList << texts.at(0) << elidedText;
        simpleWrapText = simpleList.join("\n");
    }else{
        simpleWrapText = texts.at(0);
    }
    m_texts.clear();
    m_texts = texts;
    m_fullWrapText = fullWrapText;
    m_simpleWrapText = simpleWrapText;
    if (fm.width(this->text()) != this->width()) {
        showSimpleWrapText();
        if (text() != m_fullText) {
            this->setToolTip(m_fullText.left(1024));
        } else {
            this->setToolTip("");
        }
    }
}

void ElidedLabel::showSimpleWrapText(){
    QFontMetrics fm = fontMetrics();
    setFixedHeight(fm.lineSpacing() * 2);
    setText(m_simpleWrapText);
}


void ElidedLabel::showFullWrapText(){
    QFontMetrics fm = fontMetrics();
    setFixedHeight(fm.lineSpacing() * m_texts.length());
    setText(m_fullWrapText);
}
