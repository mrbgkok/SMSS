#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidgetItem>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddButtonClicked();
    void onUpdateButtonClicked();
    void onDeleteButtonClicked();
    void onClearButtonClicked();
    void onSearchButtonClicked();
    void onRefreshButtonClicked();
    void onTableItemClicked(int row, int column);
    void onExitAction();
    void onAboutAction();

private:
    Ui::MainWindow *ui;
    Database *database;
    int selectedItemId;

    void loadTableData(const QList<StockItem> &items);
    void clearInputFields();
    void showMessage(const QString &title, const QString &message);
};

#endif
