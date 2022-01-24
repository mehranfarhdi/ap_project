#include "book.h"
#include "ui_book.h"
#include "qdb.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QElapsedTimer>

QDBLite::DB db;

BookManagement::BookManagement(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BookManagement)
{
    ui->setupUi(this);
    db.dbstate = db.Connect("C:/Users/mehran/Downloads/Compressed/ap_project-main/ap_project-main/db.db");
    ui->winStack->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(1);

    ui->passwordBox->setEchoMode(QLineEdit::Password);
    ui->passwordBox->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    ui->pBox->setEchoMode(QLineEdit::Password);
    ui->pBox->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    ui->pBox_2->setEchoMode(QLineEdit::Password);
    ui->pBox_2->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
}

BookManagement::~BookManagement()
{
    delete ui;
}

void BookManagement::on_loginButton_clicked()
{

    qDebug() << QTime::currentTime().toString();
    this->loggedIn = Login(ui->usernameBox->text(), ui->passwordBox->text());

    if(this->loggedIn)
    {
        this->username = ui->usernameBox->text();
        this->password = ui->passwordBox->text();

        ui->loginLabel->setText("");
        ui->winStack->setCurrentIndex(2);
    }
    else
    {
        ui->loginLabel->setText("Login failed: Invalid credentials!");
    }
}

bool BookManagement::Login(QString u, QString p)
{
    ui->adminButton->setVisible(false);
    ui->deletebook->setVisible(false);

    bool exists = false;

    QSqlQuery checkQuery(db.db);
    checkQuery.prepare("SELECT username FROM users WHERE username = (:un) AND passwd = (:pw)");
    checkQuery.bindValue(":un", u);
    checkQuery.bindValue(":pw", p);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
            qDebug() << QTime::currentTime().toString();
        }
    }

    return exists;
}


void BookManagement::on_regButton_clicked()
{
    qDebug() << QTime::currentTime().toString();
    ui->uBox->setText(ui->usernameBox->text());
    ui->pBox->setText(ui->passwordBox->text());
    ui->winStack->setCurrentIndex(1);
}

void BookManagement::on_logoutButton_clicked()
{
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                       "Book Managmentnt", "Are you sure you want to logout?",
                                       QMessageBox::Yes|QMessageBox::No).exec())
    {
        this->loggedIn = false;
        ui->passwordBox->setText("");
        ui->loginLabel->setText("You signed out!");
        ui->winStack->setCurrentIndex(0);
        qDebug() << QTime::currentTime().toString();
    }
}

