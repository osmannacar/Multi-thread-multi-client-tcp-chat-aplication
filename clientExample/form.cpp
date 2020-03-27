#include "form.h"
#include "ui_form.h"
#include "ListViewDelegate.h"

Form::Form(const QString _username, QWidget *parent) :
    QWidget(parent), name(_username),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->ui->lineEdit_group_name->setValidator(new QRegExpValidator(QRegExp("[a-z-A-Z_]+")));
    this->ui->widget_chat->setHidden(true);
    this->ui->widget_user_chat->setHidden(false);
    this->ui->widget_users->setHidden(true);
    
    // create a view and set our data
    this->ui->listView_chat->setWordWrap(true);
    this->ui->listView_chat->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setFixedSize(300, 500);

    this->ui->listView_chat->setMinimumHeight(350);
    
    // NOW tell the view to rely on out custom delegate for drawing
    this->ui->listView_chat->setItemDelegate(new ListViewDelegate());
    
    this->ui->listView_chat->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->ui->listView_chat->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    this->ui->listWidget_chat_users->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->ui->listView_chat->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->menu = new QMenu(this);
    this->actionUser = new QAction("Users");
    this->actionGroup = new QAction("Create Group");
    this->menu->addAction(this->actionUser);
    this->menu->addAction(this->actionGroup);
    this->ui->btn_menu->setMenu(this->menu);

    //activated
    this->clientClass = new ClientClass(QString("127.0.0.1"), this->name, 1337);

    QObject::connect(this->ui->textEdit_chat, SIGNAL(enterPress()), this, SLOT(outGoingMessage()));
    QObject::connect(this->ui->listWidget_chat_users, SIGNAL(clicked(QModelIndex)), this, SLOT(onUserChatClick(QModelIndex)));
    QObject::connect(this->ui->listWidget_users, SIGNAL(pressed(QModelIndex)), this, SLOT(onUserClick(QModelIndex)));
    QObject::connect(this->ui->btn_back, SIGNAL(clicked()), this, SLOT(returnUserChat()));
    QObject::connect(this->ui->btn_back_user_chat, SIGNAL(clicked()), this, SLOT(returnUserChat()));
    QObject::connect(this->actionUser, SIGNAL(triggered()), this, SLOT(showAllUser()));
    QObject::connect(this->actionGroup, SIGNAL(triggered()), this, SLOT(showCreateGroupWidget()));
    QObject::connect(this->ui->btn_create_group, SIGNAL(clicked()), this, SLOT(createGroup()));
    QObject::connect(this->clientClass, SIGNAL(userJoin(QString)), this, SLOT(addUserToList(QString)));
    QObject::connect(this->clientClass, SIGNAL(userLeft(QString)), this, SLOT(userLeft(QString)));
    QObject::connect(this->clientClass, SIGNAL(createdGroup(QString,QStringList)), this, SLOT(addGroupToChatList(QString,QStringList)));
    QObject::connect(this->clientClass, SIGNAL(incomingUserMessage(QString,QString)), this, SLOT(inComingMessage(QString,QString)));
    QObject::connect(this->clientClass, SIGNAL(incomingGroupMessage(QString,QString,QString)), this, SLOT(incomingGroupMessage(QString,QString,QString)));
    QObject::connect(this->clientClass, SIGNAL(welcomeMessage()), this, SLOT(welcomeMessage()));
    QObject::connect(this->clientClass, SIGNAL(oldUser(QStringList)), this, SLOT(addOldUser(QStringList)));
}

Form::~Form()
{
    delete ui;
}

void Form::addUserToList(QString user_name)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setFont(QFont("Fantasy", 7));
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    item->setText(user_name);
    QVariant v;
    v.setValue(this->userlist.size());
    item->setData(Qt::UserRole, v);
    this->ui->listWidget_users->addItem(item);
    this->userlist.insert(user_name, true);
}

void Form::addUserToChatlist(QString user_name)
{
    bool flag = true;
    int row;
    for (int i = 0; i < this->ui->listWidget_chat_users->count(); ++i) {
        if(this->ui->listWidget_chat_users->item(i)->text() == user_name){
            flag = false;
            row = i;
            break;
        }
    }
    if(flag){
        QListWidgetItem *item = new QListWidgetItem();
        item->setFont(QFont("Fantasy", 7));
        item->setText(user_name);
        this->ui->listWidget_chat_users->addItem(item);

        QStandardItemModel *model = new QStandardItemModel();
        this->chatModels.push_back(model);
        row = this->ui->listWidget_chat_users->count() - 1;
    }

    this->setChat(row);
}

void Form::inComingMessage(QString _user_name, QString _message)
{
    if(!_user_name.isEmpty() && !_message.isEmpty()){
        QMap<QString, bool>::iterator itr  =  this->userlist.find(_user_name);
        if(itr.key() == _user_name){
            QStandardItem *item = new QStandardItem("<b>" + itr.key() + "</b> <br>" + _message);
            item->setData("Incoming", Qt::UserRole + 1);
            bool flag = true;
            for (int i = 0; i < this->ui->listWidget_chat_users->count(); ++i) {
                QString header =this->ui->listWidget_chat_users->item(i)->text();
                if(!header.contains("\n") && header == itr.key()){
                    flag = false;
                    break;
                }
            }
            if(flag){
                this->addUserToChatlist(itr.key());
            }
            for (int i = 0; i < this->ui->listWidget_chat_users->count(); ++i) {
                QString header =this->ui->listWidget_chat_users->item(i)->text();
                if(!header.contains("\n") && header == itr.key()){
                    this->chatModels[i]->appendRow(item);
                    this->ui->listView_chat->scrollToBottom();
                    break;
                }
            }
        }

    }
}

