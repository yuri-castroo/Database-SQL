#include "pch.h"
using namespace std;

void Database::createTable(string tableName, Table t)
{
	// Error-checking: Ensure table doesn't already exist!
	auto it = database.find(tableName);
	if (it != database.end())
	{
		printf("Error: Table with name '%s' already exists in database!\n", tableName.c_str());
		exit(-1);
	}

	// Error-checking: Key field cannot be null!
	if (t.getKey() == "")
	{
		printf("Error: Table '%s' has an invalid key!\n", tableName.c_str());
		exit(-1);
	}

	// Insert table into database
	database.insert(std::pair<string, Table>(tableName, t));

	//cout << "The createTable member function was called." << endl;
}

void Database::drop(string tableName)
{
	if (database.size() == 0) // If database is empty, print error 
	{
		printf("Error: Database is empty!\n");
		exit(-1);
	}

	// Find table via iterator
	auto it = database.find(tableName);
	if (it != database.end()) // Delete table if exists
	{
		database.erase(it);
	}
	else // If table does not exist, print error
	{
		printf("Error: Table '%s' does not exist!\n", tableName.c_str());
		exit(-1);
	}

	//cout << "The drop member function was called." << endl;
}

void Database::save(string filename)
{
	// open fstream for writing to file "filename"
	std::ofstream fileStream;
	fileStream.open(filename);

	fileStream << getDatabaseName() << endl << endl << endl;

	// Iterate over database
	for (auto it : database)
	{
		// Call write function on every Table
		// Note: writeToFile() is implemented in Table class
		fileStream << it.second.getTableName() << endl;
		it.second.writeToFile(fileStream);
		fileStream << endl;
	}
	//cout << "The save member function was called." << endl;
}

void Database::load(string filename)
{
	// Iterate over database
	string line = "", myDatabaseName = "";
	std::ifstream fileStream;
	fileStream.open(filename);
	getline(fileStream, myDatabaseName);
	this->setDatabaseName(myDatabaseName);

	if (fileStream.is_open()) // Read individual records from file 
	{
		while (getline(fileStream >> std::ws, line)) // skip whitespace 
		{
			Table myTable;
			string myTableName = line;
			myTable = myTable.readFromFile(fileStream); // Get record from file
			myTable.setTableName(myTableName);
			this->createTable(myTable.getTableName(), myTable);
		}
	}

	//cout << "The load member function was called." << endl;
}

vector<string> Database::getTableNames() const
{
	vector<string> tableNameVect;

	// Iterate over database
	for (auto it : database)
	{
		// Add table name to list
		tableNameVect.push_back(it.first);
	}
	// Return list of table names
	return tableNameVect;

	//cout << "The getTableNames member function was called." << endl;
	//return tableNameVect;
}

Table Database::getTable(std::string tableName) const
{
	if (database.size() == 0) // If database is empty, print error 
	{
		printf("Error: Database '%s' is empty!\n", databaseName.c_str());
		exit(-1);
	}

	// Find table via iterator
	auto it = database.find(tableName);
	// Return table if exists
	if (it != database.end()) return it->second;
	// If table does not exist, print error
	else
	{
		printf("Error: Table '%s' does not exist!\n", tableName.c_str());
		exit(-1);
	}

	/*cout << "The getTable member function was called." << endl;
	Table t = Table();
	return t;*/
}

vector<Table> Database::getTables() const
{
	// Create vector to hold all tables
	vector<Table> tableList;

	// Iterate over database
	for (auto it : database)
	{
		// Add table to vector
		tableList.push_back(it.second);
	}

	// Return list of tables 
	//cout << "The getTables member function was called." << endl;
	return tableList;
}

// Extra Credit!
void Database::merge(Database d)
{
	// Error-checking: Empty database!
	if (d.getTables().size() == 0)
	{
		printf("Error: Cannot merge empty Database!\n");
		exit(-1);
	}

	for (Table t : d.getTables())
	{
		// Note: All error-checking performed in createTable() function
		this->createTable(t.getTableName(), t);
	}
}

