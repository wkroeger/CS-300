//============================================================================
// Name        : Final Project.cpp
// Author      : Willem Kroeger
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string courseName;
    vector<string> prerequisites; //Vector list of prerequisites
    
    Course() {
        
    }
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
            Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);


public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
        void Insert(Course );
      void Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {

    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {

    this->inOrder(root); //Calls private method and logic

}


/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {

    
    if (root == nullptr) { //If empty...
        root = new Node(course); //Create new node
    }
    else { // if not empty...
        this->addNode(root, course); //Call addNode logic and add course
    }
     
}



/**
 * Search for a course
 */
void BinarySearchTree::Search(string courseId) {

    
    Node* curr = root;
    
    while (curr != nullptr) {

        if (curr->course.courseId.compare(courseId) == 0) { //If curr course matches, return course

            cout << curr->course.courseId << ": " << curr->course.courseName << endl;
            cout << "Prerequisites: ";

            for (string prerequisites : curr->course.prerequisites) {   //For the list of prerequisites...
                if (prerequisites == curr->course.prerequisites.back()) {   //If last prerequisite in list...
                    
                    cout << prerequisites << "\n" << endl; //Print prerequisite
                }
                else {
                    cout << prerequisites << ", "; //Print prerequisite, comma, prerequisite...
                }
            }
            return;
        }
        else if(curr->course.courseId.compare(courseId) > 0)  { //If curr course is smaller, traverse left
            curr = curr->left;
        }
        else {
            curr = curr->right;  //Else, traverse right
        }
    }
  
    cout << "Course: " << courseId << " not found." << endl;
    return;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
     
    if (node->course.courseId.compare(course.courseId) > 0) { //Find node down the left subtree
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            this->addNode(node->left, course); //Recursively keep searching for empty left node
        }
    }

    if (node->course.courseId.compare(course.courseId) < 0) { //Find node down the right subtree
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            this->addNode(node->right, course);  //Recursively keep searching for empty right node
        }
    }
        }


void BinarySearchTree::inOrder(Node* node) {

       
    if (node != nullptr) {
        inOrder(node->left); //Recursively move left to bottom node
        
        cout << node->course.courseId << ": " << node->course.courseName << endl; //Print course

        inOrder(node->right); //Start recursively moving right
    }
      
}


void loadcourses(string csvPath, BinarySearchTree* bst) { //Opens file
   
    ifstream file(csvPath);

    if (file.is_open()) {
        cout << "\nCSV file found.\n" << endl;

        int count;
        string row;
        string word;

        while (getline(file, row)) {
            count = 0; //Counter for counting columns
            Node* node = new Node(); //New node for loading data

            stringstream data(row); // Defines row as data



            while (count < 2) { //While course has no prerequisites...

                getline(data, word, ','); // Grabs row from file

                if (count == 0) {
                    node->course.courseId = word; //First item is course ID
                }
                else if (count == 1) {
                    node->course.courseName = word;  //Second item is courseName
                }
                count++; //Increment column count
                
            }
            while (getline(data, word, ',')) {
                node->course.prerequisites.push_back(word); //Pushes prerequistes to end while there are prerequistes
            }

            bst->Insert(node->course); // Passes node to Insert function
        }

    }
    else {
        cout << "\nFile name not found.\n" << endl;
    }
}



/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {


    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;
    string courseSearch; // String for searching for courses & prerequistes
    string filename;

    int choice = 0;
    while (choice != 9) {

        cout << "Menu:" << endl;
        cout << "  1. Load courses" << endl;
        cout << "  2. Display All courses" << endl;
        cout << "  3. Find course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            cout << "\nPlease enter a csv file name(case sensitive, formatted as filename.csv or filename.txt): ";
            cin >> filename;
            loadcourses(filename, bst);

            break;

        case 2:

            cout << "\n";
            bst->InOrder(); //Prints in alphanumeric order
            cout << "\n";
            break;

        case 3:

            cout << endl;
            cout << "Please enter a course ID (case sensitive): ";
            cin >> courseSearch; 
            cout << "\n";

            bst->Search(courseSearch);
            break;
              
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
