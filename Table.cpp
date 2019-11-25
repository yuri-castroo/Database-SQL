#include "pch.h"
using namespace std;

// Custom parametrized constructor to create Table with many pre-set values.
Table::Table(std::string aTableName, std::string aKey, std::vector<std::string> aAttributeNameList)
{
	// Error-checking: Empty attribute list
	if (aAttributeNameList.size() == 0)
	{
		printf("Error: Attribute Name List is empty!\n");
		exit(-1);
	}

	// Error-checking: Nonexistant key attribute in attribute name list 
	if (aAttributeNameList.at(0) != aKey)
	{
		printf("Error: Key does not exist in Attribute Name List!\n");
		exit(-1);
	}

	tableName = aTableName;
	key = aKey;
	attributeNameList = aAttributeNameList;
}

// Note: Private function! 
int Table::getAttributeIndex(string attribute) const
{
	// Find index of specified attribute by iterating through attribute list
	for (int i = 0; i < getAttributeNameList().size(); ++i)
	{
		if (attributeNameList.at(i) == attribute)
		{
			return i;
		}
	}
	// Note: Ensure attribute exists by performing error-checking on idx in function usage
	return -1; // If attribute does not exist 
}

void Table::addAttribute(string attribute)
{
	// Error-checking: make sure attribute doesn't already exist!
	auto it = find(attributeNameList.begin(), attributeNameList.end(), attribute);
	if (it != attributeNameList.end())
	{
		printf("Error: Attribute %s already exists!\n", attribute.c_str());
		exit(-1);
	}

	// Add attribute to list of Table attributes
	attributeNameList.push_back(attribute);
	// Iterate over Table
	for (auto it : table)
	{
		// Call addAttribute() (implemented in Record class) on every Record 
		table[it.first].addAttribute(attribute);
	}
}

void Table::deleteAttribute(string attribute)
{
	auto it = find(attributeNameList.begin(), attributeNameList.end(), attribute);
	if (it == attributeNameList.end()) // If attribute does not exist, print error
	{
		printf("Error: Attribute %s does not exist!\n", attribute.c_str());
		exit(-1);
	}
	else
	{
		attributeNameList.erase(it); // Delete attribute from list of attributes 
		int idx = getAttributeIndex(attribute);
		// Iterate over Table
		for (auto elem : table)
		{
			// Call deleteAttribute() (implemented in Record class) on every Record 
			table[elem.first].deleteAttribute(idx);
		}
	}
}

/* Error-checking:
1. Ensure attribute numbers match
2. Non-null key value
3. No duplicate key values
*/
void Table::insertRecord(std::string key, Record r)
{
	// Error checking#1: ensure attribute numbers match
	if (r.getSize() != getAttributeNameList().size())
	{
		vector<string>tmp = getAttributeNameList(); // For debugging! 
		//printf("Error: Incorrect number of attributes provided!\n");
		return;
		//exit(-1);
	}

	// Error checking#2: If specified key is empty
	if (r[0] == "")
	{
		// If key value is NULL, print error
		printf("Error: Key value cannot be NULL!\n");
		exit(-1);
	}

	// Error checking#3: If key provided is not 1st attribute in Record 
	if (key != r[0])
	{
		printf("Error: The specified Record key '%s' does not match first Record attribute '%s!'\n", key.c_str(), r[0].c_str());
		exit(-1);
	}

	else // If key value is not-NULL
	{
		string key = r[0]; // Get key

		// Error checking#3: If key already exists
		auto it = table.find(key);
		if (it != table.end()) // If key already exists in table 
		{
			// If key already exists, print error
			printf("Error: Item with key %s already exists!\n", key.c_str());
			exit(-1);
		}

		// Else insert Record into table
		else table.insert(std::pair<string, Record>(make_pair(key, r))); // Insert Record into table
	}
}

/* Error-checking:
1. Non-empty table
2. Non-existent key value
*/
void Table::deleteRecord(string key)
{
	auto it = table.find(key);

	if (table.size() == 0) // If table is empty 
	{
		printf("Error: Cannot delete from empty table!\n");
		exit(-1);
	}

	// If key doesn't exist, print error - don't need this!
	if (it == table.end())
	{
		printf("Error: Item with key %s does not exist!\n", key.c_str());
		exit(-1);
	}
	else // else delete record 
	{
		table.erase(it);
	}
}

// TODO determine what the database means - ??? 
Record Table::getRecord(string aKey) const
{
	// Find Record via key
	auto it = table.find(aKey);

	// Return Record if exists
	if (it != table.end()) return it->second;

	// If Record does not exist, print error
	else
	{
		printf("Error: Record '%s' does not exist!\n", aKey.c_str());
		exit(-1);
	}
}

