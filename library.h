#ifndef LIBRARY_H
#define LIBRARY_H
#include "user.h"
#include "book.h"
#include <vector>

class Library {
public:
    void addBook(BookSharedPtr book);
    bool removeBook(const QString& title);
    void listBooks() const;
    BookWeakPtr findBookByTitle(const QString& title) const;
    BookWeakPtr findBookByAuthor(const QString& author) const;
    void addUser(UserSharedPtr user);
    bool removeUser(int id);
    void listUsers() const;
    bool lendBook(const QString& bookTitle, int userId);
    bool returnBook(const QString& bookTitle, int userId);
    UserSharedPtr findUserById(int id) const;
    const std::vector<BookSharedPtr>& getBooks() const { return books; }
    const std::vector<UserSharedPtr>& getUsers() const { return users; }
private:

    std::vector<BookSharedPtr> books;
    std::vector<UserSharedPtr> users;
};

#endif // LIBRARY_H
