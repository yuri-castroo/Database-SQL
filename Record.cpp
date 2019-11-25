#include "pch.h"
#include <fstream>
using namespace std;

void Record::addAttribute(string attribute)
{
	attributeList.push_back(attribute);
}

void Record::deleteAttribute(int idx)
{
	if (idx < attributeList.size()) attributeList.erase(attributeList.begin() + idx);
	else // If attribute index is out of range, print error 
	{
		printf("Error: Attribute index out of range!\n");
		exit(-1);
	}
}

void Record::renameRecordAttribute(std::string attribute1, std::string attribute2)
{
	// Find Attribute1
	auto it = find(attributeList.begin(), attributeList.end(), attribute1);

	// Error-checking
	if (it == attributeList.end()) // If Attribute1 does not exist in Record
	{
		printf("Error: Attribute '%s' does not exist in Record!\n", attribute1.c_str());
		exit(-1);
	}
	if (attribute2 == "") // If Attribute is empty, print error
	{
		printf("Error: Cannot replace attribute with empty string!\n");
		exit(-1);
	}
	else // Replace Attribute1 with Attribute2
	{
		*it = attribute2;
	}
}

string& Record::operator[](int idx)
{
	// Error checking on range subscript: Exits function!
	if (idx >= attributeList.size() || idx < 0) // If index is out of range
	{
		printf("Error: Index out of range!\n");
		exit(-1);
	}

	else return attributeList[idx]; // Return attribute at index

}

// Called by Table Class
void Record::writeToFile(ofstream &fileStream)
{
	// Iterate over attribute list
	for (int i = 0; i < attributeList.size(); ++i)
	{
		// Write every attribute to file
		fileStream << attributeList.at(i) << "^";
	}
	fileStream << endl;
}

// Called by Table Class
Record Record::readFromFile(string line)
{
	std::vector<string> v;

	// Iterate over attribute list
	stringstream ss(line);
	string word = "";
	int i = 0;
	
	while (getline(ss, word, '^'))
	{
		v.push_back(word); // Add attributes to vector 
	}

	return Record(v); // Return Record object 
}