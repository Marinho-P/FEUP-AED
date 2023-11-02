#include "Menu.h"
#include <iostream>

using namespace std;
Menu ::Menu() {
}


int Menu::run() {
    while (true) {
        int option = options();
        switch (option) {
            case 0: {
                cout << ">> Exiting the menu." << endl;
                return 0;
            }
            case 1: {
                //checkUcSchedule(); Isto não está implementado
                //input();
                break;
            }
            case 2: {
                CheckStudent();
                break;
            }
            case 3: {
                ClassSchedule();
                break;
            }
            case 4: {
                checkStudentSchedule();
                break;
            }
            case 5: {
                requests();
                break;
            }
            default:
                cout << ">> Invalid option try again." << endl;
        }
    }
}
int Menu::options() const{
    int choice;
    cout << "Menu Options:" << endl;
    cout << "1. Check UC Schedule" << endl; // not possible? (overlapping schedules)
    cout << "2. Check Students" << endl;
    cout << "3. Check Class Schedule" << endl;
    cout << "4. Check Student Schedule" << endl;
    cout << "5. Requests" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice:";
    cin >> choice;
    return choice;
}

void Menu::ClassSchedule() {
    string classCode;
    cout << "Enter a Class code:";
    cin >> classCode;
    cout << "\n";
    dataProcessor.scheduleOfClass(classCode);
}
void Menu::CheckStudent() {
    cout << "1. See all students in a class" << endl;
    cout << "2. See all students in a year" << endl;
    cout << "3. See all students in a course" << endl;
    cout << "Press any other key to return to menu" << endl;
    cout << "Enter your choice:";
    int choice;
    cin >> choice;
    cout << endl;
    string sort = sorting();
    if (sort == "exit"){
        cout << ">> Exiting the menu." << endl;
        return;
    }
    switch (choice) {
        case 1: {
            string classCode;
            cout << "Enter a class code:";
            cin >> classCode;
            cout << "\n";
            dataProcessor.studentsInClass(classCode,sort);
            break;
        }
        case 2:{
            string year;
            cout << "Enter a year (ex. 2020):";
            cin >> year;
            cout << "\n";
            dataProcessor.studentsInYear(year,sort);
            break;
        }
        case 3:{
            string course;
            cout << "Enter a course (ex. LEIC):";
            cin >> course;
            cout << "\n";
            dataProcessor.studentsInCourse(course,sort);
            break;
        }
        default:{
            cout << ">> Returning to menu" << endl;
        }
    }
}
void Menu :: requests(){
    cout << "1. Add new request to list of pending requests" << endl;
    cout << "2. Process request(s)" << endl;
    cout << "3. Discard request(s)" << endl;
    cout << "4. List pending requests" << endl;
    cout << "5. List successful requests" << endl;
    cout << "6. Undo Successful request" << endl;
    cout << "Press any other key to return to menu" << endl;
    cout << "Enter your choice:" ;
    int option;
    cin >> option;
    cout << endl;
    switch(option){
        case 1:{
            addRequestoList();
            break;
        }
        case 2:{
            processRequests();
            break;
        }
        case 3:{
            discardRequests();
            break;
        }
        case 4:{
            listPendingRequests();
            break;
        }
        case 5:{
            listSuccessfullRequests();
            break;
        }
        case 6:{
            undoRequest();
        }
         default:{
            cout << ">> Returning to menu" << endl;
            break;
        }

    }
}

void Menu::addRequestoList() {
    string id;
    cout << "Enter student id:";
    cin >> id;
    cout << endl;
    Student find_Student = Student();
    find_Student.setId(id);
    set<Student> test = dataProcessor.getStudents();
    auto it  =  test.find(find_Student);
    if (it == dataProcessor.getStudents().end()) {
        cout << "Student not found" << endl;
        return;
    }
    cout << "1. Change class of an UC" << endl;
    cout << "2. Enroll in a new UC" << endl;
    cout << "3. Leave UC" << endl;
    cout << "Press any other key to return to menu" << endl;
    cout << "Enter your choice:";
    int choice;
    cin >> choice;
    cout << endl;
    switch (choice) {
        case 1:{
            string UcCode,startCode,endCode;
            cout << "Enter the UcCode:";
            cin  >> UcCode;
            cout << endl << "Enter the classCode of the class that will be changed:";
            cin >> startCode;
            cout << endl << "Enter the classCode of the new class:";
            cin >> endCode;
            cout << endl;

            dataProcessor.addPendingRequest(Request(const_cast<Student&> (*it),"switch",UcCode,startCode, endCode ));
            break;
        }
        case 2:{
            string UcCode;
            cout << "Enter a UcCode:";
            cin >> UcCode;
            cout << endl;
            dataProcessor.addPendingRequest(Request(const_cast<Student&> (*it),"add",UcCode,"-", "-" ));
            break;

        }
        case 3:{
            string UcCode;
            cout << "Enter a UcCode:";
            cin >> UcCode;
            cout << endl;
            dataProcessor.addPendingRequest(Request(const_cast<Student&> (*it),"remove",UcCode,"-", "-" ));
            break;
        }
        default:{
            cout << ">> Returning to menu" << endl;
            break;
        }
    }


}

void Menu::processRequests() {
    int RequestId;
    cout << "Enter a Pending Request Id ( or 0 for all requests):";
    cin >> RequestId;
    dataProcessor.processRequest(RequestId);


}

void Menu::discardRequests() {
    int RequestId;
    cout << "Enter a Pending Request Id ( or 0 for all requests):";
    cin >> RequestId;
    cout << endl;
    dataProcessor.discardRequest(RequestId);
}

void Menu::listPendingRequests() {
    dataProcessor.printPendingRequests();

}

void Menu::listSuccessfullRequests() {
    dataProcessor.lookupAllRequests();

}

void Menu::undoRequest() {
    int RequestId;
    cout << "Enter a successful Request Id:";
    cin >> RequestId;
    dataProcessor.undoRequest(RequestId);

}



string Menu::sorting() const {
    cout << "1. A to Z" << endl;
    cout << "2. Z to A" << endl;
    cout << "3. Numerical" << endl;
    cout << "4. Numerical in reverse" << endl;
    cout << "Press any other key to return to menu" << endl;
    cout << "In which order do you want the students to be displayed? ";
    int option; cin >> option; cout << endl;
    switch (option) {
        case 1:
            return "A to Z";
        case 2:
            return "Z to A";
        case 3:
            return "numerical";
        case 4:
            return "reverse numerical";
        default:{
            return "exit";
        }
    }
}

void Menu::checkStudentSchedule()  {
    string upNumber;
    cout << "Insert the student's UP number: ";
    cin >> upNumber;
    cout << endl;
    Student student = Student();
    student.setId(upNumber);
    dataProcessor.scheduleOfStudent(*(dataProcessor.getStudents().find(student)));
}

