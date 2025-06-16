#include<iostream>
using namespace std;
#ifndef STR_H
#define STR_H
class Str {
	char* Cs;
	int len;
	int cap;

	void strCopy(char* dest, const char* src);
	void regrow();
	char* itos(int integer);
public:
	Str();
	int length(const char* s) const;
	Str& operator=(const Str& s);
	
	Str(int integer);
	Str(const char* s);
	Str(int n, char* ch);
	Str(const Str& s);
	int stoi(const Str s);
	const char* return_array() const;
	void insert_at(int i, char ch);
	void insert_at(int i, const Str sub);
	void display()const;
	void replace_first(char c);
	int stringtonumber();
	Str& trim();
	/*char operator[](int i)const
	{
		return Cs[i];
	}*/
	char& operator[](int i);
	const char& operator[](int i) const;
	bool is_equal(const Str& M)const;
	bool is_less(const Str& M) const;
	bool is_greater(const Str& M);
	Str* split(char delim, int& count) const;
	Str* tokenize(const char* delim, int& count) const;
	Str concat(const Str& s2) const;
	/*Str operator+(const Str& s2)
	{
		Str result = this->concat(s2);
		return result;
	}*/
	Str operator+=(const Str& s2);
	Str& append(const Str& s2);
	int* all_sub_string(const char* sub_string, int& count)const;
	int find_first(char ch);
	int find_last(char ch);
	int find_first(const Str& a) const;
	int find_last(const Str& a) const;
	int* find_all(char ch, int& C);
	int* find_all(const Str& a, int& C) const;
	void remove_at(int i);
	void remove_first(char ch);
	void remove_last(char ch);
	void remove_all(char ch);
	void to_upper();
	void to_lower();
	void clear();
	bool empty()const;
	Str resize();
	bool operator==(const Str& other)const;
	bool operator!=(const Str& other);
	~Str();
	Str operator+(const Str& other) const;
	friend ostream& operator<<(ostream& out, const Str& s);
	friend istream& operator>>(istream& in, Str& s);
	friend Str operator+(const char* lhs, const Str& rhs);
	void writeToBinary(ofstream& out) const;
	void readFromBinary(ifstream& in);
	Str substring(int start, int end) const;
	bool operator>(const Str& other) const;
	bool operator<(const Str& other) const;
	bool operator>=(const Str& other) const;
	bool operator<=(const Str& other) const;
};
#endif


