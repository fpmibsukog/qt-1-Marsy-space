#ifndef USER_H
#define USER_H

#include <QString>
#include <vector>
#include <memory>
#include "book.h"

class User {
public:
    User(const QString& name, int id);


    QString getName() const;
    int getId() const;
    void borrowBook(BookSharedPtr book);
    void returnBook(const QString& title);
    QString getInfo() const;
    QString getBorrowedBooks() const;
    const std::vector<BookSharedPtr>& getBorrowedBooksList() const { return borrowedBooks; }



private:
    QString name;
    int id;
    std::vector<BookSharedPtr> borrowedBooks;
};

using UserSharedPtr = std::shared_ptr<User>;

#endif // USER_H
