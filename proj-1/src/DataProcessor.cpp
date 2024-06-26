#include "DataProcessor.h"


/**
 * @brief Constructor of DataProcessor.Reads all data from the given csv files and stores them in the corresponding data structures
 * @details Time complexity - O(n+m*log(h)+w) with n, m, w being the number of lines of students_classes.csv, classes.csv, classes_per_uc.csv respectively and h the number of all classes' schedules
 */

DataProcessor::DataProcessor(){
    students_classes();
    classes();
    classes_per_uc();
}
/**
 * @brief Processes data of the file "classes_per_uc.csv".Stores all existing ClassesUcs in a set.
 * @details Time complexity - O(n) with n being the number of lines of the processed file
 */

void DataProcessor::classes_per_uc() {
    ifstream file("classes_per_uc.csv");
    string header;
    getline(file,header);
    string line;
    while (getline(file,line)){
        istringstream separated_comma(line);
        string classCode , ucCode;
        getline(separated_comma,ucCode,',');
        getline(separated_comma,classCode,',');
        existingClassesUc.insert(Class_UC(classCode,ucCode));
    }
}
/**
 * @brief Getter for all ClassesUcs available
 * @details Time complexity - O(1)
 * @return a set of all ClassesUcs
 */
const set<Class_UC> &DataProcessor::getClassUc() const{
    return existingClassesUc;
}
/**
 * @brief Getter of all classes' schedules
 * @details Time complexity - O(1)
 * @return a set of schedules
 */
const set<Schedule> &DataProcessor::getSchedules() const{
    return schedules;
}
/**
 * @brief Processes data of the file "students_classes.csv".Stores all students in a set and their corresponding ClassesUCs.
 * @details Time complexity - O(n) with n being the number of lines of the file
 */
void DataProcessor::students_classes() {
    ifstream file("students_classes.csv");
    string header;
    getline(file,header);
    string line;
    Student lastSeenStudent;
    set<Class_UC> setOfClasses;
    bool first_it = true;
    while (getline(file,line)){
        istringstream separate_comma(line);
        string ucCode,classCode,studentName,studentCode;
        getline(separate_comma,studentCode,',');
        getline(separate_comma,studentName,',');
        getline(separate_comma,ucCode,',');
        separate_comma >> classCode;
        if(first_it) {
            setOfClasses.insert(Class_UC(classCode, ucCode));
            lastSeenStudent = Student(studentName, studentCode, setOfClasses);
            first_it = false;
        }

        if (lastSeenStudent.getId() == studentCode){
            lastSeenStudent.addClassUc(Class_UC(classCode,ucCode));
        } else{
            students.insert(lastSeenStudent);
            setOfClasses = set<Class_UC>();
            lastSeenStudent = Student(studentName,studentCode,setOfClasses);
            lastSeenStudent.addClassUc(Class_UC(classCode,ucCode));
        }

    }
    students.insert(lastSeenStudent);

}

/**
 * @brief Processes data of the file "classes.csv". Stores the information about each classes' schedule
 * @details Time complexity - O(m*log(n)) with m being the number of lines of the file and n the number of all classes' schedules
 */

void DataProcessor::classes(){
    ifstream file("classes.csv");
    string header;
    getline(file,header);
    string line;
    while (getline(file,line)){
        istringstream separate_comma(line);
        string classCode,ucCode,weekday,type;
        float duration,startHour;
        getline(separate_comma, classCode, ',');
        getline(separate_comma, ucCode, ',');
        getline(separate_comma, weekday, ',');
        separate_comma >> startHour;
        separate_comma.ignore();
        separate_comma >> duration;
        separate_comma.ignore();
        separate_comma >> type;
        Lecture lecture = Lecture(duration,startHour,type,weekday,ucCode);
        auto it = schedules.find(classCode);
        if (it != schedules.end()) {
            Schedule modifiedSchedule = *it;
            modifiedSchedule.addLecture(lecture);
            it = schedules.erase(it);
            schedules.insert(modifiedSchedule);
        } else {
            Schedule newSchedule(classCode);
            newSchedule.addLecture(lecture);
            schedules.insert(newSchedule);
        }
    }
}

/**
 * @brief Checks the number of students registered in at least n UCs
 * @details Time complexity - O(w) with w being the total number of existing students
 * @param n being the number of UCs
 */
void DataProcessor::studentsInAtLeastNUCs(int n) {
    int res = 0;
    for (const Student& student: students){
        if (student.getClassesUcs().size() >= n){
            res++;
        }
    }
    cout <<"There are currently " << res << " students enrolled in at least " << n << " UCs" << endl;
}

