#include "pch.h"
#include "Info.h"

Student::Student() {
	//memset(this, 0, sizeof(Student));
}

Student::Student(uint64_t id, string name ,string sex , uint32_t math, uint32_t cpp, uint32_t english) {
	m_id = id;
	m_name = name;
	m_sex = sex;
	m_math = math;
	m_cpp = cpp;
	m_english = english;

	m_average = (m_math + m_cpp + m_english) / 3.0;
}


Student::~Student() {

}

uint64_t Student::getId() {
	return m_id;
}

string Student::getName() {
	return m_name;
}

string Student::getSex()
{
	return m_sex;
}


uint32_t Student::getMath() {
	return m_math;
}
uint32_t Student::getCPP() {
	return m_cpp;
}
uint32_t Student::getEnglish() {
	return m_english;
}

float Student::getAverage()
{
	return m_average;
}

void Student::loadFile(ifstream& in)
{
	in >> m_id;
	in>>m_name;
	in >> m_sex;
	in >> m_math;
	in >> m_cpp;
	in >> m_english;
	in >> m_average;
}

void Student::saveFile(ofstream& out)
{
	out << m_id << "\t";
	out << m_name << "\t";
	out << m_sex<<"\t";
	out << m_math << "\t";
	out << m_cpp << "\t";
	out << m_english << "\t";
	out << m_average << "\n";

}
