#include <QApplication>
#include <iostream>
#include "form.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::cout << "\033]0;" << "Client" << "\007" << "Client" << std::endl;
    Form *form;
    bool ok;
    QString text = QInputDialog::getText(0, "Input dialog", "User Name", QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
            form = new Form(text);
            form->show();
    }


    return a.exec();
}