/**
 * @brief Getter of all given students
 * @details Time complexity - O(1)
 * @return a set of the existing students
 */
set<Student> &DataProcessor::getStudents(){
    return students;
}
/**
 * @brief Auxiliary function to print a given vector of students.It is printed in a certain format regarding the sortOption
 * @details Time complexity - O(n*log(n)) with n being number of students stored in a given vector
 * @param temp vector of students
 * @param sortOption has 4 different inputs : A to Z, Z to A, numerical and reversed numerical
 */
void printStudents(vector<Student> students,const string& sortOption){
    bool alphabetic = false;
    if (sortOption == "A to Z") {
        sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.getName() < b.getName();
        });
        alphabetic = true;
    } else if (sortOption == "Z to A") {
        sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.getName() > b.getName();
        });
        alphabetic = true;
    } else if (sortOption == "numerical") {
        sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.getId() < b.getId();
        });
    } else if (sortOption == "reverse numerical") {
        sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.getId() > b.getId();
        });
    }
    cout << "------------------------------" << endl;
    if (alphabetic){
        cout << "Name / ID" << endl;
        cout << "------------------------------" << endl;
        for (const Student& student : students){
            cout << student.getName() << " / " << student.getId() << endl;
        }
    }
    else{
        cout << "ID / Name" << endl;
        cout << "------------------------------" << endl;
        for (const Student& student : students){
            cout << student.getId() << " / " << student.getName() << endl;
        }
    }
    cout << "------------------------------" << endl;
}
/**
 * @brief Checks which students are in a given class and prints them
 * @details Time complexity - O(n*log(n)) with n being number of students stored in the temp vector (Worst case scenario)
 * @param classCode The code of a given class
 * @param sortOption has 4 different inputs : A to Z, Z to A, numerical and reversed numerical
 */
void DataProcessor::studentsInClass(const string& classCode,const string& sortOption) {
    vector<Student> temp;
    for (const Student& student:students){
        for (const Class_UC& classUc:student.getClassesUcs()){
            if (classCode == classUc.getClassCode()){
                temp.push_back(student);
                break;
            }
        }
    }
    if (!temp.empty()){
        cout << "Students in class " << classCode << " are:" << endl;
        printStudents(temp,sortOption);
    }
    else{
        cout << "------------------------------------" << endl;
        cout << "There are no students in this class." << endl;
        cout << "------------------------------------" << endl;
    }
}
/**
 * @brief Checks how many and which students are enrolled in a given UC
 * @details Time complexity - O(n*log(n)) with n being number of students stored in the temp vector (Worst case scenario)
 * @param ucCode
 * @param sortOption
 */
void DataProcessor::studentsInUc(const std::string &ucCode, const std::string &sortOption) {
    vector<Student> temp;
    for(const Student& student:students){
        for(const Class_UC& classUc:student.getClassesUcs()){
            if(ucCode == classUc.getUcCode()){
                temp.push_back(student);
                break;
            }
        }
    }
    if (!temp.empty()){
        cout << "There are " << temp.size() << " students enrolled in the UC " << ucCode << ", who are:" << endl;
        printStudents(temp,sortOption);
    }
    else{
        cout << "------------------------------------------" << endl;
        cout << "There are no students enrolled in this UC." << endl;
        cout << "------------------------------------------" << endl;
    }
}

/**
 * @brief Checks how many students are in LEIC course and prints them
 * @details Time complexity - O(n*log(n)) with n being number of students stored in the temp vector (Worst case scenario)
 * @param course A given course
 * @param sortOption has 4 different inputs : A to Z, Z to A, numerical and reversed numerical
 */
void DataProcessor::studentsInCourse(string course,const string& sortOption) {
    transform(course.begin(), course.end(), course.begin(), ::toupper);
    if (course == "LEIC") {
        cout << "Students in course " << course << " are:" << endl;
        vector<Student> temp;
        copy(students.begin(),students.end(), back_inserter(temp));
        printStudents(temp,sortOption);
    }
    else{
        cout << "-------------------------------------" << endl;
        cout << "There are no students in that course." << endl;
        cout << "-------------------------------------" << endl;
    }

}

/**
 * @brief Checks the students from a given year and prints them
 * @details Time complexity - O(n*log(n)) with n being number of students stored in the temp vector (Worst case scenario)
 * @param year A given year (ex:2022)
 * @param sortOption has 4 different inputs : A to Z, Z to A, numerical and reversed numerical
 */
