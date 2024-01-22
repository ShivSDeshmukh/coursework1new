
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

// Forward declaration of the Member class
class Member;

// Structure to represent a date in the library
struct LibraryDate {
    int libraryDay;
    int libraryMonth;
    int libraryYear;
};

// Function to get a LibraryDate from the user
LibraryDate getLibraryDateFromUser() {
    LibraryDate inputLibraryDate;
    std::cout << "Enter date in DD MM YYYY format: " << std::endl;

    std::cout << "Day: ";
    std::cin >> inputLibraryDate.libraryDay;

    std::cout << "Month: ";
    std::cin >> inputLibraryDate.libraryMonth;

    std::cout << "Year: ";
    std::cin >> inputLibraryDate.libraryYear;

    std::cout << "The issued date of this book is " << inputLibraryDate.libraryDay << "/"
              << inputLibraryDate.libraryMonth << "/" << inputLibraryDate.libraryYear << std::endl;

    return inputLibraryDate;
}

// Function to add days to a LibraryDate
LibraryDate addDaysToLibraryDate(const LibraryDate& currentLibraryDate, int daysToAdd) {
    const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    LibraryDate result = currentLibraryDate;
    result.libraryDay += daysToAdd;

    while (result.libraryDay > daysInMonth[result.libraryMonth - 1]) {
        result.libraryDay -= daysInMonth[result.libraryMonth - 1];
        result.libraryMonth++;
        if (result.libraryMonth > 12) {
            result.libraryMonth = 1;
            result.libraryYear++;
        }
    }

    return result;
}

// Function to calculate the difference in days between two LibraryDates
int dateDifference(const LibraryDate& date1, const LibraryDate& date2) {
    std::tm tm1 = {0, 0, 0, date1.libraryDay, date1.libraryMonth - 1, date1.libraryYear - 1900};
    std::tm tm2 = {0, 0, 0, date2.libraryDay, date2.libraryMonth - 1, date2.libraryYear - 1900};

    std::time_t time1 = std::mktime(&tm1);
    std::time_t time2 = std::mktime(&tm2);

    const int secondsPerDay = 24 * 60 * 60;
    double difference = std::difftime(time2, time1) / secondsPerDay;

    return static_cast<int>(difference);
}

// Class to represent a Book
class Book {
private:
    int bookID;
    std::string bookName, authorFirstName, authorLastName, bookType;
    LibraryDate dueLibraryDate;
    Member* borrower;
    bool isBorrowed;

public:
    // Structure to represent library books with due date
    struct LibraryBooksWithDueDate {
        int bookID;
        int memberID;
        LibraryDate issueLibraryDate;
        LibraryDate dueLibraryDate;
    };

    // Constructor for Book
    Book(int bookId, const std::string& nameBook, const std::string& FirstNameAuthor, const std::string& LastNameAuthor)
        : bookID(bookId), bookName(nameBook), authorFirstName(FirstNameAuthor), authorLastName(LastNameAuthor), isBorrowed(false), borrower(nullptr) {}

    // Getter for Book ID
    std::string getBookID() const {
        return std::to_string(bookID);
    }

    // Getter for Book Name
    std::string getBookName() const {
        return bookName;
    }

    // Getter for Author's First Name
    std::string getAuthorFirstName() const {
        return authorFirstName;
    }

    // Getter for Author's Last Name
    std::string getAuthorLastName() const {
        return authorLastName;
    }

    // Setter for Due Library Date
    void setDueLibraryDate(const LibraryDate& DueLibraryDate) {
        dueLibraryDate = DueLibraryDate;
    }

    // Getter for Due Library Date
    LibraryDate getDueLibraryDate() const {
        return dueLibraryDate;
    }

    // Method to return a borrowed book
    void returnBook() {
        isBorrowed = false;
        borrower = nullptr;
    }

    // Structure to hold book information
    LibraryBooksWithDueDate bookInfo;

    // Method to borrow a book
    void borrowBook(Member& borrower, const LibraryDate& dueLibraryDate) {
        this->borrower = &borrower;
        this->dueLibraryDate = dueLibraryDate;
    }
};

