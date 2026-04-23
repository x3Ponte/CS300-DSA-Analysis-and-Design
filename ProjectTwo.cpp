//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Tristan Ponte
// Description : ABCU Advising Assistance Program
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

//============================================================================
// Course structure just holds all data for a single course..
//============================================================================

struct Course {
    string courseNumber;                // the unique identifier (like CSCI100)
    string name;                       // the course title
    vector<string> prerequisites;      // list of prerequisite course numbers
};

//============================================================================
// Helper function to convert a string to uppercase thats used for the case insensitive course lookups
//============================================================================

string toUpper(string str) {
    for (unsigned int i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
    }
    return str;
}

//============================================================================
// Helper function to trim whitespace and carriage returns from a string
//============================================================================

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return str.substr(start, end - start + 1);
}

//============================================================================
// Load courses from file into hash table
//
// @param fileName  Path to the CSV file
// @param courseTable  Reference to the hash table to populate
//============================================================================

void loadCourses(string fileName, unordered_map<string, Course>& courseTable) {
    // opens the file
    ifstream file(fileName);

    // just a check that the file opened successfully
    if (!file.is_open()) {
        cout << "Error: could not open file " << fileName << endl;
        return;
    }

    string line;

    // reads each line from the file.
    while (getline(file, line)) {
        // then skips empty lines
        if (trim(line).empty()) {
            continue;
        }

        stringstream ss(line);
        string token;
        vector<string> tokens;

        // splits the line by comma delimiter
        while (getline(ss, token, ',')) {
            token = trim(token);
            tokens.push_back(token);
        }

        // validates each line must have at least 2 parameters
        if (tokens.size() < 2) {
            cout << "Warning: skipping malformed line." << endl;
            continue;
        }

        // builds the course object from parsed tokens.
        Course course;
        course.courseNumber = tokens[0];
        course.name = tokens[1];

        // parse prerequisites and skips empty strings
        for (unsigned int i = 2; i < tokens.size(); i++) {
            if (!tokens[i].empty()) {
                course.prerequisites.push_back(tokens[i]);
            }
        }

        // insert course into hash table using courseNumber as key
        courseTable[course.courseNumber] = course;
    }

    // closes the file
    file.close();

    cout << "Data loaded successfully." << endl;
}

//============================================================================
// Print all courses in alphanumeric order
// @param courseTable  Reference to the hash table containing courses
//============================================================================

void printCourseList(unordered_map<string, Course>& courseTable) {
    // collects all of the course numbers into a vector for sorting
    vector<string> courseNumbers;

    for (auto& pair : courseTable) {
        courseNumbers.push_back(pair.first);
    }

    // sorts alphanumerically from the lowest to the highest
    sort(courseNumbers.begin(), courseNumbers.end());

    // print the sorted list..
    cout << "Here is a sample schedule:" << endl;
    cout << endl;

    for (unsigned int i = 0; i < courseNumbers.size(); i++) {
        Course course = courseTable[courseNumbers[i]];
        cout << course.courseNumber << ", " << course.name << endl;
    }
}

//============================================================================
// Search for a course and print its information and prerequisites
// @param courseTable  Reference to the hash table containing courses
//============================================================================

void printCourseInfo(unordered_map<string, Course>& courseTable) {
    string courseNumber;
    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    // converts input to uppercase for case insensitive lookup
    courseNumber = toUpper(courseNumber);

    // searchs for the course in the hash table
    auto it = courseTable.find(courseNumber);

    // if course isnt found then displays error message..
    if (it == courseTable.end()) {
        cout << "Course " << courseNumber << " not found." << endl;
        return;
    }

    // course found then prints course number and title
    Course course = it->second;
    cout << course.courseNumber << ", " << course.name << endl;

    // print prerequisites if any exist...
    if (!course.prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (unsigned int i = 0; i < course.prerequisites.size(); i++) {
            cout << course.prerequisites[i];
            // prints a comma between prerequisites but not after the last one
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

//============================================================================
// Main method runs the menu loop for the advising program
//============================================================================

int main() {
    // hash table to store all courses...
    unordered_map<string, Course> courseTable;

    // tracks whether data has been loaded or not
    bool dataLoaded = false;

    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    // the main menu loop
    while (choice != 9) {
        // displays the menu options
        cout << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";

        // handles non integer input.
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {

        case 1: {
            // prompts the user for the file name
            string fileName;
            cout << "Enter the file name: ";
            cin >> fileName;

            // clears the table if reloading
            courseTable.clear();

            // loads courses from file into hash table here
            loadCourses(fileName, courseTable);

            // sets flag if courses were loaded successfully..
            if (!courseTable.empty()) {
                dataLoaded = true;
            }
            break;
        }

        case 2:
            // checks that data has been loaded before printing.
            if (!dataLoaded) {
                cout << "Please load the data first (Option 1)." << endl;
            } else {
                printCourseList(courseTable);
            }
            break;

        case 3:
            // check that data has been loaded before searching
            if (!dataLoaded) {
                cout << "Please load the data first (Option 1)." << endl;
            } else {
                printCourseInfo(courseTable);
            }
            break;

        case 9:
            // the exit message
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            // error message for invalid menu option..
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}