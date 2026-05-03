#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 🔤 ШРИФТ (твій код)
    int fontId = QFontDatabase::addApplicationFont(
        "C:/QtProjects/EatAndChill/resources/fonts/EBGaramond-Regular.ttf"
        );

    QStringList families = QFontDatabase::applicationFontFamilies(fontId);

    if(!families.isEmpty()) {
        QFont appFont(families.at(0));
        appFont.setPointSize(12);
        a.setFont(appFont);
    }

    // 🧪 ПЕРЕВІРКА ДРАЙВЕРІВ
    qDebug() << QSqlDatabase::drivers();

    // 🔌 ПІДКЛЮЧЕННЯ SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/QtProjects/EatAndChill/MY_SUPER_DB.db");

    if (!db.open()) {
        qDebug() << "DB error:" << db.lastError().text();
    } else {
        qDebug() << "DB connected!";
    }

    // 🧱 СТВОРЕННЯ ТАБЛИЦІ
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS menu ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name TEXT,"
               "price REAL,"
               "category TEXT,"
               "image TEXT)");

    // 🧾 таблиця замовлень
    query.exec("CREATE TABLE IF NOT EXISTS orders ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "datetime TEXT,"
               "total INTEGER)");

    // 📦 товари в замовленні
    query.exec("CREATE TABLE IF NOT EXISTS order_items ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "order_id INTEGER,"
               "name TEXT,"
               "price INTEGER,"
               "count INTEGER)");



    MainWindow w;
    w.show();

    return a.exec();
}