void DataProcessor::studentsInYear(const string &year,const string& sortOption) {
    vector<Student> temp;
    for (const Student& student:students){
        if(year == student.getYear()){
            temp.push_back(student);
        }
    }
    if (!temp.empty()){
        cout << "Students from " << year << " are:" << endl;
        printStudents(temp,sortOption);
    }
    else{
        cout << "-------------------------------------" << endl;
        cout << "There are no students from that year." << endl;
        cout << "-------------------------------------" << endl;
    }
}

/**
 * @brief Checks the Uc or Ucs with the most number of enrolled students and prints the ucCode and the number of students
 * @details Time complexity - O(n*m) with n being the number of existing students and m the number of ClassUcs of a student
 */
void DataProcessor::ucGreatestNumberStudents() {
    unordered_map<string,int> numberUcStudents;
    for (const Student& student:students){
        for (const Class_UC& classesUc :student.getClassesUcs() ){
            numberUcStudents[classesUc.getUcCode()]++;
        }
    }
    int maxNumberStudent = INT_MIN;
    vector<string> ucMax;
    for (const auto& pair:numberUcStudents){
        if (pair.second > maxNumberStudent){
            maxNumberStudent = pair.second;
            ucMax.clear();
            ucMax.push_back(pair.first);
        }
        else if (pair.second == maxNumberStudent){
            ucMax.push_back(pair.first);
        }
    }
    if (ucMax.size() > 1){
        int lastIteration = (int)ucMax.size()-1;
        cout << "-----------------------------------" << endl;
        cout << "The UCs with the most students are: " << endl;
        cout << "-----------------------------------" << endl;
        for (const string& ucs :ucMax){
            if (!lastIteration--){
                cout << ucs << endl;
            }
            cout << ucs <<  "/";
        }
        cout << "Having " << maxNumberStudent << " enrolled students." << endl;
        cout << "-----------------------------------" << endl;
    } else{
        cout << "---------------------------------" << endl;
        cout << "The UC with the most students is: " << endl;
        cout << "---------------------------------" << endl;
        cout << ucMax.front() << endl;
        cout << "Having " << maxNumberStudent << " enrolled students." << endl;
        cout << "---------------------------------" << endl;
    }
}

/**
 * @brief Checks the schedule of a given class and prints it
 * @details Time complexity - O(n+m) with n being the number of all classes' schedules and m the number of lectures in a schedule
 * @param classCode A given class
 */
void DataProcessor::scheduleOfClass(const string &classCode) {
    for(const Schedule& schedule:schedules){
        if (schedule.getClassCode() == classCode){
            schedule.printSchedule();
            return;
        }
    }
    cout << "No schedule found." << endl;
}
/**
 * @brief Creates the schedule of a given student and prints its schedule
 * @details Time complexity - O(n*log(k)+m*log(m)) with n being the number of classUCs of a student, k the number of schedules of a class and m the number of Lectures to print
 * @param student Given a student
 */

void DataProcessor::scheduleOfStudent(const Student &student) {
    Schedule new_schedule = createStudentSchedule(student);
    new_schedule.printSchedule();
}
/**
 * @brief Creates the schedule for a student
 * @details Time complexity - O(n*log(m)) with n being the number of classUCs of a student and m the number of schedules of a class
 * @param student Given a student
 * @return a student's schedule
 */

Schedule DataProcessor:: createStudentSchedule(const Student &student) {
    Schedule new_schedule;
    for (const Class_UC &classUc:student.getClassesUcs()){
        auto it = schedules.find(Schedule(classUc.getClassCode()));
        if (it != schedules.end()){
            for (const Lecture& lecture:(*it).getLectures()){
                if (lecture.getUcCode() == classUc.getUcCode()){
                    new_schedule.addLecture(lecture);
                }
            }
        }
    }
    return new_schedule;
}
/**
 * @brief Adds a pending request to a vector containing all pending requests
 * @details Time complexity - O(1)
 * @param request
 */
void DataProcessor::addPendingRequest(const Request &request) {

    PendingRequests.push_back(request);
    cout << "Request added successfully" << endl;
    cout << ">> Returning to menu" << endl;
}
/**
 * @brief Processes one or all requests at once given a RequestId else doesn't do any if invalid RequestId
 * RequestId = 0 (Process all requests)
 * @details Time complexity - O(n) with n being the number of pending requests
 * @param RequestID
 */
void DataProcessor::processRequest(int RequestID) {
    if( RequestID< 0 || RequestID > PendingRequests.size() ){
        cout << "Request Id match not found";
        return;
    }
    if( RequestID == 0){
        for(Request &request:PendingRequests){
            performRequest(request);
        }
        discardRequest(RequestID);

    }
    else{
        performRequest(PendingRequests[RequestID-1]);
        discardRequest(RequestID);
    }

}
/**
 * @brief Perform a given request of type: add, remove or switch
 * @details O(n+w*m) with n being the number of unique Class_UCs, w the number of students and m the number of Class_UCs per student
 * @param request
 */
