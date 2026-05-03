#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <qgridlayout.h>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    QListWidget *historyList;
    QLabel *statsLabel;
    QTableWidget *cartTable;

    QPushButton *btnAdmin;

    QPushButton *btnCartTop;

    QGridLayout *productsGrid;

    void loadMenuFromDB();

    void updateCartTable();
    void setupMenuLayout();
    void setupLoginLayout();

    int totalOrders;
    int totalRevenue;
    void updateStats();

    int orderCounter;
    void updateTotal();
    void updateCartCount();

    bool isEditing = false;
    int editingRow = -1;

    bool sortAscending = false;

    QString currentCategoryGroup;
    QString currentCategory;

};

#endif // MAINWINDOW_H
