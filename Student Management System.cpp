#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <fstream>
using namespace std;
const string FILE_NAME = "students.txt";
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void pauseScreen() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
void printLine(char ch = '-', int len = 60) {
    cout << string(len, ch) << "\n";
}
void printHeader(const string& title) {
    clearScreen();
    printLine('=');
    cout << "  FAST-NUCES Karachi | Student Management System\n";
    printLine('=');
    cout << "  " << title << "\n";
    printLine('-');
}
class Person {
private:
    string name;
    int    age;
public:
    Person() : name(""), age(0) {}
    Person(const string& n, int a) : name(n), age(a) {}
    string getName() const { return name; }
    int    getAge()  const { return age;  }
    void setName(const string& n) { name = n; }
    void setAge(int a) {
        if (a > 0 && a < 120) age = a;
        else cout << "  [!] Invalid age entered.\n";
    }
    virtual void display() const {
        cout << "  Name : " << name << "\n";
        cout << "  Age  : " << age  << "\n";
    }
    virtual ~Person() {}
};
class Student : public Person {
private:
    int    rollNumber;
    string section;
    float  marks[5];
    string subjectNames[5];
    float  totalMarks;
    float  percentage;
    string grade;

    static int totalStudents;
    void calculateGrade() {
        totalMarks = 0;
        for (int i = 0; i < 5; i++) totalMarks += marks[i];
        percentage = totalMarks / 5.0f;
        if      (percentage >= 90) grade = "A+";
        else if (percentage >= 85) grade = "A";
        else if (percentage >= 80) grade = "A-";
        else if (percentage >= 75) grade = "B+";
        else if (percentage >= 70) grade = "B";
        else if (percentage >= 65) grade = "B-";
        else if (percentage >= 60) grade = "C+";
        else if (percentage >= 55) grade = "C";
        else if (percentage >= 50) grade = "D";
        else                       grade = "F";
    }
public:
    Student() : Person(), rollNumber(0), section(""), totalMarks(0),
                percentage(0), grade("") {
        string defaults[5] = {"Math","Physics","English","CS","OOP"};
        for (int i = 0; i < 5; i++) {
            marks[i]       = 0;
            subjectNames[i] = defaults[i];
        }
        totalStudents++;
    }
    Student(const string& name, int age, int roll,
            const string& sec, float m[5])
        : Person(name, age), rollNumber(roll), section(sec) {
        string defaults[5] = {"Math","Physics","English","CS","OOP"};
        for (int i = 0; i < 5; i++) {
            marks[i]        = m[i];
            subjectNames[i] = defaults[i];
        }
        calculateGrade();
        totalStudents++;
    }
    ~Student() { totalStudents--; }
    int    getRollNumber()  const { return rollNumber; }
    string getSection()     const { return section;    }
    float  getTotalMarks()  const { return totalMarks; }
    float  getPercentage()  const { return percentage; }
    string getGrade()       const { return grade;      }
    void setRollNumber(int r)        { rollNumber = r;  }
    void setSection(const string& s) { section   = s;  }
    void setMarks(float m[5]) {
        for (int i = 0; i < 5; i++) marks[i] = m[i];
        calculateGrade();
    }
    void setSubjectName(int index, const string& sName) {
        if (index >= 0 && index < 5) subjectNames[index] = sName;
    }
    static int getTotalStudents() { return totalStudents; }
    float getMarks(int i) const {
        if (i >= 0 && i < 5) return marks[i];
        return 0;
    }
    void display() const override {
        Person::display();
        cout << "  Roll No   : " << rollNumber  << "\n";
        cout << "  Section   : " << section     << "\n";
        printLine('-');
        cout << "  Subject Results:\n";
        cout << "  " << left << setw(15) << "Subject"
             << setw(10) << "Marks(/100)" << "\n";
        cout << "  " << string(25, '-') << "\n";
        for (int i = 0; i < 5; i++) {
            cout << "  " << left  << setw(15) << subjectNames[i]
                 << setw(10) << fixed << setprecision(1) << marks[i] << "\n";
        }
        printLine('-');
        cout << "  Total     : " << fixed << setprecision(1) << totalMarks << " / 500\n";
        cout << "  Percentage: " << fixed << setprecision(2) << percentage  << "%\n";
        cout << "  Grade     : " << grade  << "\n";
    }
    void display(bool compact) const {
        if (compact) {
            cout << "  " << left  << setw(5)  << rollNumber
                 << setw(22) << getName()
                 << setw(10) << section
                 << setw(10) << fixed << setprecision(2) << percentage << "%"
                 << setw(5)  << grade << "\n";
        } else {
            display();
        }
    }
};
int Student::totalStudents = 0;
class StudentManager {
private:
    vector<Student> students;
    int getIntInput(const string& prompt) {
        int val;
        while (true) {
            cout << prompt;
            if (cin >> val) { cin.ignore(); return val; }
            cout << "  [!] Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    float getFloatInput(const string& prompt) {
        float val;
        while (true) {
            cout << prompt;
            if (cin >> val && val >= 0 && val <= 100) {
                cin.ignore();
                return val;
            }
            cout << "  [!] Enter a valid marks value (0–100).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    string getStringInput(const string& prompt) {
        string val;
        cout << prompt;
        getline(cin, val);
        return val;
    }
    int findStudent(int roll) const {
        for (int i = 0; i < (int)students.size(); i++)
            if (students[i].getRollNumber() == roll) return i;
        return -1;
    }
    bool rollExists(int roll) const {
        return findStudent(roll) != -1;
    }
public:
    void addStudent() {
        printHeader("ADD NEW STUDENT");
        string name    = getStringInput("  Enter Name        : ");
        int    age     = getIntInput   ("  Enter Age         : ");
        int    roll    = getIntInput   ("  Enter Roll Number : ");
        if (rollExists(roll)) {
            cout << "\n  [!] A student with Roll No " << roll
                 << " already exists.\n";
            pauseScreen();
            return;
        }
        string section = getStringInput("  Enter Section     : ");
        cout << "\n  Enter marks (out of 100) for 5 subjects:\n";
        string subjects[5] = {"Math","Physics","English","CS","OOP"};
        float m[5];
        for (int i = 0; i < 5; i++) {
            m[i] = getFloatInput("    " + subjects[i] + " : ");
        }
        Student s(name, age, roll, section, m);
        students.push_back(s);
        cout << "\n  [✓] Student added successfully!\n";
        pauseScreen();
    }
    void displayAll() {
        printHeader("ALL STUDENTS");
        if (students.empty()) {
            cout << "  No records found.\n";
            pauseScreen();
            return;
        }
        cout << "  " << left << setw(5)  << "Roll"
             << setw(22) << "Name"
             << setw(10) << "Section"
             << setw(12) << "Percentage"
             << setw(5)  << "Grade" << "\n";
        printLine('-');
        for (const auto& s : students)
            s.display(true);
        printLine('-');
        cout << "  Total enrolled: " << Student::getTotalStudents() << "\n";
        pauseScreen();
    }
    void searchStudent() {
        printHeader("SEARCH STUDENT");
        int roll = getIntInput("  Enter Roll Number to search: ");
        int idx  = findStudent(roll);
        if (idx == -1) {
            cout << "\n  [!] Student with Roll No " << roll << " not found.\n";
        } else {
            cout << "\n";
            printLine('-');
            students[idx].display();
            printLine('-');
        }
        pauseScreen();
    }
    void updateStudent() {
        printHeader("UPDATE STUDENT");
        int roll = getIntInput("  Enter Roll Number to update: ");
        int idx  = findStudent(roll);
        if (idx == -1) {
            cout << "\n  [!] Student not found.\n";
            pauseScreen();
            return;
        }
        cout << "\n  What would you like to update?\n";
        cout << "  [1] Name\n";
        cout << "  [2] Age\n";
        cout << "  [3] Section\n";
        cout << "  [4] Marks\n";
        cout << "  [5] All of the above\n";
        int choice = getIntInput("  Choice: ");
        if (choice == 1 || choice == 5) {
            string name = getStringInput("  New Name    : ");
            students[idx].setName(name);
        }
        if (choice == 2 || choice == 5) {
            int age = getIntInput("  New Age     : ");
            students[idx].setAge(age);
        }
        if (choice == 3 || choice == 5) {
            string sec = getStringInput("  New Section : ");
            students[idx].setSection(sec);
        }
        if (choice == 4 || choice == 5) {
            cout << "  Enter new marks for 5 subjects:\n";
            string subjects[5] = {"Math","Physics","English","CS","OOP"};
            float m[5];
            for (int i = 0; i < 5; i++)
                m[i] = getFloatInput("    " + subjects[i] + " : ");
            students[idx].setMarks(m);
        }
        cout << "\n  [✓] Record updated successfully!\n";
        pauseScreen();
    }
    void deleteStudent() {
        printHeader("DELETE STUDENT");
        int roll = getIntInput("  Enter Roll Number to delete: ");
        int idx  = findStudent(roll);
        if (idx == -1) {
            cout << "\n  [!] Student not found.\n";
            pauseScreen();
            return;
        }
        cout << "\n  Student found: " << students[idx].getName() << "\n";
        cout << "  Are you sure you want to delete? (y/n): ";
        char confirm;
        cin >> confirm;
        cin.ignore();
        if (confirm == 'y' || confirm == 'Y') {
            students.erase(students.begin() + idx);
            cout << "\n  [✓] Student deleted successfully!\n";
        } else {
            cout << "\n  Deletion cancelled.\n";
        }
        pauseScreen();
    }
    void gradeReport() {
        printHeader("GRADE REPORT");
        if (students.empty()) {
            cout << "  No records available.\n";
            pauseScreen();
            return;
        }
        string grades[] = {"A+","A","A-","B+","B","B-","C+","C","D","F"};
        for (const auto& g : grades) {
            bool found = false;
            for (const auto& s : students)
                if (s.getGrade() == g) { found = true; break; }
            if (!found) continue;
            cout << "  Grade " << g << ":\n";
            for (const auto& s : students) {
                if (s.getGrade() == g) {
                    cout << "    Roll " << s.getRollNumber()
                         << " — " << s.getName()
                         << "  (" << fixed << setprecision(2)
                         << s.getPercentage() << "%)\n";
                }
            }
        }
        cout << "\n  Total Students: " << Student::getTotalStudents() << "\n";
        pauseScreen();
    }
    void saveToFile() {
        ofstream file(FILE_NAME);
        if (!file) {
            cout << "\n  [!] Could not open file for saving.\n";
            pauseScreen();
            return;
        }
        file << students.size() << "\n";
        for (const auto& s : students) {
            file << s.getName()       << "\n";
            file << s.getAge()        << "\n";
            file << s.getRollNumber() << "\n";
            file << s.getSection()    << "\n";
            for (int i = 0; i < 5; i++) {
                file << s.getMarks(i);
                if (i < 4) file << " ";
            }
            file << "\n";
        }
        file.close();
        cout << "\n  [✓] Data saved to \"" << FILE_NAME << "\" successfully!\n";
        pauseScreen();
    }
    void loadFromFile() {
        ifstream file(FILE_NAME);

        if (!file) {
            return;
        }
        int count = 0;
        file >> count;
        file.ignore();
        students.clear();
        for (int i = 0; i < count; i++) {
            string name, section;
            int    age, roll;
            float  m[5];
            getline(file, name);
            file >> age;   file.ignore();
            file >> roll;  file.ignore();
            getline(file, section);
            for (int j = 0; j < 5; j++) file >> m[j];
            file.ignore();
            Student s(name, age, roll, section, m);
            students.push_back(s);
        }
        file.close();
        cout << "  [✓] " << students.size()
             << " record(s) loaded from \"" << FILE_NAME << "\".\n";
    }
    void showStatistics() {
        printHeader("STATISTICS");
        if (students.empty()) {
            cout << "  No records available.\n";
            pauseScreen();
            return;
        }
        float highest = students[0].getPercentage();
        float lowest  = students[0].getPercentage();
        float sum     = 0;
        int   topIdx  = 0, lowIdx = 0;
        for (int i = 0; i < (int)students.size(); i++) {
            float p = students[i].getPercentage();
            sum += p;
            if (p > highest) { highest = p; topIdx = i; }
            if (p < lowest)  { lowest  = p; lowIdx = i; }
        }
        float avg = sum / students.size();
        cout << "  Total Students  : " << Student::getTotalStudents() << "\n";
        cout << "  Class Average   : " << fixed << setprecision(2) << avg << "%\n";
        cout << "  Highest Score   : " << highest << "% — "
             << students[topIdx].getName() << " (Roll "
             << students[topIdx].getRollNumber() << ")\n";
        cout << "  Lowest Score    : " << lowest  << "% — "
             << students[lowIdx].getName() << " (Roll "
             << students[lowIdx].getRollNumber() << ")\n";
        int pass = 0, fail = 0;
        for (const auto& s : students) {
            if (s.getGrade() == "F") fail++;
            else pass++;
        }
        cout << "  Passed          : " << pass << "\n";
        cout << "  Failed          : " << fail << "\n";
        pauseScreen();
    }
};
int main() {
    StudentManager manager;
    int choice;
    printHeader("LOADING DATA");
    manager.loadFromFile();
    pauseScreen();
    do {
        printHeader("MAIN MENU");
        cout << "  [1] Add New Student\n";
        cout << "  [2] Display All Students\n";
        cout << "  [3] Search Student\n";
        cout << "  [4] Update Student Record\n";
        cout << "  [5] Delete Student\n";
        cout << "  [6] Grade Report\n";
        cout << "  [7] Statistics\n";
        cout << "  [8] Save Data to File\n";
        cout << "  [0] Save & Exit\n";
        printLine('-');
        cout << "  Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        } else {
            cin.ignore();
        }
        switch (choice) {
            case 1: manager.addStudent();      break;
            case 2: manager.displayAll();      break;
            case 3: manager.searchStudent();   break;
            case 4: manager.updateStudent();   break;
            case 5: manager.deleteStudent();   break;
            case 6: manager.gradeReport();     break;
            case 7: manager.showStatistics();  break;
            case 8: manager.saveToFile();      break;
            case 0:
                manager.saveToFile();
                printHeader("GOODBYE");
                cout << "  Thank you for using Student Management System!\n";
                cout << "  FAST-NUCES Karachi | OOP Project\n\n";
                break;
            default:
                cout << "\n  [!] Invalid choice. Try again.\n";
                pauseScreen();
        }
    } while (choice != 0);
    return 0;
}