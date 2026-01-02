#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedItemId(-1)
{
    ui->setupUi(this);

    database = new Database();
    if (!database->connect()) {
        showMessage("Database Error", "Failed to connect to database!");
    }

    ui->stockTable->setColumnWidth(0, 50);
    ui->stockTable->setColumnWidth(1, 250);
    ui->stockTable->setColumnWidth(2, 100);
    ui->stockTable->setColumnWidth(3, 100);
    ui->stockTable->setColumnWidth(4, 100);

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchButtonClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshButtonClicked);
    connect(ui->stockTable, &QTableWidget::cellClicked, this, &MainWindow::onTableItemClicked);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExitAction);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAboutAction);

    onRefreshButtonClicked();

    ui->statusbar->showMessage("Ready");
}

MainWindow::~MainWindow()
{
    delete database;
    delete ui;
}

void MainWindow::onAddButtonClicked()
{
    QString name = ui->nameEdit->text().trimmed();
    double price = ui->priceSpinBox->value();
    double profit = ui->profitSpinBox->value();
    int unit = ui->unitSpinBox->value();

    if (name.isEmpty()) {
        showMessage("Validation Error", "Item name cannot be empty!");
        return;
    }

    if (database->addItem(name, price, profit, unit)) {
        showMessage("Success", "Item added successfully!");
        clearInputFields();
        onRefreshButtonClicked();
        ui->statusbar->showMessage("Item added", 3000);
    } else {
        showMessage("Error", "Failed to add item!");
    }
}

void MainWindow::onUpdateButtonClicked()
{
    if (selectedItemId == -1) {
        showMessage("Selection Error", "Please select an item from the table to update!");
        return;
    }

    QString name = ui->nameEdit->text().trimmed();
    double price = ui->priceSpinBox->value();
    double profit = ui->profitSpinBox->value();
    int unit = ui->unitSpinBox->value();

    if (name.isEmpty()) {
        showMessage("Validation Error", "Item name cannot be empty!");
        return;
    }

    if (database->updateItem(selectedItemId, name, price, profit, unit)) {
        showMessage("Success", "Item updated successfully!");
        clearInputFields();
        onRefreshButtonClicked();
        ui->statusbar->showMessage("Item updated", 3000);
    } else {
        showMessage("Error", "Failed to update item!");
    }
}

void MainWindow::onDeleteButtonClicked()
{
    if (selectedItemId == -1) {
        showMessage("Selection Error", "Please select an item from the table to delete!");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete",
                                  "Are you sure you want to delete this item?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (database->deleteItem(selectedItemId)) {
            showMessage("Success", "Item deleted successfully!");
            clearInputFields();
            onRefreshButtonClicked();
            ui->statusbar->showMessage("Item deleted", 3000);
        } else {
            showMessage("Error", "Failed to delete item!");
        }
    }
}

void MainWindow::onClearButtonClicked()
{
    clearInputFields();
}

void MainWindow::onSearchButtonClicked()
{
    QString searchTerm = ui->searchEdit->text().trimmed();

    if (searchTerm.isEmpty()) {
        onRefreshButtonClicked();
        return;
    }

    QList<StockItem> items = database->searchItems(searchTerm);
    loadTableData(items);
    ui->statusbar->showMessage(QString("Found %1 item(s)").arg(items.size()), 3000);
}

void MainWindow::onRefreshButtonClicked()
{
    QList<StockItem> items = database->getAllItems();
    loadTableData(items);
    ui->searchEdit->clear();
    ui->statusbar->showMessage("Data refreshed", 3000);
}

void MainWindow::onTableItemClicked(int row, int column)
{
    Q_UNUSED(column);

    QTableWidgetItem *idItem = ui->stockTable->item(row, 0);
    if (idItem) {
        selectedItemId = idItem->text().toInt();

        StockItem item = database->getItemById(selectedItemId);
        if (item.id != -1) {
            ui->nameEdit->setText(item.name);
            ui->priceSpinBox->setValue(item.price);
            ui->profitSpinBox->setValue(item.profit);
            ui->unitSpinBox->setValue(item.unit);
            ui->statusbar->showMessage(QString("Selected: %1").arg(item.name), 3000);
        }
    }
}

void MainWindow::onExitAction()
{
    QApplication::quit();
}

void MainWindow::onAboutAction()
{
    QMessageBox::about(this, "About Stock Manager",
                       "Stock Manager v1.0\n\n"
                       "A simple inventory management system.\n"
                       "Built with Qt6 and SQLite.");
}

void MainWindow::loadTableData(const QList<StockItem> &items)
{
    ui->stockTable->setRowCount(0);

    for (const StockItem &item : items) {
        int row = ui->stockTable->rowCount();
        ui->stockTable->insertRow(row);

        ui->stockTable->setItem(row, 0, new QTableWidgetItem(QString::number(item.id)));
        ui->stockTable->setItem(row, 1, new QTableWidgetItem(item.name));
        ui->stockTable->setItem(row, 2, new QTableWidgetItem(QString::number(item.price, 'f', 2)));
        ui->stockTable->setItem(row, 3, new QTableWidgetItem(QString::number(item.profit, 'f', 2)));
        ui->stockTable->setItem(row, 4, new QTableWidgetItem(QString::number(item.unit)));
    }
}

void MainWindow::clearInputFields()
{
    ui->nameEdit->clear();
    ui->priceSpinBox->setValue(0.0);
    ui->profitSpinBox->setValue(0.0);
    ui->unitSpinBox->setValue(0);
    selectedItemId = -1;
    ui->statusbar->showMessage("Fields cleared", 3000);
}

void MainWindow::showMessage(const QString &title, const QString &message)
{
    QMessageBox::information(this, title, message);
}