// Extra Credit!
Database::Database(const Database &d2) // Copy constructor!
{
	// No dynamic memory allocation -> No pointer reassignation
	databaseName = d2.databaseName;
	database = d2.database;
}

vector<string> Database::parseSelectCommand(string s)
{
	vector<string> attributeNames;
	int i = 0;
	string tmp = "";
	while (i < s.size())
	{
		if (s.at(i) != ',') // Strip commas
		{
			tmp += s.at(i);
		}
		else
		{
			attributeNames.push_back(tmp); // Add string to attributes list
			tmp = ""; // Reset tmp string
			i++; // Remove whitespace character 
		}
		i++;
	}
	attributeNames.push_back(tmp); // Add last attribute to list (no following comma) 
	return attributeNames;
}

string Database::simpleExpression(string simpleExpr)
{
	// We want to find the middle operator which can only be one of a few things
	size_t middleOperatorLoc;
	string token1 = "";
	string token2 = "";
	string token3 = "";
	if ((middleOperatorLoc = simpleExpr.find("=")) != string::npos)
	{
		if (middleOperatorLoc < (middleOperatorLoc + simpleExpr.substr(middleOperatorLoc).find("\"")))
		{
			middleOperatorLoc = simpleExpr.substr(middleOperatorLoc+1).find("=");
		}
		token2 = "=";
		if (simpleExpr.find("\"") != string::npos)
		{
			token1 = simpleExpr.substr(1, middleOperatorLoc - 3);
			token3 = simpleExpr.substr(middleOperatorLoc + 2);
		}
		else
		{
			token1 = simpleExpr.substr(0, middleOperatorLoc - 1);
			token3 = simpleExpr.substr(middleOperatorLoc + 2);
		}
	}
	else if ((middleOperatorLoc = simpleExpr.find("<>")) != string::npos)
	{
		if (middleOperatorLoc < (middleOperatorLoc + simpleExpr.substr(middleOperatorLoc).find("\"")))
		{
			middleOperatorLoc = simpleExpr.find("<>");
		}
		token2 = "<>";
		if (simpleExpr.find("\"") != string::npos)
		{
			token1 = simpleExpr.substr(1, middleOperatorLoc - 3);
			token3 = simpleExpr.substr(middleOperatorLoc + 3);
		}
		else
		{
			token1 = simpleExpr.substr(0, middleOperatorLoc - 1);
			token3 = simpleExpr.substr(middleOperatorLoc + 3);
		}
	}
	else if ((middleOperatorLoc = simpleExpr.find("<")) != string::npos)
	{
		if (middleOperatorLoc < (middleOperatorLoc + simpleExpr.substr(middleOperatorLoc).find("\"")))
		{
			middleOperatorLoc = simpleExpr.find("<");
		}
		token2 = "<";
		if (simpleExpr.find("\"") != string::npos)
		{
			token1 = simpleExpr.substr(1, middleOperatorLoc - 3);
			token3 = simpleExpr.substr(middleOperatorLoc + 2);
		}
		else
		{
			token1 = simpleExpr.substr(0, middleOperatorLoc - 1);
			token3 = simpleExpr.substr(middleOperatorLoc + 2);
		}
	}
	else if ((middleOperatorLoc = simpleExpr.find(">")) != string::npos)
	{
		if (middleOperatorLoc < (middleOperatorLoc + simpleExpr.substr(middleOperatorLoc).find("\"")))
		{
			middleOperatorLoc = simpleExpr.find(">");
		}
		token2 = ">";
		if (simpleExpr.find("\"") != string::npos)
		{
			token1 = simpleExpr.substr(1, middleOperatorLoc - 3);
			token3 = simpleExpr.substr(middleOperatorLoc + 2);
		}
		else
		{
			token1 = simpleExpr.substr(0, middleOperatorLoc - 1);
			token3 = simpleExpr.substr(middleOperatorLoc + 2);
		}
	}
	else if ((middleOperatorLoc = simpleExpr.find(">=")) != string::npos)
	{
		if (middleOperatorLoc < (middleOperatorLoc + simpleExpr.substr(middleOperatorLoc).find("\"")))
		{
			middleOperatorLoc = simpleExpr.find(">=");
		}
		token2 = ">=";
		if (simpleExpr.find("\"") != string::npos)
		{
			token1 = simpleExpr.substr(1, middleOperatorLoc - 3);
			token3 = simpleExpr.substr(middleOperatorLoc + 3);
		}
		else
		{
			token1 = simpleExpr.substr(0, middleOperatorLoc - 1);
			token3 = simpleExpr.substr(middleOperatorLoc + 3);
		}
	}
	else if ((middleOperatorLoc = simpleExpr.find("<=")) != string::npos)
	{
		if (middleOperatorLoc < (middleOperatorLoc + simpleExpr.substr(middleOperatorLoc).find("\"")))
		{
			middleOperatorLoc = simpleExpr.find("<=");
		}
		token2 = "<=";
		if (simpleExpr.find("\"") != string::npos)
		{
			token1 = simpleExpr.substr(1, middleOperatorLoc - 3);
			token3 = simpleExpr.substr(middleOperatorLoc + 3);
		}
		else
		{
			token1 = simpleExpr.substr(0, middleOperatorLoc - 1);
			token3 = simpleExpr.substr(middleOperatorLoc + 3);
		}
	}
	else if ((middleOperatorLoc = simpleExpr.find("and")) != string::npos)
	{
		if (middleOperatorLoc < (middleOperatorLoc + simpleExpr.substr(middleOperatorLoc).find("\"")))
		{
			middleOperatorLoc = simpleExpr.find("and");
		}
		token2 = "and";
		if (simpleExpr.find("\"") != string::npos)
		{
			token1 = simpleExpr.substr(1, middleOperatorLoc - 3);
			token3 = simpleExpr.substr(middleOperatorLoc + 4);
		}
		else
		{
			token1 = simpleExpr.substr(0, middleOperatorLoc - 1);
			token3 = simpleExpr.substr(middleOperatorLoc + 4);
		}
	}
	else if ((middleOperatorLoc = simpleExpr.find("or")) != string::npos)
	{
		if (middleOperatorLoc < (middleOperatorLoc + simpleExpr.substr(middleOperatorLoc).find("\"")))
		{
			middleOperatorLoc = simpleExpr.find("or");
		}
		token2 = "or";
		if (simpleExpr.find("\"") != string::npos)
		{
			token1 = simpleExpr.substr(1, middleOperatorLoc - 3);
			token3 = simpleExpr.substr(middleOperatorLoc + 3);
		}
		else
		{
			token1 = simpleExpr.substr(0, middleOperatorLoc - 1);
			token3 = simpleExpr.substr(middleOperatorLoc + 3);
		}
	}
	// The second token should now be checked and used to compare
	// We check first if we are comparing strings or integers
	double token1d = 0.0;
	double token3d = 0.0;
	bool isDecimal1 = true;
	bool isDouble1 = false;
	for (int i = 0; i < token1.length(); i++)
	{
		if ((token1.at(i) > 58) || (token1.at(i) < 47))
		{
			if (token1.at(i) == '.')
			{
				isDouble1 = true;
			}
			else
			{
				isDecimal1 = false;
			}
		}
	}
	if (isDouble1 && isDecimal1)
	{
		token1d = stod(token1);
	}
	else if (isDecimal1)
	{
		token1d = stod(token1);
	}
	bool isDecimal3 = true;
	bool isDouble3 = false;
	for (int i = 0; i < token3.length(); i++)
	{
		if ((token3.at(i) > 58) || (token3.at(i) < 47))
		{
			if (token3.at(i) == '.')
			{
				isDouble3 = true;
			}
			else
			{
				isDecimal3 = false;
			}
		}
	}
	if (isDouble3 && isDecimal3)
	{
		token3d = stod(token3);
	}
	else if (isDecimal3)
	{
		token3d = stod(token3);
	}
	if (token2 == "=")
	{
		// In this case we check to see if the expression on the right is the same as the expression on the left.
		if (isDouble1 && isDouble3)
		{
			if (token1d == token3d)
			{
				return "true";
			}
			return "false";
		}
		if (isDecimal1 && isDecimal3)
		{
			if (token1d == token3d)
			{
				return "true";
			}
			return "false";
		}
		if (token1 == token3)
		{
			return "true";
		}
		return "false";
	}
	else if (token2 == "<>")
	{
		// This case indicates that we do not want the expression on the right to be the same as the expression on the left.
		if (isDouble1 && isDouble3)
		{
			if (token1d != token3d)
			{
				return "true";
			}
			return "false";
		}
		if (isDecimal1 && isDecimal3)
		{
			if (token1d != token3d)
			{
				return "true";
			}
			return "false";
		}
		if (token1 != token3)
		{
			return "true";
		}
		return "false";
	}
	else if (token2 == "<")
	{
		// Even if the token1 and token3 are integers in string format it should operate the same.
		if (isDouble1 && isDouble3)
		{
			if (token1d < token3d)
			{
				return "true";
			}
			return "false";
		}
		if (isDecimal1 && isDecimal3)
		{
			if (token1d < token3d)
			{
				return "true";
			}
			return "false";
		}
		if (token1 < token3)
		{
			return "true";
		}
		return "false";
	}
	else if (token2 == ">")
	{
		if (isDouble1 && isDouble3)
		{
			if (token1d > token3d)
			{
				return "true";
			}
			return "false";
		}
		if (isDecimal1 && isDecimal3)
		{
			if (token1d > token3d)
			{
				return "true";
			}
			return "false";
		}
		if (token1 > token3)
		{
			return "true";
		}
		return "false";
	}
	else if (token2 == ">=")
	{
		if (isDouble1 && isDouble3)
		{
			if (token1d >= token3d)
			{
				return "true";
			}
			return "false";
		}
		if (isDecimal1 && isDecimal3)
		{
			if (token1d >= token3d)
			{
				return "true";
			}
			return "false";
		}
		if (token1 >= token3)
		{
			return "true";
		}
		return "false";
	}
	else if (token2 == "<=")
	{
		if (isDouble1 && isDouble3)
		{
			if (token1d <= token3d)
			{
				return "true";
			}
			return "false";
		}
		if (isDecimal1 && isDecimal3)
		{
			if (token1d <= token3d)
			{
				return "true";
			}
			return "false";
		}
		if (token1 <= token3)
		{
			return "true";
		}
		return "false";
	}
	else if (token2 == "and")
	{
		// This function only works if the left and right values are true or false.
		if (((token1 == "false") || (token1 == "true")) && ((token3 == "false") || (token3 == "true")))
		{
			// This is where we change token1 and token3 to true or false.
			if ((token1 == "true") && (token3 == "true"))
			{
				return "true";
			}
			else if ((token1 == "true") && (token3 == "false"))
			{
				return "false";
			}
			else if ((token1 == "false") && (token3 == "true"))
			{
				return "false";
			}
			else if ((token1 == "false") && (token3 == "false"))
			{
				return "false";
			}
		}
		else {
			cerr << "ERROR: Unrecognized boolean expression, please check the expression" << endl;
			exit(-1);
		}
	}
	else if (token2 == "or")
	{
		// This function only works if the left and right values are true or false.
		if (((token1 == "false") || (token1 == "true")) && ((token3 == "false") || (token3 == "true")))
		{
			// This is where we change token1 and token3 to true or false.
			if ((token1 == "true") && (token3 == "true"))
			{
				return "true";
			}
			else if ((token1 == "true") && (token3 == "false"))
			{
				return "true";
			}
			else if ((token1 == "false") && (token3 == "true"))
			{
				return "true";
			}
			else if ((token1 == "false") && (token3 == "false"))
			{
				return "false";
			}
		}
		else {
			cerr << "ERROR: Unrecognized boolean expression, please check the expression" << endl;
			exit(-1);
		}
	}
	else
	{
		cerr << "ERROR: Unrecognized boolean expression, please check the expression" << endl;
		exit(-1);
	}
	// If the program gets here then there is an error.
	cerr << "Unrecognized boolean expression, please check the expression" << endl;
	exit(-1);
}

