#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
using namespace std;

template<class T>
void test(const string& testName, T expected, T actual)
{
	if(actual != expected)
	{
		cout << "test " << testName << " failed \n"
			<< "expected: " << expected << "\n"
			<< "actual: " << actual << "\n";
		abort();
	}
}

using intType = uint64_t;
class Generator
{
	public:
	Generator(intType factor, intType startValue)
	: factor(factor)
	, currentValue(startValue)
	{}
	
	virtual intType generate()
	{
		return currentValue = currentValue * factor % divisor;
	}
	
	protected:
	intType currentValue;
	
	private:
	const intType factor;
	static const intType divisor = 2147483647;
};

class Generator2 : public Generator
{
	public:
	Generator2(intType factor, intType startValue, intType stopMultiple)
	: Generator(factor, startValue)
	, stopMultiple(stopMultiple)
	{};
	
	intType generate() override
	{
		do
		{
			Generator::generate();
		}
		while(currentValue%stopMultiple != 0);
		return currentValue;
	}
	
	private:
	const intType stopMultiple;
};

void testGenerateMulti(Generator& gen, const vector<intType>& values)
{
	for_each(values.cbegin(), values.cend(), [&gen](const auto value){
		testGenerate(gen, value);
	});
}

void testGenerate(Generator& gen, intType expected)
{
	const auto actual(gen.generate());
	test("testGenerate", expected, actual);
}

unsigned int lower16bitMatches(Generator& genA, Generator& genB, const unsigned int maxLoops)
{
	auto matches(0u);
	for(auto loop(0); loop < maxLoops; ++loop)
	{
		if((genA.generate() & 0xFFFF) == (genB.generate() & 0xFFFF))
			++matches;
	}
	
	return matches;
}

int main() {
	// your code goes here
	Generator genA(16807, 65);
	Generator genB(48271, 8921);
	Generator genC = genA;
	Generator genD = genB;
	
	testGenerateMulti(genA, {1092455, 1181022009, 245556042, 1744312007, 1352636452}); 
	testGenerateMulti(genB, {430625591, 1233683848, 1431495498, 137874439, 285222916});
	
	// testLower16bitMatches(genC, genD, 5, 1);
	test("test after 40M", 588u, lower16bitMatches(genC, genD, 40000000));
	
	Generator genE(16807, 679);
	Generator genF(48271, 771);
	
	cout << lower16bitMatches(genE, genF, 40000000) << endl;
	
	Generator2 genG(16807, 65, 4);
	Generator2 genH(48271, 8921, 8);
	Generator2 genI = genG;
	Generator2 genJ = genH;
	
	Generator2 genK = genG;
	Generator2 genL = genH;
	
	testGenerateMulti(genG, {1352636452, 1992081072, 530830436, 1980017072, 740335192});
	testGenerateMulti(genH, {1233683848, 862516352, 1159784568, 1616057672, 412269392}); 
	
	test("Match on 1056th value", 1u, lower16bitMatches(genI, genJ, 1056));
	
	test("Matches after 5M", 309u, lower16bitMatches(genK, genL, 5000000));
	
	Generator2 genM(16807, 679, 4);
	Generator2 genN(48271, 771, 8);
	
	cout << lower16bitMatches(genM, genN, 5000000) << endl;
	
	
	return 0;
}
