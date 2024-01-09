#pragma once
#include"Info.h"
 class DataInterFace
{
public:
	bool openFile(CString FilePath);
	void addStudent(Student Info); 
	void deleteId(uint64_t id);

	void changeId(int64_t id,Student student);

	Student searchId(uint64_t id);
	Student searchName(string name);

	bool saveFile(CString FilePath);

	void ascendingSort();
	void descendingSort();
	uint64_t passedStudent();

	vector<Student> vStudent;
};

