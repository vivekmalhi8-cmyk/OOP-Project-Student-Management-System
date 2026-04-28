#include <iostream>
#include <fstream>
#include <string>
using namespace std;
const int MAX_STUDENTS   = 50;
const int TOTAL_SUBJECTS = 5;
const string FILE_NAME   = "students.txt";
template <typename T>
T findMax(T arr[], int size) {
    T maxVal = arr[0];
    for (int i = 1; i < size; i++)
        if (arr[i] > maxVal) maxVal = arr[i];
    return maxVal;
}
template <typename T>
T findMin(T arr[], int size) {
    T minVal = arr[0];
    for (int i = 1; i < size; i++)
        if (arr[i] < minVal) minVal = arr[i];
    return minVal;
}
void printLine(char ch = '-', int len = 60) {
    for (int i = 0; i < len; i++) cout << ch;
    cout << "\n";
}
void printHeader(const string& title) {
    printLine('=');
    cout << "  FAST-NUCES Karachi | Student Management System\n";
    printLine('=');
    cout << "  " << title << "\n";
    printLine('-');
}

void pauseScreen() {
    cout << "\n  Press Enter to continue...";
    cin.ignore();
    cin.get();
}
class StudentException {
private:
    string message;
public:
    StudentException(const string& msg) : message(msg) {}
    string getMessage() const { return message; }
};
class DuplicateRollException : public StudentException {
public:
    DuplicateRollException(int roll)
        : StudentException("Student with Roll No " + to_string(roll) + " already exists.") {}
};
class StudentNotFoundException : public StudentException {
public:
    StudentNotFoundException(int roll)
        : StudentException("Student with Roll No " + to_string(roll) + " not found.") {}
};
class InvalidMarksException : public StudentException {
public:
    InvalidMarksException()
        : StudentException("Marks must be between 0 and 100.") {}
};
class StorageFullException : public StudentException {
public:
    StorageFullException()
        : StudentException("Storage is full. Cannot add more students.") {}
};
class FileException : public StudentException {
public:
    FileException(const string& msg) : StudentException(msg) {}
};
class Person {
private:
    string name;
    int  age;
public:
    Person() : name(""), age(0) {}
    Person(const string& n, int a) : name(n), age(a) {}
    void setName(const string& n) { name = n; }
    void setAge(int a) {
        if (a > 0 && a < 120) age = a;
        else cout << "  [!] Invalid age.\n";
    }
    string getName() const { return name; }
    int    getAge()  const { return age;  }
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
    float  marks[TOTAL_SUBJECTS];
    string subjectNames[TOTAL_SUBJECTS];
    float  totalMarks;
    float  percentage;
    string grade;
    static int totalStudents;
    void calculateGrade() {
        totalMarks = 0;
        for (int i = 0; i < TOTAL_SUBJECTS; i++)
            totalMarks += marks[i];
        percentage = totalMarks / (float)TOTAL_SUBJECTS;
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
    Student() : Person(), rollNumber(0), section(""),
                totalMarks(0), percentage(0), grade("F") {
        string defaults[TOTAL_SUBJECTS] = {"Math","Physics","English","CS","OOP"};
        for (int i = 0; i < TOTAL_SUBJECTS; i++) {
            marks[i]        = 0;
            subjectNames[i] = defaults[i];
        }
        totalStudents++;
    }
    Student(const string& name, int age, int roll,
            const string& sec, float m[TOTAL_SUBJECTS])
        : Person(name, age), rollNumber(roll), section(sec),
          totalMarks(0), percentage(0), grade("") {
        string defaults[TOTAL_SUBJECTS] = {"Math","Physics","English","CS","OOP"};
        for (int i = 0; i < TOTAL_SUBJECTS; i++) {
            marks[i]        = m[i];
            subjectNames[i] = defaults[i];
        }
        calculateGrade();
        totalStudents++;
    }
    ~Student() { totalStudents--; }
    void setRollNumber(int r)        { rollNumber = r; }
    void setSection(const string& s) { section    = s; }
    void setMarks(float m[TOTAL_SUBJECTS]) {
        for (int i = 0; i < TOTAL_SUBJECTS; i++)
            marks[i] = m[i];
        calculateGrade();
    }
    int    getRollNumber() const { return rollNumber; }
    string getSection()    const { return section;    }
    float  getTotalMarks() const { return totalMarks; }
    float  getPercentage() const { return percentage; }
    string getGrade()      const { return grade;      }
    float  getMarks(int i) const {
        return (i >= 0 && i < TOTAL_SUBJECTS) ? marks[i] : 0;
    }
    static int getTotalStudents() { return totalStudents; }
    void display() const override {
        Person::display();
        cout << "  Roll No    : " << rollNumber << "\n";
        cout << "  Section    : " << section    << "\n";
        printLine('-');
        cout << "  Subject          Marks\n";
        cout << "  " << string(25, '-') << "\n";
        for (int i = 0; i < TOTAL_SUBJECTS; i++)
            cout << "  " << subjectNames[i] << "\t\t " << marks[i] << "\n";
        printLine('-');
        cout << "  Total      : " << totalMarks << " / 500\n";
        cout << "  Percentage : " << percentage << "%\n";
        cout << "  Grade      : " << grade      << "\n";
    }
    void display(bool compact) const {
        if (compact) {
            cout << "  Roll: " << rollNumber
                 << "  Name: " << getName()
                 << "  Section: " << section
                 << "  " << percentage << "%"
                 << "  Grade: " << grade << "\n";
        } else {
            display();
        }
    }
};
int Student::totalStudents = 0;
class StudentManager {
private:
    Student students[MAX_STUDENTS];
    int     count;
    int findStudent(int roll) const {
        for (int i = 0; i < count; i++)
            if (students[i].getRollNumber() == roll) return i;
        return -1;
    }
public:
    StudentManager() : count(0) {}
    void addStudent() {
        printHeader("ADD NEW STUDENT");
        try {
            if (count >= MAX_STUDENTS)
                throw StorageFullException();
            string name, section;
            int    age, roll;
            float  m[TOTAL_SUBJECTS];
            cout << "  Enter Name        : ";
            getline(cin, name);
            cout << "  Enter Age         : ";
            cin >> age;
            cin.ignore();
            cout << "  Enter Roll Number : ";
            cin >> roll;
            cin.ignore();
            if (findStudent(roll) != -1)
                throw DuplicateRollException(roll);
            cout << "  Enter Section     : ";
            getline(cin, section);
            string subjects[TOTAL_SUBJECTS] = {"Math","Physics","English","CS","OOP"};
            cout << "\n  Enter marks (0-100) for each subject:\n";
            for (int i = 0; i < TOTAL_SUBJECTS; i++) {
                cout << "    " << subjects[i] << " : ";
                cin >> m[i];
                cin.ignore();
                if (m[i] < 0 || m[i] > 100)
                    throw InvalidMarksException();
            }
            students[count] = Student(name, age, roll, section, m);
            count++;
            cout << "\n  [+] Student added successfully!\n";
        } catch (StudentException& e) {
            cout << "\n  [!] Error: " << e.getMessage() << "\n";
        }
        pauseScreen();
    }
    void displayAll() {
        printHeader("ALL STUDENTS");
        if (count == 0) {
            cout << "  No records found.\n";
            pauseScreen();
            return;
        }
        for (int i = 0; i < count; i++)
            students[i].display(true);
        printLine('-');
        cout << "  Total enrolled: " << count << "\n";
        pauseScreen();
    }
    void searchStudent() {
        printHeader("SEARCH STUDENT");
        try {
            int roll;
            cout << "  Enter Roll Number: ";
            cin >> roll;
            cin.ignore();
            int idx = findStudent(roll);
            if (idx == -1)
                throw StudentNotFoundException(roll);
            cout << "\n";
            printLine('-');
            students[idx].display();
            printLine('-');
        } catch (StudentException& e) {
            cout << "\n  [!] Error: " << e.getMessage() << "\n";
        }
        pauseScreen();
    }
    void updateStudent() {
        printHeader("UPDATE STUDENT");
        try {
            int roll;
            cout << "  Enter Roll Number to update: ";
            cin >> roll;
            cin.ignore();
            int idx = findStudent(roll);
            if (idx == -1)
                throw StudentNotFoundException(roll);
            cout << "\n  What to update?\n";
            cout << "  [1] Name\n  [2] Age\n  [3] Section\n  [4] Marks\n  [5] All\n";
            cout << "  Choice: ";
            int choice;
            cin >> choice;
            cin.ignore();
            if (choice == 1 || choice == 5) {
                string name;
                cout << "  New Name    : ";
                getline(cin, name);
                students[idx].setName(name);
            }
            if (choice == 2 || choice == 5) {
                int age;
                cout << "  New Age     : ";
                cin >> age;
                cin.ignore();
                students[idx].setAge(age);
            }
            if (choice == 3 || choice == 5) {
                string sec;
                cout << "  New Section : ";
                getline(cin, sec);
                students[idx].setSection(sec);
            }
            if (choice == 4 || choice == 5) {
                string subjects[TOTAL_SUBJECTS] = {"Math","Physics","English","CS","OOP"};
                float m[TOTAL_SUBJECTS];
                cout << "  Enter new marks:\n";
                for (int i = 0; i < TOTAL_SUBJECTS; i++) {
                    cout << "    " << subjects[i] << " : ";
                    cin >> m[i];
                    cin.ignore();
                    if (m[i] < 0 || m[i] > 100)
                        throw InvalidMarksException();
                }
                students[idx].setMarks(m);
            }
            cout << "\n  [+] Record updated successfully!\n";
        } catch (StudentException& e) {
            cout << "\n  [!] Error: " << e.getMessage() << "\n";
        }
        pauseScreen();
    }
    void deleteStudent() {
        printHeader("DELETE STUDENT");
        try {
            int roll;
            cout << "  Enter Roll Number to delete: ";
            cin >> roll;
            cin.ignore();
            int idx = findStudent(roll);
            if (idx == -1)
                throw StudentNotFoundException(roll);
            cout << "\n  Student: " << students[idx].getName() << "\n";
            cout << "  Confirm delete? (y/n): ";
            char confirm;
            cin >> confirm;
            cin.ignore();
            if (confirm == 'y' || confirm == 'Y') {
                for (int i = idx; i < count - 1; i++)
                    students[i] = students[i + 1];
                count--;
                cout << "\n  [+] Student deleted.\n";
            } else {
                cout << "\n  Deletion cancelled.\n";
            }
        } catch (StudentException& e) {
            cout << "\n  [!] Error: " << e.getMessage() << "\n";
        }
        pauseScreen();
    }
    void gradeReport() {
        printHeader("GRADE REPORT");
        if (count == 0) {
            cout << "  No records available.\n";
            pauseScreen();
            return;
        }
        string grades[] = {"A+","A","A-","B+","B","B-","C+","C","D","F"};
        for (int g = 0; g < 10; g++) {
            bool found = false;
            for (int i = 0; i < count; i++)
                if (students[i].getGrade() == grades[g]) { found = true; break; }
            if (!found) continue;
            cout << "\n  Grade " << grades[g] << ":\n";
            for (int i = 0; i < count; i++) {
                if (students[i].getGrade() == grades[g]) {
                    cout << "    Roll " << students[i].getRollNumber()
                         << "  " << students[i].getName()
                         << "  (" << students[i].getPercentage() << "%)\n";
                }
            }
        }
        cout << "\n  Total Students: " << count << "\n";
        pauseScreen();
    }
    void showStatistics() {
        printHeader("STATISTICS");
        if (count == 0) {
            cout << "  No records available.\n";
            pauseScreen();
            return;
        }
        float percentages[MAX_STUDENTS];
        float sum = 0;
        for (int i = 0; i < count; i++) {
            percentages[i] = students[i].getPercentage();
            sum += percentages[i];
        }
        float avg     = sum / count;
        float highest = findMax(percentages, count);
        float lowest  = findMin(percentages, count);
        int topIdx = 0, lowIdx = 0;
        int pass = 0, fail = 0;
        for (int i = 0; i < count; i++) {
            if (students[i].getPercentage() == highest) topIdx = i;
            if (students[i].getPercentage() == lowest)  lowIdx = i;
            if (students[i].getGrade() == "F") fail++;
            else pass++;
        }
        cout << "  Total Students : " << count   << "\n";
        cout << "  Class Average  : " << avg      << "%\n";
        cout << "  Highest Score  : " << highest  << "%  - "
             << students[topIdx].getName()
             << " (Roll " << students[topIdx].getRollNumber() << ")\n";
        cout << "  Lowest Score   : " << lowest   << "%  - "
             << students[lowIdx].getName()
             << " (Roll " << students[lowIdx].getRollNumber() << ")\n";
        cout << "  Passed         : " << pass << "\n";
        cout << "  Failed         : " << fail << "\n";
        pauseScreen();
    }
    void saveToFile() {
        printHeader("SAVE TO FILE");
        try {
            ofstream file(FILE_NAME);
            if (!file)
                throw FileException("Could not open file: " + FILE_NAME);
            file << count << "\n";
            for (int i = 0; i < count; i++) {
                file << students[i].getName()       << "\n";
                file << students[i].getAge()        << "\n";
                file << students[i].getRollNumber() << "\n";
                file << students[i].getSection()    << "\n";
                for (int j = 0; j < TOTAL_SUBJECTS; j++) {
                    file << students[i].getMarks(j);
                    if (j < TOTAL_SUBJECTS - 1) file << " ";
                }
                file << "\n";
            }
            file.close();
            cout << "\n  [+] Data saved to \"" << FILE_NAME << "\" successfully!\n";
        } catch (StudentException& e) {
            cout << "\n  [!] Error: " << e.getMessage() << "\n";
        }
        pauseScreen();
    }
    void loadFromFile() {
        try {
            ifstream file(FILE_NAME);
            if (!file)
                throw FileException("No save file found. Starting fresh.");
            int fileCount = 0;
            file >> fileCount;
            file.ignore();
            count = 0;
            for (int i = 0; i < fileCount; i++) {
                string name, section;
                int    age, roll;
                float  m[TOTAL_SUBJECTS];
                getline(file, name);
                file >> age;   file.ignore();
                file >> roll;  file.ignore();
                getline(file, section);
                for (int j = 0; j < TOTAL_SUBJECTS; j++)
                    file >> m[j];
                file.ignore();
                students[count] = Student(name, age, roll, section, m);
                count++;
            }
            file.close();
            cout << "  [+] " << count << " record(s) loaded from \""
                 << FILE_NAME << "\".\n";
        } catch (StudentException& e) {
            cout << "  [!] " << e.getMessage() << "\n";
        }
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
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: manager.addStudent();     break;
            case 2: manager.displayAll();     break;
            case 3: manager.searchStudent();  break;
            case 4: manager.updateStudent();  break;
            case 5: manager.deleteStudent();  break;
            case 6: manager.gradeReport();    break;
            case 7: manager.showStatistics(); break;
            case 8: manager.saveToFile();     break;
            case 0:
                manager.saveToFile();
                printHeader("GOODBYE");
                cout << "  Thank you for using the Student Management System!\n";
                cout << "  FAST-NUCES Karachi\n\n";
                break;
            default:
                cout << "\n  [!] Invalid choice. Try again.\n";
                pauseScreen();
        }
    } while (choice != 0);
    return 0;
}
