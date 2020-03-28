#ifndef FORM_H
#define FORM_H

#include <QtWidgets>
#include "clientclass.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(const QString _ip, const unsigned int _port ,const QString _username, QWidget *parent = nullptr);
    ~Form();
    void addUserToChatlist(QString user_name);
    void setChat(int row);
    void setUserUnChecked();
public slots:
    void addUserToList(QString user_name);
    void outGoingMessage();
    void onUserClick(QModelIndex index);
    void onUserChatClick(QModelIndex index);
    void goBackUserChat();
    void showAllUser();
    void showCreateGroupWidget();
    void createGroup();
    void addGroupToChatList(QString _groupname, QStringList _users);
    void userLeft(QString _username);
    void inComingMessage(QString _user_name, QString _message);
    void incomingGroupMessage(QString _groupname, QString _username, QString _message);
    void welcomeMessage();
    void addOldUser(QStringList _users);
private:
    Ui::Form *ui;
    QVector<QStandardItemModel*> chatModels;
    QMap<QString, bool> userlist;
    QMultiMap<QString, QString> groupList;
    QString name;
    QString ip;
    unsigned int port;
    QMenu *menu;
    QAction *actionUser;
    QAction *actionGroup;
    ClientClass *clientClass;
};

#endif // FORM_H