void DataProcessor::performRequest(Request &request) {

    if(request.getType() == "add"){
        AddRequest(*(request.getStudent()),request.getUcCode(),true);
    }
    if(request.getType() == "remove"){
        RemoveRequest(*(request.getStudent()),request.getUcCode(),true);
    }
    if(request.getType() == "switch"){
        SwitchRequest(*(request.getStudent()),request.getStartCode(),request.getEndCode(),request.getUcCode(),true);
    }
}

static int Cap = 30;
/**
 * @brief Executes an add request given a student, UcCode and "save" boolean that determines whether the request is saved or not
 * @details Time complexity - O(n+w*m) with n being the number of unique Class_UCs, w the number of students and m the number of Class_UCs per student
 * (Worst case scenario where checkAdd is time dominant)
 * @param student
 * @param UcCode
 * @param save
 */
// each line of RequestHistory.csv goes : id, StudentName, type (add, remove or switch), UcCode, startCode(if any), endCode(if any)
void DataProcessor::AddRequest(const Student &student,const string &UcCode, bool save ) {

    for(const Class_UC &classUc: student.getClassesUcs()){
        if(classUc.getUcCode() == UcCode){
            cout << "Request denied: Student is already enrolled in specified UC" << endl;
            return;
        }
    }
    bool checkExistence = false;
    for( const Class_UC &classUc: existingClassesUc){
        if(classUc.getUcCode() == UcCode){
            checkExistence = true;
            break;
        }
    }
    if(!(checkExistence)){
        cout << "Request denied: No matching UcCode" << endl;
        return;
    }
    if(student.getClassesUcs().size() >= 7){
        cout << "Request denied: A student cannot be registered in more than 7 UCs at any given time" << endl;
        return;
    }
    for( const Schedule &schedule: schedules){
        Schedule old_schedule = createStudentSchedule(student);
        Class_UC classUcToCheck(schedule.getClassCode(),UcCode);
        if(checkAdd(student,classUcToCheck)){
            if(checkScheduleCollisions( FuseSchedules(old_schedule,schedule,UcCode) )){

                student.addClassUc(Class_UC(schedule.getClassCode(), UcCode));
                if(save){
                    saveRequest(student,  "add",  UcCode, "-", "-");

                }
                ChangeFileAdd(student, schedule.getClassCode(), UcCode);
                cout << "Request accepted" << endl;
                return;
            }
        }

    }
    cout << "Request denied: No class with vacancy found" << endl;

}
/**
 * @brief Executes a remove request given a student, UcCode and "save" boolean that determines whether the request is saved or not
 * @details Time complexity - O(n+w*m) with n being the number of unique Class_UCs, w the number of students and m the number of Class_UCs per student
 * (Worst case scenario where checkRemove is time dominant)
 * @param student
 * @param UcCode
 * @param save
 */
void DataProcessor::RemoveRequest(const Student &student, const string& UcCode, bool save){
    bool check = false;
    string classCode; // class for the UC enrolled
    for( const Class_UC &classUc: student.getClassesUcs()){
        if(classUc.getUcCode() == UcCode){
            check = true;
            classCode = classUc.getClassCode();
            break;
        }
    }
    if(!(check)){
        cout << "Request denied: Student isn't enrolled on specified UC" << endl;
        return;
    }

    Class_UC classUcToCheck(classCode,UcCode);
    if(checkRemove(student,classUcToCheck)){
        student.removeClassUc(classUcToCheck);
        if(save){
            saveRequest(student,  "remove",  UcCode, "-", "-");
        }
        ChangeFileRemove(student,UcCode);
        cout << "Request accepted" << endl;
    }
    else{
        cout << "Request denied: removing Student from UC would disturb the balance between classes" << endl;
    }
}
/**
 * @brief Executes a switch request given a student, their currentClassCode and newClassCode, the corresponding UcCode and a "save" boolean that determines whether the request is saved or not
 * @details Time complexity - O(n+w*m) with n being the number of unique Class_UCs , w the number of students and m the number of Class_UCs per student
 * (Worst case scenario where checkSwitch is time dominant)
 * @param student
 * @param oldClassCode
 * @param newClassCode
 * @param UcCode
 * @param save
 */
