#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>


class LibraryItem {
protected: 
    int itemID;
    std::string title;
    bool isIssued;

public:
    LibraryItem(int id, const std::string& title)
        : itemID(id), title(title), isIssued(false) {}

    virtual ~LibraryItem() {} 

    
    virtual void displayDetails() const = 0; 

    
    int getID() const { return itemID; }
    std::string getTitle() const { return title; }
    bool getIsIssued() const { return isIssued; }

    
    void setIssued(bool status) { isIssued = status; }
};



class Book : public LibraryItem {
private:
    std::string author;
    int pageCount;

public:
    Book(int id, const std::string& title, const std::string& author, int pages)
        : LibraryItem(id, title), author(author), pageCount(pages) {}

    
    void displayDetails() const override;

    
    std::string getAuthor() const { return author; }
    int getPageCount() const { return pageCount; }
};


void Book::displayDetails() const {
    std::cout << "--- Book Details ---" << std::endl;
    std::cout << "ID: \t\t" << itemID << std::endl;
    std::cout << "Title: \t\t" << title << std::endl;
    std::cout << "Author: \t" << author << std::endl;
    std::cout << "Page Count: \t" << pageCount << std::endl;
    std::cout << "Status: \t" << (isIssued ? "Issued" : "Available") << std::endl;
    std::cout << "--------------------" << std::endl;
}



class Member {
protected:
    std::string name;
    int memberID;
    
    
    static int nextID; 

public:
    Member(const std::string& name); 
    virtual ~Member() {} 

    
    std::string getName() const { return name; }
    int getID() const { return memberID; }

    
    virtual void displayDetails() const;
};


int Member::nextID = 1001;


Member::Member(const std::string& name) : name(name) {
    this->memberID = nextID++; 
}

void Member::displayDetails() const {
    std::cout << "--- Member Details ---" << std::endl;
    std::cout << "ID: \t" << this->memberID << std::endl;
    std::cout << "Name: \t" << this->name << std::endl;
    std::cout << "----------------------" << std::endl;
}



class StudentMember : public Member {
private:
    std::string studentID; 

public:
    StudentMember(const std::string& name, const std::string& studentID);

    
    void displayDetails() const override;

    
    std::string getStudentID() const { return studentID; }
};


StudentMember::StudentMember(const std::string& name, const std::string& studentID)
    : Member(name) { 
    this->studentID = studentID;
}

void StudentMember::displayDetails() const {
    std::cout << "--- Student Member ---" << std::endl;
    std::cout << "ID: \t" << getID() << std::endl;
    std::cout << "Name: \t" << getName() << std::endl;
    std::cout << "Stu. ID: " << this->studentID << std::endl;
    std::cout << "----------------------" << std::endl;
}



class Library {
private:
    std::vector<LibraryItem*> m_items;
    std::vector<Member*> m_members;

    void loadData();
    void saveData();

public:
    Library();
    ~Library();

    void addItem(LibraryItem* item);
    void addMember(Member* member);

    void displayAllItems() const;
    void displayAllMembers() const;

    
    LibraryItem* findBook(int itemID);
    Member* findMember(int memberID);

    
    void issueBook(int bookID, int memberID, int loanDays = 14);
};


Library::Library() {
    std::cout << "Library object created. Loading data..." << std::endl;
    try {
        loadData();
    } catch (const std::exception& e) {
        std::cerr << "Error loading data: " << e.what() << std::endl;
    }
}

Library::~Library() {
    std::cout << std::endl << "Library object destroying. Saving data..." << std::endl;
    try {
        saveData();
    } catch (const std::exception& e) {
        std::cerr << "Error saving data: " << e.what() << std::endl;
    }
    
    std::cout << "Cleaning up memory..." << std::endl;
    for (LibraryItem* item : m_items) { delete item; }
    for (Member* member : m_members) { delete member; }
}


void Library::addItem(LibraryItem* item) {
    m_items.push_back(item);
}

void Library::addMember(Member* member) {
    m_members.push_back(member);
}

void Library::displayAllItems() const {
    std::cout << std::endl << "--- All Library Items ---" << std::endl;
    if (m_items.empty()) {
        std::cout << "(No items in library)" << std::endl;
        return;
    }
    for (const auto& item : m_items) {
        item->displayDetails(); 
    }
}

void Library::displayAllMembers() const {
    std::cout << std::endl << "--- All Library Members ---" << std::endl;
    if (m_members.empty()) {
        std::cout << "(No members in library)" << std::endl;
        return;
    }
    for (const auto& member : m_members) {
        member->displayDetails(); 
    }
}


LibraryItem* Library::findBook(int itemID) {
    for (auto* item : m_items) {
        if (item->getID() == itemID) {
            return item;
        }
    }
    throw std::runtime_error("Book not found: ID " + std::to_string(itemID));
}


Member* Library::findMember(int memberID) {
    for (auto* member : m_members) {
        if (member->getID() == memberID) {
            return member;
        }
    }
    throw std::runtime_error("Member not found: ID " + std::to_string(memberID));
}

void Library::issueBook(int bookID, int memberID, int loanDays) {
    LibraryItem* book = findBook(bookID);
    Member* member = findMember(memberID); 

    if (book->getIsIssued()) {
        throw std::runtime_error("Book is already issued.");
    }

    book->setIssued(true);
    std::cout << "Book '" << book->getTitle() << "' issued to "
              << member->getName() << " for " << loanDays << " days." << std::endl;
}