bool Database::booleanEvaluater(string boolExpression)
{
	// Looping through the boolean expression and then changing all the characters to lower case.
	for (int i = 0; i < boolExpression.length(); i++)
	{
		boolExpression.at(i) = tolower(boolExpression.at(i));
	}
	for (int i = 0; i < boolExpression.length(); i++)
	{
		if (boolExpression.at(i) == '\'')
		{
			boolExpression.erase(i, 1);
		}
	}
	// These will represent the indexes of the current opening and closing parenthesis to replace.
	int closingIndex = INFINITY;
	int startingIndex = INFINITY;
	string replaceString = "";
	int k = 0;
	while (true)
	{
		if ((boolExpression == "false") || (boolExpression == "true"))
		{
			break;
		}
		// Need a check to see if there are no parenthesis left and then use the simple Expression to return
		if (boolExpression.find(')') == string::npos)
		{
			boolExpression = simpleExpression(boolExpression);
			continue;
		}
		string boolExprTest = boolExpression.substr(1);
		// Loop until we see a closing parenthesis, then loop until the next earliest open parenthesis, then convert that into a base boolean string.
		for (int i = 0; i < boolExpression.length(); i++)
		{
			if ((boolExpression.at(i) == ')') && (i > boolExprTest.find("\"")))
			{
				// Now we save the ith index in closing index value.
				closingIndex = i;
				// Now we loop back until we find the first (
				for (int j = i; i >= 0; j--)
				{
					if (boolExpression.at(j) == '(')
					{
						startingIndex = j;
						break;
					}
				}
				// By now we should have the beginning and end of the expression to replace, we just need to call the simpleExpression carefully.
				boolExpression.erase(closingIndex, 1);
				boolExpression.erase(startingIndex, 1);
				replaceString = boolExpression.substr(startingIndex, closingIndex - startingIndex - 1);
				// Now evaluating the simpleExpression.
				if (replaceString == "les")
				{
					cout << "STOP" << endl;
				}
				replaceString = simpleExpression(replaceString);
				boolExpression.replace(startingIndex, closingIndex - startingIndex - 1, replaceString);
				// Now we can perform the same actions again until the last thing we replace is a true or false.
				break;
			}
			else if ((boolExpression == "false") || (boolExpression == "true"))
			{
				break;
			}
			else
			{
				boolExpression = simpleExpression(boolExpression);
			}
		}

	}
	// returning the correct value of the expression.
	if (boolExpression == "false")
	{
		return false;
	}

	return true;
}