void DataProcessor::SwitchRequest(const Student &student, const string &oldClassCode, const string &newClassCode, const string &UcCode, bool save) {

    if (existingClassesUc.find(Class_UC(oldClassCode, UcCode)) != existingClassesUc.end() ||
        existingClassesUc.find(Class_UC(newClassCode, UcCode)) != existingClassesUc.end()) {
        cout << "Request denied: Classes given don't have a matching UC" << endl;
        return;
    }
    Class_UC oldClassUc(oldClassCode,UcCode);
    Class_UC newClassUc(newClassCode,UcCode);
    if(checkSwitch(student,oldClassUc,newClassUc)){
        Schedule new_schedule = *(schedules.find(newClassCode));
        Schedule old_schedule = createStudentSchedule(student);
        if(checkScheduleCollisions(switchFuseSchedules(old_schedule,new_schedule,UcCode))){

            student.removeClassUc(Class_UC(oldClassCode,UcCode));
            student.addClassUc(Class_UC(newClassCode,UcCode));
            if(save){
                saveRequest(student,"switch",UcCode, oldClassCode,newClassCode);

            }
            ChangeFileSwitch(student,UcCode,newClassCode);
            cout << "Request accepted" << endl;
        }
        else{
            cout << "Request denied: schedule collision" << endl;
        }

    }
    else{
        cout << "Request denied: changing class of Student would disturb the balance between classes " << endl;
    }
}
/**
 * @brief Lists all successful requests saved in 'RequestHistory.csv'
 * @details Time complexity - O(n) where n is the number of lines in 'RequestHistory.csv'
 */
void DataProcessor::lookupAllRequests() {
    ifstream file("RequestHistory.csv");
    if(!(file.is_open())){
        cout << "Couldn't find 'RequestHistory.csv' file" << endl;
        return ;
    }
    string line;
    int RequestID = 1;
    while(getline(file, line)){
        istringstream separate_comma(line);
        string StudentName, id, type , UcCode, startCode, endCode;

        if(type == "switch"){
            getline(separate_comma, id, ',');
            getline(separate_comma, StudentName, ',');
            getline(separate_comma, type, ',');
            getline(separate_comma, UcCode, ',');
            getline(separate_comma, startCode, ',');
            separate_comma >> endCode;
            cout <<"RequestID:" << RequestID << " Student Name:" <<  StudentName << "(" << id << ") Type: switch(" << UcCode<< ") " << startCode << " -> " << endCode << endl;
        }
        else{
            getline(separate_comma, id, ',');
            getline(separate_comma, StudentName, ',');
            getline(separate_comma, type, ',');
            getline(separate_comma, UcCode, ',');
            cout <<"RequestID:" << RequestID << " Student Name:" << StudentName << "(" << id << ") Type: "<< type << "(" << UcCode << ")" << endl;

        }
        RequestID++;


    }
    cout << "Total number of requests: " << RequestID-1 << " >> Returning to menu" << endl;


}


/**
 * @brief Saves a request to the file 'RequestHistory.csv'.
 * @details Time complexity - O(1)
 * @param student
 * @param type
 * @param UcCode
 * @param startCode
 * @param endCode
 */
void DataProcessor::saveRequest(const Student &student, const string &type, const string &UcCode, const  string &startCode, const string &endCode) {
    ofstream adder("RequestHistory.csv",std::ios_base::app | std::ios_base::out);
    if(!(adder.is_open())) {
        cout << "Couldn't find 'RequestHistory.csv' file";
        return;
    }

    adder << student.getId() <<"," <<student.getName() << "," <<type << ","<< UcCode << "," << startCode << "," << endCode << "\n";

    adder.close();

}
/**
 * @brief Given a RequestId it removes from the RequestHistory.csv file the request with that Id and undoes the changes made by that add/remove/switch request
 * @details Time complexity - O(n) with n being the number of lines of the RequestHistory.csv file.
 * (Having in mind that when n->+∞ the time complexity of n overshadows the operations made within the loop)
 * @param RequestID
 */
void DataProcessor::undoRequest(int RequestID) {
    int counter = 1;
    ifstream file("RequestHistory.csv");
    ofstream temp("tempFile_undo.csv");
    if(!(file.is_open())) {
        cout << "Couldn't find 'RequestHistory.csv' file";
        return;

    }
    string line;
    while(getline(file,line))   {
        if(counter == RequestID){ // skipped adding line
            istringstream separate_comma(line);
            string id, StudentName,type, UcCode,  startCode , endCode;
            getline(separate_comma, id, ',');
            getline(separate_comma, StudentName, ',');
            getline(separate_comma, type, ',');
            getline(separate_comma, UcCode, ',');
            getline(separate_comma, startCode, ',');
            separate_comma >> endCode;
            Student find_student = Student(); // stub student to find real student
            find_student.setId(id); // search criterion for student class
            if(type == "switch"){


                SwitchRequest( *(students.find(find_student)) ,endCode,startCode,UcCode,false) ;
            }
            if(type == "remove"){
                AddRequest( *(students.find(find_student)) ,UcCode,false) ;
            }
            if(type=="add"){
                RemoveRequest( *(students.find(find_student)) ,UcCode,false) ;
            }


        }
        else{
            temp << line << "\n";
        }
        counter++;
    }
    remove("RequestHistory.csv");
    rename("tempFile_undo.csv","RequestHistory.csv");
    file.close();
}
/**
 * @brief Compares two lectures based on startHour and Weekday.
 * @details Time complexity - O(1)
 * @param a
 * @param b
 * @return true if Lecture a starts first then Lecture b else false
 */

