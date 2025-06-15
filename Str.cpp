#include"Str.h"
#include<iostream>
#include<fstream>
using namespace std;

int Str::length(const char* s) const {
	if (s == nullptr) return 0; 
	int i = 0;
	while (s[i] != '\0') {
		i++;
	}
	return i;
}
void Str::strCopy(char* dest, const char* src) {
	if (src == nullptr || dest == nullptr) {
		std::cerr << "Error: Null pointer passed to strCopy()" << std::endl;
		return;
	}

	int i = 0;
	while (src[i] != '\0' && i < 9999) { 
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

void Str::regrow() {
	cap *= 2;
	char* temp = new char[cap];
	strCopy(temp, Cs);
	delete[] Cs;
	Cs = temp;
}
char* Str::itos(int integer) {
	Str temp;
	temp.Cs = new char[cap];
	int count = 0;
	int originalInteger = integer;
	if (integer == 0) {
		char* string = new char[2];
		string[0] = '0';
		string[1] = '\0';
		return string;
	}
	while (integer != 0) {
		integer /= 10;
		count++;
	}
	char* string = new char[count + 1];
	integer = originalInteger;
	for (int i = count - 1; i >= 0; i--)
	{
		string[i] = (integer % 10) + '0';
		integer /= 10;
	}
	string[count] = '\0';
	return string;
}

Str::Str() :len(0), cap(1) {
	Cs = new char[cap] {'\0'};
}
Str& Str::operator=(const Str& s)
{
	if (this == &s) {
		return *this;
	}
	delete[] Cs;
	len = s.len;
	cap = s.cap;
	Cs = new char[cap];
	strCopy(Cs, s.Cs);
	return *this;
}
Str::Str(int integer) {
	char* temp = itos(integer);
	len = length(temp);
	cap = len + 1;
	Cs = new char[cap];
	strCopy(Cs, temp);
	delete[] temp;
}
Str::Str(const char* s) {
	if (s == nullptr) {
		len = 0;
		cap = 1;
		Cs = new char[1];
		Cs[0] = '\0';
		return;
	}
	len = length(s);
	cap = len + 1;
	Cs = new char[cap];
	strCopy(Cs, s);
}
Str::Str(int n, char* ch) {
	len = n;
	cap = len + 1;
	Cs = new char[cap];
	strCopy(Cs, ch);
}
Str::Str(const Str& s) {
	len = s.len;
	cap = s.cap;
	Cs = new char[cap];
	strCopy(Cs, s.Cs);
}


void Str::insert_at(int i, char ch) {
	if (i < 0 || i > len) {
		cout << "Invalid index" << endl;
		return;
	}
	if (len == cap - 1) {
		cap *= 2;
		char* temp = new char[cap];
		for (int j = 0; j < i; j++) {
			temp[j] = Cs[j];
		}
		temp[i] = ch;
		for (int j = i; j < len; j++) {
			temp[j + 1] = Cs[j];
		}
		temp[len + 1] = '\0';
		delete[] Cs;
		Cs = temp;
		temp = nullptr;
		len++;
	}
	else {
		for (int j = len; j > i; j--) {
			Cs[j] = Cs[j - 1];
		}
		Cs[i] = ch;
		len++;
	}
}
void Str::insert_at(int i, const Str sub) {
	if (i < 0 || i > len) {
		cout << "Invalid index" << endl;
		return;
	}
	if (len + sub.len >= cap - 1) {
		cap = len + sub.len + 1;
		char* temp = new char[cap];
		for (int j = 0; j < i; j++) {
			temp[j] = Cs[j];
		}
		for (int j = 0; j < sub.len; j++) {
			temp[i + j] = sub.Cs[j];
		}
		for (int j = i; j < len; j++) {
			temp[j + sub.len] = Cs[j];
		}
		temp[len + sub.len] = '\0';
		delete[] Cs;
		Cs = temp;
		len += sub.len;
	}
	else {
		for (int j = len; j >= i; j--) {
			Cs[j + sub.len] = Cs[j];
		}
		for (int j = 0; j < sub.len; j++) {
			Cs[i + j] = sub.Cs[j];
		}
		len += sub.len;
	}

}
void Str::display()const {
	for (int i = 0; i < len; i++) {
		cout << Cs[i];
	}
	cout << endl;
}
void Str::replace_first(char c)
{
	Cs[0] = c;
}
int Str::stringtonumber()
{
	int result = this->stoi(this->Cs);
	return result;
}
Str& Str::trim()
{
	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (int(Cs[i]) > 32)
		{
			Cs[j] = Cs[i];
			j++;
		}
	}
	Cs[j] = '\0';
	len = j;
	return *this;
}
/*char operator[](int i)const
{
	return Cs[i];
}*/
char& Str::operator[](int i)
{
	return Cs[i];
}
const char& Str::operator[](int i) const
{
	return Cs[i];
}
bool Str::is_equal(const Str& M) const {
	if (len != M.len) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if (tolower(Cs[i]) != tolower(M.Cs[i])) {  // Compare lowercase versions
			return false;
		}
	}
	return true;
}
bool Str::is_less(const Str& M) const
{
	int minLen = 0;
	if (len < M.len)
	{
		minLen = len;
	}
	else
	{
		minLen = M.len;
	}
	for (int i = 0; i < minLen; i++)
	{
		if (Cs[i] < M.Cs[i])
			return true;
		else if (Cs[i] > M.Cs[i])
			return false;
	}
	return len < M.len;
}

