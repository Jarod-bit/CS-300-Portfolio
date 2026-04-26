#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

string toUpperCase(string text) {
    for (char& ch : text) {
        ch = toupper(static_cast<unsigned char>(ch));
    }
    return text;
}

string trim(const string& text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    size_t end = text.find_last_not_of(" \t\r\n");

    if (start == string::npos) {
        return "";
    }

    return text.substr(start, end - start + 1);
}

bool loadCoursesFromFile(const string& fileName, vector<Course>& courses) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file " << fileName << endl;
        return false;
    }

    courses.clear();
    string line;

    while (getline(inputFile, line)) {
        if (trim(line).empty()) {
            continue;
        }

        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(trim(token));
        }

        if (tokens.size() >= 2) {
            Course course;
            course.courseNumber = toUpperCase(tokens[0]);
            course.courseTitle = tokens[1];

            for (size_t i = 2; i < tokens.size(); ++i) {
                if (!tokens[i].empty()) {
                    course.prerequisites.push_back(toUpperCase(tokens[i]));
                }
            }

            courses.push_back(course);
        }
    }

    inputFile.close();
    return true;
}

void printCourseList(vector<Course> courses) {
    if (courses.empty()) {
        cout << "No course data loaded." << endl;
        return;
    }

    sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
    });

    cout << "Here is a sample schedule:" << endl << endl;

    for (const Course& course : courses) {
        cout << course.courseNumber << ", " << course.courseTitle << endl;
    }
}

Course* findCourse(vector<Course>& courses, const string& courseNumber) {
    string searchKey = toUpperCase(courseNumber);

    for (Course& course : courses) {
        if (course.courseNumber == searchKey) {
            return &course;
        }
    }

    return nullptr;
}

void printCourseInformation(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No course data loaded." << endl;
        return;
    }

    string courseNumber;
    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    Course* course = findCourse(courses, courseNumber);

    if (course == nullptr) {
        cout << "Course " << toUpperCase(courseNumber) << " not found." << endl;
        return;
    }

    cout << course->courseNumber << ", " << course->courseTitle << endl;

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    } else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

void printMenu() {
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << endl;
    cout << "What would you like to do? ";
}

int main() {
    vector<Course> courses;
    int choice = 0;
    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        printMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
            case 1: {
                string fileName;
                cout << "Enter the file name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, fileName);

                if (loadCoursesFromFile(fileName, courses)) {
                    cout << "Data loaded successfully." << endl;
                    dataLoaded = true;
                } else {
                    cout << "Data was not loaded." << endl;
                }
                break;
            }

            case 2:
                if (!dataLoaded) {
                    cout << "Please load the data file first." << endl;
                } else {
                    printCourseList(courses);
                }
                break;

            case 3:
                if (!dataLoaded) {
                    cout << "Please load the data file first." << endl;
                } else {
                    printCourseInformation(courses);
                }
                break;

            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;

            default:
                cout << choice << " is not a valid option." << endl;
                break;
        }
    }

    return 0;
}