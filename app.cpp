#include <iostream>
#include <algorithm>
//#include <iterator> // for ostream_iterator
#include <string>
#include <vector>
#include <cmath>
using namespace std;

int N_PREFS;

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
        vector<int> prefs;
        int curr_score;
    public:
        Student(string name, vector<int> prefs) {
            name = name;
            prefs = prefs;
        };
        string getInfo() {
            return name;
        }

        void moveTo(int i_course) {
            std::vector<int>::iterator iter = std::find(prefs.begin(), prefs.end(), i_course);
  
            // If element was found
            if (iter != prefs.end())
            {
                int index = iter - prefs.begin();
                curr_score = index;
            } else {
                curr_score = std::pow(N_PREFS, 2);
            }
        }

        int getMoveToScore(int i_course) { // negative score is making things better
            int moveto_score;
            int next_score;
            int index;

            std::vector<int>::iterator iter = std::find(prefs.begin(), prefs.end(), i_course);
  
            // If element was found
            if (iter != prefs.end()) 
            {
                index = iter - prefs.begin();
            } else {
                index = N_PREFS;
            }

            int next_score = std::pow(index, 2);

            moveto_score = next_score - curr_score;
            return moveto_score;
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
                    cout << "AG " + name + " mit " + to_string(n_students) + " Plätzen hinzugefügt";
                } else {
                    adding_courses = false;
                }
            }
        }
        void removeCourses() {
            bool removing_courses = true;
            while (removing_courses) {
                cout << "Kursnummer eingeben (x eingeben um andere Aktion zu wählen): ";
                string input;
                cin >> input;
                if (input != "x") {
                    int index = stoi(input);
                    courses.erase(courses.begin() + index);
                } else {
                    removing_courses = false;
                }
            }
        }

        void addStudents() {
            bool adding_students = true;
            while (adding_students) {
                cout << "Teilnehmer-Name eingeben (x eingeben um andere Aktion zu wählen): ";
                string name;
                cin >> name;
                if (name != "x") {
                    vector<int> prefs;
                    for (int n_pref = 0; n_pref < 3; n_pref++) {
                        cout << to_string(n_pref) + ". Wunsch(Zahl): ";
                        int pref;
                        cin >> pref;
                        prefs.push_back(pref);
                    }

                    Student student(name, prefs);
                    students.push_back(student);
                    cout << "Teilnehmer " + name + " mit Wünschen ";
                    // prints all prefs
                    //std::copy(prefs.begin(), prefs.end(), std::ostream_iterator<char>(std::cout, " "));
                    cout << " hinzugefügt\n";
                } else {
                    adding_students = false;
                }
            }
        }
        void removeStudents() {
            bool removing_students = true;
            while (removing_students) {
                cout << "Teilnehmernummer eingeben (x eingeben um andere Aktion zu wählen): ";
                string input;
                cin >> input;
                if (input != "x") {
                    int index = stoi(input);
                    students.erase(students.begin() + index);
                } else {
                    removing_students = false;
                }
            }
        }
        void FCFSAssignment() {
            // First Come First Serve Assignment
            for (auto student : students) {

            }

        }

        void smartAssignment() {

        }

        void optimizeAssignment() {

        }
        for each course compute what studs could join based on how dissatisfied they are
};



int main()
{   
    cout << "Willkommen zur AG Einteilungs App!\n";

    Case useCase;

    cout << "Zuerst müssen alle AGs und deren maximale Teilnehmerzahl eingetragen werden:\n";
    bool course_edit = true;
    bool students_edit = false;
    while (course_edit) {
        
        cout << "Aktionen:\n";
        cout << "\t1 - AGs hinzufügen\n";
        cout << "\t2 - AGs entfernen\n";
        cout << "\t3 - Aktuelle AGs anzeigen\n";
        cout << "\t4 - !AGs bestätigen und zum Eintragen der Teilnehmer übergehen!";
        cout << "Ziffer eingeben um Aktion zu wählen:";
        int action;
        cin >> action;

        switch(action) {
            case 1:
                useCase.addCourses();
            case 2:
                useCase.removeCourses();
            case 3:
                useCase.showCourses();
            case 4:
                course_edit = false;
                students_edit = true;
        }
    }
    cout << "Anzahl der Wünsche pro Teilnehmer eingeben:";
    cin >> N_PREFS;
    while (students_edit) {
        cout << "Aktionen:\n";
        cout << "\t1 - Teilnehmer hinzufügen\n";
        cout << "\t2 - Teilnehmer entfernen\n";
        cout << "\t3 - Aktuelle Teilnehmer anzeigen\n";
        cout << "\t4 - !Teilnehmer Bestätigen und AG-Zuordnung generieren!\n";
        cout << "Ziffer eingeben um Aktion zu wählen:";
        int action;
        cin >> action;
        switch(action) {
            case 1:
                useCase.addStudents();
            case 2:
                useCase.removeStudents();
            case 3:
                useCase.showStudents();
            case 4:
                course_edit = true;
                students_edit = false;
                useCase.FCFSAssignment();
                useCase.optimizeAssignment();
        }
    }
    

    
}