void Form::incomingGroupMessage(QString _groupname, QString _username, QString _message)
{
    if(!_groupname.isEmpty() && !_username.isEmpty() && !_message.isEmpty()){
        for (int i = 0; i < this->ui->listWidget_chat_users->count(); ++i) {
            QString header =this->ui->listWidget_chat_users->item(i)->text();
            if(header.split("\n")[0].contains(_groupname)){
                this->setChat(i);
                QStandardItem *item = new QStandardItem("<b>" + _username + "</b> <br>" + _message);
                item->setData("Incoming", Qt::UserRole + 1);
                this->chatModels[i]->appendRow(item);
                this->ui->listView_chat->scrollToBottom();
                break;
            }
        }
    }
}

void Form::welcomeMessage()
{
    QMessageBox::information (this, "Information", QString("Welcome %1").arg(this->name));
}

void Form::addOldUser(QStringList _users)
{
    for (int i = 0; i < _users.size(); ++i) {
        this->addUserToList(_users.at(i));
    }
}

void Form::setChat(int row)
{
    QAbstractItemModel *model = this->ui->listWidget_chat_users->model();
    this->ui->listWidget_chat_users->setCurrentIndex(model->index(row, 0));
    this->ui->listView_chat->setModel(this->chatModels[row]);
    this->ui->lbl_chat_name->setText(this->ui->listWidget_chat_users->item(row)->text());

    this->ui->widget_chat->setHidden(false);
    this->ui->widget_user_chat->setHidden(true);
    this->ui->widget_users->setHidden(true);
}

void Form::setUserUnChecked()
{
    this->ui->lineEdit_group_name->clear();
    for (int i = 0; i < this->ui->listWidget_users->count(); ++i) {
        if(this->ui->listWidget_users->item(i)->checkState() == Qt::Checked){
            this->ui->listWidget_users->item(i)->setCheckState(Qt::Unchecked);
        }
    }
}

void Form::outGoingMessage()
{
    QModelIndex index = this->ui->listWidget_chat_users->currentIndex();
    if(!this->ui->textEdit_chat->toPlainText().isEmpty() && index.isValid())
    {
        QStandardItem *item = new QStandardItem("<b>" + this->name + "</b> <br>" + this->ui->textEdit_chat->toPlainText());
        item->setData("Outgoing", Qt::UserRole + 1);
        this->chatModels[index.row()]->appendRow(item);
        this->ui->listView_chat->scrollToBottom();
        this->ui->listView_chat->setModel(this->chatModels.at(index.row()));
        if(this->ui->lbl_chat_name->text().contains("\n")){
            this->clientClass->outgoingGroupMessage(this->ui->lbl_chat_name->text().split("\n")[0], this->ui->textEdit_chat->toPlainText());
        }else {
            this->clientClass->outgoingUserMessage(this->ui->lbl_chat_name->text(), this->ui->textEdit_chat->toPlainText());
        }
        this->ui->textEdit_chat->clear();

    }
}

void Form::onUserClick(QModelIndex index)
{
    this->addUserToChatlist(this->ui->listWidget_users->item(index.row())->text());
}

void Form::onUserChatClick(QModelIndex index)
{
    this->setChat(index.row());
}

void Form::returnUserChat()
{
    this->ui->widget_chat->setHidden(true);
    this->ui->widget_user_chat->setHidden(false);
    this->ui->widget_users->setHidden(true);
}

void Form::showAllUser()
{
    this->ui->widget_chat->setHidden(true);
    this->ui->widget_user_chat->setHidden(true);
    this->ui->widget_users->setHidden(false);
    this->ui->widget_group_create->setHidden(true);
    this->setUserUnChecked();
}

void Form::showCreateGroupWidget()
{
    this->ui->widget_chat->setHidden(true);
    this->ui->widget_user_chat->setHidden(true);
    this->ui->widget_users->setHidden(false);
    this->ui->widget_group_create->setHidden(false);
    this->setUserUnChecked();
}

void Form::createGroup()
{
    QStringList users;
    for (int i = 0; i < this->ui->listWidget_users->count(); ++i) {
        if(this->ui->listWidget_users->item(i)->checkState() == Qt::Checked){
            users.push_back(this->ui->listWidget_users->item(i)->text());
        }
    }

    if(this->ui->lineEdit_group_name->text().isEmpty()){
        QMessageBox::warning(this, "Users", "Please fill in the group name!");
    }else if(users.count()  < 2) {
        QMessageBox::warning(this, "Users", "Please must be selected at least two User");
    }else {
        users.push_back(this->name);
        this->addGroupToChatList(this->ui->lineEdit_group_name->text(), users);
        this->clientClass->createGroup(this->ui->lineEdit_group_name->text(), users);
    }
}

void Form::addGroupToChatList(QString _groupname, QStringList _users)
{
    QString alluser;
    for (int i = 0; i < _users.count(); ++i) {
        this->groupList.insert(_groupname, _users.at(i));
        alluser.append(_users.at(i));
        if(_users.count() - i != 1)
            alluser.append(" ,");
    }
    QListWidgetItem *item = new QListWidgetItem();
    item->setFont(QFont("Fantasy", 7));
    item->setText(_groupname + "\n" + alluser);
    this->ui->listWidget_chat_users->addItem(item);

    QStandardItemModel *model = new QStandardItemModel();
    this->chatModels.push_back(model);
    this->setChat(this->ui->listWidget_chat_users->count() - 1);
}

void Form::userLeft(QString _username)
{
    this->userlist[_username] = false;
    for (int i = 0; i < this->ui->listWidget_users->count(); ++i) {
        if(this->ui->listWidget_users->item(i)->text() == _username){
            this->ui->listWidget_users->removeItemWidget(this->ui->listWidget_users->item(i));
            break;
        }
    }
}
