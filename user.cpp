#include "user.h"
#include <QDebug>

User::User(const QString& name, int id) : name(name), id(id) {}

QString User::getName() const { return name; }
int User::getId() const { return id; }

void User::borrowBook(BookSharedPtr book) {
    if (book) {
        borrowedBooks.push_back(book);
        qDebug() << "User" << name << "borrowed book:" << book->getTitle();
    }
}

void User::returnBook(const QString& title) {
    auto it = std::remove_if(borrowedBooks.begin(), borrowedBooks.end(),
                             [&title](const BookSharedPtr& b) {
                                 return b->getTitle() == title;
                             });

    if (it != borrowedBooks.end()) {
        borrowedBooks.erase(it, borrowedBooks.end());
        qDebug() << "User" << name << "returned book:" << title;
    }
}

QString User::getInfo() const {
    return QString("User: %1 (ID: %2)").arg(name).arg(id);
}

QString User::getBorrowedBooks() const {
    QString result;
    for (const auto& book : borrowedBooks) {
        result += "  - " + book->getTitle() + "\n";
    }
    return result.isEmpty() ? "  No books borrowed\n" : result;
}
