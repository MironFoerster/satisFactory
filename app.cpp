#include <iostream>
#include <algorithm>
//#include <iterator> // for ostream_iterator
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <cstdio>

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


        struct Path {
            std::vector<std::pair<Course&, Student>> path;
            int score;
        };

class Student {
    private:
        std::string name;
        std::string info;
        // list of course indices with decreasing preference
        std::vector<Course*> prefs;
        int curr_score;

    public:
        Student(std::string name_in = "dflt", std::vector<Course*> prefs_in = std::vector<Course*> (), std::string info_in = "") {
            name = name_in;
            prefs = prefs_in;
            info = info_in;
            curr_score = std::pow(N_PREFS, 2)+1;
        };

        bool operator==(const Student other) {
            return name == other.name;
        }

        std::string getInfo() {
            return info;
        }

        std::string getName() {
            return name;
        }

        int getCurrScore() {
            return curr_score;
        }

        Course* getWish(int i_wish) {
            return prefs[i_wish];
        }

        std::vector<Course*> getPrefs() {
            return prefs;
        }

        int getCourseScore(Course* course) {
            int index = indexof<Course*>(course, prefs);
            // If element was found
            if (index == -1) {
                return std::pow(N_PREFS, 2);
            } else {
                return std::pow(index, 2);
            }
        }

        void updateCurrentScore(Course* course) {
            curr_score = getCourseScore(course);
        }

        int getMoveToScoreD(Course* course) { // negative scoreD is making things better
            int moveto_scoreD;
            int next_score = getCourseScore(course);

            moveto_scoreD = next_score - curr_score;
            return moveto_scoreD;
        }
};

class Course {
    private:
        std::string name;
        std::string info;
        int max_studs;
        std::vector<Student> students;
        // map (course -> (sorted)vector); vectors of pairs sorted by pair.second; pairs <student, score_in_course>
        std::map<Course, std::vector<std::pair<Student, int>>> moveToCourseMap;
    public:
        Course(std::string name_in = "dflt", int max_studs_in = 0, std::string info_in = "") {
            name = name_in;
            max_studs = max_studs_in;
            students.reserve(max_studs);
            info = info_in;
        };
        
        bool operator==(const Course& other) {
            return name == other.name;
        }
        bool operator<(const Course& other) const {
            return name < other.name;
        }

        std::string getInfo() const {
            return info;
        }
        std::string getName() const {
            return name;
        }
        int getNStuds() {
            return students.size();
        }

        void initMap(std::vector<Course>& courses) {

            for (auto& course : courses) {
                if (!(course.name == this->name)) {
                    moveToCourseMap[course] = std::vector<std::pair<Student, int>>();
                }
            }
        }

        std::pair<Student, int> getBestMoveToCourse(Course& course) {
            return moveToCourseMap.at(course)[0];
        }

        void moveStudentIn(Student* student) {
            students.push_back(*student);
            for (auto& [course, sorted_students] : moveToCourseMap) {
                Course course_cp = course;
                std::pair<Student, int> student_scoreD(*student, (*student).getMoveToScoreD(&course_cp));
                auto is_better = [student_scoreD](std::pair<Student, int> other_student_scoreD) {
                    return student_scoreD.second >= other_student_scoreD.second;
                };
                auto iter = std::find_if(sorted_students.begin(), sorted_students.end(), is_better);
                sorted_students.insert(iter, student_scoreD);
            }
        }

