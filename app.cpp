#include <iostream>
#include <algorithm>
//#include <iterator> // for ostream_iterator
#include <string>
#include <vector>
#include <cmath>
#include <map>

int N_PREFS;
int N_COURSES;
int N_STUDENTS;

class Student;
class Course;

template<typename T>
int indexof(T& obj, std::vector<T>& vec) {
    auto it = std::find(vec.begin(), vec.end(), obj);
    if (it != vec.end()) {
        return std::distance(vec.begin(), it);
    } else {
        return -1;
    }
}


class Student {
    private:
        std::string name;
        // list of course indices with decreasing preference
        std::vector<Course&> prefs;
        int curr_score;
    public:
        Student(std::string name, std::vector<Course&> prefs) {
            name = name;
            prefs = prefs;
        };

        bool operator==(const Student& other) {
            return name == other.name;
        }

        std::string getInfo() {
            return name;
        }

        int getCurrScore() {
            return curr_score;
        }

        Course getWish(int i_wish) {
            return prefs[i_wish];
        }

        int getCourseScore(Course& course) {
            int index = indexof<Course&>(course, prefs);
            // If element was found
            if (index == -1) {
                return std::pow(N_PREFS, 2);
            } else {
                return std::pow(index, 2);
            }
        }

        void updateCurrentScore(Course& course) {
            curr_score = getCourseScore(course);
        }

        int getMoveToScoreD(Course& course) { // negative scoreD is making things better
            int moveto_scoreD;
            int next_score = getCourseScore(course);

            moveto_scoreD = next_score - curr_score;
            return moveto_scoreD;
        }
};

class Course {
    private:
        std::string name;
        int max_studs;
        std::vector<Student&> students;
        // map (course -> (sorted)vector); vectors of pairs sorted by pair.second; pairs <student, score_in_course>
        std::map<Course&, std::vector<std::pair<Student&, int>>> moveToCourseMap;
    public:
        Course(std::string name, int max_studs) {
            name = name;
            max_studs = max_studs;
        };
        std::string getInfo() {
            return name;
        }

        void moveStudentIn(Student& student) {
            for (auto& [course, sorted_students] : moveToCourseMap) {
                std::pair<Student&, int> student_scoreD(student, student.getMoveToScoreD(course));
                auto is_better = [&student_scoreD](std::pair<Student&, int> other_student_scoreD) {
                    return student_scoreD.second >= other_student_scoreD.second;
                };
                auto iter = std::find_if(sorted_students.begin(), sorted_students.end(), is_better);
                sorted_students.insert(iter, student_scoreD);
            }
        }

        void moveStudentOut(Student& student) {
            for (auto& [course, sorted_students] : moveToCourseMap) {
                auto is_student = [&student](std::pair<Student&, int> other_student_scoreD) {
                    return student == other_student_scoreD.first;
                };
                auto iter = std::find_if(sorted_students.begin(), sorted_students.end(), is_student);
                sorted_students.erase(iter);
            }
        }

        bool isNotFull() {
            if (students.size() == max_studs) {
                return false;
            } else {
                return true;
            }
        }

        void displayStudents() {
            std::cout << name << ":\n";
            for (auto student : students) {
                std::cout << student.getInfo() << "\n";
            }
        }
};



class Case {
    private:
        std::vector<Course> courses;
        std::vector<Student> students;

    public:
        int getNumCourses() {
            return courses.size();
        }
        int getNumStudents() {
            return courses.size();
        }
        void showCourses() {
            std::cout << "Aktuelle AGs:\n";
            if (getNumCourses() >= 1) {
                for (auto course : courses) {
                    std::cout << course.getInfo();
                }
            } else {
                std::cout << "Keine AGs";
            }
        }

        void showStudents() {
            std::cout << "Aktuelle Teilnehmer:\n";
            if (getNumStudents() >= 1) {
                for (auto student : students) {
                    std::cout << student.getInfo();
                }
            } else {
                std::cout << "Keine Teilnehmer";
            }
        }

