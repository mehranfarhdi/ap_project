#ifndef BOOK_H
#define BOOK_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QSqlTableModel>

namespace Ui {
class BookManagement;
}

class BookManagement : public QMainWindow
{
    Q_OBJECT

public:
    explicit BookManagement(QWidget *parent = 0);
    ~BookManagement();
    bool Login(QString u, QString p);
    bool loggedIn;
    QString picName;
    QString bookname;
    QString writer;
    QString category;
    QString picbook = "";
    QString picDir = QCoreApplication::applicationDirPath()+"/../../LogSys/users/avatar";
    QSqlTableModel *tblMdl;
    QSqlTableModel *tblbook;

private slots:
    void on_loginButton_clicked();

    void on_logoutButton_clicked();

    void on_completeRegButton_clicked();

    void on_backButton_clicked();

    void on_regButton_clicked();

    void on_backButton_2_clicked();

    void on_editButton_clicked();

    void on_delButton_clicked();

    void on_editedButton_clicked();

    void on_winStack_currentChanged(int arg1);

    void on_uplButton_clicked();

    void on_uplButton_2_clicked();

    void on_adminButton_clicked();

    void on_pageButton_clicked();

    void on_editedButton_2_clicked();

    void on_backButton_5_clicked();

    void on_userBrowse_clicked();

    void on_delUButton_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_adminBrowse_clicked();

    void on_delAButton_clicked();

    void on_bookButton_clicked();

    void on_addbo_clicked();


    void on_back_clicked();

    void on_back_2_clicked();

    void on_back_3_clicked();

    void on_loadbook_clicked();

    void on_listbook_clicked();

    void on_search_clicked();

    void on_uplButton_3_clicked();

    void on_searchbook_clicked();

    void on_deletebook_clicked();

    void on_back_4_clicked();

    void on_search_2_clicked();

    void on_search_3_clicked();

    void on_save_clicked();

private:
    Ui::BookManagement *ui;
    QString username;
    QString password;
};

#endif // BOOK_H
