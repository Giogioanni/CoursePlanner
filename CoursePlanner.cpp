#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
/* WORK OF Giogioanni Morales */

// Struct to hold the course information
struct Course {
    string courseId;
    string courseName;
    vector<string> preList;
};

class BinarySearchTree { // binary search tree to hold the courses
private:
    struct Node {
        Course course;
        Node* right;
        Node* left;

        // Default constructor
        Node() {
            left = nullptr;
            right = nullptr;
        }

        // Initialize with a course
        Node(Course aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };
     
    Node* root; 
    void inOrder(Node* node); 
    int size = 0;

public: 
    BinarySearchTree(); 
    void InOrder();
    void Insert(Course aCourse);
    Course Search(string courseId);
    int Size();
};

// Default constructor
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}

// Traverse the tree in order
void BinarySearchTree::InOrder() {
    inOrder(root);
}

// Insert a course
void BinarySearchTree::Insert(Course aCourse) {
    Node* currentNode = root; // Start at the root

    if (root == nullptr) { // If the tree is empty, add the course as the root
        root = new Node(aCourse);
    }
    else {
        while (currentNode != nullptr) { // Traverse the tree
            if (aCourse.courseId < currentNode->course.courseId) { // If the course ID is less than the current node's course ID
                if (currentNode->left == nullptr) {
                    currentNode->left = new Node(aCourse);
                    currentNode = nullptr;
                }
                else { // Move to the left
                    currentNode = currentNode->left;
                }
            }
            else {  // Move to the right
                if (currentNode->right == nullptr) {
                    currentNode->right = new Node(aCourse);
                    currentNode = nullptr;
                }
                else {
                    currentNode = currentNode->right;
                }
            }
        }
    }
    size++; // Increment the size of the tree
}

// Search for a course
Course BinarySearchTree::Search(string courseId) { // Return the course with the given ID
    Course aCourse;
    Node* currentNode = root; // Start at the root

    while (currentNode != nullptr) { // Traverse the tree
        if (currentNode->course.courseId == courseId) {
            return currentNode->course;
        }
        else if (courseId < currentNode->course.courseId) { // Move to the left
            currentNode = currentNode->left;
        }
        else { // Move to the right
            currentNode = currentNode->right;
        }
    }
    // Not found
    return aCourse;
}

void BinarySearchTree::inOrder(Node* node) { // Traverse the tree in order
    if (node == nullptr) {
        return;
    }
    inOrder(node->left); // Traverse the left subtree
    // Split the course name by comma and print only the first part (course name)
    size_t pos = node->course.courseName.find(',');     // Find the comma
    string courseName = node->course.courseName.substr(0, pos); // Extract the course name
    cout << node->course.courseId << ", " << courseName << endl; // Print the course ID and name
    inOrder(node->right); // Traverse the right subtree
}

int BinarySearchTree::Size() { // Return the size of the tree
    return size;
}

void loadCourses(const string& filePath, BinarySearchTree* courseList) {
    // Create a data structure and add to the collection of courses
    ifstream file(filePath); // Open the file
    if (!file.is_open()) { // Check if the file is open
        cerr << "Error: Unable to open file." << endl; // Error message
        return;
    }

    string line; // Read each line from the file
    while (getline(file, line)) {
        Course aCourse; // Create a new struct for each line
        size_t pos = line.find(','); // Find the comma
        if (pos == string::npos) { // If there is no comma, skip the line
            cerr << "Error: Invalid format. Skipping line." << endl;
            continue;
        }
        aCourse.courseId = line.substr(0, pos); // Extract the course ID
        aCourse.courseName = line.substr(pos + 1); // Include the comma

        // Check if there are prerequisites: this was the most frustrating part of the assignment for me
        size_t preReqPos = line.find(',', pos + 1); // Find the next comma
        if (preReqPos != string::npos) { // If there is a comma, there are prerequisites
            string preReqSubstring = line.substr(preReqPos + 1); // Extract the prerequisites
            size_t commaPos = preReqSubstring.find(','); // Find the first comma
            while (commaPos != string::npos) { // While there are more commas
                string preReq = preReqSubstring.substr(0, commaPos); // Extract the prerequisite
                aCourse.preList.push_back(preReq);
                preReqSubstring = preReqSubstring.substr(commaPos + 1); // Move to the next prerequisite
                commaPos = preReqSubstring.find(',');
            }
            aCourse.preList.push_back(preReqSubstring); // Add the last prerequisite
        }
        courseList->Insert(aCourse); // Add the course to the list
    }
    file.close(); // Close the file
}


// Force the case of any string to uppercase
void convertCase(string& toConvert) { 
    for (char& c : toConvert) {
        c = toupper(c);
    }
}

int main() {
    // Define the file path
    string filePath = "C:/Users/gioan/source/repos/CoursePlanner/CoursePlanner/CoursePlanner/course_data.txt"; //  this can be changed to your file path

    // Define a table to hold all the courses
    BinarySearchTree* courseList = new BinarySearchTree();

    // Load courses from file
    loadCourses(filePath, courseList);

    int choice = 0; // Define the choice variable
    while (choice != 9) { // Loop until the user chooses to exit
        cout << "Welcome to the course planner." << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";

        cin >> choice; // Get the user's choice

        switch (choice) {
        case 1:
            // Already loaded from file
            break;
        case 2: // Print the course list
            cout << "Here is a sample schedule:" << endl;
            courseList->InOrder();
            break;
        case 3: { // Print a specific course
            string courseID;
            cout << "What course do you want to know about? ";
            cin >> courseID; 
            convertCase(courseID); // Convert the course ID to uppercase
            Course foundCourse = courseList->Search(courseID); // Search for the course
            if (!foundCourse.courseId.empty()) {
                // Print the course name without prerequisites at the end
                size_t pos = foundCourse.courseName.find(',');
                string courseName = foundCourse.courseName.substr(0, pos); // Extract the course name
                cout << foundCourse.courseId << ", " << courseName << endl; // Print the course ID and name
                if (!foundCourse.preList.empty()) { // If there are prerequisites
                    cout << "Prerequisites: "; 
                    for (size_t i = 0; i < foundCourse.preList.size(); ++i) {       
                        cout << foundCourse.preList[i];  
                        if (i != foundCourse.preList.size() - 1) { // If it's not the last prerequisite
                            cout << ", ";
                        }
                    }
                    cout << endl; 
                }
                else {
                    cout << endl; // No prerequisites, so end the line
                }
            }
            else {
                cout << "Course not found." << endl;  
            }
            break;
        }
        case 9: // Exit the program
            cout << "Thank you for using the course planner!" << endl; 
            break;
        default: // Invalid choice
            cout << choice << " is not a valid option." << endl;
        }
    }
    delete courseList; // Free the memory allocated for the binary search tree
    return 0; // Exit the program
}