#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <map>
using namespace std;

template<class T>
void test(const string& name, const T& expected, const T& actual)
{
	if(expected != actual)
	{
		cout << "test " << name << " failed\n"
			<< "expected: " << expected << "\n"
			<< "actual: " << actual << "\n";
 
		abort();
	}
}

void spin(string& input, int amount)
{
	std::rotate(input.begin(), input.end() - amount, input.end());
}

void exchange(string& input, int pos1, int pos2)
{
	std::swap(input[pos1], input[pos2]);
}

void partner(string& input, char first, char second)
{
	auto pos1 = input.find(first);
	auto pos2 = input.find(second);
	std::swap(input[pos1], input[pos2]);
}

int findNextInt(const string& input, int& cursor, char delimiter)
{
	// auto amountStart = distance(input.cbegin(), cursor);
	auto amountEnd = input.find(delimiter, cursor);
	// cout << amountStart << "," << amountEnd << endl;
	string amountAsString = input.substr(cursor, amountEnd - cursor);
	cursor = amountEnd;
	return atoi(amountAsString.c_str());
}

void completeParse(string& programs, const string& input)
{
	// auto cursor = input.cbegin();
	auto cursor(0);
	while(cursor != input.size())
	{
		// auto danceMove = *cursor;
		// cout << danceMove << ":";
		auto& danceMove = input[cursor];
		switch (danceMove)
		{
			case 's':
			{
				++cursor;
				auto amount = findNextInt(input, cursor, ',');
				// cout << amount << endl;
				spin(programs, amount);
				break;
			}
			case 'x':
			{
				++cursor;
				auto pos1 = findNextInt(input, cursor, '/');
				++cursor;
				auto pos2 = findNextInt(input, cursor, ',');
				// cout << pos1 << "," << pos2 << endl;
				exchange(programs, pos1, pos2);
				break;
			}
			case 'p':
			{
				auto program1 = input[++cursor];
				++cursor; //skip slash
				auto program2 = input[++cursor];
				// cout << program1 << "," << program2 << endl;
				partner(programs, program1, program2);
				break;
			}
		}
		++cursor; 
		// cout << programs << endl;
	}
}

int main() {
	// // your code goes here
	// test("spin", string("cdeab"), spin("abcde", 3));
	// test("exchange", string("eabdc"), exchange("eabcd", 3, 4));
	// test("partner", string("baedc"), partner("eabdc", 'e', 'b'));
	
	string testInput("s1,x3/4,pe/b");
	
	string testPrograms = "abcde";
	completeParse(testPrograms, testInput);
		
	test("final", string("baedc"), testPrograms);
	
	string realInput;
	getline(cin, realInput);
	
	string programs = "abcdefghijklmnop";
	std::map<std::string, int> lookup;
	for (int rep{0}; lookup.emplace(programs, rep++).second; )
		completeParse(programs, realInput);
	int rem(1000000000 % lookup.size());
	for (auto const & stringIntPair : lookup) 
	{
		if (stringIntPair.second == rem) 
		{
			cout << stringIntPair.first << '\n';
		}
	}
	
	return 0;
}