        void moveStudentOut(Student* student) {
            auto iter = std::find(students.begin(), students.end(), *student);
            
            if (iter != students.end()) {
                students.erase(iter);
                for (auto& [course, sorted_students] : moveToCourseMap) {
                    auto is_student = [student](std::pair<Student, int> other_student_scoreD) {
                        return (*student) == other_student_scoreD.first;
                    };
                    auto iter = std::find_if(sorted_students.begin(), sorted_students.end(), is_student);
                    sorted_students.erase(iter);
                }
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
            for (auto& student : students) {
                std::cout << "\t" << student.getName() << "\n";
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
            return students.size();
        }
        void showCourses() {
            int num_courses = getNumCourses();
            std::cout << "Aktuelle AGs(" << num_courses << "):\n";
            std::cout << "No. | Name | Plaetze\n";

            if (num_courses >= 1) {
                for (int i = 0; i < courses.size(); i++) {
                    std::cout << i+1 << ". " << courses[i].getInfo() << "\n";
                }
            } else {
                std::cout << "Keine AGs\n";
            }
        }

        void showStudents() {
            int num_students = getNumStudents();
            std::cout << "Aktuelle Teilnehmer(" << num_students << "):\n";
            std::cout << "No. | Name | 1. | 2. | 3. | ...\n";
            if (num_students >= 1) {
                for (int i = 0; i < students.size(); i++) {
                    std::cout << i+1 << ". " << students[i].getInfo() << "\n";
                }
            } else {
                std::cout << "Keine Teilnehmer\n";
            }
        }

        void reserveCourses(int n_courses) {
            courses.reserve(n_courses);
        }
        void reserveStudents(int n_students) {
            courses.reserve(n_students);
        }

        void addCourses() {
            bool adding_courses = true;
            while (adding_courses) {
                std::cout << "\nAG-Name eingeben (x eingeben um andere Aktion zu waehlen): ";
                std::string info;
                std::string name;
                std::cin >> name;
                if (name != "x") {
                    
                    std::cout << "Teilnehmerzahl eingeben: ";
                    int max_studs;
                    std::cin >> max_studs;
                    info = name + "  " + std::to_string(max_studs);
                    Course course(name, max_studs, info);
                    courses.push_back(course);
                    std::cout << "AG " + name + " mit " + std::to_string(max_studs) + " Plaetzen hinzugefuegt";
                } else {
                    adding_courses = false;
                }
            }
        }

        void removeCourses() {
            bool removing_courses = true;
            while (removing_courses) {
                std::cout << "Kursnummer eingeben (x eingeben um andere Aktion zu waehlen): ";
                std::string input;
                std::cin >> input;
                if (input != "x") {
                    int index = stoi(input)-1;
                    std::cout << "Kurs " << courses[index].getName() << " erfolgreich entfernt!\n";
                    courses.erase(courses.begin() + index);
                } else {
                    removing_courses = false;
                }
            }
        }

        void addStudents() {
            bool adding_students = true;
            while (adding_students) {
                std::cout << "Teilnehmer-Name eingeben (x eingeben um andere Aktion zu waehlen): ";
                std::string info = "";
                std::string name;
                std::cin >> name;
                if (name != "x") {
                    info += name + " ";
                    std::vector<Course*> prefs;
                    for (int n_pref = 0; n_pref < N_PREFS; n_pref++) {
                        std::cout << std::to_string(n_pref+1) + ". Wunsch(Kursziffer): ";
                        std::string pref;
                        std::cin >> pref;
                        info += " " + pref;
                        prefs.push_back(&courses[stoi(pref)-1]);
                    }
                    
                    Student student(name, prefs, info);
                    students.push_back(student);
                    std::cout << "Teilnehmer " + name + " hinzugefuegt\n";
                } else {
                    adding_students = false;
                }
            }
        }
        void removeStudents() {
            bool removing_students = true;
            while (removing_students) {
                std::cout << "Teilnehmernummer eingeben (x eingeben um andere Aktion zu waehlen): ";
                std::string input;
                std::cin >> input;
                if (input != "x") {
                    int index = stoi(input)-1;
                    students.erase(students.begin() + index);
                } else {
                    removing_students = false;
                }
            }
        }

        void initCourseMaps() {
            for (auto& course : courses) {
                course.initMap(courses);
            }
        }
        void FCFSAssignment() {
            // First Come First Serve Assignment
            for (int i_wish = 0; i_wish< N_PREFS; i_wish++) {
                std::cout << "Wish:" << i_wish << "\n";
                for (Student& student : students) {
                    std::cout << "stud: " << student.getName() << " " << student.getCurrScore() << "\n";
                    if (student.getCurrScore() > std::pow(i_wish, 2)) {
                        std::cout << "current Score > than wish^2\n";
                        Course* wished_course = student.getWish(i_wish);
                        if ((*wished_course).isNotFull()) {
                            std::cout << "Accepted!\n";
                            (*wished_course).moveStudentIn(&student);
                            student.updateCurrentScore(wished_course);
                        std::cout << "new Score:" << student.getCurrScore() << "\n";
                        }
                    }
                }
            }
        }

        void moveStudFromTo(Student* student, Course& from, Course& to) {
            std::cout << "Move " << (*student).getName() << " from " << from.getName() << " to " << to.getName() << "\n";
            std::cout << "out\n";
            from.moveStudentOut(student);
            std::cout << "in\n";

            to.moveStudentIn(student);

            std::cout << "done\n";
            (*student).updateCurrentScore(&to);
        }


        void executePath(Path& path) {
            std::cout << "printpath\n";
            for (auto dings : path.path) {
            std::cout << dings.first.getName() << "\n";
            std::cout << dings.second.getName() << "\n";
                
            }
            std::cout << "printpathend\n";

            for (int i_node = 0; i_node < path.path.size()-1; i_node++) {
                std::cout << "befnode\n";
                if (path.path[i_node+1].second.getName() != "") {
                    Course& from = (path.path[i_node+1].first.getName() != "dflt") ? courses[indexof<Course>(path.path[i_node+1].first, courses)] : path.path[i_node+1].first;
                    Course& to = courses[indexof<Course>(path.path[i_node].first, courses)];
                    moveStudFromTo(&path.path[i_node+1].second, from, to);
                }
                std::cout << "aftnode\n";
            }             
        }

        Path findBestPath(int i_course, std::vector<Course> left_courses) {

            Course& curr_course = courses[indexof<Course>(left_courses[i_course], courses)];
            left_courses.erase(left_courses.begin() + i_course);
            std::cout << "pathfind\n";
            std::cout << curr_course.getName() << "\n";
            if (curr_course.isNotFull()) {
                // initiate tree collapse
                Student dflt_stud;
                std::cout << dflt_stud.getName() << "\n";
                
                Path path_tip {std::vector<std::pair<Course&, Student>> {{curr_course, dflt_stud}}, 0};
                std::cout << "terminate\n";
                return path_tip;
            } else {
                // continue building tree
                std::cout << "build\n";
                Path best_path = findBestPath(0, left_courses);
                std::cout << "collapse\n";
                std::pair best_move = curr_course.getBestMoveToCourse(courses[indexof<Course>(left_courses[0], courses)]);
                std::cout << best_move.first.getName() << "\n";
                best_path.path.push_back(std::pair<Course&, Student> (curr_course, best_move.first));
                best_path.score += best_move.second;
                for (int i=1; i < left_courses.size(); i++) {
                    Path temp_path = findBestPath(i, left_courses);
                    std::pair best_move = curr_course.getBestMoveToCourse(courses[indexof<Course>(left_courses[i], courses)]);
                    temp_path.path.push_back(std::pair<Course&, Student> (curr_course, best_move.first));
                    temp_path.score += best_move.second;

                    if (temp_path.score < best_path.score) {
                        best_path = temp_path;
                    }
                }
                return best_path;
            }
        }

        void smartAssignment() {
            bool optimal = false;
            Course unassignedCourse;
            std::cout << "\n\n";
            while (!optimal) {
                
                
                std::cout << "nonoptimal\n";
                optimal = true;
                for (auto student : students) {
                    std::cout << student.getName() << "\n";

                    // find best scored move for current student
                    Path best_path;
                    for (auto pref : student.getPrefs()) {
                        std::cout << (*pref).getName() << "\n";

                        // find best path for each of the preferences
                        Path best_pref_path;
                        
                        if ((*pref).isNotFull()) {
                            Student dflt_stud;
                            best_pref_path = Path {std::vector<std::pair<Course&, Student>> {{(*pref), dflt_stud}}, 0};
                            std::cout << "nonfull\n";
                        } else {
                            std::cout << "full\n";
                            std::cout << indexof<Course>((*pref), courses) << "\n";
                            best_pref_path = findBestPath(indexof<Course>((*pref), courses), courses);
                            std::cout << "full\n";
                            std::cout << best_pref_path.score << "\n";
                            for (auto s : best_pref_path.path) {
                            std::cout << s.first.getName() << "\n";

                            }
                            
                        }

                        // append path root
                        
                        
                        best_pref_path.path.push_back(std::pair<Course&, Student> {unassignedCourse, student});
                        
                        best_pref_path.score += student.getMoveToScoreD(pref);
                        if (best_path.score > best_pref_path.score || best_path.path.size()==0) {
                            best_path = best_pref_path;
                        }

                    
                    }
                    
                    // execute best path if it decreases the overall score
                    if (best_path.score < 0) {
                        executePath(best_path);
                        optimal = false;
                    }
                    optimal = true;
                }
            }

        }

        void optimizeAssignment() {

        }

        void displayAssignment() {
            for (auto& course : courses) {
                course.displayStudents();
            }
        }
};



int main() {
    std::cout << "Willkommen zur AG Einteilungs App!\n";

    Case useCase;

    std::cout << "Zuerst muessen alle AGs und deren maximale Teilnehmerzahl eingetragen werden:\n";
    bool course_edit = true;
    bool students_edit = false;
    std::cout << "Anzahl AGs eingeben:";
    int n_courses;
    std::cin >> n_courses;
    useCase.reserveCourses(n_courses);
    while (course_edit) {        
        std::cout << "Aktionen:\n";
        std::cout << "\t1 - AGs hinzufuegen\n";
        std::cout << "\t2 - AGs entfernen\n";
        std::cout << "\t3 - Aktuelle AGs anzeigen\n";
        std::cout << "\t4 - !AGs bestaetigen und zum Eintragen der Teilnehmer uebergehen!";
        std::cout << "\nZiffer eingeben um Aktion zu waehlen:";
        int action;
        std::cin >> action;

        switch(action) {
            case 1:
                useCase.addCourses();
                break;
            case 2:
                useCase.removeCourses();
                break;
            case 3:
                useCase.showCourses();
                break;
            case 4:
                course_edit = false;
                students_edit = true;
                break;
        }
    }
    std::cout << "Anzahl Teilnehmer eingeben:";
    int n_students;
    std::cin >> n_students;
    useCase.reserveStudents(n_students);
    useCase.initCourseMaps();
    std::cout << "Anzahl der Wuensche pro Teilnehmer eingeben:";
    std::cin >> N_PREFS;
    while (students_edit) {
        std::cout << "Aktionen:\n";
        std::cout << "\t1 - Teilnehmer hinzufuegen\n";
        std::cout << "\t2 - Teilnehmer entfernen\n";
        std::cout << "\t3 - Aktuelle Teilnehmer anzeigen\n";
        std::cout << "\t4 - !Teilnehmer Bestaetigen und AG-Zuordnung generieren!\n";
        std::cout << "Ziffer eingeben um Aktion zu waehlen:";
        int action;
        std::cin >> action;
        switch(action) {
            case 1:
                useCase.addStudents();
                break;
            case 2:
                useCase.removeStudents();
                break;
            case 3:
                useCase.showStudents();
                break;
            case 4:
                course_edit = true;
                students_edit = false;
                // useCase.FCFSAssignment();
                useCase.smartAssignment();
                //useCase.optimizeAssignment();
                useCase.displayAssignment();
                std::cout << "Finished!";
                break;
        }
    }
}