// This function is the mainstay of the database class. This will execute a standard query command based on the arguments below.
Table* Database::Query(std::string selectInput, std::string fromInput, std::string whereInput)
{
	// Error-checking: Null command strings 
	if (selectInput == "" || fromInput == "" || whereInput == "")
	{
		printf("Error: One or more invalid input strings!\n");
		exit(-1);
	}
	string tableName = fromInput; // FROM argument is 1 table name
	auto table_iterator = database.find(tableName); // Look for table name in database

	// Error-checking: Nonexistent table
	if (table_iterator == database.end())
	{
		printf("Error: Database does not contain Table '%s!'\n", tableName.c_str());
		exit(-1);
	}
	// Error-checking: Empty table
	else if (table_iterator->second.getSize() == 0) // If table does not have any column names (empty)
	{
		printf("Error: Table '%s' is empty!\n", tableName.c_str());
		exit(-1);
	}
	Table myTable;
	myTable = table_iterator->second; // FINAL TABLE TO QUERY!
	vector<string> tableAttributeNameList; // FINAL LIST OF ATTRIBUTES! 

	// If SELECT is * then take ALL attributes names from table 
	if (selectInput == "*") tableAttributeNameList = table_iterator->second.getAttributeNameList();
	else tableAttributeNameList = parseSelectCommand(selectInput);

	// Ensure all selected attributes exist within chosen table 
	for (int i = 0; i < tableAttributeNameList.size(); i++)
	{
		string attribute = tableAttributeNameList.at(i);
		// Search Table's attribute list for desired attribute 

		vector<string> v = myTable.getAttributeNameList();
		auto it = find(v.begin(), v.end(), attribute);

		// Error-checking: If Table's attribute list does not contain desired attribute
		if (it == v.end())
		{
			printf("Error: Table '%s' does not contain Attribute '%s!'\n", tableName.c_str(), attribute.c_str());
			exit(-1);
		}
	}

	// First we check if the table given to us in the fromInput exists
	Table* tableInfoCollector = new Table();
	// Adding all the attributes to the return table:
	for (int i = 0; i < tableAttributeNameList.size(); i++)
	{
		tableInfoCollector->addAttribute(tableAttributeNameList.at(i));
	}
	// Developing a vector of boolean expressions from the last argument
	// We want to get the attributes from the table we aquired, note that this is NOT the same as the attribute list vector above
	vector<string> allAttributes = myTable.getAttributeNameList();
	vector<string> booleanExpressions = vector<string>(myTable.getSize(), whereInput);
	// Now that we have all of the attributes we can go through each string and replace it with the correct values.
	// A string iterator is needed when using the find function.
	size_t myStringInit = 0;
	string currentAttribute = "";
	string currentBoolean = "";
	string replaceValue = "";
	int i = 0;
	map<string, Record> workingTable = myTable.getTable();
	std::map<std::string, Record>::iterator it = workingTable.begin();
	std::map<std::string, Record>::iterator itEND = workingTable.end();
	vector<int> replacePositions = vector<int>();
	for (it = it; it != itEND; it++)
	{
		// Every expression must replace all of the attributes in the string with their data value.
		for (int j = 0; j < allAttributes.size(); j++)
		{
			currentAttribute = allAttributes.at(j);
			currentBoolean = booleanExpressions.at(i);
			myStringInit = currentBoolean.find(currentAttribute);
			if (myStringInit != string::npos)
			{
				replacePositions.push_back(myStringInit);
			}
			while (myStringInit != string::npos)
			{
				// Here is where we replace the string at the current iterator location with the value in the attribute table.
				// Collecting the value of the replacement from the table.				

				currentBoolean.replace(myStringInit, currentAttribute.length(), "");
				myStringInit = currentBoolean.find(currentAttribute);
				if (myStringInit != string::npos)
				{
					replacePositions.push_back(myStringInit);
				}
				// replacing in the boolean Expression vector
				booleanExpressions.at(i) = currentBoolean;
			}
			// Looping through and replacing all of the values
			for (int k = 0; k < replacePositions.size(); k++)
			{
				currentBoolean.replace(replacePositions.at(k), 0, it->second.getAttributeList().at(j));
				if (k == (replacePositions.size() - 1))
				{
					// replacing in the boolean Expression vector
					booleanExpressions.at(i) = currentBoolean;				
				}
			}
			replacePositions.clear();
		}
		// Making sure to reset the value of myStringInit
		i++;
		myStringInit = 0;
	}
	// Private Helper function That returns the boolean result of the formula.

	vector<bool> boolList = vector<bool>(booleanExpressions.size());
	for (int i = 0; i < booleanExpressions.size(); i++)
	{
		// Populating the boolean list with the result of the boolean expression.
		boolList.at(i) = booleanEvaluater(booleanExpressions.at(i));
	}
	// Now that we have a list of the records to keep and remove. We loop through the table and keep records accordingly.
	// Variable k goes along with the iterator and determines if the current record is to be used.
	int k = 0;
	it = workingTable.begin();
	itEND = workingTable.end();
	for (it = it; it != itEND; it++)
	{
		if (boolList.at(k))
		{
			// Creating a record to put in the return table.
			Record currentRec = Record();
			vector<string>::iterator myIt;
			string currentFind = "";
			int attributeInd = 0;
			for (int i = 0; i < myTable.getAttributeNameList().size(); i++)
			{
				currentFind = myTable.getAttributeNameList().at(i);
				if ((myIt = find(tableAttributeNameList.begin(), tableAttributeNameList.end(), currentFind)) != tableAttributeNameList.end())
				{
					// Compute the distance to the iterator and then add to the current record.
					attributeInd = std::distance(tableAttributeNameList.begin(), myIt);
					currentRec.addAttribute(it->second[attributeInd]);
				}
			}
			// Adding the record to the table.
			tableInfoCollector->insertRecord(it->second.getKey(), currentRec);
		}
		k++;
	}

	return tableInfoCollector; 
}