#include "/mainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setApplicationName("Stock Manager");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("YourCompany");
    QApplication::setOrganizationDomain("yourcompany.com");
    a.setWindowIcon(QIcon("C:/Users/klein/Desktop/installer/icons/dbsmss.ico"));

    MainWindow w;
    w.show();

    return a.exec();
}