// Class to represent a Person
class Person {
private:
    std::string name, address, email;

public:
    // Setter for Name
    void setName(const std::string& Name) {
        name = Name;
    }

    // Getter for Name
    std::string getName() const {
        return name;
    }

    // Setter for Address
    void setAddress(const std::string& Address) {
        address = Address;
    }

    // Getter for Address
    std::string getAddress() const {
        return address;
    }

    // Setter for Email
    void setEmail(const std::string& Email) {
        email = Email;
    }

    // Getter for Email
    std::string getEmail() const {
        return email;
    }
};

// Class to represent a Member, inheriting from Person
class Member : public Person {
private:
    int memberID;
    std::vector<Book> booksLoaned;

public:
    // Constructor for Member
    Member(int memberID, const std::string& Name, const std::string& Address, const std::string& Email)
        : memberID(memberID) {
        setName(Name);
        setAddress(Address);
        setEmail(Email);
    }

    // Getter for Member ID
    std::string getMemberID() const {
        return std::to_string(memberID);
    }

    // Getter for Member ID as an integer
    int getMemberIDInt() const {
        return memberID;
    }

    // Getter for books borrowed by the member
    std::vector<Book> getBooksBorrowed() const {
        return booksLoaned;
    }
};

// Class to represent a Librarian, inheriting from Person
class Librarian : public Person {
private:
    int staffID;
    int salary;
    std::vector<Member> members;
    std::vector<Book::LibraryBooksWithDueDate> issuedLibraryBooks;

public:
    // Constructor for Librarian
    Librarian(int staffID, const std::string& Name, const std::string& Address, const std::string& Email, int Salary)
        : staffID(staffID), salary(Salary) {
        setName(Name);
        setAddress(Address);
        setEmail(Email);
    }

    // Check if a member already exists based on ID, Name, Address, and Email
    bool memberExists(int memberID, const std::string& Name, const std::string& Address, const std::string& Email) const {
        for (const Member& member : members) {
            if (member.getMemberID() == std::to_string(memberID) ||
                (member.getName() == Name && member.getAddress() == Address && member.getEmail() == Email)) {
                return true;
            }
        }
        return false;
    }

    // Add a new member to the system
    void addMember(int memberID, const std::string& Name, const std::string& Address, const std::string& Email) {
        if (!memberExists(memberID, Name, Address, Email)) {
            Member newMember(memberID, Name, Address, Email);
            members.push_back(newMember);
            std::cout << "New member is added!!\n";
        } else {
            std::cout << "Member exists!!.\n";
        }
    }

    // Display information about library books from a CSV file
    void displayLibraryBooks() const {
        std::fstream myFile;
        myFile.open("library_books.csv", std::ios::in);
        if (myFile.is_open()) {
            std::string line;
            while (getline(myFile, line)) {
                std::cout << line << std::endl;
                std::cout << std::endl;
            }
            myFile.close();
        }
    }

    // Check if a book is already issued
    bool isBookIssued(int bookId) const {
        for (const Book::LibraryBooksWithDueDate& issuedLibraryBook : issuedLibraryBooks) {
            if (issuedLibraryBook.bookID == bookId) {
                return true;
            }
        }
        return false;
    }

    // Issue a book to a member
    void issueBook(int memberId, int bookId) {
        if (isBookIssued(bookId)) {
            std::cout << "Book is already issued!!" << std::endl;
            return;
        }

        LibraryDate issueLibraryDate = getLibraryDateFromUser();
        LibraryDate dueLibraryDate = addDaysToLibraryDate(issueLibraryDate, 3);
        Book::LibraryBooksWithDueDate newIssuedLibraryBook = {bookId, memberId, dueLibraryDate};
        issuedLibraryBooks.push_back(newIssuedLibraryBook);
        std::cout << "Book is successfully issued by" << memberId << std::endl;
    }

