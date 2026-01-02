#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <QDebug>

struct StockItem {
    int id;
    QString name;
    double price;
    double profit;
    int unit;
};

class Database
{
public:
    Database();
    ~Database();

    bool connect();
    bool createTable();


    bool addItem(const QString &name, double price, double profit, int unit);
    bool updateItem(int id, const QString &name, double price, double profit, int unit);
    bool deleteItem(int id);
    QList<StockItem> getAllItems();
    QList<StockItem> searchItems(const QString &searchTerm);
    StockItem getItemById(int id);

private:
    QSqlDatabase db;
    QString dbPath;
};

#endif
