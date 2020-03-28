#include <QApplication>
#include <iostream>
#include "form.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::cout << "\033]0;" << "Client" << "\007" << "Client" << std::endl;
    Form *form;
    bool ok;
    QString username = QInputDialog::getText(0, "Input dialog", "User Name", QLineEdit::Normal, "", &ok);
    if (ok && !username.isEmpty()) {
            form = new Form(QString("127.0.0.1"), 1337, username);
            form->show();
    }else {
        exit(0);
    }


    return a.exec();
}