void BookManagement::on_completeRegButton_clicked()
{
    bool halt = false;

    if(ui->uBox->text() == "")
    {
        ui->uBox->setPlaceholderText("Username EMPTY!");
        halt = true;
    }

    if(ui->pBox->text() == "")
    {
        ui->pBox->setPlaceholderText("Password EMPTY!");
        halt = true;
    }

    if(ui->eBox->text() == "")
    {
        ui->eBox->setPlaceholderText("E-mail EMPTY!");
        halt = true;
    }

    if(ui->fBox->text() == "")
    {
        ui->fBox->setPlaceholderText("First Name EMPTY!");
        halt = true;
    }

    if(ui->mBox->text() == "")
    {
        ui->mBox->setPlaceholderText("Middle Name (optional)");
        halt = false;
    }

    if(ui->lBox->text() == "")
    {
        ui->lBox->setPlaceholderText("Last Name EMPTY!");
        halt = true;
    }

    QSqlQuery cQuery(db.db);
    cQuery.prepare("SELECT username FROM users WHERE username = (:un)");
    cQuery.bindValue(":un", ui->uBox->text());

    if(cQuery.exec())
    {
        if(cQuery.next())
        {
            ui->uBox->setText("");
            ui->uBox->setPlaceholderText("Choose a different Username!");
            halt = true;
        }
    }
    qDebug() << QTime::currentTime().toString();
    QSqlQuery cQuery2(db.db);
    cQuery2.prepare("SELECT email FROM users WHERE email = (:em)");
    cQuery2.bindValue(":em", ui->eBox->text());

    if(cQuery2.exec())
    {
        if(cQuery2.next())
        {
            ui->eBox->setText("");
            ui->eBox->setPlaceholderText("Use another E-mail!");
            halt = true;
        }
    }


    if(halt)
    {
        ui->regLabel->setText("Please correct your mistakes.");
    }
    else
    {
        if (this->picName != "")
        {
            QString to = this->picDir+"/"+ui->uBox->text();

            if (QFile::exists(to))
            {
                QFile::remove(to);
            }

            QFile::copy(this->picName, to);
            this->picName = "";
        }

        ui->regLabel->setText("");
        QSqlQuery iQuery(db.db);
        iQuery.prepare("INSERT INTO users(username, passwd, fname, mname, lname, email)"\
                       "VALUES(:un, :pw, :fn, :mn, :ln, :em)");
        iQuery.bindValue(":un", ui->uBox->text());
        iQuery.bindValue(":pw", ui->pBox->text());
        iQuery.bindValue(":fn", ui->fBox->text());
        iQuery.bindValue(":mn", ui->mBox->text());
        iQuery.bindValue(":ln", ui->lBox->text());
        iQuery.bindValue(":em", ui->eBox->text());

        if(iQuery.exec())
        {
            ui->uBox->setText("");
            ui->pBox->setText("");
            ui->eBox->setText("");
            ui->fBox->setText("");
            ui->mBox->setText("");
            ui->lBox->setText("");
            ui->rpLabel->setText("<img src=\":user.png\" />");
            ui->loginLabel->setText("Registration Successful! You can now login.");
            ui->winStack->setCurrentIndex(0);
        }

    }
}

void BookManagement::on_backButton_clicked()
{
    qDebug() << QTime::currentTime().toString();
    ui->loginLabel->setText("");
    ui->winStack->setCurrentIndex(0);
}


void BookManagement::on_backButton_2_clicked()
{
    qDebug() << QTime::currentTime().toString();
    ui->winStack->setCurrentIndex(2);
}

void BookManagement::on_editButton_clicked()
{
    qDebug() << QTime::currentTime().toString();
    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM users WHERE username = (:un) AND passwd = (:pw)");
    fetcher.bindValue(":un", this->username);
    fetcher.bindValue(":pw", this->password);
    fetcher.exec();

    int idUsername = fetcher.record().indexOf("username");
    int idPasswd = fetcher.record().indexOf("passwd");
    int idEmail = fetcher.record().indexOf("email");
    int idFname = fetcher.record().indexOf("fname");
    int idMname = fetcher.record().indexOf("mname");
    int idLname = fetcher.record().indexOf("lname");

    while (fetcher.next())
    {
        ui->uBox_2->setText(fetcher.value(idUsername).toString());
        ui->pBox_2->setText(fetcher.value(idPasswd).toString());
        ui->eBox_2->setText(fetcher.value(idEmail).toString());
        ui->fBox_2->setText(fetcher.value(idFname).toString());
        ui->mBox_2->setText(fetcher.value(idMname).toString());
        ui->lBox_2->setText(fetcher.value(idLname).toString());
    }

    ui->winStack->setCurrentIndex(3);
}

void BookManagement::on_delButton_clicked()
{
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                       "Book Managment", "Are you sure you want to delete your account?",
                                       QMessageBox::Yes|QMessageBox::No).exec())
    {
        QString to = this->picDir+"/"+this->username;

        if (QFile::exists(to))
        {
            QFile::remove(to);
        }

        QSqlQuery dQuery(db.db);
        dQuery.prepare("DELETE FROM users WHERE username = (:un)");
        dQuery.bindValue(":un", this->username);

        if(dQuery.exec())
        {
            ui->usernameBox->setText("");
            ui->passwordBox->setText("");
            ui->loginLabel->setText("Account deleted!");
            ui->winStack->setCurrentIndex(0);
        }
    }
    qDebug() << QTime::currentTime().toString();
}

