#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include "library.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addBook();
    void removeBook();
    void addUser();
    void removeUser();
    void lendBook();
    void returnBook();
    void searchBook();
    void updateViews();

private:
    void setupUI();
    void setupModels();
    void setupConnections();

    Library library;


    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QTableView *booksTable;
    QTableView *usersTable;
    QPushButton *addBookBtn;
    QPushButton *removeBookBtn;
    QPushButton *addUserBtn;
    QPushButton *removeUserBtn;
    QPushButton *lendBookBtn;
    QPushButton *returnBookBtn;
    QPushButton *searchBookBtn;


    QStandardItemModel *booksModel;
    QStandardItemModel *usersModel;
};

#endif // MAINWINDOW_H