bool compareLectures(const Lecture& a, const Lecture& b) {
    if (a.getWeekday() == b.getWeekday()) {
        return a.getStartHour() < b.getStartHour();
    }
    return a.getWeekday() < b.getWeekday();
}
/**
 * @brief Checks if there are collisions between lectures on a given vector of lectures.
 * @details Time complexity - O(n) where n is the number of lectures
 * @param lectures
 * @return false if there are overlapping lectures else true
 */
bool DataProcessor::checkScheduleCollisions(vector<Lecture> lectures){
    sort(lectures.begin(),lectures.end(),compareLectures);
    for (size_t i = 0; i < lectures.size() - 1; ++i) {
        const Lecture& currentLecture = lectures[i];
        const Lecture& nextLecture = lectures[i + 1];

        // Check for overlap on the same day
        if (currentLecture.getWeekday() == nextLecture.getWeekday()) {
            float currentEndTime = currentLecture.getStartHour() + currentLecture.getDuration();
            if (currentEndTime > nextLecture.getStartHour()) {
                return false;
            }
        }
    }

    return true;
}
/**
 * @brief Checks if an add request is going to disturb the balance between ClassUcs
 * @details Time complexity - O(n+w*m+k) with n being the size of the given map numberOfStudentsPerClass, w the number of students, m the number of Class_UCs per student and k the number of classes associated with the student to be added
 * @param studentToAdd
 * @param classUcToCheck
 * @return true if doesn't disturb the balance else false
 */
bool DataProcessor:: checkAdd(const Student &studentToAdd, const Class_UC &classUcToCheck ){
    for( const Class_UC &classUc : studentToAdd.getClassesUcs()){ // if student is already in a class then it's always available
        if(classUc == classUcToCheck){
            return true;
        }
    }
    map<Class_UC,int> numberOfStudentsPerClass;
    numberOfStudentsPerClass = getNumberOfStudents(numberOfStudentsPerClass);
    pair<int, int > result = getMostAndLeastStudents(numberOfStudentsPerClass);
    int Pre_Add_MostStudents = result.first;
    int Pre_Add_LeastStudents = result.second;
    numberOfStudentsPerClass[classUcToCheck]++; // add student to desired class
    result = getMostAndLeastStudents(numberOfStudentsPerClass);
    if(result.first-result.second > Pre_Add_MostStudents-Pre_Add_LeastStudents ||result.first > Cap ){
        return false;
    }
    else{
        return true;
    }


}
/**
 * @brief Returns a map that associates each Class_UC with the number of students associated with it.
 * @details Time complexity - O(n*m) where n is the number of students and m is the number of Class_UCs per student
 * @param numberOfStudentsPerClass
 * @return map with Class_UC and number of students
 */
map<Class_UC, int> &DataProcessor::getNumberOfStudents(map<Class_UC, int> &numberOfStudentsPerClass) {
    for (const Student& student: students){ // get number of students per class
        set<Class_UC> ClassesPerStudent;
        for (const Class_UC& classUc :student.getClassesUcs() ) {
            ClassesPerStudent.insert(classUc);
        }

        for(const Class_UC &classUc : ClassesPerStudent ){
            numberOfStudentsPerClass[classUc]++;
        }

    }
    return numberOfStudentsPerClass;
}
/**
 * @brief Checks the ClassesUcs with the most and least students enrolled
 * @details Time complexity - O(n) with n being the size of the given map numberOfStudentsPerClass
 * @param numberOfStudentsPerClass
 * @return a pair with the biggest and least number of students in a ClassUc
 */