        void addCourses() {
            bool adding_courses = true;
            while (adding_courses) {
                std::cout << "AG-Name eingeben (x eingeben um andere Aktion zu wählen): ";
                std::string name;
                std::cin >> name;
                if (name != "x") {
                    std::cout << "Teilnehmerzahl eingeben: ";
                    int max_studs;
                    std::cin >> max_studs;
                    
                    Course course(name, max_studs);
                    courses.push_back(course);
                    std::cout << "AG " + name + " mit " + std::to_string(max_studs) + " Plätzen hinzugefügt";
                } else {
                    adding_courses = false;
                }
            }
        }
        void removeCourses() {
            bool removing_courses = true;
            while (removing_courses) {
                std::cout << "Kursnummer eingeben (x eingeben um andere Aktion zu wählen): ";
                std::string input;
                std::cin >> input;
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
                std::cout << "Teilnehmer-Name eingeben (x eingeben um andere Aktion zu wählen): ";
                std::string name;
                std::cin >> name;
                if (name != "x") {
                    std::vector<Course&> prefs;
                    for (int n_pref = 0; n_pref < 3; n_pref++) {
                        std::cout << std::to_string(n_pref) + ". Wunsch(Zahl): ";
                        int pref;
                        std::cin >> pref;
                        prefs.push_back(courses[pref]);
                    }

                    Student student(name, prefs);
                    students.push_back(student);
                    std::cout << "Teilnehmer " + name + " mit Wünschen ";
                    // prints all prefs
                    //std::copy(prefs.begin(), prefs.end(), std::ostream_iterator<char>(std::std::cout, " "));
                    std::cout << " hinzugefügt\n";
                } else {
                    adding_students = false;
                }
            }
        }
        void removeStudents() {
            bool removing_students = true;
            while (removing_students) {
                std::cout << "Teilnehmernummer eingeben (x eingeben um andere Aktion zu wählen): ";
                std::string input;
                std::cin >> input;
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
            for (int i_wish = 0; i_wish< N_PREFS; i_wish++) {
                for (Student& student : students) {
                    if (student.getCurrScore() > std::pow(i_wish, 2)) {
                        Course& wished_course = student.getWish(i_wish);
                        if (wished_course.isNotFull()) {
                            wished_course.moveStudentIn(student);
                            student.updateCurrentScore(wished_course);
                        }
                    }
                }
            }
        }

        void smartAssignment() {
            for (auto student : students) {


            }

        }

        void optimizeAssignment() {

        }

        void displayAssignment() {
            for (auto course : courses) {
                course.displayStudents();
            }
        }
};



int main()
{   
    std::cout << "Willkommen zur AG Einteilungs App!\n";

    Case useCase;

    std::cout << "Zuerst müssen alle AGs und deren maximale Teilnehmerzahl eingetragen werden:\n";
    bool course_edit = true;
    bool students_edit = false;
    while (course_edit) {
        
        std::cout << "Aktionen:\n";
        std::cout << "\t1 - AGs hinzufügen\n";
        std::cout << "\t2 - AGs entfernen\n";
        std::cout << "\t3 - Aktuelle AGs anzeigen\n";
        std::cout << "\t4 - !AGs bestätigen und zum Eintragen der Teilnehmer übergehen!";
        std::cout << "Ziffer eingeben um Aktion zu wählen:";
        int action;
        std::cin >> action;

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
    std::cout << "Anzahl der Wünsche pro Teilnehmer eingeben:";
    std::cin >> N_PREFS;
    while (students_edit) {
        std::cout << "Aktionen:\n";
        std::cout << "\t1 - Teilnehmer hinzufügen\n";
        std::cout << "\t2 - Teilnehmer entfernen\n";
        std::cout << "\t3 - Aktuelle Teilnehmer anzeigen\n";
        std::cout << "\t4 - !Teilnehmer Bestätigen und AG-Zuordnung generieren!\n";
        std::cout << "Ziffer eingeben um Aktion zu wählen:";
        int action;
        std::cin >> action;
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
                //useCase.optimizeAssignment();
                useCase.displayAssignment();
        }
    }
}