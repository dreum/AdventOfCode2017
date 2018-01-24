#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

ostream& operator<<(ostream& s, vector<int> v)
{
	for_each(v.begin(), v.end(), [&s](auto i) {
		s << i << " ";
	});
	
	return s;
}
template<typename T, typename U>
void test(const string& test_name, const T& expected, const U& actual)
{
	if(expected != actual)
	{
		cout << test_name << " failed\n"
			<< "expected: " << expected << "\n"
			<< "actual: " << actual << endl;
	}
}

class spinlock
{
	public:
	spinlock(int step_size) :
	buffer(),
	step_size(step_size),
	index(0),
	value(0)
	{
		buffer.emplace_back(0);
	}
	
	virtual void step()
	{
		index += step_size;
		index = wrapIndex(index);
		auto pos = buffer.begin() + index;
		buffer.insert(pos + 1, ++value);
		++index;
	}
	
	void stepUntil(int until)
	{
		while(value != until)
			step();
	}
	
	int findElementAfter(int after)
	{
		auto element = find(buffer.begin(), buffer.end(), after);
		++element;
		return *element;
	}
	
	vector<int> buffer;
	private:
	int wrapIndex(int i)
	{
		return i % buffer.size();
	}
	
	protected:
	int step_size;
	int index;
	int value;
};

class spinlock2 : public spinlock
{
	public:
	spinlock2(int step_size):
	spinlock(step_size),
	fake_size(1)
	{}
	
	void step() override
	{
		index += step_size;
		index %= fake_size;
		++value;
		if(0 == index)
			buffer.insert(buffer.begin() + 1, value);
		
		++fake_size;
		++index;
	}
	private:
	int fake_size;
};

int main() {
	// your code goes here
	spinlock s(3);
	test("initial state is size one", 1, s.buffer.size());
	test("initial state first element is zero", 0, s.buffer[0]);
	
	s.step();
	test("after 1 step", vector<int>{0,1}, s.buffer);
	s.step();
	test("after 2 steps", vector<int>{0, 2, 1}, s.buffer);
	s.step();
	test("after 3 steps", vector<int>{0, 2, 3, 1}, s.buffer);
	
	s.stepUntil(9);
	test("after 9 steps", vector<int>{0, 9, 5, 7, 2, 4, 3, 8, 6, 1}, s.buffer);
	
	s.stepUntil(2017);
	auto element = s.findElementAfter(2017);
	test("after 2017 steps", 638, element);
	
	spinlock t(312);
	t.stepUntil(2017);
	cout << t.findElementAfter(2017) << endl;
	
	spinlock2 u(312);
	u.stepUntil(50000000);
	cout << u.findElementAfter(0) << endl;
	
	return 0;
}
