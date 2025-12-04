#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <utility>

using namespace std;

class Author {
private:
    string name;
    int birthYear;
public:
    Author() : name("Unknown"), birthYear(1900) {}
    Author(const string& n, int y) : name(n) { setBirthYear(y); }

    string getName() const { return name; }
    int getBirthYear() const { return birthYear; }

    void setBirthYear(int y) {
        if (y < 1850 || y > 2025)
            throw invalid_argument("Birth year out of range");
        birthYear = y;
    }

    string to_string() const {
        return name + " (" + std::to_string(birthYear) + ")";
    }
};

class Book {
private:
    string title;
    Author author;
    int year;
    double price;
    string isbn;
    static int totalBooks;

public:
    Book() : title("Unknown"), author(), year(2000), price(0), isbn("Unknown") { ++totalBooks; }

    Book(const string& t, const Author& a, int y, double p, const string& i)
        : title(t), author(a), year(0), price(0), isbn(i) {
        setYear(y);
        setPrice(p);
        ++totalBooks;
    }

    Book(const Book& other) = default;
    Book(Book&& other) noexcept = default;
    Book& operator=(const Book& other) = default;
    Book& operator=(Book&& other) noexcept = default;
    ~Book() = default;

    string getTitle() const { return title; }
    Author getAuthor() const { return author; }
    int getYear() const { return year; }
    double getPrice() const { return price; }
    string getISBN() const { return isbn; }

    void setPrice(double p) {
        if (p < 0) throw invalid_argument("Price cannot be negative");
        price = p;
    }

    void setYear(int y) {
        if (y < 1500 || y > 2025) throw invalid_argument("Year out of range");
        year = y;
    }

    string to_string() const {
        return title + " by " + author.getName() + " (" + std::to_string(year) + ") - " +
               std::to_string(price) + " lv. ISBN: " + isbn;
    }

    static int getTotalBooks() { return totalBooks; }
};

int Book::totalBooks = 0;

class Member {
private:
    string name;
    string memberId;
    int yearJoined;
public:
    Member() : name("Unknown"), memberId("Unknown"), yearJoined(2000) {}
    Member(const string& n, const string& id, int y) : name(n), memberId(id), yearJoined(y) {
        if (memberId.empty()) throw invalid_argument("Member ID cannot be empty");
    }

    string getName() const { return name; }
    string getMemberId() const { return memberId; }
    int getYearJoined() const { return yearJoined; }

    string to_string() const {
        return name + " (ID: " + memberId + ", Joined: " + std::to_string(yearJoined) + ")";
    }
};

class Loan {
private:
    string isbn;
    string memberId;
    string startDate;
    string dueDate;
    bool returned;
public:
    Loan(const string& i, const string& m, const string& start, const string& due)
        : isbn(i), memberId(m), startDate(start), dueDate(due), returned(false) {
        if (dueDate < startDate)
            throw invalid_argument("Due date cannot be earlier than start date");
    }

    void markReturned() { returned = true; }
    bool isReturned() const { return returned; }

    bool isOverdue(const string& today) const {
        return !returned && today > dueDate;
    }

    string to_string() const {
        return "Loan: " + isbn + " to " + memberId + ", from " + startDate + " to " + dueDate +
               (returned ? " (returned)" : " (active)");
    }

    string getISBN() const { return isbn; }
    string getMemberId() const { return memberId; }
};

class Library {
private:
    vector<Book> books;
    vector<Member> members;
    vector<Loan> loans;
public:
    void addBook(const Book& b) { books.push_back(b); }
    void addMember(const Member& m) { members.push_back(m); }

    bool hasBook(const string& isbn) const {
        return any_of(books.begin(), books.end(), [&](const Book& b){ return b.getISBN() == isbn; });
    }

    bool isBookAvailable(const string& isbn) const {
        return none_of(loans.begin(), loans.end(), [&](const Loan& l){ return l.getISBN() == isbn && !l.isReturned(); });
    }

    bool loanBook(const string& isbn, const string& memberId, const string& start, const string& due) {
        if (!hasBook(isbn) || !isBookAvailable(isbn))
            return false;
        loans.emplace_back(isbn, memberId, start, due);
        return true;
    }

    bool returnBook(const string& isbn, const string& memberId) {
        for (auto& l : loans) {
            if (l.getISBN() == isbn && l.getMemberId() == memberId && !l.isReturned()) {
                l.markReturned();
                return true;
            }
        }
        return false;
    }

    vector<Book> findByAuthor(const string& authorName) const {
        vector<Book> result;
        for (const auto& b : books)
            if (b.getAuthor().getName().find(authorName) != string::npos)
                result.push_back(b);
        return result;
    }

    string to_string() const {
        int activeLoans = count_if(loans.begin(), loans.end(), [](const Loan& l){ return !l.isReturned(); });
        return "Library: " + std::to_string(books.size()) + " books, " +
               std::to_string(members.size()) + " members, " +
               std::to_string(activeLoans) + " active loans";
    }
};

int main() {
    Library lib;
    Author a{"Ivan Vazov", 1850};
    Book b1{"Pod igoto", a, 1894, 25.50, "ISBN-001"};
    Book b2{"Nema zemya", a, 1900, 18.90, "ISBN-002"};

    lib.addBook(b1);
    lib.addBook(b2);
    lib.addMember(Member{"Petar Petrov", "M001", 2023});

    cout << lib.to_string() << "\n";

    if (lib.loanBook("ISBN-001", "M001", "2025-11-03", "2025-11-17"))
        cout << "Loan created.\n";

    cout << "Available ISBN-001? " << boolalpha << lib.isBookAvailable("ISBN-001") << "\n";

    lib.returnBook("ISBN-001", "M001");

    cout << "Available ISBN-001? " << boolalpha << lib.isBookAvailable("ISBN-001") << "\n";

    for (const auto& bk : lib.findByAuthor("Vazov")) {
        cout << bk.to_string() << "\n";
    }

    cout << "Total books created: " << Book::getTotalBooks() << endl;
}
