#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <memory>

class Book {
public:
    Book(const QString& title, const QString& author, int year);

    bool isBorrowed() const { return borrowed; }
    void setBorrowed(bool status) { borrowed = status; }
    QString getTitle() const;
    QString getAuthor() const;
    int getYear() const;
    QString getInfo() const;

private:
    QString title;
    QString author;
    int year;
    bool borrowed = false;
};

using BookUniquePtr = std::unique_ptr<Book>;
using BookSharedPtr = std::shared_ptr<Book>;
using BookWeakPtr = std::weak_ptr<Book>;

void printBookInfo(BookUniquePtr book);

#endif // BOOK_H