void BookManagement::on_editedButton_clicked()
{

    qDebug() << QTime::currentTime().toString();
    bool halt = false;

    if(ui->uBox_2->text() == "")
    {
        ui->uBox_2->setPlaceholderText("Username EMPTY!");
        halt = true;
    }

    if(ui->pBox_2->text() == "")
    {
        ui->pBox_2->setPlaceholderText("Password EMPTY!");
        halt = true;
    }

    if(ui->eBox_2->text() == "")
    {
        ui->eBox_2->setPlaceholderText("E-mail EMPTY!");
        halt = true;
    }

    if(ui->fBox_2->text() == "")
    {
        ui->fBox_2->setPlaceholderText("First Name EMPTY!");
        halt = true;
    }

    if(ui->mBox_2->text() == "")
    {
        ui->mBox_2->setPlaceholderText("Middle Name (optional)");
        halt = false;
    }

    if(ui->lBox_2->text() == "")
    {
        ui->lBox_2->setPlaceholderText("Last Name EMPTY!");
        halt = true;
    }

    QSqlQuery cQuery(db.db);
    cQuery.prepare("SELECT username FROM users WHERE username = (:un)");
    cQuery.bindValue(":un", ui->uBox->text());
    qDebug() << QTime::currentTime().toString();
    if(cQuery.exec())
    {
        if(cQuery.next() && ui->uBox_2->text() != cQuery.value(0).toString())
        {
            ui->uBox_2->setText("");
            ui->uBox_2->setPlaceholderText("Choose a different Username!");
            halt = true;
        }
    }

    QSqlQuery cQuery2(db.db);
    cQuery2.prepare("SELECT email FROM users WHERE email = (:em)");
    cQuery2.bindValue(":em", ui->eBox_2->text());

    if(cQuery2.exec())
    {
        if(cQuery2.next() && ui->eBox_2->text() != cQuery2.value(0).toString())
        {
            ui->eBox_2->setText("");
            ui->eBox_2->setPlaceholderText("Use another E-mail!");
            halt = true;
        }
    }


    if(halt)
    {

    }
    else
    {
        if (this->picName != "")
        {
            QString to = this->picDir+"/"+ui->uBox_2->text();

            if (QFile::exists(to))
            {
                QFile::remove(to);
            }

            QFile::copy(this->picName, to);
            this->picName = "";
        }

        QSqlQuery iQuery(db.db);
        iQuery.prepare("UPDATE users SET username=(:un), passwd=(:pw), fname=(:fn), mname=(:mn), lname=(:ln), email=(:em) WHERE username=(:uno)");
        iQuery.bindValue(":un", ui->uBox_2->text());
        iQuery.bindValue(":pw", ui->pBox_2->text());
        iQuery.bindValue(":fn", ui->fBox_2->text());
        iQuery.bindValue(":mn", ui->mBox_2->text());
        iQuery.bindValue(":ln", ui->lBox_2->text());
        iQuery.bindValue(":em", ui->eBox_2->text());
        iQuery.bindValue(":uno", ui->uBox_2->text());

        if(iQuery.exec())
        {
            ui->winStack->setCurrentIndex(2);
        }

    }
}

