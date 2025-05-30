//============================================================================
// Name        : VectorSorting.cpp
// Author      : Willem Kroeger
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Vector Sorting Algorithms
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information using console (std::in)
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bids.push_back(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
    int lowIndex = begin;                          //Sets lowIndex to beginning of Index values
    int highIndex = end;                           //Sets highIndex to end of Index values
    int mid = 0;                                   //Initiates midpoint variable


    mid = lowIndex + (highIndex - lowIndex) / 2;  //Sets midpoint variable to midpoint of partition
    string pivot = bids.at(mid).title;            //Sets pivot point to midpoint of partition
    bool done = false;                            //Varible for determining when sorting is complete

    while (!done) {

        //increment low index while < pivot
        while (bids.at(lowIndex).title.compare(pivot) < 0) {
            lowIndex += 1;
        }

        //decrement high index while > pivot
        while (pivot.compare(bids.at(highIndex).title) < 0) {
            highIndex -= 1;
        }

        //Determines if sorting is complete.  If zero or one elements remain, all bids are partitioned, and returns highIndex
        if (lowIndex >= highIndex) {
            done = true;
        }

        //Swaps variables at low and high index values and updates index values
        else {
            swap(bids.at(lowIndex), bids.at(highIndex));
            lowIndex += 1;
            highIndex -= 1;
        }
    }
    return highIndex;
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
    int mid = 0;

    /* Base case: If there are 1 or zero bids to sort,
    partition is already sorted. 
    If begin >= end then returns the sorted value. */
    if (begin >= end) {
        return;
    }

    /* Partition bids into low and high such that
    midpoint is location of last element in low */
    mid = partition(bids, begin, end);

    quickSort(bids, begin, mid);      // recursively sort low partition (begin to mid)
    quickSort(bids, mid + 1, end);    // recursively sort high partition (mid+1 to end)

    return;

}

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {


    int indexSmallest; // initiates indexSmallest to track lowest bid
    for (int i = 0; i < bids.size(); ++i) {

        // Find index of smallest remaining element
        indexSmallest = i;
        for (int j = i + 1; j < bids.size(); ++j) {

            /*Compare bid.title at j index with indexSmallest
             if compare is < 0, either the value of the first character that does not match is lower in the compared string,
             or all compared characters match but the compared string is shorter. Credit: https://cplusplus.com/reference/string/string/compare/
             */

            if (bids.at(j).title.compare(bids.at(indexSmallest).title) < 0){ 
                indexSmallest = j;
            }
        }

        //swap elements at i & j with builtin vector swap function
        if (indexSmallest != i) { // if indexSmallest = j, a swap is needed
            swap(bids.at(i), bids.at(indexSmallest));
        }

    }
}


/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
    }

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            bids = loadBids(csvPath);

            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 2:
            // Loop and display the bids read
            for (int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;

            break;

        case 3:
            //selectionSort and timing results
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Calls selectionSort method
            selectionSort(bids);

            cout << bids.size() << " bids sorted" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            
            break;

        case 4:
            //quickSort and timing results

            // Initialize a timer variable before loading bids
            ticks = clock();

            //Calls quickSort method and sets lowIndex to 0, highIndex to size of bids Index (bids size - 1)
            quickSort(bids, 0, bids.size() - 1);

            cout << bids.size() << " bids sorted" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

         }
    }

    cout << "Good bye." << endl;

    return 0;
}