bool Str::is_greater(const Str& M)
{
	int minLen = 0;
	if (len < M.len)
	{
		minLen = len;
	}
	else
	{
		minLen = M.len;
	}
	for (int i = 0; i < minLen; i++)
	{
		if (Cs[i] > M.Cs[i])
			return true;
		else if (Cs[i] < M.Cs[i])
			return false;
	}
	return len > M.len;
}
Str* Str::split(char delim, int& count) const
{
	count = 0;
	int strLen = length(Cs);
	for (int i = 0; i < strLen; i++)
	{
		if (Cs[i] == delim)
		{
			count++;
		}
	}
	count++;

	Str* result = new Str[count];

	int Index = 0;
	int start = 0;

	for (int i = 0; i <= strLen; i++)
	{
		if (i == strLen || Cs[i] == delim)
		{
			int Length = i - start;
			if (Length == 0)
			{
				result[Index] = Str("");
			}
			else
			{
				result[Index] = Str(Cs + start);
				result[Index].Cs[Length] = '\0';
				result[Index].len = Length;
			}
			Index++;
			start = i + 1;
		}
	}
	return result;
}

Str* Str::tokenize(const char* delim, int& count) const
{
	count = 0;
	bool inToken = false;
	for (int i = 0; i < len; i++)
	{
		bool isDelim = false;
		for (int j = 0; j < length(delim); j++)
		{
			if (Cs[i] == delim[j])
			{
				isDelim = true;
				break;
			}
		}
		if (!isDelim && !inToken)
		{
			inToken = true;
			count++;
		}
		else if (isDelim)
		{
			inToken = false;
		}
	}
	Str* result = new Str[count];
	int index = 0;
	int start = 0;
	inToken = false;
	for (int i = 0; i <= len; i++)
	{
		bool isDelim = false;
		bool endOfString = (i == len);

		if (!endOfString)
		{
			for (int j = 0; j < length(delim); j++)
			{
				if (Cs[i] == delim[j])
				{
					isDelim = true;
					break;
				}
			}
		}
		if ((inToken && isDelim) || (inToken && endOfString))
		{
			int tokenLength = i - start;
			result[index].Cs = new char[tokenLength + 1];
			for (int k = 0; k < tokenLength; k++)
			{
				result[index].Cs[k] = Cs[start + k];
			}
			result[index].Cs[tokenLength] = '\0';
			result[index].len = tokenLength;
			result[index].cap = tokenLength + 1;

			index++;
			inToken = false;
		}
		else if (!isDelim && !inToken)
		{
			start = i;
			inToken = true;
		}
	}

	return result;
}
Str Str::concat(const Str& s2) const
{
	Str result;
	result.len = len + s2.len;
	result.cap = result.len + 1;
	result.Cs = new char[result.cap];

	int i = 0;
	for (i = 0; i < len; i++)
	{
		result.Cs[i] = Cs[i];
	}
	for (int j = 0; j < s2.len; j++)
	{
		result.Cs[i] = s2.Cs[j];
		i++;
	}
	result.Cs[i] = '\0';
	return result;
}
/*Str operator+(const Str& s2)
{
	Str result = this->concat(s2);
	return result;
}*/
Str Str::operator+=(const Str& s2)
{
	*(this) = this->append(s2);
	return *(this);
}
Str& Str::append(const Str& s2)
{
	while (cap < len + s2.len + 1)
	{
		this->regrow();
	}
	for (int i = 0; i < s2.len; i++)
	{
		Cs[len + i] = s2.Cs[i];
	}
	len = len + s2.len;
	Cs[len] = '\0';
	return *this;
}
int* Str::all_sub_string(const char* sub_string, int& count)const
{
	count = 0;
	int sub_length = length(sub_string);
	int* ptr = new int[len];
	if (sub_length == 0)
	{
		return ptr;
	}
	for (int i = 0; i <= len - sub_length; i++)
	{
		if (Cs[i] == sub_string[0])
		{
			bool found = true;
			for (int j = 0; j < sub_length; j++)
			{
				if (Cs[i + j] != sub_string[j])
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				ptr[count] = i;
				count++;
			}
		}
	}
	return ptr;
}
int Str::find_first(char ch)
{
	for (int i = 0; i < len; i++)
	{
		if (Cs[i] == ch)
		{
			return i;
		}
	}
}
int Str::find_last(char ch)
{
	for (int i = len; i > 0; i--)
	{
		if (Cs[i] == ch)
		{
			return i;
		}
	}
}
int Str::find_first(const Str& a) const
{
	int count = 0;
	int* ptr = this->all_sub_string(a.Cs, count);
	return ptr[0];
}
int Str::find_last(const Str& a) const
{
	int count = 0;
	int* ptr = this->all_sub_string(a.Cs, count);
	return ptr[count - 1];
}
int* Str::find_all(char ch, int& C)
{
	C = 0;
	int* ptr = new int[len];
	for (int i = 0; i < len; i++)
	{
		if (Cs[i] == ch)
		{
			ptr[C] = i;
			C++;
		}
	}
	return ptr;
}
int* Str::find_all(const Str& a, int& C) const
{
	int* ptr = this->all_sub_string(a.Cs, C);
	return ptr;
}
void Str::remove_at(int i)
{
	if (i<0 || i>len)
	{
		return;
	}
	for (int j = i; j < len; j++)
	{
		Cs[j] = Cs[j + 1];
	}
	len--;
}
void Str::remove_first(char ch)
{
	for (int i = 0; i < len; i++)
	{
		if (Cs[i] == ch)
		{
			for (int j = i; j < len; j++)
			{
				Cs[j] = Cs[j + 1];
			}
			len--;
			break;
		}
	}
}
void Str::remove_last(char ch)
{
	for (int i = len; i > 0; i--)
	{
		if (Cs[i] == ch)
		{
			for (int j = i; j < len; j++)
			{
				Cs[j] = Cs[j + 1];
			}
			len--;
			break;
		}
	}
}
void Str::remove_all(char ch)
{
	for (int i = 0; i < len; i++)
	{
		if (Cs[i] == ch)
		{
			for (int j = i; j < len; j++)
			{
				Cs[j] = Cs[j + 1];
			}
			len--;
		}
	}
}
void Str::to_upper()
{
	for (int i = 0; i < len; i++)
	{
		if (Cs[i] >= 'a' && Cs[i] <= 'z')
		{
			Cs[i] = Cs[i] - 32;
		}
	}
}

