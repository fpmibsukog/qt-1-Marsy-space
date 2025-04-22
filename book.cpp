#include "book.h"
#include <QDebug>

Book::Book(const QString& title, const QString& author, int year)
    : title(title), author(author), year(year) {}

QString Book::getTitle() const { return title; }
QString Book::getAuthor() const { return author; }
int Book::getYear() const { return year; }

QString Book::getInfo() const {
    return QString("Title: %1\nAuthor: %2\nYear: %3\n")
    .arg(title).arg(author).arg(year);
}

void printBookInfo(BookUniquePtr book) {
    if (book) {
        qDebug() << "Book info (unique_ptr):\n" << book->getInfo();
    }
}