pair<int, int> DataProcessor::getMostAndLeastStudents(map<Class_UC, int> &numberOfStudentsPerClass) const {
    Class_UC ClassWithMostStudents;
    Class_UC ClassWithLeastStudents;
    int MostStudents = INT_MIN;
    int LeastStudents = INT_MAX;
    for( const auto& pair: numberOfStudentsPerClass){
        if(pair.second > MostStudents){
            ClassWithMostStudents = pair.first;
            MostStudents = pair.second;
        }
        if(pair.second < LeastStudents){
            ClassWithLeastStudents = pair.first;
            LeastStudents = pair.second;
        }
    }
    return {MostStudents,LeastStudents};
}
/**
 * @brief Adds Lectures from schedule_to_add that are associated with the UcCode to the lectures of old_schedule
 * @details Time complexity - O(n) where n is the number of Lectures in schedule_to_add
 * @param old_schedule
 * @param schedule_to_add
 * @param UcCode
 * @return updated vector of Lectures
 */
vector<Lecture> DataProcessor :: FuseSchedules(const Schedule &old_schedule, const Schedule &schedule_to_add, const string &UcCode){
    vector<Lecture> result = old_schedule.getLectures();
    for(const Lecture &lecture: schedule_to_add.getLectures()){
        if(UcCode == lecture.getUcCode()){
            result.push_back(lecture);
        }
    }
    return result;
}
/**
 * @brief Modifies the csv file "students_classes" after processing a add request.
 * To do that we need to create a new file and copy the contents of the other, skip the elements we want to remove and then rename it to the name of the old file
 * @details Time complexity - O(n) with n being the number of lines of the file students_classes.csv
 * @param student
 * @param NewClassCode
 * @param NewUcCode
 */
void DataProcessor :: ChangeFileAdd(const Student &student, const string &NewClassCode, const string &NewUcCode) {
    ifstream file("students_classes.csv");
    ofstream temp("tempFile.csv");
    string line;
    bool check = true;
    while(getline(file,line)){
        istringstream separate_comma(line);
        string id, StudentName, UcCode , ClassCode;
        getline(separate_comma, id , ',');
        getline(separate_comma,StudentName, ',');
        getline(separate_comma, UcCode, ',');
        separate_comma >> ClassCode;

        if( id == student.getId() && check){
            temp << id << "," << StudentName << "," << NewUcCode << "," << NewClassCode << "\n";
            check = false;
        }

        temp << line << "\n";
    }
    remove("student_classes.csv");
    rename("tempFile.csv","students_classes.csv");
    file.close();
}
/**
 * @brief Checks if a remove request is going to disturb the balance between ClassUcs
 * @details Time complexity - O(n+w*m) with n being the size of the given map numberOfStudentsPerClass, w the number of students and m the number of Class_UCs per student
 * @param student
 * @param classUc
 * @return true if doesn't disturb the balance else false
 */
bool DataProcessor::checkRemove(const Student &student, const Class_UC &classUc) {
    map<Class_UC,int> numberOfStudentsPerClass;
    numberOfStudentsPerClass = getNumberOfStudents(numberOfStudentsPerClass);
    pair<int, int > result = getMostAndLeastStudents(numberOfStudentsPerClass);
    int Pre_Add_MostStudents = result.first;
    int Pre_Add_LeastStudents = result.second;
    numberOfStudentsPerClass[classUc]--; // remove student from desired class
    result = getMostAndLeastStudents(numberOfStudentsPerClass);
    if(result.first-result.second > Pre_Add_MostStudents-Pre_Add_LeastStudents ||result.first > Cap ){
        return false;
    }
    else{
        return true;
    }
}
/**
 * @brief Checks if a switch request is going to disturb the balance between ClassUcs
 * @details Time complexity - O(n+w*m) with n being the size of the given map numberOfStudentsPerClass, w the number of students and m the number of Class_UCs per student
 * @param student
 * @param oldClassUc
 * @param newClassUc
 * @return true if doesn't disturb the balance else false
 */
bool DataProcessor::checkSwitch(const Student &student, const Class_UC &oldClassUc, const Class_UC &newClassUc) {
    map<Class_UC,int> numberOfStudentsPerClass;
    numberOfStudentsPerClass = getNumberOfStudents(numberOfStudentsPerClass);
    pair<int, int > result = getMostAndLeastStudents(numberOfStudentsPerClass);
    int Pre_Add_MostStudents = result.first;
    int Pre_Add_LeastStudents = result.second;
    numberOfStudentsPerClass[oldClassUc]--; // remove student from old class
    numberOfStudentsPerClass[newClassUc]++; // add student to new class
    result = getMostAndLeastStudents(numberOfStudentsPerClass);
    if(result.first-result.second > Pre_Add_MostStudents-Pre_Add_LeastStudents ||result.first > Cap ){
        return false;
    }
    else{
        return true;
    }
}
/**
 * @brief Modifies the csv file "students_classes" after processing a remove request.
 * To do that we need to create a new file and copy the contents of the other, skip the elements we want to remove and then rename it to the name of the old file
 * @details Time complexity - O(n) with n being the number of lines of the file students_classes.csv
 * @param student
 * @param UcCodeToRemove
 */
