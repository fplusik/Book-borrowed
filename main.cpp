#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    int year;
    bool available;

public:
    Book(int i, string t, string a, int y) : 
        id(i), title(t), author(a), year(y), available(true) {}

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getYear() const { return year; }
    bool isAvailable() const { return available; }

    void setAvailable(bool status) { available = status; }

    string serialize() const {
        return to_string(id) + "|" + title + "|" + author + "|" + 
               to_string(year) + "|" + (available ? "1" : "0");
    }

    static Book deserialize(const string& line) {
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);
        size_t pos4 = line.find('|', pos3 + 1);

        int id = stoi(line.substr(0, pos1));
        string title = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string author = line.substr(pos2 + 1, pos3 - pos2 - 1);
        int year = stoi(line.substr(pos3 + 1, pos4 - pos3 - 1));
        bool available = line.substr(pos4 + 1) == "1";

        Book book(id, title, author, year);
        book.setAvailable(available);
        return book;
    }

    void display() const {
        cout << "ID: " << id << " | " << title << " by " << author 
             << " (" << year << ") - " << (available ? "Available" : "Borrowed") << endl;
    }
};

class Library {
private:
    vector<Book> books;
    string filename;

public:
    Library(string file) : filename(file) {
        loadBooks();
    }

    void loadBooks() {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                books.push_back(Book::deserialize(line));
            }
        }
        file.close();
    }

    void saveBooks() {
        ofstream file(filename);
        for (const auto& book : books) {
            file << book.serialize() << endl;
        }
        file.close();
    }

    void addBook(string title, string author, int year) {
        int newId = books.empty() ? 1 : books.back().getId() + 1;
        books.push_back(Book(newId, title, author, year));
        saveBooks();
        cout << "Book added with ID: " << newId << endl;
    }

    void deleteBook(int id) {
        books.erase(remove_if(books.begin(), books.end(),
            [id](const Book& b) { return b.getId() == id; }), books.end());
        saveBooks();
        cout << "Book deleted" << endl;
    }

    void borrowBook(int id) {
        for (auto& book : books) {
            if (book.getId() == id) {
                if (book.isAvailable()) {
                    book.setAvailable(false);
                    saveBooks();
                    cout << "Book borrowed successfully" << endl;
                } else {
                    cout << "Book is already borrowed" << endl;
                }
                return;
            }
        }
        cout << "Book not found" << endl;
    }

    void returnBook(int id) {
        for (auto& book : books) {
            if (book.getId() == id) {
                if (!book.isAvailable()) {
                    book.setAvailable(true);
                    saveBooks();
                    cout << "Book returned successfully" << endl;
                } else {
                    cout << "Book is not borrowed" << endl;
                }
                return;
            }
        }
        cout << "Book not found" << endl;
    }

    void listAll() {
        if (books.empty()) {
            cout << "No books in library" << endl;
            return;
        }
        for (const auto& book : books) {
            book.display();
        }
    }

    void searchByTitle(string query) {
        transform(query.begin(), query.end(), query.begin(), ::tolower);
        bool found = false;
        for (const auto& book : books) {
            string title = book.getTitle();
            transform(title.begin(), title.end(), title.begin(), ::tolower);
            if (title.find(query) != string::npos) {
                book.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No books found" << endl;
        }
    }

    void searchByAuthor(string query) {
        transform(query.begin(), query.end(), query.begin(), ::tolower);
        bool found = false;
        for (const auto& book : books) {
            string author = book.getAuthor();
            transform(author.begin(), author.end(), author.begin(), ::tolower);
            if (author.find(query) != string::npos) {
                book.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No books found" << endl;
        }
    }

    void listAvailable() {
        bool found = false;
        for (const auto& book : books) {
            if (book.isAvailable()) {
                book.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No available books" << endl;
        }
    }
};

int main() {
    Library library("library.txt");
    int choice;
    
    while (true) {
        cout << "\n=== Library Management System ===" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Delete Book" << endl;
        cout << "3. Borrow Book" << endl;
        cout << "4. Return Book" << endl;
        cout << "5. List All Books" << endl;
        cout << "6. Search by Title" << endl;
        cout << "7. Search by Author" << endl;
        cout << "8. List Available Books" << endl;
        cout << "9. Exit" << endl;
        
        cout << "\nEnter choice: ";
        cin >> choice;
        cin.ignore();
        
        if (choice == 1) {
            string title, author;
            int year;
            cout << "Title: ";
            getline(cin, title);
            cout << "Author: ";
            getline(cin, author);
            cout << "Year: ";
            cin >> year;
            library.addBook(title, author, year);
        } else if (choice == 2) {
            int id;
            cout << "Book ID: ";
            cin >> id;
            library.deleteBook(id);
        } else if (choice == 3) {
            int id;
            cout << "Book ID: ";
            cin >> id;
            library.borrowBook(id);
        } else if (choice == 4) {
            int id;
            cout << "Book ID: ";
            cin >> id;
            library.returnBook(id);
        } else if (choice == 5) {
            library.listAll();
        } else if (choice == 6) {
            string query;
            cout << "Search title: ";
            getline(cin, query);
            library.searchByTitle(query);
        } else if (choice == 7) {
            string query;
            cout << "Search author: ";
            getline(cin, query);
            library.searchByAuthor(query);
        } else if (choice == 8) {
            library.listAvailable();
        } else if (choice == 9) {
            break;
        } else {
            cout << "Invalid choice" << endl;
        }
    }
    
    return 0;
}