void Str::to_lower()
{
	for (int i = 0; i < len; i++)
	{
		if (Cs[i] >= 'A' && Cs[i] <= 'Z')
		{
			Cs[i] = Cs[i] + 32;
		}
	}
}
void Str::clear() {
	delete[] Cs;
	cap = 1;
	len = 0;
	Cs = new char[cap];
	Cs[0] = '\0';
}
Str::~Str()
{
	delete[] Cs;
	Cs = nullptr;
	len = 0;
	cap = 1;
}
int Str::stoi(const Str s)
{
	int len = length(s.Cs);
	int value = 0;
	for (int i = 0; i < len; i++)
	{
		value = (value * 10) + (s.Cs[i] - 48);
	}
	return value;
}
const char* Str::return_array() const {
	return (Cs != nullptr) ? Cs : "";
}

ostream& operator<<(ostream& out, const Str& s)
{
	for (int i = 0; i < s.len; i++)
	{
		out << s.Cs[i];
	}
	return out;
}
Str Str::resize()
{
	int count = 0;
	Str* ptr = this->split(',', count);
	Str first = ptr[0];
	delete[] ptr;
	return first;
}
istream& operator>>(istream& in, Str& s) {
	char buffer[1024];
	in >> buffer;
	s = Str(buffer);
	return in;
}
bool Str::operator==(const Str& other)const{
	return this->is_equal(other);
}
void Str::writeToBinary(ofstream& out) const {
	int length = this->length(this->return_array());
	out.write(reinterpret_cast<const char*>(&length), sizeof(length));
	out.write(this->return_array(), length);
}

void Str::readFromBinary(std::ifstream& in) {
	int length;
	in.read(reinterpret_cast<char*>(&length), sizeof(length));
	if (length < 0 || length > 1000000) { 
		throw std::runtime_error("Invalid string length in binary file");
	}

	char* buffer = new char[length + 1];
	in.read(buffer, length);

	if (!in) {
		delete[] buffer;
		throw std::runtime_error("Failed to read string content");
	}

	buffer[length] = '\0';
	*this = Str(buffer);
	delete[] buffer;
}
Str Str::operator+(const Str& other) const {
	return this->concat(other);
}

Str operator+(const char* lhs, const Str& rhs) {
	return Str(lhs).concat(rhs);
}
bool Str::empty()const
{
	if (len==0)
	{
		return true;
	}
	return false;
}
bool Str::operator!=(const Str& other)
{
	return !(this->is_equal(other));
}
Str Str::substring(int start, int end) const {
	if (start < 0 || end > len || start > end) {
		return Str();
	}
	Str result;
	result.len = end - start;
	result.cap = result.len + 1;
	result.Cs = new char[result.cap];
	for (int i = 0; i < result.len; i++) {
		result.Cs[i] = Cs[start + i];
	}
	result.Cs[result.len] = '\0';

	return result;
}