#include "pch.h"
#include "DataInterFace.h"

bool DataInterFace::openFile(CString FilePath)
{
	ifstream in(FilePath,ios::in);

	if (in.is_open()) {
		int num;	//记录有多少条数据
		in >> num;
		for (int i = 0; i < num; i++) {
			Student student;
			student.loadFile(in);
			vStudent.push_back(student);
		}
		return true;
	}
	return false;
}

void DataInterFace::addStudent(Student student)
{
	vStudent.push_back(student);
}

void DataInterFace::deleteId(uint64_t id)
{
	vStudent.erase(vStudent.begin() + id);
}

void DataInterFace::changeId(int64_t id, Student student)
{
	vStudent[id] = student;
}

Student DataInterFace::searchId(uint64_t id)
{
	for (int i = 0; i < vStudent.size(); i++) {
		if (vStudent[i].getId() == id) {
			return vStudent[i];
		}
	}
	return Student(-1, TEXT(""), TEXT(""), atoi( TEXT("")), atoi(TEXT("")), atoi(TEXT("")));
}

Student DataInterFace::searchName(string name)
{
	for (int i = 0; i < vStudent.size(); i++) {
		if (vStudent[i].getName() == name) {
			return vStudent[i];
		}
	}
	return Student(-1, TEXT(""), TEXT(""), atoi(TEXT("")), atoi(TEXT("")), atoi(TEXT("")));
}

bool DataInterFace::saveFile(CString FilePath)
{
	ofstream out(FilePath, ios::out);
	if (out.is_open()) {
		out << vStudent.size() << endl;
		for (int i = 0; i < vStudent.size(); i++) {
			vStudent[i].saveFile(out);
		}
		out.close();
		return true;
	}
	return false;
}

void DataInterFace::ascendingSort()
{
	for (int i = 0; i < vStudent.size(); i++) {
		for (int j = 0; j < vStudent.size() - 1; j++) {
			if (vStudent[j].getAverage() > vStudent[j + 1].getAverage()) {
				Student temp = vStudent[j];
				vStudent[j] = vStudent[j + 1];
				vStudent[j + 1] = temp;
			}
		}
	}
}

void DataInterFace::descendingSort()
{
	for (int i = 0; i < vStudent.size(); i++) {
		for (int j = 0; j < vStudent.size() - 1; j++) {
			if (vStudent[j].getAverage() < vStudent[j + 1].getAverage()) {
				Student temp = vStudent[j];
				vStudent[j] = vStudent[j + 1];
				vStudent[j + 1] = temp;
			}
		}
	}
}
 