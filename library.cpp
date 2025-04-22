#include "library.h"
#include <QDebug>
#include <algorithm>

void Library::addBook(BookSharedPtr book) {
    if (book) {
        books.push_back(book);
        qDebug() << "Added book:" << book->getTitle();
    }
}

bool Library::removeBook(const QString& title) {
    auto it = std::remove_if(books.begin(), books.end(),
                             [&title](const BookSharedPtr& b) {
                                 return b->getTitle() == title;
                             });

    if (it != books.end()) {
        books.erase(it, books.end());
        qDebug() << "Removed book:" << title;
        return true;
    }
    return false;
}

void Library::listBooks() const {
    qDebug() << "\n=== Library Contents ===";
    for (const auto& book : books) {
        qDebug() << book->getInfo();
    }
}

BookWeakPtr Library::findBookByTitle(const QString& title) const {
    for (const auto& book : books) {
        if (book->getTitle() == title) {
            return book;
        }
    }
    return BookWeakPtr();
}

BookWeakPtr Library::findBookByAuthor(const QString& author) const {
    for (const auto& book : books) {
        if (book->getAuthor() == author) {
            return book;
        }
    }
    return BookWeakPtr();
}

void Library::addUser(UserSharedPtr user) {
    if (user) {
        users.push_back(user);
        qDebug() << "Added user:" << user->getName();
    }
}

bool Library::removeUser(int id) {
    auto it = std::remove_if(users.begin(), users.end(),
                             [id](const UserSharedPtr& u) {
                                 return u->getId() == id;
                             });

    if (it != users.end()) {
        users.erase(it, users.end());
        qDebug() << "Removed user with ID:" << id;
        return true;
    }
    return false;
}

void Library::listUsers() const {
    qDebug() << "\n=== Library Users ===";
    for (const auto& user : users) {
        qDebug() << user->getInfo() << "\nBorrowed books:\n" << user->getBorrowedBooks();
    }
}


bool Library::lendBook(const QString& bookTitle, int userId) {
    auto user = findUserById(userId);
    auto bookPtr = findBookByTitle(bookTitle).lock();

    if (!user || !bookPtr) return false;

    if (bookPtr->isBorrowed()) {
        qDebug() << "Book already borrowed";
        return false;
    }

    bookPtr->setBorrowed(true);
    user->borrowBook(bookPtr);
    return true;
}


bool Library::returnBook(const QString& bookTitle, int userId) {
    auto user = findUserById(userId);
    auto bookPtr = findBookByTitle(bookTitle).lock();

    if (!user || !bookPtr) return false;

    bookPtr->setBorrowed(false);
    user->returnBook(bookTitle);
    return true;
}


UserSharedPtr Library::findUserById(int id) const {
    for (const auto& user : users) {
        if (user->getId() == id) {
            return user;
        }
    }
    return nullptr;
}

