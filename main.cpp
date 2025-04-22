#include <QApplication>
#include "book.h"
#include "library.h"
#include "mainwindow.h"


void demonstrateUniquePtr() {
    qDebug() << "=== Demonstrating unique_ptr ===";

    auto book = std::make_unique<Book>("The Great Gatsby", "F. Scott Fitzgerald", 1925);

    printBookInfo(std::move(book));

    if (!book) {
        qDebug() << "Book pointer is now null after move";
    }
}

void demonstrateSharedPtr() {
    qDebug() << "\n=== Demonstrating shared_ptr ===";

    Library library;

    auto book1 = std::make_shared<Book>("1984", "George Orwell", 1949);
    auto book2 = std::make_shared<Book>("To Kill a Mockingbird", "Harper Lee", 1960);
    auto book3 = std::make_shared<Book>("Pride and Prejudice", "Jane Austen", 1813);

    library.addBook(book1);
    library.addBook(book2);
    library.addBook(book3);

    library.listBooks();

    if (library.removeBook("1984")) {
        qDebug() << "Book '1984' removed successfully";
    }

    library.listBooks();

    qDebug() << "Reference count for 'Pride and Prejudice':" << book3.use_count();
}

void demonstrateWeakPtr(Library& library) {
    qDebug() << "\n=== Demonstrating weak_ptr ===";

    auto weakBook = library.findBookByTitle("To Kill a Mockingbird");

    if (auto sharedBook = weakBook.lock()) {
        qDebug() << "Book found:\n" << sharedBook->getInfo();
    } else {
        qDebug() << "Book not found or has been deleted";
    }

    weakBook = library.findBookByAuthor("Jane Austen");
    if (auto sharedBook = weakBook.lock()) {
        qDebug() << "Book by Jane Austen found:\n" << sharedBook->getInfo();
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    demonstrateUniquePtr();

    Library library;
    demonstrateSharedPtr();
    demonstrateWeakPtr(library);

    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
