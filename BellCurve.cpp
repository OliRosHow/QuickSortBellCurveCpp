#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


/*
This stucture is used to store students according to their name with thier average and assigned grade.
*/
struct Student
{
    string name; // the name of the student. first and last name
    double average; // the average of the student across all grades obtained in the class
    char grade; // the letter grade assigned by whatever convention you desire. in the case of this program it is a bell curve
};

/*
This function takes a vector of doubles and returns their average
*/
double calculateAverage(vector<double> exams)
{   
    double average = 0;
    for(int i = 0; i < exams.size(); i++) // sum all of the exams
    {
        average = average + exams[i];
    }
    average = average / exams.size(); // divide them by the total number of exams for the average.

    return average;
} 

/*
This funciton is used in the partition function to abbreviate the process of switching Students' indices 
*/
void swap (int i, int j, vector<Student>& S)
{
    Student temp = S[i];
    S[i] = S[j];
    S[j] = temp;
}
/*
this function swaps the indices of Students acording to their average and updates the value of our pivotpoint in the next call for quicksort
*/
void partition(int low, int high, int& pivotpoint, vector<Student>& S)
{
        int i, j; // iterative variables
        Student pivotitem = S[low]; // pick an item to be the pivot arbitrarilly

        j = low; // start j from the first index of the provided section
        for(i = low + 1; i <= high; i++)// index i will start immediatly above j and work up
        {
            if(S[i].average < pivotitem.average) // if we find an item less than our pivot value
            {
                j++; // move j forward
                swap(i, j, S); //swap i and j to place them in the relave vacinity of where they should go
            }
        }
        pivotpoint = j; // this is the absolute location our pivot value should be 

        swap(low, pivotpoint, S); // so move the pivot value to the pivot point
}
/*
This is a textbook definition of a recursive quicksort algorithm. sorts according to Student averages in accending order
*/
void quicksort (int low, int high, vector<Student>& S)
{
    int pivotpoint;

    if(high > low) // if low ever equals or exceeds high we have partition as much as we can
    {
        partition(low, high, pivotpoint, S); // partition th sections according the pivot value
        quicksort(low, pivotpoint - 1, S); // sort those new sections provided by the partitions recusivly 
        quicksort(pivotpoint + 1, high, S);
    }
}
/*
searches for a name in a provided Student vector and returns the index of the entry that matches the name. returns -1 if the name is not in the vector
*/
int search(string name, vector<Student> S)
{
    for(int i = 0; i < S.size(); i++) // this is a very simple linear search 
    {
        if(name == S[i].name)
            return i;
    }
    return -1;

}
/*
reads data from a provided file name and stores it in a vector of Student structures. Includes exeptions for if the file cant be read, or if the 
file lacks continuity (the file may be corrupt)
*/
void read(string fileName, vector<Student>& students)
{   
    int numberOfStudents; // to store the number at the top of the file we are reading
    ifstream fileIn; // out input file stream
    vector<double> grades; // a buffer vector to hold our grades so we can average them
    string name; // name holder
    string line; //line is mostly used to parse doubles via a string stream
    double grade; // this holds a single grade. this could be grades in transit to be averaged or the average grade itself
    Student student;// placeholder student variable to create our structure to store in the vector
    
    fileIn.open(fileName);
    if (!fileIn) // if there isn't a file you cant open it
    {
        cout << "Couldn't open " << fileName << "\n";
    }
    else
    {
        getline(fileIn, line);// get the number of students at the top of the file
        numberOfStudents = stoi(line);

        while(!fileIn.eof()) // read through the entire file
        {
            
            getline(fileIn, name); // take in the name
            getline(fileIn, line); // take in a line for a string stream
            
            stringstream ss(line); // create the string stream
            
            while(ss >> grade) //take in each token delimited by a space as a double
            {     
                grades.push_back(grade);
            }
            grade = calculateAverage(grades); // get our average
            grades.clear();// clear the vector for use again
            
            student = Student{name, grade, 'I'}; // create our student structure. I is used as a placeholder grade
            students.push_back(student); // add that student to our vector in main

        }
        fileIn.close();
    }

    if((numberOfStudents != students.size()) && fileIn) // if there was a file but the size of our final array doesn't match the number at the top then somthing is wrong
        cout << "WARNING: the number of students listed at the top of the file doen't match the number of students read. File may be corrupt.\n";
}
/*
this funciton takes a vector of Student Structs and assigns them the necessary grade according to a bell curve based on thier class average. this method only works for 
vectors of size 10 and above that are multiples of 10. This function also maintains the integrety of the order of the original vector.
*/
void assignGrades(vector<Student>& students)
{
    vector<int> gradeDistribution;// holds our grade distribution according to a bell curve
    vector<Student> tempStudents; //used so we dont alter the order of the original vector


    for(int i = 0; i < students.size(); i++)
    {     
        tempStudents.push_back(Student{students[i].name, students[i].average, students[i].grade}); // copy the original vector to our temp vector
    }

    gradeDistribution.push_back((students.size()/10) * 1); // the number of Fs we can give
    gradeDistribution.push_back((students.size()/10) * 2); // the number of Ds we can give
    gradeDistribution.push_back((students.size()/10) * 4); // the number of Cs we can give
    gradeDistribution.push_back((students.size()/10) * 2); // the number of Bs we can give
    gradeDistribution.push_back((students.size()/10) * 1); // the number of As we can give

    quicksort(0, tempStudents.size() -1, tempStudents); // sort our temp vector in accending order

/*
The following tripple nested loop feeding to a switch is easiest explained graphiclly with a table
      || k = 0 | k = 1 | k = 2| k = 3 ||
======================================||
j = 0 || i = 0 |       |      |       || F
j = 1 || i = 1 | i = 2 |      |       || D
j = 2 || i = 3 | i = 4 |i = 5 | i = 6 || C
j = 3 || i = 7 | i = 8 |      |       || B
j = 4 || i = 9 |       |      |       || A

*/
    for(int i = 0; i < tempStudents.size(); i++)
    {
       for(int j = 0; j < gradeDistribution.size(); j++)
       {
            for(int k = 0; k < gradeDistribution[j]; k++)
            {
                switch(j)
                {
                    case 0:
                        tempStudents[i].grade = 'F';
                        break;
                    case 1:
                        tempStudents[i].grade = 'D';
                        break;
                    case 2:
                        tempStudents[i].grade = 'C';
                        break;
                    case 3:
                        tempStudents[i].grade = 'B';
                        break;
                    case 4:
                        tempStudents[i].grade = 'A';
                        break;
                    default:
                        break;
                }
                i++;
            }
            
       }
    }

    // transfer the assigned grades from the temp vector to our ordered vector
    for(int i = 0; i < tempStudents.size(); i++)
    {   

        students[search(tempStudents[i].name, students)].grade = tempStudents[i].grade;
    }


}
/*
this main function reads a file, and assigns grades to students based on thier average according to a bell curve. The output is to the terminal.
*/
int main()
{

    vector<Student> students; // the vector that contains our student structures in order
    read("input.txt", students); // read from the file named accordingly and assign students
    assignGrades(students); // give students thier letter grade

    for(int i = 0; i < students.size(); i++) // post grades to terminal
    {
        cout << students[i].name << " - "  << students[i].grade << "\n";
    }

    return 0;
}