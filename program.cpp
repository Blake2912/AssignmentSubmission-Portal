#include<iostream>
#include<sqlite3.h>
#include<string.h>

using namespace std;

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: \n", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("\t%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

int main(int argc, char* argv[]){
    int selection,subject_select;
    string name,usn;
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    string sql;
    const char* data = "Callback function called";

    cout<<"Which subject do you want to access\n\t1. Physics\n\t2. Chemistry\n\t3. Maths"<<endl;
    cin>>subject_select;
    if(subject_select==1){
        // Open DB
        rc = sqlite3_open("physics.db", &db);
        if(rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return(0);
        }
        else {
            fprintf(stdout, "Opened database successfully\n");
        }
    }
    else if(subject_select == 2){
        // Open DB
        rc = sqlite3_open("chemistry.db", &db);
        if(rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return(0);
        }
        else {
            fprintf(stdout, "Opened database successfully\n");
        }

    }
    else if(subject_select == 3){
        rc = sqlite3_open("maths.db", &db);
        if(rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return(0);
        }
        else {
            fprintf(stdout, "Opened database successfully\n");
        }
    }
    else{
        cout<<"Please relaunch and enter the correct option!"<<endl;
        exit(0);
    }

    /* Create SQL statement */
    sql = "CREATE TABLE STUDENT("\
        "NAME           TEXT            NOT NULL," \
        "USN            TEXT            NOT NULL," \
        "Q1_MARK        INT," \
        "Q2_MARK        INT," \
        "Q3_MARK        INT," \
        "Q4_MARK        INT," \
        "Q5_MARK        INT," \
        "TOTAL_MARKS     INT," \
        "LINK_TO_SUBMIT_ASSIGNMENT TEXT," \
        "SUBMISSION_DUE_DATE TEXT," \
        "CORRECTION_DUE_DATE TEXT);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    cout<<"Who is logging in?\n1.Student\n2.Teacher"<<endl;
    cin>>selection;
    if(selection == 1){
        // Start with student
        cout<<"Are you registerd?, if yes type '1'and if not type '0': ";
        int isregistered;
        cin>>isregistered;

        if(isregistered==1){
            cout<<"Enter your USN(IN ALL CAPS): ";
            string usn;
            cin>>usn;
            // SQL Statement to find the student
            sql = "SELECT * from STUDENT WHERE USN LIKE '"+usn+"' ";
            rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);
            if( rc != SQLITE_OK ) {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else {
                fprintf(stdout, "Operation done successfully\n");
            }
        }

        else if(isregistered == 0){
            // register_student(name,usn);
            // SQL Statement to register students
            cout<<"Enter your name: ";
            cin>>name;
            cout<<"Enter your USN: ";
            cin>>usn;
            sql = "INSERT INTO STUDENT(NAME,USN) VALUES ('"+name+"','"+usn+"');";
            rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } 
            else {
                fprintf(stdout, "Records created successfully\n");
            }
        }
    }
    else if(selection == 2){
        // Write for teacher
        cout<<"Please tell what you want to do\n\t1.View Marks\n\t2.Allot Marks\n\t3.Assign Assignment"<<endl;
        cout<<"Enter the correct option: ";
        int option1;
        cin>>option1;
        if(option1 == 1){
            cout<<"List of All students with their marks"<<endl;
            sql = "SELECT * FROM STUDENT";
            rc = sqlite3_exec(db,sql.c_str(),callback, (void*)data, &zErrMsg);
            if( rc != SQLITE_OK ) {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else {
                fprintf(stdout, "Operation done successfully\n");
            }
        }
        else if(option1 == 2){
            cout<<"Enter the USN of the student to enter marks: ";
            string usn;
            cin>>usn;
            sql = "SELECT * from STUDENT WHERE USN LIKE '"+usn+"' ";
            rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);
            if( rc != SQLITE_OK ) {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else {
                fprintf(stdout, "Operation done successfully\n");
            }
            int q1Mark,q2Mark,q3Mark,q4Mark,q5Mark,totalMark;
            cout<<"Please enter the mark for question 1: ";
            cin>>q1Mark;
            cout<<"Please enter the mark for question 2: ";
            cin>>q2Mark;
            cout<<"Please enter the mark for question 3: ";
            cin>>q3Mark;
            cout<<"Please enter the mark for question 4: ";
            cin>>q4Mark;
            cout<<"Please enter the mark for question 5: ";
            cin>>q5Mark;

            totalMark = q1Mark+q2Mark+q3Mark+q4Mark+q5Mark;

            sql = "UPDATE STUDENT SET Q1_MARK = '"+to_string(q1Mark)+"', Q2_MARK = '"+to_string(q2Mark)+"',Q3_MARK = '"+to_string(q3Mark)+"',Q4_MARK = '"+to_string(q4Mark)+"',Q5_MARK = '"+to_string(q5Mark)+"',TOTAL_MARKS = '"+to_string(totalMark)+"' WHERE USN LIKE '"+usn+"'";
            rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } 
            else {
                fprintf(stdout, "Records created successfully\n");
            }

        }

        else if(option1 == 3){
            string submission_due,correction_due,link;
            cout<<"Enter the date by which the student should submit: ";
            cin>>submission_due;
            cout<<"Enter the date by which correction will be completed: ";
            cin>>correction_due;
            cout<<"Enter the link where students can upload their assignment: ";
            cin>>link;
            sql = "UPDATE STUDENT SET SUBMISSION_DUE_DATE = '"+submission_due+"', CORRECTION_DUE_DATE = '"+correction_due+"',LINK_TO_SUBMIT_ASSIGNMENT = '"+link+"' ";
            rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } 
            else {
                fprintf(stdout, "Records created successfully\n");
            }

        }

        else{
            cout<<"Please relauch the program and enter the correct option!"<<endl;
        }
    }
    else{
        cout<<"Enter the correct option!"<<endl;
    }
    sqlite3_close(db);
    return 0;
}