int Table::getCount(string attribute) const
{
	// Find index of attribute
	int idx = getAttributeIndex(attribute);
	if (idx != -1) // if attribute exists
	{
		int ctr = 0;
		for (auto it : table) // Iterate over map
		{
			Record record = it.second;
			if (record[idx] != "") ctr++;
		}
		return ctr;
	}
	else // If Attribute does not exist, print error
	{
		printf("Error: Attribute %s does not exist!\n", attribute.c_str());
		return -1;
	}
}

double Table::getMin(string attribute) const
{
	// Find index of attribute
	int idx = getAttributeIndex(attribute);
	if (idx != -1) // if attribute exists
	{
		double min = 2147483648; // Equate to max value
		for (auto it : table) // Iterate over map
		{
			Record record = it.second;
			if (record[idx] != "")
			{
				if ((int)record[idx].at(0) >= 48 && (int)record[idx].at(0) <= 57) // If 1st digit is between 0-9 (parseable to int)
				{
					min = (stod(record[idx]) < min) ? stod(record[idx]) : min; // Convert string -> double
				}
			}
		}
		return min;
	}
	else // If Attribute does not exist, print error
	{
		printf("Error: Attribute %s does not exist!\n", attribute.c_str());
		exit(-1);
	}
	return 0;
}

double Table::getMax(string attribute) const
{
	// Find index of attribute
	int idx = getAttributeIndex(attribute);
	if (idx != -1) // if attribute exists
	{
		double max = -2147483647; // Equate to min value
		for (auto it : table) // Iterate over map
		{
			Record record = it.second;
			if (record[idx] != "")
			{
				if ((int)record[idx].at(0) >= 48 && (int)record[idx].at(0) <= 57) // If 1st digit is between 0-9 (parseable to int)
				{
					max = (stod(record[idx]) > max) ? stod(record[idx]) : max; // Convert string -> double
				}
			}
		}
		return max;
	}
	else // If Attribute does not exist, print error
	{
		printf("Error: Attribute %s does not exist!\n", attribute.c_str());
		return -1;
	}
	return 0;
}

// Function called by Database Class
void Table::writeToFile(ofstream &fileStream)
{
	// Write all table attributes (column names) to the file 
	for (int i = 0; i < attributeNameList.size(); i++)
	{
		fileStream << attributeNameList.at(i) << "^";
	}
	fileStream << endl;

	// Iterate over table
	for (auto it : table)
	{
		// Write each Table's Records to the file (writeToFile() is implemented in Record class)
		it.second.writeToFile(fileStream);
	}
	fileStream << endl;
}

// Gets list of attributes (column names) from file 
std::vector<std::string> Table::readAttributeNameList(std::string & line)
{
	vector<string> myAttributeNameList; // list of column names 
	stringstream ss(line);
	string word = "";
	while (getline(ss, word, '^'))
	{
		myAttributeNameList.push_back(word);
	}
	return myAttributeNameList;
}

// Function called by Database Class
Table Table::readFromFile(ifstream &fileStream)
{
	string line = "", myTableName = "";
	getline(fileStream, line); // Read rest of the lines from file 
	vector<string> myAttributeNameList = readAttributeNameList(line); // vector of column names

	Table t(myTableName, myAttributeNameList.at(0), myAttributeNameList); // Name, Key, AttributeList  

	if (fileStream.is_open()) // Read individual records from file 
	{
		Record myRecord;
		while (getline(fileStream, line) && line != "")
		{
			myRecord = myRecord.readFromFile(line); // Get record from file 
			string key = myRecord.getKey(); // Get key
			t.insertRecord(myRecord.getKey(), myRecord); // Insert Record in table
		}
	}
	return t; // Return table (loaded from file) 
}

Table Table::crossJoin(Table *t2)
{
	// Calculate size of joined table 
	int resultSize = this->getSize() * t2->getSize();

	// Get attribute lists from each table
	vector<string> attributes1 = this->getAttributeNameList();
	vector<string> attributes2 = t2->getAttributeNameList();
	vector<string> resultAttributes;

	// Add "Count" column (distinguishes between identical keys) 
	resultAttributes.push_back("Count");

	// Combine both attribute name lists 
	for (int i = 0; i < attributes1.size(); i++) {
		resultAttributes.push_back(attributes1.at(i));
	}
	for (int i = 0; i < attributes2.size(); i++) {
		resultAttributes.push_back(attributes2.at(i));
	}

	// Create final table (Cross Join) 
	Table resultT = Table("result", resultAttributes.at(0), resultAttributes);

	// Create all Record combinations from Table#1 and Table#2 (makes Table#1 size * Table#2 size records)
	int count = 0;
	for (auto it_1 : this->table) {
		for (auto it_2 : t2->table)
		{
			// Get records and attribute lists 
			Record r1 = it_1.second;
			Record r2 = it_2.second;
			vector<string> r1Attributes = r1.getAttributeList();
			vector<string> r2Attributes = r2.getAttributeList();

			// Create resultant attribute list 
			vector<string> newRecAttributes = r1Attributes;
			// Add "Count" attribute
			newRecAttributes.insert(newRecAttributes.begin(), to_string(count));
			// Append attribute lists
			newRecAttributes.insert(newRecAttributes.end(), r2Attributes.begin(), r2Attributes.end());

			// Create new record
			Record newRec = Record(newRecAttributes);
			// Insert record in Table
			resultT.insertRecord(to_string(count), newRec);
			count++;
		}
	}
	return resultT; // Return Cross-Joined Table
}

