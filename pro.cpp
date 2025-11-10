#include <iostream>
#include <fstream>
using namespace std;

class LibraryItem {
public: 
    int itemID;
    char title[100];
    int isIssued;

    LibraryItem(int id, const char* titleStr) {
        itemID = id;
        
        int i = 0;
        while (titleStr[i] != '\0') {
            title[i] = titleStr[i];
            i++;
        }
        title[i] = '\0';
        
        isIssued = 0; 
    }

    virtual void displayDetails() = 0; 

    int getID() { return itemID; }
    const char* getTitle() { return title; }
    int getIsIssued() { return isIssued; }
    void setIssued(int status) { isIssued = status; }
};

class Book : public LibraryItem {
public:
    char author[100];
    int pageCount;

    Book(int id, const char* titleStr, const char* authorStr, int pages)
        : LibraryItem(id, titleStr) {
        
        int i = 0;
        while (authorStr[i] != '\0') {
            author[i] = authorStr[i];
            i++;
        }
        author[i] = '\0';
        
        this->pageCount = pages;
    }

    void displayDetails() {
        cout << "--- Book Details ---" << endl;
        cout << "ID: \t\t" << getID() << endl;
        cout << "Title: \t\t" << getTitle() << endl;
        cout << "Author: \t" << author << endl;
        cout << "Page Count: \t" << pageCount << endl;
        cout << "Status: \t" << (getIsIssued() == 1 ? "Issued" : "Available") << endl;
        cout << "--------------------" << endl;
    }
};

class Member {
public:
    char name[100];
    int memberID;
    
    static int nextID; 

    Member(const char* nameStr) {
        int i = 0;
        while (nameStr[i] != '\0') {
            name[i] = nameStr[i];
            i++;
        }
        name[i] = '\0';

        this->memberID = nextID++;
    }

    void displayDetails() {
        cout << "--- Member Details ---" << endl;
        cout << "ID: \t" << getID() << endl;
        cout << "Name: \t" << getName() << endl;
        cout << "----------------------" << endl;
    }

    const char* getName() { return name; }
    int getID() { return memberID; }
};

int Member::nextID = 1001;

const int MAX_SIZE = 100;

LibraryItem* g_items[MAX_SIZE];
Member* g_members[MAX_SIZE];
int g_itemCount = 0;
int g_memberCount = 0;

void addItem(LibraryItem* item) {
    if (g_itemCount < MAX_SIZE) {
        g_items[g_itemCount] = item;
        g_itemCount++;
    } else {
        cout << "Error: Library book storage is full." << endl;
    }
}

void addMember(Member* member) {
    if (g_memberCount < MAX_SIZE) {
        g_members[g_memberCount] = member;
        g_memberCount++;
    } else {
        cout << "Error: Library member storage is full." << endl;
    }
}

void displayAllItems() {
    cout << endl << "--- All Library Items ---" << endl;
    if (g_itemCount == 0) {
        cout << "(No items in library)" << endl;
        return;
    }
    for (int i = 0; i < g_itemCount; i++) {
        g_items[i]->displayDetails(); 
    }
}

void displayAllMembers() {
    cout << endl << "--- All Library Members ---" << endl;
    if (g_memberCount == 0) {
        cout << "(No members in library)" << endl;
        return;
    }
    for (int i = 0; i < g_memberCount; i++) {
        g_members[i]->displayDetails(); 
    }
}

LibraryItem* findBook(int itemID) {
    for (int i = 0; i < g_itemCount; i++) {
        if (g_items[i]->getID() == itemID) {
            return g_items[i];
        }
    }
    return nullptr;
}

Member* findMember(int memberID) {
    for (int i = 0; i < g_memberCount; i++) {
        if (g_members[i]->getID() == memberID) { 
            return g_members[i];
        }
    }
    return nullptr;
}

void issueBook(int bookID, int memberID) {
    LibraryItem* book = findBook(bookID);
    if (book == nullptr) {
        cout << "Error: Book not found." << endl;
        return;
    }

    Member* member = findMember(memberID);
    if (member == nullptr) {
        cout << "Error: Member not found." << endl;
        return;
    }

    if (book->getIsIssued() == 1) {
        cout << "Error: Book is already issued." << endl;
        return;
    }

    book->setIssued(1);
    
    cout << "\n--- RECEIPT ---" << endl;
    cout << "Book Issued Successfully." << endl;
    cout << "-----------------" << endl;
    cout << "Book Title: " << book->getTitle() << endl;
    cout << "Member Name: " << member->getName() << endl;
    cout << "Loan Period: 14 days." << endl;
    cout << "-----------------\n" << endl;
}

void saveData() {
    ofstream itemFile("items.csv");
    if (!itemFile) { 
        cout << "Error: Could not open items.csv for saving." << endl;
        return;
    }
    for (int i = 0; i < g_itemCount; i++) {
        Book* book = (Book*)g_items[i];
        itemFile << "B," << book->getID() << "," << book->getTitle() << ","
                 << book->author << "," << book->pageCount << "\n";
    }
    itemFile.close();

    ofstream memberFile("members.csv");
    if (!memberFile) { 
        cout << "Error: Could not open members.csv for saving." << endl;
        return;
    }
    for (int i = 0; i < g_memberCount; i++) {
        Member* member = g_members[i];
        memberFile << "M," << member->getID() << "," << member->getName() << "\n";
    }
    memberFile.close();
    cout << "Data saved successfully." << endl;
}

