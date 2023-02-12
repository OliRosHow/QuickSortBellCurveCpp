#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;



struct Student
{
    string name;
    double average;
};

double calculateAverage(vector<double> exams)
{   
    double average = 0;
    for(int i = 0; i < exams.size(); i++)
    {
        average = average + exams[i];
    }
    average = average / exams.size();

    return average;
} 
void swap (int i, int j, vector<Student>& S)
{
    Student temp = S[i];
    S[i] = S[j];
    S[j] = temp;
}
void partition(int low, int high, int& pivotpoint, vector<Student>& S)
{
        int i, j;
        Student pivotitem = S[low];
        Student temp;

        j = low;
        for(i = low + 1; i <= high; i++)
        {
            if(S[i].average < pivotitem.average)
            {
                j++;
                swap(i, j, S);
            }
        }
        pivotpoint = j;

        swap(low, pivotpoint, S);
}

void quicksort (int low, int high, vector<Student>& S)
{
    int pivotpoint;

    if(high > low)
    {
        partition(low, high, pivotpoint, S);
        quicksort(low, pivotpoint -1, S);
        quicksort(pivotpoint + 1, high, S);
    }
}

int search(string name, vector<Student> S)
{
    for(int i = 0; i < S.size(); i++)
    {
        if(name == S[i].name)
            return i;
    }
    return -1;

}

int main()
{
    ifstream fileIn;
    int numberOfStudents;
    vector<int> gradeDistribution;
    vector<string> alphabetOrderedNames;
    vector<double> grades;
    vector<Student> students;
    string name;
    string line;
    double grade;
    Student student;




    fileIn.open("input.txt");
    
    getline(fileIn, line);
    numberOfStudents = stoi(line);
    
    gradeDistribution.push_back((numberOfStudents/10) * 1);
    gradeDistribution.push_back((numberOfStudents/10) * 2);
    gradeDistribution.push_back((numberOfStudents/10) * 4);
    gradeDistribution.push_back((numberOfStudents/10) * 2);
    gradeDistribution.push_back((numberOfStudents/10) * 1);
    
    while(!fileIn.eof())
    {
        
        getline(fileIn, name);
        getline(fileIn, line);
        
        stringstream ss(line);
        
        while(ss >> grade)
        {     
            grades.push_back(grade);
        }
        grade = calculateAverage(grades);
        grades.clear();
        
        student = Student{name, grade};
        alphabetOrderedNames.push_back(name);
        students.push_back(student);
        

        

    }
    quicksort(0, students.size() - 1, students);

    for(int i = 0; i < students.size(); i++)
    {
        cout << students[i].name << ": " << students[i].average << "\n";
    }




    fileIn.close();
    return 0;
}