void DataProcessor::ChangeFileRemove(const Student &student, const string &UcCodeToRemove) {
    ifstream file("students_classes.csv");
    ofstream temp("tempFile.csv");
    string line;
    while(getline(file,line)){
        istringstream separate_comma(line);
        string id, StudentName, UcCode , ClassCode;
        getline(separate_comma, id , ',');
        getline(separate_comma,StudentName, ',');
        getline(separate_comma, UcCode, ',');
        separate_comma >> ClassCode;

        if( id == student.getId() && UcCode == UcCodeToRemove ){
            continue;
        }

        temp << line << "\n";
    }
    remove("student_classes.csv");
    rename("tempFile.csv","students_classes.csv");
    file.close();
}
/**
 * @brief Modifies the csv file "students_classes" after processing a switch request.
 * To do that we need to create a new file and copy the contents of the other, add the switch changes and then rename it to the name of the old file
 * @details Time complexity - O(n) with n being the number of lines of the file students_classes.csv
 * @param student
 * @param UcCodeToChange
 * @param newClassCode
 */
void DataProcessor::ChangeFileSwitch(const Student &student, const string &UcCodeToChange, const string &newClassCode) {
    ifstream file("students_classes.csv");
    ofstream temp("tempFile.csv");
    string line;
    while(getline(file,line)){
        istringstream separate_comma(line);
        string id, StudentName, UcCode , ClassCode;
        getline(separate_comma, id , ',');
        getline(separate_comma,StudentName, ',');
        getline(separate_comma, UcCode, ',');
        separate_comma >> ClassCode;

        if( id == student.getId() && UcCode == UcCodeToChange ){
            temp << id << "," << StudentName << "," << UcCode << ","  << newClassCode << "\n";
            continue;
        }

        temp << line << "\n";
    }
    remove("student_classes.csv");
    rename("tempFile.csv","students_classes.csv");
    file.close();
}
/**
 * @brief Discards one or all pending requests stored in the vector PendingRequests
 * @details Time complexity - O(n) with n being the number of all stored pending requests
 * @param RequestID
 */
void DataProcessor::discardRequest(int RequestID) {
    if(RequestID < 0 || RequestID > PendingRequests.size()){
        cout << "Request id match not found" << endl;
        return;
    }
    if( RequestID == 0){
        PendingRequests.clear();
    }
    else{
        PendingRequests.erase(PendingRequests.begin() + RequestID -1);
    }
}
/**
 * @brief Prints all current stored pending requests having two different formats between switch and add/remove
 * @details Time complexity - O(n) with n being the number of requests currently stored
 */
void DataProcessor::printPendingRequests() {
    int requestId = 1;
    for( Request &request : PendingRequests){
        if(request.getType() == "switch") {
            cout <<"RequestID:" << requestId << " Student Name:" <<  (request.getStudent())->getName() << "(" << (request.getStudent())->getId() << ") Type: switch(" << request.getUcCode()<< ") " << request.getStartCode() << " -> " << request.getEndCode() <<endl;
        }
        else{
            cout <<"RequestID:" << requestId << " Student Name:" << (request.getStudent())->getName() << "(" << (request.getStudent())->getId() << ") Type: "<< request.getType() << "(" << request.getUcCode() << ")" << endl;

        }
        requestId++;
    }
    cout << "Total number of requests: " << requestId-1 << " >> Returning to menu" << endl;
}

/**
 * @brief Removes the old Ucs from the student's current schedule and adds the new ones
 * @details Time complexity - O(n+m) with n being the number of lectures of schedule1 and m the number of lectures of schedule2
 * @param old_schedule
 * @param schedule_to_add
 * @param UcCode
 * @return new updated vector of lectures
 */
vector<Lecture> DataProcessor::switchFuseSchedules(const Schedule &old_schedule, const Schedule &schedule_to_add, const string &UcCode) {
    vector<Lecture> schedule1 = old_schedule.getLectures();
    vector<Lecture> schedule2 = schedule_to_add.getLectures();
    vector<Lecture> result;
    for(const Lecture &lecture : schedule1 ){
        if(lecture.getUcCode() != UcCode){
            result.push_back(lecture);
        }
    }
    for(const Lecture &lecture: schedule2){
        if(lecture.getUcCode() == UcCode){
            result.push_back(lecture);
        }
    }
    return result;
}




