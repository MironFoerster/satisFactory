#include <iostream>
#include <vector>
using namespace std;

class Course {
    private:
        string name;
    public:
        Course(string name, int n_students) {
            name = name;
            n_students = n_students;
        };
        string getInfo() {
            return name;
        }
};

class Student {
    private:
        string name;
    public:
        Student(string name) {
            name = name;
        };
        string getInfo() {
            return name;
        }
};

class Case {
    private:
        vector<Course> courses;
        vector<Student> students;

    public:
        int getNumCourses() {
            return courses.size();
        }
        int getNumStudents() {
            return courses.size();
        }
        void showCourses() {
            cout << "Aktuelle AGs:\n";
            if (getNumCourses() >= 1) {
                for (auto course : courses) {
                    cout << course.getInfo();
                }
            } else {
                cout << "Keine AGs";
            }
        }

        void showStudents() {
            cout << "Aktuelle Teilnehmer:\n";
            if (getNumStudents() >= 1) {
                for (auto student : students) {
                    cout << student.getInfo();
                }
            } else {
                cout << "Keine Teilnehmer";
            }
        }

        void addCourses() {
            bool adding_courses = true;
            while (adding_courses) {
                cout << "AG-Name eingeben (x eingeben um andere Aktion zu wählen): ";
                string name;
                cin >> name;
                if (name != "x") {
                    cout << "Teilnehmerzahl eingeben: ";
                    int n_students;
                    cin >> n_students;
                    
                    Course course(name, n_students);
                    courses.push_back(course);
                    cout << "AG " << name << " mit " << n_students << " Plätzen hinzugefügt";
                } else {
                    adding_courses = false;
                }
            }
        }
        void removeCourses() {
            bool removing_courses = true;
            while (removing_courses) {
                cout << "Kursnummer eingeben (x eingeben um andere Aktion zu wählen): ";
                string index;
                cin >> index;
                if (index != "x") {
                    int index = index;
                    courses.erase(courses.begin() + index);
                } else {
                    removing_courses = false;
                }
            }
        }
};



int main()
{
    
    Case useCase;

    cout << "Willkommen zur AG Einteilungs App!\n";
    bool course_edit = true;
    bool students_edit = false;
    while (!course_edit) {
        
        cout << "Zuerst müssen alle AGs und deren maximale Teilnehmerzahl eingetragen werden:\n";
        cout << "Ziffer eingeben um Aktion zu wählen:\n";
        cout << "\t1 - AGs hinzufügen\n";
        cout << "\t2 - AGs entfernen\n";
        cout << "\t3 - Aktuelle AGs anzeigen\n";
        cout << "\t4 - AGs bestätigen und zum Eintragen der Teilnehmer übergehen Achtung: Beim Eintragen der Teilnehmer können die AGs nicht mehr bearbeitet werden!";
        int action;
        cin >> action;

        switch(action) {
            case 1:
                useCase.addCourses();
            case 2:
                useCase.removeCourses();
            case 3:
                useCase.addCourses();
            case 4:
                course_edit = false;
                students_edit = true;
        }

        cout << "\t1 - Teilnehmer hinzufügen\n";
        cout << "\t2 - Teilnehmer entfernen\n";
        cout << "\t3 - Aktuelle Teilnehmer anzeigen\n";

        int n_courses;
        cin >> n_courses;
    }
    

    
}