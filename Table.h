#pragma once
#ifndef TABLE_H
#define TABLE_H

#include "pch.h"

class Table
{
public:
	// Constructors
	Table() {};
	~Table() {};
	Table(std::string aTableName, std::string aKey, std::vector<std::string> aAttributeNameList); // Custom constructor

	// (Column Name) Attribute functions
	void addAttribute(std::string attribute);
	void deleteAttribute(std::string attribute);

	// Record functions 
	void insertRecord(std::string key, Record r);
	void deleteRecord(std::string key);

	// Join functions
	Table crossJoin(Table *t2);
	Table naturalJoin(Table *t2);

	// Functions called by Database Class - Not by User! 
	void writeToFile(std::ofstream &fileStream);
	Table readFromFile(std::ifstream &fileStream);
	std::vector<std::string> readAttributeNameList(std::string &line);

	// Getter and Setters
	std::string getKey() const { return key; }
	std::string getTableName() const { return tableName; }
	std::vector<std::string> getAttributeNameList() const { return attributeNameList; }; // Get list of Table attributes 
	int getSize() const { return table.size(); } // Return table size 
	Record getRecord(std::string aKey) const;
	int getCount(std::string attribute) const; // FIXME: getCount / getSum?
	double getMin(std::string attribute) const;
	double getMax(std::string attribute) const;
	void setKey(std::string aKey) { key = aKey; }
	void setTableName(std::string aTableName) { tableName = aTableName; }

	// The following function is used just for the query function.
	std::map<std::string, Record> getTable() { return table; }

	// Extra Credit!
	void renameAttribute(std::string attribute1, std::string attribute2);

private:
	std::string key; // Table key 
	std::string tableName;
	std::vector<std::string> attributeNameList; // List of all attribute names (column names)
	std::map<std::string, Record> table; // Maps Record key to Record object

	// Private Function
	int getAttributeIndex(std::string attribute) const; // Gets index of attribute in attributeNameList

	// Print Table (for debugging)
	friend std::ostream& operator<<(std::ostream& os, const Table& t)
	{
		os << t.getTableName() << std::endl;
		int i = 0;
		// Prints each row of a table seperately
		for (auto elem : t.table)
		{
			for (auto elem2 : elem.second.getAttributeList())
			{
				os << t.getAttributeNameList().at(i) << ": ";
				os << elem.second.getAttributeList()[i] << std::endl;
				i++;
			}
			os << std::endl;
			i = 0;
		}
		os << std::endl << std::endl;
		return os;
	}
};

#endif