void BookManagement::on_winStack_currentChanged(int arg1)
{
    qDebug() << QTime::currentTime().toString();
    if(arg1 == 3 && this->loggedIn)
    {
        if(QFile::exists(this->picDir+"/"+this->username))
        {
            ui->rpLabel_2->setText("<img src=\"file:///"+this->picDir+"/"+this->username+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");
        }
    }

    if(arg1 == 2 && this->loggedIn)
    {
        if(QFile::exists(this->picDir+"/"+this->username))
        {
            ui->loggedPic->setText("<img src=\"file:///"+this->picDir+"/"+this->username+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");
        }

        QSqlQuery fetcher;
        fetcher.prepare("SELECT * FROM users WHERE username = (:un)");
        fetcher.bindValue(":un", this->username);
        fetcher.exec();

        int idFname = fetcher.record().indexOf("fname");
        int idMname = fetcher.record().indexOf("mname");
        int idLname = fetcher.record().indexOf("lname");
        int idRank = fetcher.record().indexOf("rank");
        int idEmail = fetcher.record().indexOf("email");

        QString fullname, email, rank;

        while (fetcher.next())
        {
            fullname = fetcher.value(idFname).toString();
            fullname += " " + fetcher.value(idMname).toString();
            fullname += " " + fetcher.value(idLname).toString();
            rank = fetcher.value(idRank).toString();
            email = fetcher.value(idEmail).toString();
        }
        if(rank == "-1")
        {
            ui->adminButton->setVisible(true);
            ui->deletebook->setVisible(true);
        }
        ui->nameLabel->setText(fullname);
        ui->rankLabel->setText(rank);
        ui->emailLabel->setText(email);
    }

    if(arg1 == 4 && this->loggedIn)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }

    if(arg1 == 6 && this->loggedIn) {
        this->tblbook = new QSqlTableModel;
        this->tblbook->setTable("books");
        this->tblbook->select();
        ui->tableView_3->setModel(this->tblbook);
        this->tblbook->database().transaction();
    }
}

void BookManagement::on_uplButton_clicked()
{
    qDebug() << QTime::currentTime().toString();
    this->picName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
    ui->rpLabel->setText("<img src=\"file:///"+this->picName+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");

}

void BookManagement::on_uplButton_2_clicked()
{
    qDebug() << QTime::currentTime().toString();
    this->picName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
    ui->rpLabel_2->setText("<img src=\"file:///"+this->picName+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");
}

void BookManagement::on_adminButton_clicked()
{
    qDebug() << QTime::currentTime().toString();
    ui->winStack->setCurrentIndex(4);
}

void BookManagement::on_pageButton_clicked()
{
    qDebug() << QTime::currentTime().toString();
    ui->winStack->setCurrentIndex(2);
}

void BookManagement::on_editedButton_2_clicked()
{
    qDebug() << QTime::currentTime().toString();
    if(this->tblMdl->submitAll())
    {
        this->tblMdl->database().commit();
    }
    else
    {
        this->tblMdl->database().rollback();
    }
}

void BookManagement::on_backButton_5_clicked()
{
    qDebug() << QTime::currentTime().toString();
    this->tblMdl->revertAll();
    this->tblMdl->database().rollback();
}

void BookManagement::on_userBrowse_clicked()
{
    qDebug() << QTime::currentTime().toString();
    ui->stackedWidget->setCurrentIndex(0);
}


void BookManagement::on_delUButton_clicked()
{
    qDebug() << QTime::currentTime().toString();
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                           "Book Managment", "Are you sure you want to erase all accounts?",
                                           QMessageBox::Yes|QMessageBox::No).exec())
    {
        QSqlQuery dQuery(db.db);
        dQuery.prepare("DELETE FROM users WHERE rank != 0 AND rank != -1");

    }
}

void BookManagement::on_stackedWidget_currentChanged(int arg1)
{      
    qDebug() << QTime::currentTime().toString();
    if(arg1 == 0 && this->loggedIn) {
        ui->headLabel->setText("USERS");
        this->tblMdl = new QSqlTableModel;
        this->tblMdl->setTable("users");
        this->tblMdl->setFilter("rank != -1 AND rank != 0");
        this->tblMdl->select();
        ui->tableView->setModel(this->tblMdl);
        this->tblMdl->database().transaction();
    }

    if(arg1 == 1 && this->loggedIn) {
        ui->headLabel->setText("ADMINS");
        this->tblMdl = new QSqlTableModel;
        this->tblMdl->setTable("users");
        this->tblMdl->setFilter("rank == -1 OR rank == 0");
        this->tblMdl->select();
        ui->tableView_2->setModel(this->tblMdl);
        this->tblMdl->database().transaction();
    }

}

void BookManagement::on_adminBrowse_clicked()
{
    qDebug() << QTime::currentTime().toString();
    ui->stackedWidget->setCurrentIndex(1);
}

void BookManagement::on_delAButton_clicked()
{
    qDebug() << QTime::currentTime().toString();
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                           "Book Managment", "Are you sure you want to erase all administrators?"\
                                           "\n(This won't erase regular users and you)",
                                           QMessageBox::Yes|QMessageBox::No).exec())
    {
        QSqlQuery dQuery(db.db);
        dQuery.prepare("DELETE FROM users WHERE rank != 1 AND username != \"" + this->username + "\"");

        if(dQuery.exec())
        {

        }
    }
}



