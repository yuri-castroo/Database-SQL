#pragma once
#ifndef RECORD_H
#define RECORD_H

#include "pch.h"

// Record: holds all information pertaining to a Table record
class Record
{
public:
	Record() {};
	~Record() {};
	Record(std::vector<std::string> aAttributeList) : attributeList(aAttributeList) {}; // Custom Constructor

	// Attribute functions
	void addAttribute(std::string attribute);
	void deleteAttribute(int idx);
	void renameRecordAttribute(std::string attribute1, std::string attribute2);

	// Overload [] operator for ease of access
	std::string& operator[](int idx);

	// Getters and Setters
	int getSize() { return (attributeList.size()); }
	std::string getKey() { return attributeList.at(0); }
	std::vector<std::string> getAttributeList() { return (attributeList); }

	// Functions called by Table Class - Not by User! 
	void writeToFile(std::ofstream &fileStream);
	Record readFromFile(std::string line);

private:
	std::vector<std::string> attributeList;

	// Print a record (for debugging) 
	friend std::ostream&operator<<(std::ostream&os, const Record& record)
	{
		for (int i = 0; i < record.attributeList.size(); i++)
		{
			std::cout << record.attributeList.at(i) << " ";
		}
		return os;
	}
};

#endif