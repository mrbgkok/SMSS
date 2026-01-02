#include "database.h"
#include <QStandardPaths>
#include <QDir>

Database::Database()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(dataPath)) {
        dir.mkpath(dataPath);
    }
    dbPath = dataPath + "/stock.db";
}

Database::~Database()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool Database::connect()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Error: connection with database failed:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully at:" << dbPath;
    return createTable();
}

bool Database::createTable()
{
    QSqlQuery query;
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS stock (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            price REAL NOT NULL,
            profit REAL NOT NULL,
            unit INTEGER NOT NULL
        )
    )";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creating table:" << query.lastError().text();
        return false;
    }

    qDebug() << "Table created or already exists";
    return true;
}

bool Database::addItem(const QString &name, double price, double profit, int unit)
{
    QSqlQuery query;
    query.prepare("INSERT INTO stock (name, price, profit, unit) VALUES (:name, :price, :profit, :unit)");
    query.bindValue(":name", name);
    query.bindValue(":price", price);
    query.bindValue(":profit", profit);
    query.bindValue(":unit", unit);

    if (!query.exec()) {
        qDebug() << "Error adding item:" << query.lastError().text();
        return false;
    }

    qDebug() << "Item added successfully";
    return true;
}

bool Database::updateItem(int id, const QString &name, double price, double profit, int unit)
{
    QSqlQuery query;
    query.prepare("UPDATE stock SET name = :name, price = :price, profit = :profit, unit = :unit WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":price", price);
    query.bindValue(":profit", profit);
    query.bindValue(":unit", unit);

    if (!query.exec()) {
        qDebug() << "Error updating item:" << query.lastError().text();
        return false;
    }

    qDebug() << "Item updated successfully";
    return true;
}

bool Database::deleteItem(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM stock WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error deleting item:" << query.lastError().text();
        return false;
    }

    qDebug() << "Item deleted successfully";
    return true;
}

QList<StockItem> Database::getAllItems()
{
    QList<StockItem> items;
    QSqlQuery query("SELECT id, name, price, profit, unit FROM stock ORDER BY id");

    while (query.next()) {
        StockItem item;
        item.id = query.value(0).toInt();
        item.name = query.value(1).toString();
        item.price = query.value(2).toDouble();
        item.profit = query.value(3).toDouble();
        item.unit = query.value(4).toInt();
        items.append(item);
    }

    return items;
}

QList<StockItem> Database::searchItems(const QString &searchTerm)
{
    QList<StockItem> items;
    QSqlQuery query;
    query.prepare("SELECT id, name, price, profit, unit FROM stock WHERE name LIKE :search ORDER BY id");
    query.bindValue(":search", "%" + searchTerm + "%");

    if (query.exec()) {
        while (query.next()) {
            StockItem item;
            item.id = query.value(0).toInt();
            item.name = query.value(1).toString();
            item.price = query.value(2).toDouble();
            item.profit = query.value(3).toDouble();
            item.unit = query.value(4).toInt();
            items.append(item);
        }
    }

    return items;
}

StockItem Database::getItemById(int id)
{
    StockItem item;
    item.id = -1;

    QSqlQuery query;
    query.prepare("SELECT id, name, price, profit, unit FROM stock WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        item.id = query.value(0).toInt();
        item.name = query.value(1).toString();
        item.price = query.value(2).toDouble();
        item.profit = query.value(3).toDouble();
        item.unit = query.value(4).toInt();
    }

    return item;
}
