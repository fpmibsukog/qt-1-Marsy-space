#include "mainwindow.h"
#include "library.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    setupModels();
    setupConnections();
    updateViews();
}

MainWindow::~MainWindow() {
    delete booksModel;
    delete usersModel;
}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    booksTable = new QTableView(this);
    usersTable = new QTableView(this);

    booksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    booksTable->setSelectionMode(QAbstractItemView::SingleSelection);
    usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    usersTable->setSelectionMode(QAbstractItemView::SingleSelection);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addBookBtn = new QPushButton("Add Book", this);
    removeBookBtn = new QPushButton("Remove Book", this);
    addUserBtn = new QPushButton("Add User", this);
    removeUserBtn = new QPushButton("Remove User", this);
    lendBookBtn = new QPushButton("Lend Book", this);
    returnBookBtn = new QPushButton("Return Book", this);
    searchBookBtn = new QPushButton("Search Book", this);

    buttonLayout->addWidget(addBookBtn);
    buttonLayout->addWidget(removeBookBtn);
    buttonLayout->addWidget(addUserBtn);
    buttonLayout->addWidget(removeUserBtn);
    buttonLayout->addWidget(lendBookBtn);
    buttonLayout->addWidget(returnBookBtn);
    buttonLayout->addWidget(searchBookBtn);

    mainLayout->addWidget(booksTable);
    mainLayout->addWidget(usersTable);
    mainLayout->addLayout(buttonLayout);

    setWindowTitle("Library Management System");
    resize(800, 600);
}

void MainWindow::setupModels() {
    booksModel = new QStandardItemModel(this);
    booksModel->setHorizontalHeaderLabels({"Title", "Author", "Year"});
    booksTable->setModel(booksModel);

    usersModel = new QStandardItemModel(this);
    usersModel->setHorizontalHeaderLabels({"ID", "Name", "Borrowed Books"});
    usersTable->setModel(usersModel);

    booksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::setupConnections() {
    connect(addBookBtn, &QPushButton::clicked, this, &MainWindow::addBook);
    connect(removeBookBtn, &QPushButton::clicked, this, &MainWindow::removeBook);
    connect(addUserBtn, &QPushButton::clicked, this, &MainWindow::addUser);
    connect(removeUserBtn, &QPushButton::clicked, this, &MainWindow::removeUser);
    connect(lendBookBtn, &QPushButton::clicked, this, &MainWindow::lendBook);
    connect(returnBookBtn, &QPushButton::clicked, this, &MainWindow::returnBook);
    connect(searchBookBtn, &QPushButton::clicked, this, &MainWindow::searchBook);
}

void MainWindow::addBook() {
    bool ok;
    QString title = QInputDialog::getText(this, "Add Book", "Title:", QLineEdit::Normal, "", &ok);
    if (!ok || title.isEmpty()) return;

    QString author = QInputDialog::getText(this, "Add Book", "Author:", QLineEdit::Normal, "", &ok);
    if (!ok || author.isEmpty()) return;

    int year = QInputDialog::getInt(this, "Add Book", "Year:", 2000, 0, 9999, 1, &ok);
    if (!ok) return;

    auto book = std::make_shared<Book>(title, author, year);
    library.addBook(book);
    updateViews();
}

void MainWindow::removeBook() {
    if (booksTable->selectionModel()->selectedRows().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a book to remove");
        return;
    }

    int row = booksTable->selectionModel()->selectedRows().first().row();
    QString title = booksModel->item(row, 0)->text();

    if (library.removeBook(title)) {
        updateViews();
    } else {
        QMessageBox::warning(this, "Error", "Failed to remove the book");
    }
}

void MainWindow::addUser() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add User", "Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    int id = QInputDialog::getInt(this, "Add User", "ID:", 1, 1, 9999, 1, &ok);
    if (!ok) return;

    auto user = std::make_shared<User>(name, id);
    library.addUser(user);
    updateViews();
}

void MainWindow::removeUser() {
    if (usersTable->selectionModel()->selectedRows().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a user to remove");
        return;
    }

    int row = usersTable->selectionModel()->selectedRows().first().row();
    int id = usersModel->item(row, 0)->text().toInt();

    if (library.removeUser(id)) {
        updateViews();
    } else {
        QMessageBox::warning(this, "Error", "Failed to remove the user");
    }
}