void loadData() {
    char line[256];
    
    ifstream itemFile("items.csv");
    if (itemFile) {
        while (itemFile.getline(line, 256)) {
            if (g_itemCount >= MAX_SIZE) break;

            char parts[5][100];
            int partIndex = 0;
            int charIndex = 0;
            
            for (int i = 0; line[i] != '\0'; i++) {
                if (line[i] == ',') {
                    parts[partIndex][charIndex] = '\0';
                    partIndex++;
                    charIndex = 0;
                } else {
                    parts[partIndex][charIndex] = line[i];
                    charIndex++;
                }
            }
            parts[partIndex][charIndex] = '\0';

            if (parts[0][0] == 'B') { 
                
                int id = 0;
                int i = 0;
                while (parts[1][i] != '\0') {
                    id = id * 10 + (parts[1][i] - '0');
                    i++;
                }

                int pages = 0;
                i = 0;
                while (parts[4][i] != '\0') {
                    pages = pages * 10 + (parts[4][i] - '0');
                    i++;
                }
                
                addItem(new Book(id, parts[2], parts[3], pages));
            }
        }
        itemFile.close();
    }

    ifstream memberFile("members.csv");
    if (memberFile) {
        while (memberFile.getline(line, 256)) {
            if (g_memberCount >= MAX_SIZE) break;
            
            char parts[3][100];
            int partIndex = 0;
            int charIndex = 0;
            
            for (int i = 0; line[i] != '\0'; i++) {
                if (line[i] == ',') {
                    parts[partIndex][charIndex] = '\0';
                    partIndex++;
                    charIndex = 0;
                } else {
                    parts[partIndex][charIndex] = line[i];
                    charIndex++;
                }
            }
            parts[partIndex][charIndex] = '\0';
            
            if (parts[0][0] == 'M') { 
                addMember(new Member(parts[2]));
            }
        }
        memberFile.close();
    }
    cout << "Data loaded." << endl;
}

void clearInput() {
    cin.clear();
    cin.ignore(100, '\n');
}

int main() {
    cout << "Library System Starting..." << endl;
    loadData();

    if (g_itemCount == 0) {
        cout << "Data files not found. Loading default books." << endl;
        addItem(new Book(101, "The C++ Book", "Some Guy", 100));
        addItem(new Book(102, "Another C++ Book", "Some Girl", 200));
        addItem(new Book(103, "C++ for Dummies", "A Smart Person", 300));
    }
    if (g_memberCount == 0) {
        cout << "Loading default member." << endl;
        addMember(new Member("Default User"));
    }

    int choice = -1;

    while (choice != 0) {
        cout << "\n--- Library Menu ---" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Add Member" << endl;
        cout << "3. Issue Book" << endl;
        cout << "4. Find Book" << endl;
        cout << "5. Find Member" << endl;
        cout << "6. Display All Items" << endl;
        cout << "7. Display All Members" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        
        cin >> choice;
        clearInput(); 

        switch (choice) {
            case 1: { 
                char title[100], author[100];
                int id, pages;
                cout << "Enter Book ID: "; cin >> id; clearInput();
                cout << "Enter Title: "; cin.getline(title, 100);
                cout << "Enter Author: "; cin.getline(author, 100);
                cout << "Enter Pages: "; cin >> pages; clearInput();
                addItem(new Book(id, title, author, pages));
                cout << "Book added!" << endl;
                break;
            }
            case 2: { 
                char name[100];
                cout << "Enter Name: "; cin.getline(name, 100);
                addMember(new Member(name));
                cout << "Member added!" << endl;
                break;
            }
            case 3: { 
                int bookID, memberID;
                cout << "Enter Book ID: "; cin >> bookID; clearInput();
                cout << "Enter Member ID: "; cin >> memberID; clearInput();
                issueBook(bookID, memberID); 
                break;
            }
            case 4: { 
                cout << "Enter Book ID: "; int id; cin >> id; clearInput();
                LibraryItem* book = findBook(id);
                if (book == nullptr) {
                    cout << "--- Book Not Found! ---" << endl;
                } else {
                    cout << "--- Book Found! ---" << endl;
                    book->displayDetails();
                }
                break;
            }
            case 5: { 
                cout << "Enter Member ID: "; int id; cin >> id; clearInput();
                Member* member = findMember(id);
                if (member == nullptr) {
                    cout << "--- Member Not Found! ---" << endl;
                } else {
                    cout << "--- Member Found! ---" << endl;
                    member->displayDetails();
                }
                break;
            }
            case 6: 
                displayAllItems();
                break;
            case 7: 
                displayAllMembers();
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
    
    cout << "Saving data before closing..." << endl;
    saveData();
    
    for (int i = 0; i < g_itemCount; i++) {
        delete g_items[i];
    }
    for (int i = 0; i < g_memberCount; i++) {
        delete g_members[i];
    }

    return 0; 
}

