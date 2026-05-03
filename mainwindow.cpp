#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "product.h"
#include "pizza.h"
#include "drink.h"
#include "utils.h"

#include <QFontDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QListWidget>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>
#include <QIntValidator>
#include <algorithm>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QFileDialog>

//додати якусь картинку на фон всюди

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // ================= Ініціалізація UI =================
    ui->setupUi(this);
    ui->btn_to_history->hide();
    ui->list_cart->hide();
    ui->btn_remove_item->hide();
    ui->btn_checkout->hide();
    ui->label_total->hide();
    ui->list_products->hide();
    ui->btn_delete_product->hide();
    ui->btn_edit_product->hide();
    ui->btn_add_to_stoplist->hide();

    ui->btn_to_history->setObjectName("cartButton");
    auto shadowHistory = new QGraphicsDropShadowEffect(this);
    shadowHistory->setBlurRadius(25);
    shadowHistory->setOffset(0, 6);
    shadowHistory->setColor(QColor(0, 0, 0, 180));

    ui->btn_to_history->setGraphicsEffect(shadowHistory);

    QVBoxLayout *mainLayout = new QVBoxLayout();

    // 🔙Кнопка назад
    QPushButton *btnBack = new QPushButton("← Назад");
    btnBack->setFixedSize(120, 40);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(btnBack);
    topLayout->addStretch();

    // Центральний блок
    QWidget *center = new QWidget();
    center->setFixedWidth(400);

    QVBoxLayout *centerLayout = new QVBoxLayout(center);
    centerLayout->setSpacing(12);

    QLineEdit *inputSearchProducts = new QLineEdit();
    auto loadProducts = [=]() {

        // Очистка
        QLayoutItem *child;
        while ((child = productsGrid->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }

        int col = 0;
        int row = 0;

        for(int i = 0; i < ui->list_menu->count(); i++) {

            QString text = ui->list_menu->item(i)->text();
            QString search = inputSearchProducts ? inputSearchProducts->text().toLower() : "";

            if(!search.isEmpty() && !text.toLower().contains(search))
                continue;

            if(!text.contains("[" + currentCategory + "]")) continue;

            QString name = text.section(" - ", 0, 0);
            int price = text.section(" - ", 1, 1).section(" грн", 0, 0).toInt();
            QString image = text.contains("| img: ") ? text.section("| img: ", 1, 1) : "";

            // 🔲 карточка
            QWidget *card = new QWidget();
            card->setFixedSize(230, 300);
            card->setStyleSheet("background:#f5f5f5; border-radius:15px;");

            QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
            shadow->setBlurRadius(30);
            shadow->setOffset(0, 8);
            shadow->setColor(QColor(0, 0, 0, 180));
            card->setGraphicsEffect(shadow);
            QVBoxLayout *cardLayout = new QVBoxLayout(card);

            // Фото
            QLabel *img = new QLabel();
            img->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            img->setScaledContents(false);
            img->setAlignment(Qt::AlignCenter);
            img->setFixedHeight(210);
            img->setAlignment(Qt::AlignCenter);
            img->setStyleSheet("background:#ccc; border-radius:10px;");
            img->setScaledContents(true);

            if(!image.isEmpty()) {

                QPixmap pix(image);

                img->setPixmap(
                    pix.scaled(
                        250,
                        170,
                        Qt::KeepAspectRatioByExpanding,
                        Qt::SmoothTransformation
                        )
                );
                img->setAlignment(Qt::AlignCenter);

            } else {

                img->setStyleSheet("background:#ccc;");
            }

            // Текст
            QLabel *label = new QLabel(name + "\n" + QString::number(price) + " грн");
            label->setStyleSheet("color: #111111; font-size:14px; font-weight:600;");
            label->setAlignment(Qt::AlignCenter);

            // Кнопка
            QPushButton *btnAddCart = new QPushButton("Додати в кошик");
            btnAddCart->setFixedHeight(36);

            QGraphicsDropShadowEffect *btnShadow = new QGraphicsDropShadowEffect(this);
            btnShadow->setBlurRadius(20);
            btnShadow->setOffset(0, 4);
            btnShadow->setColor(QColor(0, 0, 0, 150));

            btnAddCart->setGraphicsEffect(btnShadow);

            btnAddCart->setStyleSheet(R"(
                QPushButton {
                    background: #ff3b3b;
                    color: white;
                    border-radius: 14px;
                    font-weight: 600;
                    padding: 8px;
                    margin: 0px;
                    border: none;
                }
                QPushButton:hover {
                    background: #ff5c5c;
                }
            )");

            // Стоп-лист
            bool blocked = false;
            for(int j = 0; j < ui->list_stoplist->count(); j++) {
                if(ui->list_stoplist->item(j)->text() == text) {
                    blocked = true;
                    break;
                }
            }

            if(blocked) {
                card->setStyleSheet(R"(
                    QWidget {
                        background: #555;
                        border-radius: 18px;
                    }
                )");
                btnAddCart->setText("Недоступно");

                connect(btnAddCart, &QPushButton::clicked, this, [=]() {
                    QMessageBox::warning(this, "Стоп-лист",
                                         "Вибачте, товар недоступний");
                });
            } else {
                connect(btnAddCart, &QPushButton::clicked, this, [=]() {

                    btnAddCart->setText("✔ Додано");

                    QString newItem = text;

                    bool found = false;

                    for(int k = 0; k < ui->list_cart->count(); k++) {
                        if(ui->list_cart->item(k)->text().startsWith(newItem)) {

                            int count = ui->list_cart->item(k)->text().contains("x")
                            ? ui->list_cart->item(k)->text().split("x").last().toInt() : 1;

                            count++;
                            ui->list_cart->item(k)->setText(newItem + " x" + QString::number(count));
                            found = true;
                            break;
                        }
                    }

                    if(!found) {
                        ui->list_cart->addItem(newItem + " x1");
                    }

                    updateCartTable();
                    updateCartCount();

                    btnCartTop->setText("Кошик (" + QString::number(ui->list_cart->count()) + ")");
                });
            }
            cardLayout->setContentsMargins(4, 4, 4, 4);
            cardLayout->setSpacing(3);

            btnAddCart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

            cardLayout->addWidget(img);
            cardLayout->addWidget(label);
            cardLayout->addWidget(btnAddCart);

            cardLayout->addStretch();

            productsGrid->addWidget(card, row, col);

            col++;
            if(col == 6) {
                col = 0;
                row++;
            }
        }
        int totalItems = row * 3 + col;

        productsGrid->setRowStretch(row + 1, 1);
    };


    // Список кнопок
    QStringList items = {
        "Гарячі напої",
        "Газовані напої",
        "Коктейлі",
        "Соки",
        "Алкоголь"
    };
    QStringList kitchenItems = {
        "Піца",
        "Закуски",
        "Салати",
        "Гарніри",
        "Десерти"
    };

    QString categoryStyle = R"(
    QPushButton {
        background: #2b2b2b;
        color: white;
        border-radius: 14px;
        padding: 12px 18px;
        font-size: 16px;
        font-weight: 600;
        text-align: left;
    }
    QPushButton:hover {
        background: #3a3a3a;
    }
    QPushButton:pressed {
        background: #ff3b3b;
    }
    )";


    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(center, 0, Qt::AlignHCenter);
    mainLayout->addStretch();

    ui->page_bar->setLayout(mainLayout);
    QVBoxLayout *kitchenLayout = new QVBoxLayout();

    // Кнопка назад
    QPushButton *btnBackKitchen = new QPushButton("← Назад");
    connect(btnBackKitchen, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_menu->setCurrentWidget(ui->page_menu_main);
    });
    btnBackKitchen->setFixedSize(120, 40);

    btnBackKitchen->setStyleSheet(R"(
        QPushButton {
            background: #3a3a3a;
            color: white;
            border-radius: 12px;
            padding: 10px 16px;
            font-weight: 500;
        }
        QPushButton:hover {
            background: #4a4a4a;
        }
        QPushButton:pressed {
            background: #ff3b3b;
        }
    )");

    auto shadowBackKitchen = new QGraphicsDropShadowEffect(this);
    shadowBackKitchen->setBlurRadius(25);
    shadowBackKitchen->setOffset(0,5);
    shadowBackKitchen->setColor(QColor(0,0,0,150));
    btnBackKitchen->setGraphicsEffect(shadowBackKitchen);

    QHBoxLayout *topKitchen = new QHBoxLayout();
    topKitchen->addWidget(btnBackKitchen);
    topKitchen->addStretch();

    // 📦 центр
    QWidget *centerKitchen = new QWidget();
    centerKitchen->setFixedWidth(400);

    QVBoxLayout *centerKitchenLayout = new QVBoxLayout(centerKitchen);
    centerKitchenLayout->setSpacing(12);
    for (auto &text : kitchenItems) {
        QPushButton *btn = new QPushButton();

        btn->setMinimumHeight(50);

        // 🔴 робимо свій layout всередині кнопки
        QHBoxLayout *layout = new QHBoxLayout(btn);
        layout->setContentsMargins(15, 0, 15, 0);

        // текст
        QLabel *label = new QLabel(text);
        label->setStyleSheet("color: white; font-size: 16px; font-weight: 600;");

        // стрілка
        QLabel *arrow = new QLabel();
        arrow->setPixmap(QPixmap("C:/QtProjects/EatAndChill/resources/forward.png").scaled(18,18, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//шлях до іконки

        // розкладка
        layout->addWidget(label);
        layout->addStretch();   // 🔥 це прижимає стрілку вправо
        layout->addWidget(arrow);

        // стиль кнопки
        btn->setStyleSheet(R"(
QPushButton {
    background: #2b2b2b;
    border-radius: 14px;
}
QPushButton:hover {
    background: #3a3a3a;
}
QPushButton:pressed {
    background: #ff3b3b;
}
)");
        btn->setStyleSheet(categoryStyle);

        // ✅ СТИЛЬ
        btn->setStyleSheet(categoryStyle);

        // ✅ ТІНЬ
        auto shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(20);
        shadow->setOffset(0,4);
        shadow->setColor(QColor(0,0,0,140));
        btn->setGraphicsEffect(shadow);

        connect(btn, &QPushButton::clicked, this, [=]() {
            ui->stackedWidget_menu->setCurrentWidget(ui->page_products_2);
            currentCategory = text;
            loadProducts();
        });

        centerKitchenLayout->addWidget(btn);
    }


    // 🧠 збірка
    kitchenLayout->addLayout(topKitchen);
    kitchenLayout->addStretch();
    kitchenLayout->addWidget(centerKitchen, 0, Qt::AlignHCenter);
    kitchenLayout->addStretch();

    if (ui->page_kitchen->layout()) {
        delete ui->page_kitchen->layout();
    }
    ui->page_kitchen->setLayout(kitchenLayout);
    setupMenuLayout();
    setupLoginLayout();
    btnAdmin->hide();

    loadMenuFromDB();
    //loadProducts();

    ui->stackedWidget_menu->setCurrentWidget(ui->page_products_2);

    QFile file(":/resources/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString style = QLatin1String(file.readAll());
        this->setStyleSheet(style);
    }
    ui->btn_to_history->setIcon(QIcon(":/resources/history.png"));
    ui->btn_to_history->setIconSize(QSize(24, 24));
    ui->btn_to_history->setText(" Історія");

    ui->btn_to_cart->setIcon(QIcon(":/resources/cart.png"));
    ui->btn_to_cart->setIconSize(QSize(24, 24));
    ui->btn_to_cart->setText(" Кошик");

    // КУХНЯ
    auto shadowKitchen = new QGraphicsDropShadowEffect(this);
    shadowKitchen->setBlurRadius(20);
    shadowKitchen->setOffset(0, 5);
    shadowKitchen->setColor(QColor(0, 0, 0, 160));
    ui->btn_kitchen->setGraphicsEffect(shadowKitchen);

    // БАР
    auto shadowBar = new QGraphicsDropShadowEffect(this);
    shadowBar->setBlurRadius(20);
    shadowBar->setOffset(0, 5);
    shadowBar->setColor(QColor(0, 0, 0, 160));
    ui->btn_bar->setGraphicsEffect(shadowBar);

    // КОШИК
    auto shadowCart = new QGraphicsDropShadowEffect(this);
    shadowCart->setBlurRadius(25);
    shadowCart->setOffset(0, 6);
    shadowCart->setColor(QColor(0, 0, 0, 180));
    ui->btn_to_cart->setGraphicsEffect(shadowCart);

    ui->btn_to_cart->setObjectName("cartButton");

    if (ui->page_history->layout()) {
        delete ui->page_history->layout();
    }

    // 🔙 кнопка назад
    QPushButton *btnBackHistory = new QPushButton("← Назад");
    btnBackHistory->setFixedSize(160, 45);
    btnBackHistory->setObjectName("cartButton");
    btnBackHistory->setStyleSheet(R"(
QPushButton {
    background: #3a3a3a;
    color: white;
    border-radius: 12px;
    padding: 10px 16px;
}
QPushButton:hover {
    background: #4a4a4a;
}
QPushButton:pressed {
    background: #ff3b3b;
}
)");

    auto shadowBackHistory = new QGraphicsDropShadowEffect(this);
    shadowBackHistory->setBlurRadius(25);
    shadowBackHistory->setOffset(0,6);
    shadowBackHistory->setColor(QColor(0,0,0,180));
    btnBackHistory->setGraphicsEffect(shadowBackHistory);

    connect(btnBackHistory, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_menu);
    });

    // Список історії
    historyList = new QListWidget();
    historyList->setFixedWidth(400);

    // Кнопка виконано (статус замовлення)
    QPushButton *btnDone = new QPushButton("✔ Виконано");
    btnDone->setFixedSize(180, 45);
    connect(btnDone, &QPushButton::clicked, this, [=]() {

        QListWidgetItem *item = historyList->currentItem();

        if (!item) {
            QMessageBox::warning(this, "Помилка", "Оберіть замовлення!");
            return;
        }

        QString text = item->text();
        int orderId = item->data(Qt::UserRole).toInt();

        // якщо вже виконано — не чіпаємо
        if (text.contains("Виконано")) return;

        // Змінюємо статус
        text.replace("Очікує", "Виконано");
        item->setText(text);

        QSqlQuery query;

        query.prepare("UPDATE orders SET status='Виконано' WHERE id=?");
        query.addBindValue(orderId);

        query.exec();

        updateStats();

    });
    totalOrders = 0;
    totalRevenue = 0;
    // Статистика
    statsLabel = new QLabel("📊 Статистика за день:\nЗамовлень: 0\nПрибуток: 0 грн");
    statsLabel->setObjectName("statsBox");
    statsLabel->setAlignment(Qt::AlignLeft);

    // Верху (назад)
    QHBoxLayout *topHistoryLayout = new QHBoxLayout();
    topHistoryLayout->setContentsMargins(20, 20, 20, 0);
    topHistoryLayout->addWidget(btnBackHistory);
    topHistoryLayout->addStretch();

    // Середина (список + кнопка справа)
    QHBoxLayout *middleLayout = new QHBoxLayout();
    middleLayout->addStretch();
    middleLayout->addWidget(historyList);

    // Кнопка справа по центру
    QVBoxLayout *rightSide = new QVBoxLayout();
    rightSide->addStretch();
    rightSide->addWidget(btnDone);
    rightSide->addStretch();

    middleLayout->addLayout(rightSide);
    middleLayout->addStretch();

    // Внизу (статистика)
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(statsLabel);
    bottomLayout->addStretch();

    // Головний layout
    QVBoxLayout *historyLayout = new QVBoxLayout();
    historyLayout->addLayout(topHistoryLayout);
    historyLayout->addStretch();
    historyLayout->addLayout(middleLayout);
    historyLayout->addSpacing(20);
    historyLayout->addLayout(bottomLayout);
    historyLayout->addStretch();

    btnDone->setObjectName("cartButton");
    auto shadowDone = new QGraphicsDropShadowEffect(this);
    shadowDone->setBlurRadius(25);
    shadowDone->setOffset(0, 6);
    shadowDone->setColor(QColor(0, 0, 0, 180));

    btnDone->setGraphicsEffect(shadowDone);
    ui->page_history->setLayout(historyLayout);
    // ---------------- ІНІЦІАЛІЗАЦІЯ ----------------

    orderCounter = 1;

    ui->stackedWidget_menu->setCurrentWidget(ui->page_menu_main);
    ui->stackedWidget->setCurrentWidget(ui->page_login);

    ui->input_price->setValidator(new QIntValidator(0, 10000, this));

    // ---------------- Система входу (логін) ----------------

    connect(ui->btn_login, &QPushButton::clicked, this, [=]() {

        QString login = ui->input_login->text();
        QString password = ui->input_password->text();

        // Адмін
        if(login == "admin777" && password == "152634") {

            ui->btn_to_history->show();
            btnAdmin->show();

            ui->stackedWidget->setCurrentWidget(ui->page_menu);
        }

        // Гість (пусті поля)
        else if(login.isEmpty() && password.isEmpty()) {

            ui->btn_to_history->hide();
            btnAdmin->hide();

            ui->stackedWidget->setCurrentWidget(ui->page_menu);
        }

        // Неправильний логін
        else {

            QMessageBox::warning(
                this,
                "Помилка входу",
                "Неправильний логін або пароль!"
                );

        }
    });


    // ---------------- НАВІГАЦІЯ ----------------

    // Перехід до кошика
    connect(ui->btn_to_cart, &QPushButton::clicked, this, [=]() {
        updateCartTable();
        ui->stackedWidget->setCurrentWidget(ui->page_cart);
    });

    // Вихід з адмін-панелі
    connect(ui->btn_back_from_admin, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_menu);
    });

    // Перехід до історії
    connect(ui->btn_to_history, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_history);
    });

    // Назад в категорії
    connect(ui->btn_back_to_categories, &QPushButton::clicked, this, [=]() {

        if(currentCategoryGroup == "kitchen") {
            ui->stackedWidget_menu->setCurrentWidget(ui->page_kitchen);
        }
        else if(currentCategoryGroup == "bar") {
            ui->stackedWidget_menu->setCurrentWidget(ui->page_bar);
        }
    });

    // До кухні
    connect(ui->btn_kitchen, &QPushButton::clicked, this, [=]() {
        currentCategoryGroup = "kitchen";
        ui->stackedWidget_menu->setCurrentWidget(ui->page_kitchen);
    });

    // До бару
    connect(ui->btn_bar, &QPushButton::clicked, this, [=]() {
        currentCategoryGroup = "bar";
        ui->stackedWidget_menu->setCurrentWidget(ui->page_bar);
    });

    // Кнопка "Адмін"
    connect(btnAdmin, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_admin);
    });


    // ---------------- АДМІН ПАНЕЛЬ ----------------
    if (ui->page_admin->layout()) {
        delete ui->page_admin->layout();
    }
    QList<QWidget*> children = ui->page_admin->findChildren<QWidget*>();
    for (auto child : std::as_const(children)) {
        if (child != ui->page_admin) {
            child->setParent(nullptr);
        }
    }
    QPushButton *btnBackAdmin = new QPushButton("← Назад");
    btnBackAdmin->setFixedSize(160, 45);
    btnBackAdmin->setObjectName("cartButton");

    auto shadowBackAdmin = new QGraphicsDropShadowEffect(this);
    shadowBackAdmin->setBlurRadius(25);
    shadowBackAdmin->setOffset(0, 6);
    shadowBackAdmin->setColor(QColor(0, 0, 0, 180));
    btnBackAdmin->setGraphicsEffect(shadowBackAdmin);

    connect(btnBackAdmin, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_menu);
    });
    QLineEdit *inputName = new QLineEdit();
    inputName->setPlaceholderText("Додати страву");

    QLineEdit *inputPrice = new QLineEdit();
    inputPrice->setPlaceholderText("Ціна страви");

    QComboBox *comboCategory = new QComboBox();
    comboCategory->addItems({
        "Піца",
        "Закуски",
        "Салати",
        "Гарніри",
        "Десерти",
        "Гарячі напої",
        "Газовані напої",
        "Коктейлі",
        "Соки",
        "Алкоголь"
    });

    // Поле + кнопка фото
    QLineEdit *inputImage = new QLineEdit();
    inputImage->setPlaceholderText("Шлях до фото");

    QPushButton *btnBrowse = new QPushButton("📁");

    connect(btnBrowse, &QPushButton::clicked, this, [=]() {
        QString path = QFileDialog::getOpenFileName(this, "Обрати фото");
        if(!path.isEmpty()) {
            inputImage->setText(path);
        }
    });

    // Рядок категорія + фото
    QHBoxLayout *rowCategoryImage = new QHBoxLayout();
    rowCategoryImage->addWidget(comboCategory);
    rowCategoryImage->addWidget(inputImage);
    rowCategoryImage->addWidget(btnBrowse);

    // Кнопка додати
    QPushButton *btnAdd = new QPushButton("Додати страву");
    btnAdd->setObjectName("cartButton");

    // Пошук
    QLineEdit *inputSearch = new QLineEdit();
    inputSearch->setPlaceholderText("Пошук товару");

    // Список меню
    QListWidget *adminMenuList = ui->list_menu;
    adminMenuList->setMinimumHeight(250);

    // Лейаут ліво
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(inputName);
    leftLayout->addWidget(inputPrice);
    leftLayout->addLayout(rowCategoryImage);
    leftLayout->addWidget(btnAdd, 0, Qt::AlignHCenter);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(inputSearch);
    leftLayout->addWidget(adminMenuList);

    QPushButton *btnEdit = new QPushButton("Редагувати страву");
    connect(btnEdit, &QPushButton::clicked, this, [=]() {

        int row = ui->list_menu->currentRow();
        if (row < 0) return;

        QString text = ui->list_menu->item(row)->text();

        // Парсимо текст
        QString name = text.section(" - ", 0, 0);
        QString price = text.section(" - ", 1, 1).section(" грн", 0, 0);
        QString category = text.section("[", 1, 1).section("]", 0, 0);
        QString image = text.contains("| img: ") ? text.section("| img: ", 1, 1) : "";

        // Вставляємо в поля
        inputName->setText(name);
        inputPrice->setText(price);
        comboCategory->setCurrentText(category);
        inputImage->setText(image);

        // Ррежим редагування
        isEditing = true;
        editingRow = row;
    });
    QPushButton *btnDelete = new QPushButton("Видалити страву");
    connect(btnDelete, &QPushButton::clicked, this, [=]() {

        int row = ui->list_menu->currentRow();
        if (row < 0) return;

        QString text = ui->list_menu->item(row)->text();

        QString name = text.section(" - ", 0, 0);
        int price = text.section(" - ", 1, 1).section(" грн", 0, 0).toInt();

        QSqlQuery query;
        query.prepare("DELETE FROM menu WHERE name = ? AND price = ?");
        query.addBindValue(name);
        query.addBindValue(price);

        if(!query.exec()) {
            qDebug() << "Delete error:" << query.lastError().text();
        }

        // Оновлення UI
        loadMenuFromDB();

        QTimer::singleShot(50, this, [=]() {
            loadProducts();
        });
    });

    QListWidget *stopList = ui->list_stoplist;
    QPushButton *btnStop = new QPushButton("Додати в стоп-лист");
    btnStop->setIcon(QIcon(":/resources/block.png"));
    btnStop->setIconSize(QSize(20,20));

    QPushButton *btnRemoveStop = new QPushButton("Видалити зі стоп-листу");
    btnRemoveStop->setIconSize(QSize(20,20));

    // Додати в стоп-лист
    connect(btnStop, &QPushButton::clicked, this, [=]() {

        int row = ui->list_menu->currentRow();
        if (row < 0) return;

        QString item = ui->list_menu->item(row)->text();

        // перевірка щоб не дублювалось
        for(int i = 0; i < stopList->count(); i++) {
            if(stopList->item(i)->text() == item) return;
        }

        stopList->addItem(item);
    });

    //Видалити зі стоп-листу
    connect(btnRemoveStop, &QPushButton::clicked, this, [=]() {

        QListWidgetItem *currentItem = ui->list_menu->currentItem();

        if(!currentItem) return;

        QString itemText = currentItem->text();

        for(int i = 0; i < stopList->count(); i++) {

            if(stopList->item(i)->text() == itemText) {

                delete stopList->takeItem(i);

                break;
            }
        }

        setupMenuLayout();

    });

    btnEdit->setObjectName("cartButton");
    btnDelete->setObjectName("cartButton");
    btnStop->setObjectName("cartButton");
    btnRemoveStop->setObjectName("cartButton");

    QString adminBtnStyle = R"(
        QPushButton {
            background: #ff3b3b;
            color: white;
            border-radius: 12px;
            padding: 10px 16px;
            font-weight: 600;
        }
        QPushButton:hover {
            background: #ff5c5c;
        }
        QPushButton:pressed {
            background: #cc2f2f;
        }
    )";

    btnEdit->setStyleSheet(adminBtnStyle);
    btnDelete->setStyleSheet(adminBtnStyle);
    btnAdd->setStyleSheet(adminBtnStyle);
    btnStop->setStyleSheet(adminBtnStyle);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(btnEdit);
    rightLayout->addWidget(btnDelete);
    rightLayout->addSpacing(20);
    rightLayout->addWidget(stopList);
    rightLayout->addWidget(btnStop);
    rightLayout->addWidget(btnRemoveStop);
    rightLayout->addStretch();
    QHBoxLayout *mainContent = new QHBoxLayout();
    mainContent->addLayout(leftLayout, 2);
    mainContent->addLayout(rightLayout, 1);

    QHBoxLayout *topAdminLayout = new QHBoxLayout();
    topAdminLayout->addWidget(btnBackAdmin);
    topAdminLayout->addStretch();

    QVBoxLayout *adminLayout = new QVBoxLayout();
    adminLayout->addLayout(topAdminLayout);
    adminLayout->addSpacing(20);
    adminLayout->addLayout(mainContent);

    ui->page_admin->setLayout(adminLayout);

    connect(btnAdd, &QPushButton::clicked, this, [=]() {

        QString name = inputName->text();
        QString price = inputPrice->text();
        QString category = comboCategory->currentText();
        QString image = inputImage->text();

        if(name.isEmpty() || price.isEmpty()) {
            QMessageBox::warning(this, "Помилка", "Заповніть поля!");
            return;
        }
        Product *product = nullptr;

        // Піцца
        if(category == "Піца") {

            product = new Pizza(name, price.toInt(), 30);

        }
        // Напої
        else if(category.contains("напої") ||
                 category == "Коктейлі" ||
                 category == "Соки" ||
                 category == "Алкоголь") {

            product = new Drink(name, price.toInt(), true);

        }else {
            product = new Product(name, price.toInt());
        }

        // Поліморфізм
        product->setPrice(price.toDouble());
        qDebug() << product->getInfo();
        int maxPrice = getMax(price.toInt(), 200);

        Product testProduct("Тест", 200);

        //  SQL ЗАПИТ
        QSqlQuery query;

        if(isEditing) {

            QString oldText = ui->list_menu->item(editingRow)->text();
            QString oldName = oldText.section(" - ", 0, 0);

            query.prepare("UPDATE menu SET name=?, price=?, category=?, image=? WHERE name=?");

            query.addBindValue(name);
            query.addBindValue(price.toInt());
            query.addBindValue(category);
            query.addBindValue(image);
            query.addBindValue(oldName);

            isEditing = false;

        } else {

            // 🔥 INSERT
            query.prepare("INSERT INTO menu (name, price, category, image) VALUES (?, ?, ?, ?)");

            query.addBindValue(name);
            query.addBindValue(price.toInt());
            query.addBindValue(category);
            query.addBindValue(image);
        }

        query.exec();

        loadMenuFromDB();

        QTimer::singleShot(50, this, [=]() {
            loadProducts();
        });
    });
    // Додавання товару
    if (ui->page_products_2->layout()) {
        delete ui->page_products_2->layout();
    }
    QList<QWidget*> children2 = ui->page_products_2->findChildren<QWidget*>();
    for (auto child : std::as_const(children2)) {
        if (child != ui->page_products_2) {
            child->setParent(nullptr);
        }
    }
    QPushButton *btnBackProducts = new QPushButton("← Назад");

    auto shadowBackProducts = new QGraphicsDropShadowEffect(this);
    shadowBackProducts->setBlurRadius(25);
    shadowBackProducts->setOffset(0, 5);
    shadowBackProducts->setColor(QColor(0,0,0,150));
    btnBackProducts->setGraphicsEffect(shadowBackProducts);

    btnBackProducts->setFixedSize(140, 45);
    btnBackProducts->setObjectName("");

    connect(btnBackProducts, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_menu->setCurrentWidget(ui->page_menu_main);
    });

    btnCartTop = new QPushButton("Кошик (0)");
    btnCartTop->setText("Кошик (" + QString::number(ui->list_cart->count()) + ")");
    btnCartTop->setFixedSize(140, 45);
    btnCartTop->setObjectName("cartButton");

    btnCartTop->setStyleSheet(R"(
        QPushButton {
            background: #ff3b3b;
            color: white;
            border-radius: 14px;
            padding: 10px 18px;
            font-weight: 600;
        }
        QPushButton:hover {
            background: #ff5c5c;
        }
        QPushButton:pressed {
            background: #cc2f2f;
        }
    )");

    auto shadowCartMain = new QGraphicsDropShadowEffect(this);
    shadowCartMain->setBlurRadius(25);
    shadowCartMain->setOffset(0, 5);
    shadowCartMain->setColor(QColor(0,0,0,150));

    btnCartTop->setGraphicsEffect(shadowCartMain);

    connect(btnCartTop, &QPushButton::clicked, this, [=]() {
        updateCartTable();
        ui->stackedWidget->setCurrentWidget(ui->page_cart);
    });

    btnCartTop->setText("Кошик (" + QString::number(ui->list_cart->count()) + ")");

    btnBackProducts->setStyleSheet(R"(
        QPushButton {
            background: #3a3a3a;
            color: white;
            border-radius: 12px;
            padding: 10px 16px;
        }
        QPushButton:hover {
            background: #4a4a4a;
        }
        QPushButton:pressed {
            background: #ff3b3b;
        }
    )");

    auto shadowCartTop = new QGraphicsDropShadowEffect(this);
    shadowCartTop->setBlurRadius(25);
    shadowCartTop->setOffset(0, 5);
    shadowCartTop->setColor(QColor(0,0,0,150));

    btnCartTop->setGraphicsEffect(shadowCartTop);

    inputSearch->setStyleSheet(R"(
        QLineEdit {
            background: #2b2b2b;
            color: white;
            border-radius: 12px;
            padding: 10px;
            border: 1px solid #444;
        }
    )");

    // ================= Верхня панель сторінки товарів =================
    // Пошук товарів
    QGraphicsDropShadowEffect *shadowInput = new QGraphicsDropShadowEffect(this);
    shadowInput->setBlurRadius(20);
    shadowInput->setOffset(0, 3);
    shadowInput->setColor(QColor(0,0,0,120));
    inputSearch->setGraphicsEffect(shadowInput);

    QHBoxLayout *topProductsLayout = new QHBoxLayout();
    topProductsLayout->addWidget(btnBackProducts);
    topProductsLayout->addStretch();
    topProductsLayout->addWidget(btnCartTop);

    inputSearchProducts->setPlaceholderText("Знайти страву:");
    inputSearchProducts->setFixedWidth(300);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addStretch();
    searchLayout->addWidget(inputSearchProducts);
    searchLayout->addStretch();

    // Сортування товарів
    QPushButton *btnSortPrice = new QPushButton("Сортувати по ціні ↑");
    btnSortPrice->setObjectName("cartButton");

    QGraphicsDropShadowEffect *shadowSort = new QGraphicsDropShadowEffect(this);
    shadowSort->setBlurRadius(25);
    shadowSort->setOffset(0, 5);
    shadowSort->setColor(QColor(0,0,0,150));
    btnSortPrice->setGraphicsEffect(shadowSort);

    QHBoxLayout *sortLayout = new QHBoxLayout();
    sortLayout->addStretch();
    sortLayout->addWidget(btnSortPrice);
    sortLayout->addStretch();

    // Область відображення товарів
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget *scrollContent = new QWidget();
    productsGrid = new QGridLayout(scrollContent);
    productsGrid->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    productsGrid->setHorizontalSpacing(12);
    productsGrid->setVerticalSpacing(16);
    productsGrid->setContentsMargins(20, 20, 20, 20);

    scrollArea->setWidget(scrollContent);

    // ================= Пошук та сортування =================
    connect(inputSearchProducts, &QLineEdit::textChanged, this, [=](const QString &) {
        loadProducts();
    });

    connect(inputSearch, &QLineEdit::textChanged, this, [=](const QString &text){
        for(int i = 0; i < adminMenuList->count(); i++) {
            QListWidgetItem *item = adminMenuList->item(i);
            item->setHidden(!item->text().toLower().contains(text.toLower()));
        }
    });

    connect(btnSortPrice, &QPushButton::clicked, this, [=]() {

        QList<QPair<QString, int>> items;

        // збираємо товари поточної категорії
        for(int i = 0; i < ui->list_menu->count(); i++) {

            QString text = ui->list_menu->item(i)->text();

            if(!text.contains("[" + currentCategory + "]"))
                continue;

            int price = text.section(" - ", 1, 1)
                            .section(" грн", 0, 0)
                            .toInt();

            items.append({text, price});
        }

        // сортування по ціні
        std::sort(items.begin(), items.end(),
                  [](auto a, auto b) {
                      return a.second < b.second;
                  });

        // очищення старого списку
        ui->list_menu->clear();

        // додаємо назад у відсортованому порядку
        for(auto &pair : items) {
            ui->list_menu->addItem(pair.first);
        }

        // заново будуємо красиві карточки
        loadProducts();
    });

    QVBoxLayout *productsLayout = new QVBoxLayout();
    productsLayout->addLayout(topProductsLayout);
    productsLayout->addLayout(searchLayout);
    productsLayout->addLayout(sortLayout);
    productsLayout->addWidget(scrollArea);

    ui->page_products_2->setLayout(productsLayout);


    // ---------------- Кошик ----------------
    if (ui->page_cart->layout()) {
        delete ui->page_cart->layout();
    }
    // Кнопка назад
    QPushButton *btnBackCart = new QPushButton("← Назад");
    btnBackCart->setFixedSize(160, 45);
    btnBackCart->setObjectName("cartButton");

    auto shadowBackCart = new QGraphicsDropShadowEffect(this);
    shadowBackCart->setBlurRadius(25);
    shadowBackCart->setOffset(0, 6);
    shadowBackCart->setColor(QColor(0, 0, 0, 180));
    btnBackCart->setGraphicsEffect(shadowBackCart);

    connect(btnBackCart, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_menu);
    });

    // Заголовок
    QLabel *titleCart = new QLabel("Ваше замовлення:");
    titleCart->setAlignment(Qt::AlignCenter);

    titleCart->setStyleSheet(R"(
    QLabel {
        font-size: 24px;
        font-weight: 700;
        color: #ffffff;
        letter-spacing: 1px;
        padding: 12px;
    }
)");

    // Таблиця
    cartTable = new QTableWidget();
    cartTable->setColumnCount(4);
    cartTable->setHorizontalHeaderLabels({"Страва", "Ціна", "Кількість", "Сума"});

    // Вигляд таблиці
    cartTable->setFixedWidth(500);
    cartTable->verticalHeader()->setVisible(false);
    cartTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    cartTable->setSelectionMode(QAbstractItemView::SingleSelection);
    cartTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    cartTable->horizontalHeader()->setStretchLastSection(true);
    cartTable->setMinimumHeight(200);
    cartTable->horizontalHeader()->setStretchLastSection(true);
    cartTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    cartTable->setShowGrid(false);
    cartTable->setAlternatingRowColors(true);
    cartTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    // Кнопка оформлення
    QPushButton *btnCheckoutNew = new QPushButton("Оформити замовлення");
    btnCheckoutNew->setFixedSize(500, 50);
    btnCheckoutNew->setObjectName("cartButton");

    auto shadowCheckout = new QGraphicsDropShadowEffect(this);
    shadowCheckout->setBlurRadius(25);
    shadowCheckout->setOffset(0, 6);
    shadowCheckout->setColor(QColor(0, 0, 0, 180));
    btnCheckoutNew->setGraphicsEffect(shadowCheckout);

    QHBoxLayout *topCartLayout = new QHBoxLayout();
    topCartLayout->setContentsMargins(20, 20, 20, 0);
    topCartLayout->addWidget(btnBackCart);
    topCartLayout->addStretch();

    // Таблиця
    QVBoxLayout *centerCartLayout = new QVBoxLayout();
    centerCartLayout->addWidget(titleCart);
    centerCartLayout->addSpacing(10);
    centerCartLayout->setAlignment(Qt::AlignHCenter);
    centerCartLayout->addWidget(cartTable);

    QHBoxLayout *bottomCartLayout = new QHBoxLayout();
    bottomCartLayout->addStretch();
    bottomCartLayout->addWidget(btnCheckoutNew);
    bottomCartLayout->addStretch();

    // Головний layout
    QPushButton *btnPlus = new QPushButton("+");
    QPushButton *btnMinus = new QPushButton("-");

    btnPlus->setFixedSize(50, 40);
    btnMinus->setFixedSize(50, 40);

    btnPlus->setObjectName("cartButton");
    btnMinus->setObjectName("cartButton");

    QHBoxLayout *countLayout = new QHBoxLayout();
    countLayout->addStretch();
    countLayout->addWidget(btnMinus);
    countLayout->addWidget(btnPlus);
    countLayout->addStretch();

    QVBoxLayout *cartLayout = new QVBoxLayout();
    cartLayout->addLayout(topCartLayout);
    cartLayout->addStretch();
    cartLayout->addLayout(centerCartLayout);

    cartLayout->addLayout(countLayout);

    cartLayout->addSpacing(10);
    cartLayout->addLayout(bottomCartLayout);
    cartLayout->addStretch();

    ui->page_cart->setLayout(cartLayout);

    // Додати к-сть товару
    connect(btnPlus, &QPushButton::clicked, this, [=]() {

        int row = cartTable->currentRow();
        if (row < 0) return;

        if (row >= ui->list_cart->count()) return; // не чіпаємо "загальну суму"

        QListWidgetItem *item = ui->list_cart->item(row);

        QString text = item->text();

        int count = text.contains("x") ? text.split("x").last().toInt() : 1;
        count++;

        QString base = text.split(" x").first();
        item->setText(base + " x" + QString::number(count));

        updateCartTable();
    });

    // Відняти к-сть товару
    connect(btnMinus, &QPushButton::clicked, this, [=]() {

        int row = cartTable->currentRow();
        if (row < 0) return;

        if (row >= ui->list_cart->count()) return;

        QListWidgetItem *item = ui->list_cart->item(row);

        QString text = item->text();

        int count = text.contains("x") ? text.split("x").last().toInt() : 1;

        if (count > 1) {
            count--;
            QString base = text.split(" x").first();
            item->setText(base + " x" + QString::number(count));
        } else {
            delete item;
        }

        updateCartTable();
    });

    // Додавання в кошик
    connect(ui->btn_add_to_cart, &QPushButton::clicked, this, [=]() {

        QListWidgetItem *item = ui->list_menu->currentItem();
        if(!item) {
            QMessageBox::warning(this, "Помилка", "Оберіть товар!");
            return;
        }

        QString newItem = item->text();

        bool found = false;

        for(int i = 0; i < ui->list_cart->count(); i++) {
            QString existing = ui->list_cart->item(i)->text();

            QString search = inputSearchProducts->text().toLower();

            if(!search.isEmpty() && !newItem.toLower().contains(search)) {
                continue;
            }

            if(existing.startsWith(newItem)) {

                int count = existing.contains("x") ?
                                existing.split("x").last().toInt() : 1;

                count++;

                ui->list_cart->item(i)->setText(newItem + " x" + QString::number(count));
                found = true;
                break;
            }
        }

        // Перевірка стоп-листа
        for(int i = 0; i < ui->list_stoplist->count(); i++) {
            if(ui->list_stoplist->item(i)->text() == newItem) {

                QMessageBox::warning(this,
                                     "Недоступно",
                                     "Вибачте! Дана страва наразі недоступна для замовлення");

                return;
            }
        }

        updateTotal();
        updateCartCount();
        updateCartTable();
    });

    // Видалити товар
    connect(ui->btn_remove_item, &QPushButton::clicked, this, [=]() {

        QListWidgetItem *item = ui->list_cart->currentItem();
        if(!item) return;

        delete item;
        updateTotal();
        updateCartCount();
        updateCartTable();
    });

    // ---------------- Оформлення замовлення ----------------

    connect(btnCheckoutNew, &QPushButton::clicked, this, [=]() {

        if(ui->list_cart->count() == 0) return;

        int total = 0;

        QSqlQuery query;

        QString datetime = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");

        // Створюємо замовлення
        query.prepare("INSERT INTO orders (datetime, total, status) VALUES (?, ?, ?)");
        query.addBindValue(datetime);
        query.addBindValue(0);
        query.addBindValue("Очікує");

        if(!query.exec()) {
            qDebug() << "ORDER INSERT ERROR:" << query.lastError().text();
            return;
        }

        int orderId = query.lastInsertId().toInt();

        // Товари
        for(int i = 0; i < ui->list_cart->count(); i++) {

            QString text = ui->list_cart->item(i)->text();

            QString name = text.section(" - ", 0, 0);

            int price = text.section(" - ", 1, 1)
                            .section(" грн", 0, 0)
                            .toInt();

            int count = text.section(" x", 1, 1).toInt();

            total += price * count;

            QSqlQuery itemQuery;

            itemQuery.prepare("INSERT INTO order_items (order_id, name, price, count) "
                              "VALUES (?, ?, ?, ?)");

            itemQuery.addBindValue(orderId);
            itemQuery.addBindValue(name);
            itemQuery.addBindValue(price);
            itemQuery.addBindValue(count);

            if(!itemQuery.exec()) {
                qDebug() << "ITEM INSERT ERROR:" << itemQuery.lastError().text();
            }
        }

        // Оновлюємо суму
        QSqlQuery updateQuery;

        updateQuery.prepare("UPDATE orders SET total=? WHERE id=?");
        updateQuery.addBindValue(total);
        updateQuery.addBindValue(orderId);


        if(!updateQuery.exec()) {
            qDebug() << "UPDATE ERROR:" << updateQuery.lastError().text();
        }

        // UI
        QString order = "#" + QString::number(orderCounter++) + " (Очікує)\n\n";

        for(int i = 0; i < ui->list_cart->count(); i++) {
            order += ui->list_cart->item(i)->text() + "\n";
        }

        order += "\nРазом: " + QString::number(total) + " грн";

        QListWidgetItem *historyItem = new QListWidgetItem(order);

        historyItem->setData(Qt::UserRole, orderId);

        historyList->addItem(historyItem);

        ui->list_cart->clear();
        updateTotal();
        updateCartCount();
        updateStats();

        ui->stackedWidget->setCurrentWidget(ui->page_menu);
        ui->stackedWidget_menu->setCurrentWidget(ui->page_menu_main);
    });

    for (auto &text : items) {
        QPushButton *btn = new QPushButton();
        btn->setMinimumHeight(50);
        QHBoxLayout *layout = new QHBoxLayout(btn);
        layout->setContentsMargins(15, 0, 15, 0);
        QLabel *label = new QLabel(text);
        label->setStyleSheet("color: white; font-size: 16px; font-weight: 600;");
        QLabel *arrow = new QLabel();
        arrow->setPixmap(QPixmap(":/resources/forward.png").scaled(18,18, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        layout->addWidget(label);
        layout->addStretch();
        layout->addWidget(arrow);
        btn->setStyleSheet(R"(
            QPushButton {
                background: #2b2b2b;
                border-radius: 14px;
            }
            QPushButton:hover {
                background: #3a3a3a;
            }
            QPushButton:pressed {
                background: #ff3b3b;
            }
        )");

        // Тінь
        auto shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(20);
        shadow->setOffset(0,4);
        shadow->setColor(QColor(0,0,0,140));
        btn->setGraphicsEffect(shadow);

        connect(btn, &QPushButton::clicked, this, [=]() {
            ui->stackedWidget_menu->setCurrentWidget(ui->page_products_2);
            ui->input_search->clear();
            currentCategory = text;
            loadProducts();
        });

        centerLayout->addWidget(btn);
    }

    // ---------------- ПОШУК І СОРТУВАННЯ ----------------

    // Пошук
    connect(ui->input_search, &QLineEdit::textChanged, this, [=](const QString &text) {

        for(int i = 0; i < ui->list_menu->count(); i++) {

            QListWidgetItem *item = ui->list_menu->item(i);

            item->setHidden(!item->text().toLower().contains(text.toLower()));
        }
    });

    // Сортування по назві
    connect(ui->btn_sort_name, &QPushButton::clicked, this, [=]() {
        ui->list_menu->sortItems(Qt::AscendingOrder);
    });

    // Сортування по ціні
    connect(ui->btn_sort_price, &QPushButton::clicked, this, [=]() {

        QList<QPair<QString, int>> visibleItems;

        // Беремо тільки поточну категорію
        for(int i = 0; i < ui->list_menu->count(); i++) {

            QListWidgetItem *item = ui->list_menu->item(i);

            if(!item->isHidden()) {

                QString text = item->text();
                int price = text.split(" - ").last().split(" ").first().toInt();

                visibleItems.append({text, price});
            }
        }

        // Сортуємо
        std::sort(visibleItems.begin(), visibleItems.end(),
                  [](auto a, auto b) {
                      return a.second < b.second;
                  });

        // Ховаємо все решту
        for(int i = 0; i < ui->list_menu->count(); i++) {
            ui->list_menu->item(i)->setHidden(true);
        }

        // Показуємо тільки відсортовану категорію
        for(auto &pair : visibleItems) {

            for(int i = 0; i < ui->list_menu->count(); i++) {

                if(ui->list_menu->item(i)->text() == pair.first) {
                    ui->list_menu->item(i)->setHidden(false);
                    break;
                }
            }
        }
    });
}



// ---------------- ДОПОМІЖНІ ФУНКЦІЇ ----------------

// ================= Головне меню =================
void MainWindow::setupMenuLayout()
{
    if (ui->page_menu_main->layout()) {
        delete ui->page_menu_main->layout();
    }

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(40, 20, 40, 60);
    mainLayout->setSpacing(20);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QPushButton *btnLogout = new QPushButton("← Назад");

    btnLogout->setFixedSize(140, 45);
    btnLogout->setObjectName("cartButton");

    auto shadowLogout = new QGraphicsDropShadowEffect(this);
    shadowLogout->setBlurRadius(25);
    shadowLogout->setOffset(0, 6);
    shadowLogout->setColor(QColor(0, 0, 0, 180));

    btnLogout->setGraphicsEffect(shadowLogout);
    connect(btnLogout, &QPushButton::clicked, this, [=]() {

        ui->input_login->clear();
        ui->input_password->clear();

        ui->stackedWidget->setCurrentWidget(ui->page_login);
    });

    btnAdmin = new QPushButton("Адмін");
    btnAdmin->setFixedSize(140, 45);
    btnAdmin->setObjectName("cartButton");

    // Тінь
    auto shadowAdmin = new QGraphicsDropShadowEffect(this);
    shadowAdmin->setBlurRadius(25);
    shadowAdmin->setOffset(0, 6);
    shadowAdmin->setColor(QColor(0, 0, 0, 180));
    btnAdmin->setGraphicsEffect(shadowAdmin);

    topLayout->addWidget(btnLogout);
    topLayout->addWidget(btnAdmin);

    topLayout->addStretch();

    QHBoxLayout *rightButtons = new QHBoxLayout();
    rightButtons->setSpacing(10);

    rightButtons->addWidget(ui->btn_to_history);
    rightButtons->addWidget(ui->btn_to_cart);

    topLayout->addLayout(rightButtons);

    QVBoxLayout *centerLayout = new QVBoxLayout();

    QHBoxLayout *kitchenLayout = new QHBoxLayout();
    kitchenLayout->addStretch();
    kitchenLayout->addWidget(ui->btn_kitchen);
    kitchenLayout->addStretch();

    QHBoxLayout *barLayout = new QHBoxLayout();
    barLayout->addStretch();
    barLayout->addWidget(ui->btn_bar);
    barLayout->addStretch();

    centerLayout->addStretch();
    centerLayout->addLayout(kitchenLayout);
    centerLayout->addSpacing(20);
    centerLayout->addLayout(barLayout);
    centerLayout->addStretch();

    mainLayout->addLayout(topLayout);
    QGraphicsDropShadowEffect *shadow1 = new QGraphicsDropShadowEffect(this);
    shadow1->setBlurRadius(25);
    shadow1->setOffset(0, 5);
    shadow1->setColor(QColor(0, 0, 0, 180));
    ui->btn_kitchen->setGraphicsEffect(shadow1);

    QGraphicsDropShadowEffect *shadow2 = new QGraphicsDropShadowEffect(this);
    shadow2->setBlurRadius(25);
    shadow2->setOffset(0, 5);
    shadow2->setColor(QColor(0, 0, 0, 180));
    ui->btn_bar->setGraphicsEffect(shadow2);
    mainLayout->addLayout(centerLayout);

    ui->page_menu_main->setLayout(mainLayout);

    ui->btn_kitchen->setFixedSize(300, 120);
    ui->btn_bar->setFixedSize(300, 120);
}

// ================= Сторінка входу =================
void MainWindow::setupLoginLayout()
{
    if (ui->page_login->layout()) {
        delete ui->page_login->layout();
    }

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Контейнер картки
    QWidget *card = new QWidget();
    card->setObjectName("loginCard");
    card->setStyleSheet("");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(25);        // наскільки розмита тінь
    shadow->setOffset(0, 5);          // вниз трохи
    shadow->setColor(QColor(0, 0, 0, 180));  // чорна напівпрозора

    card->setGraphicsEffect(shadow);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(6, 6, 6, 6);
    cardLayout->setSpacing(5);

    cardLayout->addWidget(ui->label_login_title);
    cardLayout->addWidget(ui->input_login);
    cardLayout->addWidget(ui->input_password);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addStretch();
    topLayout->addWidget(card);

    // Кнопка по центру
    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    centerLayout->addWidget(ui->btn_login);
    QGraphicsDropShadowEffect *btnShadow = new QGraphicsDropShadowEffect(this);
    btnShadow->setBlurRadius(25);
    btnShadow->setOffset(0, 4);
    btnShadow->setColor(QColor(0, 0, 0, 180));

    ui->btn_login->setGraphicsEffect(btnShadow);
    centerLayout->addStretch();

    // Збірка
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();

    // Розміри
    card->setFixedWidth(260);
    ui->btn_login->setFixedWidth(200);

    ui->page_login->setLayout(mainLayout);
}

// ================= Обчислення загальної суми =================
void MainWindow::updateTotal() {

    int total = 0;

    for(int i = 0; i < ui->list_cart->count(); i++) {

        QString text = ui->list_cart->item(i)->text();

        int price = text.split(" - ").last().split(" ").first().toInt();
        int count = text.contains("x") ? text.split("x").last().toInt() : 1;

        total += price * count;
    }

    ui->label_total->setText("Сума: " + QString::number(total) + " грн");
}

// ================= Оновлення к-сті товарів у кошику =================
void MainWindow::updateCartCount() {

    int count = 0;

    for(int i = 0; i < ui->list_cart->count(); i++) {

        QString text = ui->list_cart->item(i)->text();

        int itemCount = text.contains("x") ?
                            text.split("x").last().toInt() : 1;

        count += itemCount;
    }

    ui->btn_to_cart->setText("Кошик (" + QString::number(count) + ")");
}

// ================= Статистика =================
void MainWindow::updateStats()
{
    QSqlQuery query;

    int ordersCount = 0;
    int revenue = 0;

    // Кількість замовлень
    if(query.exec("SELECT COUNT(*) FROM orders WHERE status='Виконано'") && query.next()) {
        ordersCount = query.value(0).toInt();
    }

    // Сума
    if(query.exec("SELECT SUM(total) FROM orders WHERE status='Виконано'") && query.next()) {
        revenue = query.value(0).toInt();
    }

    statsLabel->setText("📊 Статистика за день:\n"
                        "Замовлень: " + QString::number(ordersCount) + "\n"
                                                         "Прибуток: " + QString::number(revenue) + " грн");
}

// ================= ООновлення кошика =================
void MainWindow::updateCartTable()
{
    cartTable->setRowCount(0);

    int total = 0;

    for(int i = 0; i < ui->list_cart->count(); i++) {

        QString text = ui->list_cart->item(i)->text();

        QString name = text.split(" - ").first();
        int price = text.split(" - ").last().split(" ").first().toInt();
        int count = text.contains("x") ? text.split("x").last().toInt() : 1;

        int sum = price * count;
        total += sum;

        int row = cartTable->rowCount();
        cartTable->insertRow(row);

        cartTable->setItem(row, 0, new QTableWidgetItem(name));
        cartTable->setItem(row, 1, new QTableWidgetItem(QString::number(price) + " грн"));
        cartTable->setItem(row, 2, new QTableWidgetItem(QString::number(count)));
        cartTable->setItem(row, 3, new QTableWidgetItem(QString::number(sum) + " грн"));
    }

    // Рядок "Загальна сума"
    int row = cartTable->rowCount();
    cartTable->insertRow(row);

    cartTable->setSpan(row, 0, 1, 3);

    QTableWidgetItem *totalText = new QTableWidgetItem("Загальна сума:");
    totalText->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    cartTable->setItem(row, 0, totalText);
    cartTable->setItem(row, 3, new QTableWidgetItem(QString::number(total) + " грн"));
}

// ================= Завантаження меню з бд =================
void MainWindow::loadMenuFromDB()
{
    ui->list_menu->clear();

    QSqlQuery query;

    query.prepare("SELECT name, price, category, image FROM menu");

    if(!query.exec()) {
        qDebug() << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString name = query.value(0).toString();
        int price = query.value(1).toInt();
        QString category = query.value(2).toString();
        QString image = query.value(3).toString();

        QString item = name + " - " + QString::number(price) +
                       " грн [" + category + "]";

        if(!image.isEmpty()) {
            item += " | img: " + image;
        }

        ui->list_menu->addItem(item);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}