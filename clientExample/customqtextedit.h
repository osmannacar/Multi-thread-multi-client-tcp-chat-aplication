#ifndef TEXTEDITCUSTOM_H
#define TEXTEDITCUSTOM_H
#include <QTextEdit>
#include <QEvent>
#include <QKeyEvent>

class CustomQTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    CustomQTextEdit(QWidget  *parent = nullptr): QTextEdit(parent){
        this->installEventFilter(this);
    }

protected:
    virtual void keyPressEvent(QKeyEvent *e) override{
        if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
            if(!this->toPlainText().isEmpty()){
                this->enterPress();
                return;
            }
        }
        if(e->key() != Qt::Key_NumberSign && e->key() != Qt::Key_Slash){
            this->QTextEdit::keyPressEvent(e);
        }
    }
signals:
    void enterPress();

};

#endif // TEXTEDITCUSTOM_H