void Library::saveData() {
    std::ofstream itemFile("items.csv");
    if (!itemFile) { throw std::runtime_error("Could not open items.csv"); }
    for (const auto* item : m_items) {
        if (const Book* book = dynamic_cast<const Book*>(item)) {
            itemFile << "B," << book->getID() << "," << book->getTitle() << ","
                     << book->getAuthor() << "," << book->getPageCount() << "\n";
        }
    }
    itemFile.close();

    std::ofstream memberFile("members.csv");
    if (!memberFile) { throw std::runtime_error("Could not open members.csv"); }
    for (const auto* member : m_members) {
        if (const StudentMember* student = dynamic_cast<const StudentMember*>(member)) {
            memberFile << "S," << student->getID() << "," << student->getName() << ","
                       << student->getStudentID() << "\n";
        } else {
            memberFile << "M," << member->getID() << "," << member->getName() << "\n";
        }
    }
    memberFile.close();
    std::cout << "Data saved successfully." << std::endl;
}

void Library::loadData() {
    std::ifstream itemFile("items.csv");
    if (itemFile) {
        std::string line;
        while (std::getline(itemFile, line)) {
            std::stringstream ss(line);
            std::string segment;
            std::vector<std::string> parts;
            while (std::getline(ss, segment, ',')) { parts.push_back(segment); }

            if (parts.empty()) continue;
            try {
                if (parts[0] == "B") { 
                    addItem(new Book(std::stoi(parts[1]), parts[2], parts[3], std::stoi(parts[4])));
                }
            } catch (const std::exception& e) {
                std::cerr << "Error parsing items.csv: " << e.what() << std::endl;
            }
        }
        itemFile.close();
    }

    std::ifstream memberFile("members.csv");
    if (memberFile) {
        std::string line;
        while (std::getline(memberFile, line)) {
            std::stringstream ss(line);
            std::string segment;
            std::vector<std::string> parts;
            while (std::getline(ss, segment, ',')) { parts.push_back(segment); }
            
            if (parts.empty()) continue;
            try {
               
                if (parts[0] == "M") { 
                    addMember(new Member(parts[2]));
                } else if (parts[0] == "S") { 
                    addMember(new StudentMember(parts[2], parts[3]));
                }
            } catch (const std::exception& e) {
                std::cerr << "Error parsing members.csv: " << e.what() << std::endl;
            }
        }
        memberFile.close();
    }
    std::cout << "Data loaded." << std::endl;
}



void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}



int main() {
    Library myLibrary;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n--- Library Menu ---" << std::endl;
        std::cout << "1. Add Book" << std::endl;
        std::cout << "2. Add Member" << std::endl;
        std::cout << "3. Issue Book" << std::endl;
        std::cout << "4. Find Book" << std::endl;
        std::cout << "5. Find Member" << std::endl;
        std::cout << "6. Display All Items" << std::endl;
        std::cout << "7. Display All Members" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            clearInput();
            choice = -1;
            continue;
        }
        clearInput(); 

        try {
            switch (choice) {
                case 1: { 
                    std::string title, author;
                    int id, pages;
                    std::cout << "Enter Book ID: "; std::cin >> id; clearInput();
                    std::cout << "Enter Title: "; std::getline(std::cin, title);
                    std::cout << "Enter Author: "; std::getline(std::cin, author);
                    std::cout << "Enter Pages: "; std::cin >> pages; clearInput();
                    myLibrary.addItem(new Book(id, title, author, pages));
                    std::cout << "Book added!" << std::endl;
                    break;
                }
                case 2: { 
                    std::string name, studentID;
                    int type;
                    std::cout << "Enter Name: "; std::getline(std::cin, name);
                    std::cout << "Member type (1-Standard, 2-Student): "; std::cin >> type; clearInput();
                    if (type == 2) {
                        std::cout << "Enter Student ID: "; std::getline(std::cin, studentID);
                        myLibrary.addMember(new StudentMember(name, studentID));
                    } else {
                        myLibrary.addMember(new Member(name));
                    }
                    std::cout << "Member added!" << std::endl;
                    break;
                }
                case 3: { 
                    int bookID, memberID;
                    std::cout << "Enter Book ID: "; std::cin >> bookID; clearInput();
                    std::cout << "Enter Member ID: "; std::cin >> memberID; clearInput();
                    myLibrary.issueBook(bookID, memberID); 
                    break;
                }
                case 4: { 
                    std::cout << "Enter Book ID: "; int id; std::cin >> id; clearInput();
                    LibraryItem* book = myLibrary.findBook(id);
                    std::cout << "--- Book Found! ---" << std::endl;
                    book->displayDetails();
                    break;
                }
                case 5: { 
                    std::cout << "Enter Member ID: "; int id; std::cin >> id; clearInput();
                    Member* member = myLibrary.findMember(id);
                    std::cout << "--- Member Found! ---" << std::endl;
                    member->displayDetails();
                    break;
                }
                case 6: 
                    myLibrary.displayAllItems();
                    break;
                case 7: 
                    myLibrary.displayAllMembers();
                    break;
                case 0:
                    std::cout << "Exiting..." << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
                    break;
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    return 0; 

}