// Comparison function used in Natural join 
bool compare(const std::string &lhs, const std::string &rhs)
{
	return (lhs.size() == rhs.size()) ? lhs < rhs : lhs.size() < rhs.size();
}

// Perform a Natural Join on 2 tables.
Table Table::naturalJoin(Table *t2)
{
	// Calculate resultant size of joined table 
	int resultSize = this->getSize() * t2->getSize();

	// Get attribute lists from each table
	vector<string> attributes1 = this->getAttributeNameList();
	vector<string> attributes2 = t2->getAttributeNameList();
	vector<string> resultAttributes;

	// Sort each attribute list via custom comparator
	std::sort(attributes1.begin(), attributes1.end(), compare);
	std::sort(attributes2.begin(), attributes2.end(), compare);
	std::vector<std::string> intersection;

	// Find attributes common to both tables 
	std::set_intersection(attributes1.begin(), attributes1.end(), attributes2.begin(), attributes2.end(), std::back_inserter(intersection), compare);

	// Error-checking: Check if no common attributes 
	if (intersection.empty()) {
		printf("Error: No common attributes to join on!\n");
		exit(-1);
	}

	// Get 1st common attribute
	string sameAttr = intersection.at(0);
	int indxComm = this->getAttributeIndex(sameAttr);

	// Get attribute lists
	vector<string> att1 = this->getAttributeNameList();
	vector<string> att2 = t2->getAttributeNameList();
	vector<string> newAttributesNames = att1;

	for (int i = 0; i < att2.size(); i++) {
		if (att2.at(i) == sameAttr) {
			std::vector<string>::iterator toErase;
			toErase = std::find(att2.begin(), att2.end(), sameAttr);

			// Erase attribute if found
			if (toErase != att2.end()) {
				att2.erase(toErase);
			}
			// Append attribute lists
			newAttributesNames.insert(newAttributesNames.end(), att2.begin(), att2.end());
		}
	}

	// Create final Table (Natural Join)
	Table resultT = Table("result", newAttributesNames.at(0), newAttributesNames);

	// Iterate over Table#1 
	for (auto itr : this->table)
	{
		vector<string> newAttributes;
		Record r1 = itr.second;
		vector<string> attributes1 = r1.getAttributeList();
		newAttributes = attributes1;
		// Get index of common attribute 
		int indx1 = this->getAttributeIndex(sameAttr);

		// Get common attribute 
		string commonAttribute = r1[indx1];

		// Find common attribute in Table#2
		for (auto itr2 : t2->table) {
			Record r2 = itr2.second;
			vector<string> attributesOfRecord = r2.getAttributeList();

			// Get index of common attribute
			int indx2 = t2->getAttributeIndex(sameAttr);
			if (attributesOfRecord.at(indx2) == commonAttribute)
			{
				// Find common attribute in Table#2
				std::vector<string>::iterator toErase;
				toErase = std::find(attributesOfRecord.begin(), attributesOfRecord.end(), commonAttribute);

				// Erase attribute if found
				if (toErase != attributesOfRecord.end()) {
					attributesOfRecord.erase(toErase);
				}
				// Append attribute lists
				newAttributes.insert(newAttributes.end(), attributesOfRecord.begin(), attributesOfRecord.end());
			}
		}
		// Create new record
		Record newRec = Record(newAttributes);
		// Insert record in Table
		resultT.insertRecord(newRec.getKey(), newRec);
	}
	return resultT; // Return Natural-Joined Table
}

// Extra Credit!
void Table::renameAttribute(string attribute1, string attribute2)
{
	// Get index of attribute 
	int idx = this->getAttributeIndex(attribute1);

	if (idx == -1) // If Attribute does not exist, print error
	{
		printf("Error: Attribute '%s' does not exist in Table!\n", attribute1.c_str());
		exit(-1);
	}
	if (attribute2 == "") // If Attribute is empty, print error
	{
		printf("Error: Cannot replace attribute with empty string!\n");
		exit(-1);
	}
	else // Else rename Attribute1 to Attribute2 
	{
		this->getAttributeNameList().at(idx) = attribute2;
	}
}