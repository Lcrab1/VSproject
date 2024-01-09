#pragma once
class Student
{
public:
	Student();
	Student(uint64_t id, string name,string sex , uint32_t math, uint32_t cpp, uint32_t english);
	~Student();
	uint64_t getId();
	string getName();
	string getSex();
	uint32_t getMath();
	uint32_t getCPP();
	uint32_t getEnglish();
	float getAverage();

	void loadFile(ifstream& in);
	void saveFile(ofstream& out);
private:
	uint64_t m_id;
	string m_name;
	string m_sex;
	uint32_t m_math;
	uint32_t m_cpp;
	uint32_t m_english;
	float m_average;
	 
};

