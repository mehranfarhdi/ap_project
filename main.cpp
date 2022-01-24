#include "book.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BookManagement w;
    w.show();

    return a.exec();
}