void MainWindow::lendBook() {

    QModelIndexList bookSelection = booksTable->selectionModel()->selectedRows();
    if (bookSelection.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a book to lend");
        return;
    }
    QString bookTitle = booksModel->item(bookSelection.first().row(), 0)->text();

    QModelIndexList userSelection = usersTable->selectionModel()->selectedRows();
    if (userSelection.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a user");
        return;
    }
    int userId = usersModel->item(userSelection.first().row(), 0)->text().toInt();

    if (library.lendBook(bookTitle, userId)) {
        updateViews();
    } else {
        QMessageBox::warning(this, "Error", "Failed to lend the book");
    }
}

void MainWindow::returnBook() {

    QModelIndexList userSelection = usersTable->selectionModel()->selectedRows();
    if (userSelection.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a user");
        return;
    }
    int userId = usersModel->item(userSelection.first().row(), 0)->text().toInt();

    QStringList borrowedBooks;
    auto user = library.findUserById(userId);
    if (user) {
        QString booksInfo = user->getBorrowedBooks();
        if (booksInfo.contains("No books")) {
            QMessageBox::information(this, "Info", "This user has no borrowed books");
            return;
        }

        QStringList lines = booksInfo.split('\n', Qt::SkipEmptyParts);
        for (const QString& line : lines) {
            if (line.startsWith("  - ")) {
                borrowedBooks << line.mid(4).trimmed();
            }
        }
    }

    bool ok;
    QString bookTitle = QInputDialog::getItem(this, "Return Book",
                                              "Select book to return:",
                                              borrowedBooks, 0, false, &ok);
    if (!ok || bookTitle.isEmpty()) return;

    // Выполняем возврат
    if (library.returnBook(bookTitle, userId)) {
        QMessageBox::information(this, "Success", "Book returned successfully");
        updateViews();
    } else {
        QMessageBox::warning(this, "Error", "Failed to return the book");
    }
}

void MainWindow::searchBook() {
    bool ok;
    QString query = QInputDialog::getText(this, "Search Book",
                                          "Enter title or author:",
                                          QLineEdit::Normal, "", &ok);
    if (!ok || query.isEmpty()) return;

    QDialog resultsDialog(this);
    QTableView resultsTable;
    QStandardItemModel resultsModel;

    resultsModel.setHorizontalHeaderLabels({"Title", "Author", "Year", "Status"});

    for (const auto& book : library.getBooks()) {
        if (book->getTitle().contains(query, Qt::CaseInsensitive) ||
            book->getAuthor().contains(query, Qt::CaseInsensitive)) {

            QList<QStandardItem*> rowItems;

            rowItems.append(new QStandardItem(book->getTitle()));

            rowItems.append(new QStandardItem(book->getAuthor()));

            rowItems.append(new QStandardItem(QString::number(book->getYear())));

            QStandardItem* statusItem = new QStandardItem();
            if (book->isBorrowed()) {
                statusItem->setText("Borrowed");
                statusItem->setForeground(QBrush(Qt::red));
            } else {
                statusItem->setText("Available");
                statusItem->setForeground(QBrush(Qt::darkGreen));
            }
            rowItems.append(statusItem);

            resultsModel.appendRow(rowItems);
        }
    }

    if (resultsModel.rowCount() == 0) {
        QMessageBox::information(this, "Search Results", "No books found");
        return;
    }

    resultsTable.setModel(&resultsModel);
    resultsTable.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultsTable.setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable.setSelectionMode(QAbstractItemView::SingleSelection);

    QVBoxLayout layout(&resultsDialog);
    layout.addWidget(&resultsTable);

    QPushButton closeButton("Close", &resultsDialog);
    connect(&closeButton, &QPushButton::clicked, &resultsDialog, &QDialog::accept);
    layout.addWidget(&closeButton);

    resultsDialog.setWindowTitle("Search Results");
    resultsDialog.resize(800, 400);
    resultsDialog.exec();
}
void MainWindow::updateViews() {
    booksModel->removeRows(0, booksModel->rowCount());
    usersModel->removeRows(0, usersModel->rowCount());

    for (const auto& book : library.getBooks()) {
        QList<QStandardItem*> items;
        items.append(new QStandardItem(book->getTitle()));
        items.append(new QStandardItem(book->getAuthor()));
        items.append(new QStandardItem(QString::number(book->getYear())));
        booksModel->appendRow(items);
    }

    for (const auto& user : library.getUsers()) {
        QList<QStandardItem*> items;
        items.append(new QStandardItem(QString::number(user->getId())));
        items.append(new QStandardItem(user->getName()));
        items.append(new QStandardItem(user->getBorrowedBooks()));
        usersModel->appendRow(items);
    }
}

