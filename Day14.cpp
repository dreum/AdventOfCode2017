#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <list>
#include <set>
#include <cstdint>
#include <algorithm>
using namespace std;

using hashType = vector<uint8_t>;
const auto HASH_LENGTH_G(256);
uint8_t index(uint64_t pos)
{
	return pos % HASH_LENGTH_G;
}

void reverse(hashType& list, uint64_t start, uint64_t end)
{
	while(start < end)
	{
		std::swap(list[index(start)], list[index(end)]);
		++start;
		--end;
	}
}

hashType createDenseHash(const hashType& sparseHash)
{
	hashType denseHash;
	denseHash.reserve(16);
	for(auto index(0); index < HASH_LENGTH_G; index += 16)
	{
		auto code = sparseHash[index];
		for(auto codeIndex(1); codeIndex < 16; ++codeIndex)
		{
			code ^= sparseHash[index + codeIndex];
		}
		denseHash.push_back(code);
	}
	
	return denseHash;
}

hashType createSparseHash(hashType input)
{
	hashType hashingList;
	hashingList.reserve(HASH_LENGTH_G);
	
	for(auto i(0); i<HASH_LENGTH_G; ++i)
		hashingList.push_back(i);
	input.insert(input.end(), {17, 31, 73, 47, 23});
	
	
	auto pos(0);
	auto skip(0);
	for(auto run(0); run < 64; ++run)
	for(auto length : input)
	{
		reverse(hashingList, pos, pos + length-1);
		pos += length + skip++;
	}
	
	return hashingList;
}

hashType modifyInput(const hashType& input, uint16_t row)
{
	auto output = input;
	output.emplace_back(static_cast<uint8_t>('-'));
	stringstream s;
	s << row;
	char byte;
	while(s >> byte)
	{
		output.emplace_back(static_cast<uint8_t>(byte));
	}
	
	return output;
}

hashType createHash(const hashType& input, uint16_t row)
{
	auto inputPlusRow = modifyInput(input, row);
	auto sparseHash = createSparseHash(inputPlusRow);
	return createDenseHash(sparseHash);
}

using point = pair<uint8_t, uint8_t>;
using pointSet = set<point>;
using pointSetList = list<pointSet>;

class ConnectedGrid
{
	public:
	ConnectedGrid()
	: theSets()
	, filledSpots(0)
	{}

	void addPoint(point&& pointToAdd)
	{
		auto row = pointToAdd.first;
		auto column = pointToAdd.second;
		++filledSpots;
		
		auto setToTheLeft = findSet({row, column-1});
		if(setToTheLeft != theSets.end())
		{
			setToTheLeft->emplace(pointToAdd);
		}
		else
		{
			theSets.emplace_back(pointSet{pointToAdd});
		}
		
		auto setAbove = findSet({row-1, column});
		if(setAbove != theSets.end())
		{
			auto setToMove = findSet({row, column});
			if(setToMove != setAbove)
			{
				setAbove->insert(setToMove->cbegin(), setToMove->cend());
				theSets.erase(setToMove);
			}
		}
	}
	
	size_t setSize()
	{
		return theSets.size();
	}
	
	unsigned int pointSize()
	{
		return filledSpots;
	}
	
	private:
	pointSetList::iterator findSet(point&& pointToFind)
	{
		return find_if(theSets.begin(), theSets.end(), [&](const auto& set){ return set.count(pointToFind); });
	}
	
	pointSetList theSets;
	unsigned int filledSpots;
};

bool filledSpot(const hashType& hash, uint8_t column)
{
	const int bitwidth(8);
	auto hashIndex = static_cast<int>(column / bitwidth);
	auto bitShift = bitwidth - 1 - static_cast<uint8_t>(column % bitwidth);
	return hash[hashIndex] >> bitShift & 0x1;
}

int main() {
	char input;
	hashType lengths;
	
	while(cin >> input)
	{
		uint8_t length = static_cast<uint8_t>(input);
		lengths.push_back(length);
	}
	
	ConnectedGrid connectedGrid;

	for(auto row(0); row < 128; ++row)
	{
		auto denseHash = createHash(lengths, row);
		for(auto column(0); column < 128; ++column)
		{
			if(filledSpot(denseHash, column))
				connectedGrid.addPoint({row, column});
		}
	}
	
	cout << connectedGrid.pointSize() << endl;
	cout << connectedGrid.setSize() << endl;
	
	return 0;
}