    // Return a borrowed book
    void returnBook(int memberId, int bookId) {
        for (auto it = issuedLibraryBooks.begin(); it != issuedLibraryBooks.end();) {
            if (it->memberID == memberId && it->bookID == bookId) {
                it = issuedLibraryBooks.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Display books borrowed by a member
    void displayBorrowedBooks(int memberID) const {
        std::cout << "Books was borrowed by member " << memberID << std::endl;
        bool hasBooksIssued = false;

        for (const Book::LibraryBooksWithDueDate& issuedLibraryBook : issuedLibraryBooks) {
            if (issuedLibraryBook.memberID == memberID) {
                std::cout << "Book ID: " << issuedLibraryBook.bookID << std::endl;
                hasBooksIssued = true;
            }
        }

        if (!hasBooksIssued) {
            std::cout << "No books are issued by " << memberID << std::endl;
        }
    }

    // Calculate fines for overdue books
    void calculateFine(int memberID) const {
        int totalFine = 0;
        std::cout << "Enter the date of return " << std::endl;
        LibraryDate currentLibraryDate = getLibraryDateFromUser();

        for (const Book::LibraryBooksWithDueDate& issuedLibraryBook : issuedLibraryBooks) {
            if (issuedLibraryBook.memberID == memberID) {
                int daysOverdue = dateDifference(issuedLibraryBook.dueLibraryDate, currentLibraryDate);
                if (daysOverdue > 0) {
                    totalFine += daysOverdue * 10;
                }
            }
        }

        if (totalFine > 0) {
            std::cout << "Total fine for member " << memberID << " is £" << totalFine << std::endl;
        } else {
            std::cout << "No fine for member " << memberID << std::endl;
        }
    }

    // Getter for staff ID
    int getStaffID() const {
        return staffID;
    }

    // Getter for salary
    int getSalary() const {
        return salary;
    }
};

// Main function
int main() {
    using std::cout;
    using std::endl;

    // Creating a Librarian instance
    Librarian libraryManager(1, "LibrarianName", "LibrarianAddress", "librarian@example.com", 100);

    int userChoice;
    int i = 0;

    // Main menu loop
    while (i < 2) {
        int memberID;
        std::string Name;
        std::string Address;
        std::string Email;
        int bookID;

        // Displaying the main menu
        cout << "----------------------------------------------" << endl;
        cout << "Library Management System - Main Menu" << endl;
        cout << "Please choose an action from the options below" << endl;
        cout << "[1] Register New Members" << endl;
        cout << "[2] Issue a Book (membership required)" << endl;
        cout << "[3] Return a Borrowed Book" << endl;
        cout << "[4] Display Borrowed Books" << endl;
        cout << "[5] Calculate Fine (for overdue books)" << endl;
        cout << "[6] Exit System" << endl;
        cout << "----------------------------------------------" << endl;

        // Getting user choice
        std::cin >> userChoice;

        // Switch statement based on user choice
        switch (userChoice) {
            case 1:
                // Registering a new member
                cout << "Enter memberID: " << endl;
                std::cin >> memberID;
                cout << "Enter your name: " << endl;
                std::cin >> Name;
                cout << "Enter your Email address: " << endl;
                std::cin >> Email;
                cout << "Enter your address: " << endl;
                std::cin >> Address;
                libraryManager.addMember(memberID, Name, Email, Address);
                break;

            case 2:
                // Issuing a book
                cout << "Please have a look through our collection of books and note the BookID of the book you would like to issue" << endl;
                cout << endl;
                libraryManager.displayLibraryBooks();
                cout << "Enter memberID: " << endl;
                std::cin >> memberID;
                cout << "Enter bookID: " << endl;
                std::cin >> bookID;
                libraryManager.issueBook(memberID, bookID);
                break;


            case 3:
                cout << endl;
                cout << "Enter bookID of the book you would like to return: " << endl;
                std::cin >> bookID;
                cout << "Enter your memberId: " << endl;
                std::cin >> memberID;
                libraryManager.returnBook(memberID, bookID);
                break;

            case 4:
                cout << "Enter your memberID: " << endl;
                std::cin >> memberID;
                libraryManager.displayBorrowedBooks(memberID);
                break;

            case 5:
                cout << "Enter your memberID: " << endl;
                std::cin >> memberID;
                libraryManager.calculateFine(memberID);
                break;

            case 6:
                i = 2;
                break;
        }
    }
    return 0;
}
