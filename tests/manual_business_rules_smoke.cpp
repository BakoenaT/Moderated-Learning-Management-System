#include "business_rules.hpp"
#include <sqlite3.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
static void exec(sqlite3* db,const char* sql){char* err=nullptr;int rc=sqlite3_exec(db,sql,nullptr,nullptr,&err);if(rc!=SQLITE_OK){std::string e=err?err:sqlite3_errmsg(db);sqlite3_free(err);throw std::runtime_error(e);}}
static int scalar(sqlite3* db,const char* sql){sqlite3_stmt* st=nullptr;assert(sqlite3_prepare_v2(db,sql,-1,&st,nullptr)==SQLITE_OK);assert(sqlite3_step(st)==SQLITE_ROW);int v=sqlite3_column_int(st,0);sqlite3_finalize(st);return v;}
static std::string queryText(sqlite3* db,const char* sql){sqlite3_stmt* st=nullptr;assert(sqlite3_prepare_v2(db,sql,-1,&st,nullptr)==SQLITE_OK);assert(sqlite3_step(st)==SQLITE_ROW);auto x=sqlite3_column_text(st,0);std::string s=x?reinterpret_cast<const char*>(x):"";sqlite3_finalize(st);return s;}
int main(){
 assert(lms::calendarDaysLate("2026-10-01","2026-10-01")==0);
 assert(lms::calendarDaysLate("2026-10-01","2026-10-02")==1);
 assert(lms::calendarDaysLate("2026-10-01","2026-10-03")==2);
 assert(lms::calendarDaysLate("2026-10-01","2026-10-04")==3);
 assert(lms::calendarDaysLate("2026-10-01","2026-10-05")==4);
 assert(lms::calendarDaysLate("2026-10-05","2026-10-04")==0);
 assert(lms::calendarDaysLate("2024-02-28","2024-03-01")==2);
 auto invalidDate=[](const std::string& d,const std::string& s){try{(void)lms::calendarDaysLate(d,s);return false;}catch(const std::invalid_argument&){return true;}};
 assert(invalidDate("2025-02-29","2025-03-01"));assert(invalidDate("2026-13-01","2026-13-02"));assert(invalidDate("2026-10-01 25:00:00","2026-10-02 10:00:00"));
 assert(std::abs(lms::lateAdjustedMaximum(100,0)-100)<0.001);assert(std::abs(lms::lateAdjustedMaximum(100,1)-90)<0.001);assert(std::abs(lms::lateAdjustedMaximum(100,2)-80)<0.001);assert(std::abs(lms::lateAdjustedMaximum(100,3)-70)<0.001);assert(std::abs(lms::lateAdjustedMaximum(100,4))<0.001);
 bool invalid=false;try{(void)lms::lateAdjustedMaximum(-1,0);}catch(const std::invalid_argument&){invalid=true;}assert(invalid);invalid=false;try{(void)lms::lateAdjustedMaximum(100,-1);}catch(const std::invalid_argument&){invalid=true;}assert(invalid);
 assert(lms::submissionTimeAllowed("2026-11-01 12:00:00","2026-11-01 12:00:00",true));assert(lms::submissionTimeAllowed("2026-11-01 12:00:00","2026-11-01 11:59:59",true));assert(!lms::submissionTimeAllowed("2026-11-01 12:00:00","2026-11-01 12:00:01",true));assert(lms::submissionTimeAllowed("2026-11-01 12:00:00","2026-11-06 12:00:00",false));
 sqlite3* db=nullptr;assert(sqlite3_open(":memory:",&db)==SQLITE_OK);
 exec(db,"CREATE TABLE assignment(id INTEGER PRIMARY KEY,course_id INTEGER,lecturer_id INTEGER,deadline TEXT);CREATE TABLE enrolment(student_id INTEGER,course_id INTEGER);CREATE TABLE extension_grant(id INTEGER PRIMARY KEY,assignment_id INTEGER,student_id INTEGER,granted_by INTEGER,deadline TEXT,reason TEXT,extension_scope TEXT,UNIQUE(assignment_id,student_id));INSERT INTO assignment VALUES(10,100,7,'2026-11-01 12:00:00'),(20,200,8,'2026-11-01 12:00:00');INSERT INTO enrolment VALUES(1,100),(2,100),(3,100),(4,200);INSERT INTO extension_grant(assignment_id,student_id,granted_by,deadline,reason,extension_scope) VALUES(10,1,7,'2026-11-10 10:00:00','Individual','INDIVIDUAL');");
 auto throws=[&](auto f){try{f();return false;}catch(const std::exception&){return true;}};
 int changed=lms::grantWholeClassExtension(db,10,7,"2026-11-15 23:59:00","Class extension reason");assert(changed==2);assert(scalar(db,"SELECT COUNT(*) FROM extension_grant WHERE assignment_id=10")==3);assert(queryText(db,"SELECT extension_scope FROM extension_grant WHERE student_id=1 AND assignment_id=10")=="INDIVIDUAL");assert(queryText(db,"SELECT deadline FROM extension_grant WHERE student_id=2 AND assignment_id=10")=="2026-11-15 23:59:00");assert(throws([&]{lms::grantWholeClassExtension(db,10,7,"2026-11-12 23:59:00","Earlier class grant");}));assert(queryText(db,"SELECT deadline FROM extension_grant WHERE student_id=2 AND assignment_id=10")=="2026-11-15 23:59:00");
 int laterUpdate=lms::grantWholeClassExtension(db,10,7,"2026-11-16 23:59:00","Later class extension");assert(laterUpdate==2);assert(queryText(db,"SELECT deadline FROM extension_grant WHERE student_id=2 AND assignment_id=10")=="2026-11-16 23:59:00");assert(queryText(db,"SELECT extension_scope FROM extension_grant WHERE student_id=1 AND assignment_id=10")=="INDIVIDUAL");
 assert(throws([&]{lms::grantWholeClassExtension(nullptr,10,7,"2026-11-15 23:59:00","x");}));assert(throws([&]{lms::grantWholeClassExtension(db,0,7,"2026-11-15 23:59:00","x");}));assert(throws([&]{lms::grantWholeClassExtension(db,10,99,"2026-11-15 23:59:00","x");}));assert(throws([&]{lms::grantWholeClassExtension(db,10,7,"2026-11-15 25:00:00","x");}));assert(throws([&]{lms::grantWholeClassExtension(db,10,7,"2026-11-15 23:59:00","");}));assert(throws([&]{lms::grantWholeClassExtension(db,10,7,"2026-10-30 10:00:00","x");}));
 exec(db,"DELETE FROM enrolment WHERE course_id=100");assert(throws([&]{lms::grantWholeClassExtension(db,10,7,"2026-11-15 23:59:00","No students");}));
 assert(lms::extensionLabel("CLASS")=="Class extension");assert(lms::extensionLabel("INDIVIDUAL")=="Individual extension");assert(lms::extensionLabel("")=="No extension");

 sqlite3* atomicDb=nullptr;assert(sqlite3_open(":memory:",&atomicDb)==SQLITE_OK);
 exec(atomicDb,"CREATE TABLE assignment(id INTEGER PRIMARY KEY,course_id INTEGER,lecturer_id INTEGER,deadline TEXT);CREATE TABLE enrolment(student_id INTEGER,course_id INTEGER);CREATE TABLE extension_grant(id INTEGER PRIMARY KEY,assignment_id INTEGER,student_id INTEGER,granted_by INTEGER,deadline TEXT,reason TEXT,extension_scope TEXT,UNIQUE(assignment_id,student_id));INSERT INTO assignment VALUES(1,1,7,'2026-11-01 10:00:00');INSERT INTO enrolment VALUES(2,1),(3,1);CREATE TRIGGER reject_student_three BEFORE INSERT ON extension_grant WHEN NEW.student_id=3 BEGIN SELECT RAISE(ABORT,'forced failure'); END;");
 assert(throws([&]{lms::grantWholeClassExtension(atomicDb,1,7,"2026-11-05 10:00:00","atomic test");}));
 assert(scalar(atomicDb,"SELECT COUNT(*) FROM extension_grant")==0);
 sqlite3_close(atomicDb);
 sqlite3_close(db);std::cout<<"PASS: business rule smoke assertions\n";
}
