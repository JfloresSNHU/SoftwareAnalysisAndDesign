#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

// Define the Course class to encapsulate course information
class Course {
public:
    string courseNumber;  // Course number (e.g., CSCI100)
    string title;         // Course title (e.g., Introduction to Computer Science)
    vector<string> prerequisites;  // List of prerequisites

    // Constructor to initialize a Course object
    Course(string number, string title, vector<string> prereqs) {
        this->courseNumber = number;
        this->title = title;
        this->prerequisites = prereqs;
    }
};

// Node structure for the Binary Search Tree (BST)
struct Node {
    Course course;  // Course object stored in the node
    Node* left;     // Pointer to the left child
    Node* right;    // Pointer to the right child

    // Constructor to initialize a Node with a Course object
    Node(Course course) : course(course), left(nullptr), right(nullptr) {}
};

// Define the Binary Search Tree (BST) class
class BinarySearchTree {
private:
    Node* root;  // Root node of the BST

    // Recursive helper function for in-order traversal
    void inOrderTraversal(Node* node) {
        if (node == nullptr) return;

        inOrderTraversal(node->left);
        cout << node->course.courseNumber << ", " << node->course.title << endl;
        inOrderTraversal(node->right);
    }

    // Recursive helper function to insert a course into the BST
    Node* insert(Node* node, Course course) {
        if (node == nullptr) return new Node(course);

        if (course.courseNumber < node->course.courseNumber) {
            node->left = insert(node->left, course);
        } else {
            node->right = insert(node->right, course);
        }
        return node;
    }

    // Recursive helper function to search for a course in the BST
    Node* search(Node* node, string courseNumber) {
        if (node == nullptr || node->course.courseNumber == courseNumber) return node;

        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        } else {
            return search(node->right, courseNumber);
        }
    }

public:
    // Constructor to initialize an empty BST
    BinarySearchTree() : root(nullptr) {}

    // Function to insert a course into the BST
    void insertCourse(Course course) {
        root = insert(root, course);
    }

    // Function to print the course list in alphanumeric order
    void printCourseList() {
        if (root == nullptr) {
            cout << "No courses available. Please load the data first." << endl;
            return;
        }
        inOrderTraversal(root);
    }

    // Function to print the details of a specific course
    void printCourseDetails(string courseNumber) {
        Node* result = search(root, courseNumber);
        if (result != nullptr) {
            cout << "Course Number: " << result->course.courseNumber << endl;
            cout << "Course Title: " << result->course.title << endl;
            cout << "Prerequisites: ";
            if (result->course.prerequisites.empty()) {
                cout << "None";
            } else {
                for (const string& prereq : result->course.prerequisites) {
                    cout << prereq << " ";
                }
            }
            cout << endl;
        } else {
            cout << "Course not found." << endl;
        }
    }
};

// Function to load course data from a file into the BST
void loadCourses(string fileName, BinarySearchTree& bst) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string courseNumber, title, prereq;
        vector<string> prerequisites;

        getline(ss, courseNumber, ',');
        getline(ss, title, ',');

        while (getline(ss, prereq, ',')) {
            prerequisites.push_back(prereq);
        }

        if (courseNumber.empty() || title.empty()) {
            cerr << "Error: Invalid course data. Skipping line." << endl;
            continue;
        }

        Course course(courseNumber, title, prerequisites);
        bst.insertCourse(course);
    }

    if (file.bad()) {
        cerr << "Error: Failure while reading the file." << endl;
    }

    file.close();
    cout << "Data loaded successfully." << endl;
}

int main() {
    BinarySearchTree bst;
    int choice;
    string fileName, courseNumber;

    while (true) {
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit." << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        // Validate user input
        if (cin.fail()) {
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
            cout << "Invalid option. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
            case 1:
                cout << "Enter file name: ";
                cin >> fileName;
                loadCourses(fileName, bst);
                break;
            case 2:
                bst.printCourseList();
                break;
            case 3:
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                bst.printCourseDetails(courseNumber);
                break;
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                return 0;
            default:
                cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
