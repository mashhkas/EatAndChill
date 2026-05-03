/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QWidget *centerContainer;
    QWidget *widget_2;
    QStackedWidget *stackedWidget;
    QWidget *page_login;
    QPushButton *btn_login;
    QLineEdit *input_login;
    QLineEdit *input_password;
    QLabel *label_login_title;
    QWidget *page_cart;
    QGridLayout *gridLayout_3;
    QPushButton *btn_remove_item;
    QPushButton *btn_checkout;
    QListWidget *list_cart;
    QLabel *label_total;
    QWidget *widget;
    QWidget *page_admin;
    QLineEdit *input_name;
    QLineEdit *input_price;
    QPushButton *btn_add_product;
    QListWidget *list_products;
    QPushButton *btn_back_from_admin;
    QListWidget *list_stoplist;
    QPushButton *btn_add_to_stoplist;
    QPushButton *btn_delete_product;
    QPushButton *btn_edit_product;
    QComboBox *combo_category;
    QLabel *label_stats;
    QWidget *page_history;
    QWidget *page_products;
    QWidget *page_stock;
    QWidget *page_settings;
    QWidget *page_register;
    QWidget *page_menu;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget_menu;
    QWidget *page_menu_main;
    QPushButton *btn_to_history;
    QPushButton *btn_to_cart;
    QPushButton *btn_kitchen;
    QPushButton *btn_bar;
    QWidget *page_bar;
    QWidget *topContainer;
    QWidget *centerContainer_2;
    QWidget *page_products_2;
    QVBoxLayout *verticalLayout;
    QListWidget *list_menu;
    QPushButton *btn_add_to_cart;
    QPushButton *btn_sort_name;
    QPushButton *btn_sort_price;
    QPushButton *btn_back_to_categories;
    QLineEdit *input_search;
    QWidget *page_kitchen;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        centerContainer = new QWidget(centralwidget);
        centerContainer->setObjectName("centerContainer");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centerContainer->sizePolicy().hasHeightForWidth());
        centerContainer->setSizePolicy(sizePolicy);
        centerContainer->setMinimumSize(QSize(400, 0));
        centerContainer->setMaximumSize(QSize(400, 16777215));

        gridLayout_2->addWidget(centerContainer, 3, 0, 1, 1);

        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName("widget_2");

        gridLayout_2->addWidget(widget_2, 2, 0, 1, 1);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        page_login = new QWidget();
        page_login->setObjectName("page_login");
        btn_login = new QPushButton(page_login);
        btn_login->setObjectName("btn_login");
        btn_login->setGeometry(QRect(210, 240, 90, 29));
        input_login = new QLineEdit(page_login);
        input_login->setObjectName("input_login");
        input_login->setGeometry(QRect(480, 90, 113, 28));
        input_password = new QLineEdit(page_login);
        input_password->setObjectName("input_password");
        input_password->setGeometry(QRect(480, 130, 113, 28));
        input_password->setEchoMode(QLineEdit::EchoMode::Password);
        label_login_title = new QLabel(page_login);
        label_login_title->setObjectName("label_login_title");
        label_login_title->setGeometry(QRect(420, 50, 161, 20));
        stackedWidget->addWidget(page_login);
        page_cart = new QWidget();
        page_cart->setObjectName("page_cart");
        gridLayout_3 = new QGridLayout(page_cart);
        gridLayout_3->setObjectName("gridLayout_3");
        btn_remove_item = new QPushButton(page_cart);
        btn_remove_item->setObjectName("btn_remove_item");

        gridLayout_3->addWidget(btn_remove_item, 0, 0, 1, 1);

        btn_checkout = new QPushButton(page_cart);
        btn_checkout->setObjectName("btn_checkout");

        gridLayout_3->addWidget(btn_checkout, 1, 0, 1, 1);

        list_cart = new QListWidget(page_cart);
        list_cart->setObjectName("list_cart");

        gridLayout_3->addWidget(list_cart, 3, 0, 1, 1);

        label_total = new QLabel(page_cart);
        label_total->setObjectName("label_total");

        gridLayout_3->addWidget(label_total, 2, 0, 1, 1);

        stackedWidget->addWidget(page_cart);
        widget = new QWidget();
        widget->setObjectName("widget");
        stackedWidget->addWidget(widget);
        page_admin = new QWidget();
        page_admin->setObjectName("page_admin");
        input_name = new QLineEdit(page_admin);
        input_name->setObjectName("input_name");
        input_name->setGeometry(QRect(140, 40, 113, 28));
        input_price = new QLineEdit(page_admin);
        input_price->setObjectName("input_price");
        input_price->setGeometry(QRect(140, 80, 113, 28));
        btn_add_product = new QPushButton(page_admin);
        btn_add_product->setObjectName("btn_add_product");
        btn_add_product->setGeometry(QRect(130, 120, 131, 29));
        list_products = new QListWidget(page_admin);
        list_products->setObjectName("list_products");
        list_products->setGeometry(QRect(70, 170, 256, 192));
        btn_back_from_admin = new QPushButton(page_admin);
        btn_back_from_admin->setObjectName("btn_back_from_admin");
        btn_back_from_admin->setGeometry(QRect(400, 480, 90, 29));
        list_stoplist = new QListWidget(page_admin);
        list_stoplist->setObjectName("list_stoplist");
        list_stoplist->setGeometry(QRect(340, 170, 256, 192));
        btn_add_to_stoplist = new QPushButton(page_admin);
        btn_add_to_stoplist->setObjectName("btn_add_to_stoplist");
        btn_add_to_stoplist->setGeometry(QRect(420, 380, 90, 29));
        btn_delete_product = new QPushButton(page_admin);
        btn_delete_product->setObjectName("btn_delete_product");
        btn_delete_product->setGeometry(QRect(339, 40, 121, 29));
        btn_edit_product = new QPushButton(page_admin);
        btn_edit_product->setObjectName("btn_edit_product");
        btn_edit_product->setGeometry(QRect(340, 90, 121, 29));
        combo_category = new QComboBox(page_admin);
        combo_category->addItem(QString());
        combo_category->addItem(QString());
        combo_category->addItem(QString());
        combo_category->addItem(QString());
        combo_category->addItem(QString());
        combo_category->addItem(QString());
        combo_category->addItem(QString());
        combo_category->addItem(QString());
        combo_category->addItem(QString());
        combo_category->addItem(QString());
        combo_category->addItem(QString());
        combo_category->setObjectName("combo_category");
        combo_category->setGeometry(QRect(251, 450, 181, 28));
        combo_category->setAcceptDrops(false);
        combo_category->setEditable(false);
        label_stats = new QLabel(page_admin);
        label_stats->setObjectName("label_stats");
        label_stats->setGeometry(QRect(90, 390, 211, 81));
        stackedWidget->addWidget(page_admin);
        page_history = new QWidget();
        page_history->setObjectName("page_history");
        stackedWidget->addWidget(page_history);
        page_products = new QWidget();
        page_products->setObjectName("page_products");
        stackedWidget->addWidget(page_products);
        page_stock = new QWidget();
        page_stock->setObjectName("page_stock");
        stackedWidget->addWidget(page_stock);
        page_settings = new QWidget();
        page_settings->setObjectName("page_settings");
        stackedWidget->addWidget(page_settings);
        page_register = new QWidget();
        page_register->setObjectName("page_register");
        stackedWidget->addWidget(page_register);
        page_menu = new QWidget();
        page_menu->setObjectName("page_menu");
        gridLayout = new QGridLayout(page_menu);
        gridLayout->setObjectName("gridLayout");
        stackedWidget_menu = new QStackedWidget(page_menu);
        stackedWidget_menu->setObjectName("stackedWidget_menu");
        page_menu_main = new QWidget();
        page_menu_main->setObjectName("page_menu_main");
        btn_to_history = new QPushButton(page_menu_main);
        btn_to_history->setObjectName("btn_to_history");
        btn_to_history->setGeometry(QRect(439, 40, 141, 29));
        btn_to_cart = new QPushButton(page_menu_main);
        btn_to_cart->setObjectName("btn_to_cart");
        btn_to_cart->setGeometry(QRect(640, 40, 80, 29));
        btn_kitchen = new QPushButton(page_menu_main);
        btn_kitchen->setObjectName("btn_kitchen");
        btn_kitchen->setGeometry(QRect(290, 170, 80, 29));
        btn_bar = new QPushButton(page_menu_main);
        btn_bar->setObjectName("btn_bar");
        btn_bar->setGeometry(QRect(290, 270, 80, 29));
        stackedWidget_menu->addWidget(page_menu_main);
        page_bar = new QWidget();
        page_bar->setObjectName("page_bar");
        topContainer = new QWidget(page_bar);
        topContainer->setObjectName("topContainer");
        topContainer->setGeometry(QRect(30, 20, 120, 80));
        centerContainer_2 = new QWidget(page_bar);
        centerContainer_2->setObjectName("centerContainer_2");
        centerContainer_2->setGeometry(QRect(40, 160, 120, 80));
        stackedWidget_menu->addWidget(page_bar);
        page_products_2 = new QWidget();
        page_products_2->setObjectName("page_products_2");
        verticalLayout = new QVBoxLayout(page_products_2);
        verticalLayout->setObjectName("verticalLayout");
        list_menu = new QListWidget(page_products_2);
        list_menu->setObjectName("list_menu");

        verticalLayout->addWidget(list_menu);

        btn_add_to_cart = new QPushButton(page_products_2);
        btn_add_to_cart->setObjectName("btn_add_to_cart");

        verticalLayout->addWidget(btn_add_to_cart);

        btn_sort_name = new QPushButton(page_products_2);
        btn_sort_name->setObjectName("btn_sort_name");

        verticalLayout->addWidget(btn_sort_name);

        btn_sort_price = new QPushButton(page_products_2);
        btn_sort_price->setObjectName("btn_sort_price");

        verticalLayout->addWidget(btn_sort_price);

        btn_back_to_categories = new QPushButton(page_products_2);
        btn_back_to_categories->setObjectName("btn_back_to_categories");

        verticalLayout->addWidget(btn_back_to_categories);

        input_search = new QLineEdit(page_products_2);
        input_search->setObjectName("input_search");

        verticalLayout->addWidget(input_search);

        stackedWidget_menu->addWidget(page_products_2);
        page_kitchen = new QWidget();
        page_kitchen->setObjectName("page_kitchen");
        stackedWidget_menu->addWidget(page_kitchen);

        gridLayout->addWidget(stackedWidget_menu, 0, 1, 1, 1);

        stackedWidget->addWidget(page_menu);

        gridLayout_2->addWidget(stackedWidget, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget_menu->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btn_login->setText(QCoreApplication::translate("MainWindow", "\320\243\320\262\321\226\320\271\321\202\320\270", nullptr));
        input_login->setText(QString());
        input_login->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\321\226\321\202\321\214 \320\273\320\276\320\263\321\226\320\275:", nullptr));
        input_password->setText(QString());
        input_password->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\321\226\321\202\321\214 \320\277\320\260\321\200\320\276\320\273\321\214:", nullptr));
        label_login_title->setText(QCoreApplication::translate("MainWindow", "\320\222\321\205\321\226\320\264 \320\260\320\264\320\274\321\226\320\275\321\226\321\201\321\202\321\200\320\260\321\202\320\276\321\200\320\260", nullptr));
        btn_remove_item->setText(QString());
        btn_checkout->setText(QString());
        label_total->setText(QString());
        input_name->setText(QString());
        input_name->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260 \321\202\320\276\320\262\320\260\321\200\321\203:", nullptr));
        input_price->setText(QString());
        input_price->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\246\321\226\320\275\320\260:", nullptr));
        btn_add_product->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\264\320\260\321\202\320\270 \321\202\320\276\320\262\320\260\321\200", nullptr));
        btn_back_from_admin->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\260\320\264", nullptr));
        btn_add_to_stoplist->setText(QCoreApplication::translate("MainWindow", "\320\262 \321\201\321\202\320\276\320\277-\320\273\320\270\321\201\321\202", nullptr));
        btn_delete_product->setText(QCoreApplication::translate("MainWindow", "\320\222\320\270\320\264\320\260\320\273\320\270\321\202\320\270 \321\202\320\276\320\262\320\260\321\200", nullptr));
        btn_edit_product->setText(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\264\320\260\320\263\321\203\320\262\320\260\321\202\320\270 \321\202\320\276\320\262\320\260\321\200", nullptr));
        combo_category->setItemText(0, QCoreApplication::translate("MainWindow", "\320\222\320\270\320\261\320\265\321\200\321\226\321\202\321\214 \320\272\320\260\321\202\320\265\320\263\320\276\321\200\321\226\321\216:", nullptr));
        combo_category->setItemText(1, QCoreApplication::translate("MainWindow", "\320\237\321\226\321\206\320\260", nullptr));
        combo_category->setItemText(2, QCoreApplication::translate("MainWindow", "\320\241\320\260\320\273\320\260\321\202\320\270", nullptr));
        combo_category->setItemText(3, QCoreApplication::translate("MainWindow", "\320\227\320\260\320\272\321\203\321\201\320\272\320\270", nullptr));
        combo_category->setItemText(4, QCoreApplication::translate("MainWindow", "\320\223\320\260\321\200\320\275\321\226\321\200\320\270", nullptr));
        combo_category->setItemText(5, QCoreApplication::translate("MainWindow", "\320\224\320\265\321\201\320\265\321\200\321\202\320\270", nullptr));
        combo_category->setItemText(6, QCoreApplication::translate("MainWindow", "\320\223\320\260\321\200\321\217\321\207\321\226 \320\275\320\260\320\277\320\276\321\227", nullptr));
        combo_category->setItemText(7, QCoreApplication::translate("MainWindow", "\320\223\320\260\320\267\320\276\320\262\320\260\320\275\321\226 \320\275\320\260\320\277\320\276\321\227", nullptr));
        combo_category->setItemText(8, QCoreApplication::translate("MainWindow", "\320\232\320\276\320\272\321\202\320\265\320\271\320\273\321\226", nullptr));
        combo_category->setItemText(9, QCoreApplication::translate("MainWindow", "\320\241\320\276\320\272\320\270", nullptr));
        combo_category->setItemText(10, QCoreApplication::translate("MainWindow", "\320\220\320\273\320\272\320\276\320\263\320\276\320\273\321\214", nullptr));

        combo_category->setCurrentText(QCoreApplication::translate("MainWindow", "\320\222\320\270\320\261\320\265\321\200\321\226\321\202\321\214 \320\272\320\260\321\202\320\265\320\263\320\276\321\200\321\226\321\216:", nullptr));
        combo_category->setPlaceholderText(QString());
        label_stats->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        btn_to_history->setText(QCoreApplication::translate("MainWindow", "\320\206\321\201\321\202\320\276\321\200\321\226\321\217 \320\267\320\260\320\274\320\276\320\262\320\273\320\265\320\275\321\214", nullptr));
        btn_to_cart->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\321\210\320\270\320\272", nullptr));
        btn_kitchen->setText(QCoreApplication::translate("MainWindow", "\320\232\321\203\321\205\320\275\321\217", nullptr));
        btn_bar->setText(QCoreApplication::translate("MainWindow", "\320\221\320\260\321\200", nullptr));
        btn_add_to_cart->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\264\320\260\321\202\320\270 \320\262 \320\272\320\276\321\210\320\270\320\272", nullptr));
        btn_sort_name->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\200\321\202\321\203\320\262\320\260\321\202\320\270 \320\277\320\276 \320\275\320\260\320\267\320\262\321\226", nullptr));
        btn_sort_price->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\200\321\202\321\203\320\262\320\260\321\202\320\270 \320\277\320\276 \321\206\321\226\320\275\321\226", nullptr));
        btn_back_to_categories->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\260\320\264", nullptr));
        input_search->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\321\210\321\203\320\272 \321\202\320\276\320\262\320\260\321\200\321\203", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