void BookManagement::on_bookButton_clicked()
{
    qDebug() << QTime::currentTime().toString();
    ui->winStack->setCurrentIndex(5);
}

void BookManagement::on_addbo_clicked()
{
    qDebug() << QTime::currentTime().toString();
    QSqlQuery iQuery(db.db); 
    iQuery.prepare("INSERT INTO book (book_name, writer, year_of_publication , category, imagedir)"\
                       "VALUES(:bn, :wr, :yp, :ca, :id)");
    iQuery.bindValue(":bn", ui->bookname->text());
    iQuery.bindValue(":wr", ui->bookwriter->text());
    iQuery.bindValue(":yp", ui->Year_pub->text());
    iQuery.bindValue(":ca", ui->category->text());
    iQuery.bindValue(":id", this->picbook);
    iQuery.exec();

    ui->winStack->setCurrentIndex(2);


}

void BookManagement::on_uplButton_3_clicked()
{
    qDebug() << QTime::currentTime().toString();
    this->picbook = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
    ui->bookimg->setText("<img src=\"file:///"+this->picbook+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");
}

void BookManagement::on_back_clicked() {
    qDebug() << QTime::currentTime().toString();
    ui->bookname->setText("");
    ui->bookwriter->setText("");
    ui->Year_pub->setText("");
    ui->category->setText("");
    ui->bookimg->setText("");
    ui->winStack->setCurrentIndex(2);
}


void BookManagement::on_searchbook_clicked() {
    qDebug() << QTime::currentTime().toString();
    ui->winStack->setCurrentIndex(7);
}


void BookManagement::on_back_2_clicked() {
    qDebug() << QTime::currentTime().toString();
    ui->bookname->setText("");
    ui->bookwriter->setText("");
    ui->Year_pub->setText("");
    ui->category->setText("");
    ui->bookimg->setText("");
    ui->winStack->setCurrentIndex(2);
}

void BookManagement::on_back_3_clicked()
{
    qDebug() << QTime::currentTime().toString();
    ui->book_2->setText("");
    ui->writer_2->setText("");
    ui->y_o_p_2->setText("");
    ui->categorylable_2->setText("");
    ui->bookname_2->setText("");
    ui->bookimg_2->setText("");

    ui->winStack->setCurrentIndex(2);
}


void BookManagement::on_listbook_clicked()
{
   qDebug() << QTime::currentTime().toString();
   ui->winStack->setCurrentIndex(6);
}



void BookManagement::on_search_clicked()
{
    qDebug() << QTime::currentTime().toString();
    this->bookname = ui->bookname_2->text();
    this->writer = ui->writer_3->text();
    this->category = ui->category_2->text();

    if(bookname != "") {
        QSqlQuery fetcher;
        fetcher.prepare("SELECT * FROM book WHERE book_name like :bn");
        fetcher.bindValue(":bn", this->bookname);
        fetcher.exec();

        int idbook = fetcher.record().indexOf("book_name");
        int idwriter = fetcher.record().indexOf("writer");
        int idyop = fetcher.record().indexOf("year_of_publication");
        int idcatgory = fetcher.record().indexOf("category");
        int idimage = fetcher.record().indexOf("imagedir");

        QString book, writer, yop, catgory, image;

        while (fetcher.next()) {
            book = fetcher.value(idbook).toString();
            writer = fetcher.value(idwriter).toString();
            yop = fetcher.value(idyop).toString();
            catgory = fetcher.value(idcatgory).toString();
            image = fetcher.value(idimage).toString();
        }

        ui->book_2->setText(book);
        ui->writer_2->setText(writer);
        ui->y_o_p_2->setText(yop);
        ui->categorylable_2->setText(catgory);
        ui->bookimg_2->setText("<img src=\"file:///"+image+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");

    }

    else if(writer != "") {
        QSqlQuery fetcher;
        fetcher.prepare("SELECT * FROM book WHERE writer like :bn");
        fetcher.bindValue(":bn", this->writer);
        fetcher.exec();

        int idbook = fetcher.record().indexOf("book_name");
        int idwriter = fetcher.record().indexOf("writer");
        int idyop = fetcher.record().indexOf("year_of_publication");
        int idcatgory = fetcher.record().indexOf("category");
        int idimage = fetcher.record().indexOf("imagedir");

        QString book, writer, yop, catgory, image;

        while (fetcher.next()) {
            book = fetcher.value(idbook).toString();
            writer = fetcher.value(idwriter).toString();
            yop = fetcher.value(idyop).toString();
            catgory = fetcher.value(idcatgory).toString();
            image = fetcher.value(idimage).toString();
        }

        ui->book_2->setText(book);
        ui->writer_2->setText(writer);
        ui->y_o_p_2->setText(yop);
        ui->categorylable_2->setText(catgory);
        ui->bookimg_2->setText("<img src=\"file:///"+image+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");

    }

    else if(writer != "") {
        QSqlQuery fetcher;
        fetcher.prepare("SELECT * FROM book WHERE category like :bn");
        fetcher.bindValue(":bn", this->category);
        fetcher.exec();

        int idbook = fetcher.record().indexOf("book_name");
        int idwriter = fetcher.record().indexOf("writer");
        int idyop = fetcher.record().indexOf("year_of_publication");
        int idcatgory = fetcher.record().indexOf("category");
        int idimage = fetcher.record().indexOf("imagedir");

        QString book, writer, yop, catgory, image;

        while (fetcher.next()) {
            book = fetcher.value(idbook).toString();
            writer = fetcher.value(idwriter).toString();
            yop = fetcher.value(idyop).toString();
            catgory = fetcher.value(idcatgory).toString();
            image = fetcher.value(idimage).toString();
        }

        ui->book_2->setText(book);
        ui->writer_2->setText(writer);
        ui->y_o_p_2->setText(yop);
        ui->categorylable_2->setText(catgory);
        ui->bookimg_2->setText("<img src=\"file:///"+image+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");

    }



}

void BookManagement::on_deletebook_clicked() {
    ui->winStack->setCurrentIndex(8);
    qDebug() << QTime::currentTime().toString();
}


void BookManagement::on_back_4_clicked()
{
    qDebug() << QTime::currentTime().toString();
    ui->winStack->setCurrentIndex(2);
    ui->bookname_3->setText("");
    ui->result->setText("");

}


void BookManagement::on_search_2_clicked()
{
    qDebug() << QTime::currentTime().toString();
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                           "Book Managment", "Are you sure you want to erase all books?",
                                           QMessageBox::Yes|QMessageBox::No).exec())
    {
        qDebug() << QTime::currentTime().toString();
        QSqlQuery dQuery(db.db);
        dQuery.prepare("DELETE FROM book");
        ui->result->setText("All books are erase.");
    }

}


void BookManagement::on_search_3_clicked()
{
    qDebug() << QTime::currentTime().toString();
    this->bookname = ui->bookname_3->text();
    QSqlQuery dQuery(db.db);
    dQuery.prepare("DELETE FROM book WHERE book_name " + this->bookname + ";");
    ui->result->setText("The book" + this-> bookname + "is deleted.");
}


void BookManagement::on_loadbook_clicked()
{
    this->tblbook = new QSqlTableModel(this);
    this->tblbook->setTable("book");
    this->tblbook->select();
    ui->tableView_3->setModel(this->tblbook);
    this->tblbook->database().transaction();

}


void BookManagement::on_save_clicked()
{
    if(this->tblbook->submitAll())
    {
        this->tblbook->database().commit();
    }
    else
    {
        this->tblbook->database().rollback();
    }
}

