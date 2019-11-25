#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <algorithm>
#include <limits.h>
#include <set>
#include <map>
#include <cmath>
#include <math.h>       
#include "pch.h"       
using namespace std;

// Write attributes vector (data from Business/Review/User) to File 
void printToFile(fstream &outFile, vector<string> attributes)
{
	for (int i = 0; i < attributes.size(); i++)
	{
		outFile << attributes.at(i) << "^";
	}
	outFile << endl;
}

// Get string delimited by colon
string getColonDelimitedString(string s, int pos) // For key 
{
	string tmp = "";
	int i = pos;

	while (i < s.size())
	{
		if (s.at(i) == '"' && s.at(i + 1) == ':')
		{
			return tmp;
		}
		tmp += s.at(i);
		i++;
	}
	return tmp;
}

// Get string delimited by comma
string getCommaDelimitedString(string s, int pos) // For key 
{
	string tmp = "";
	string first = s.substr(pos, 1); // Get 1st character 
	bool isString = 0, isNum = 0, isParen = 0;

	// Check for NULL 
	if (pos + 3 < s.size() && s.substr(pos, 4) == "null") return "null";

	// Else set boolean flags
	if (first == "\"") isString = 1;
	else if (first == "{") isParen = 1;
	else isNum = 1;

	int i = pos;
	while (i < s.size())
	{
		tmp += s.at(i); // Keep adding to tmp string

		if (isString && s.at(i) == '"' && (i + 1 >= s.size() || i + 2 >= s.size() || (s.at(i + 1) == ',' && s.at(i + 2) == '"'))) return tmp; // If string
		else if (isParen && s.at(i) == '}' && (i + 1 >= s.size() || s.at(i + 1) == ',')) return tmp; // If nested loop (Parentheses)
		else if (isNum && (i + 1 >= s.size() || s.at(i + 1) == ',')) return tmp; // If numeral 

		i++;
	}
	return tmp; // Last string 
}

// Parse hours (for business.json) 
vector<string> getHours(string line)
{
	int myPos = line.find("\"hours\":");
	int i = 0, pos = myPos + 9;
	vector<string> tmp(7, "null"); // 7 days of the week
	if (line.substr(myPos + 8, 4) != "null")
	{
		while (pos < line.size() && line.at(pos) != '}')
		{
			string s1 = getColonDelimitedString(line, pos);
			pos += s1.size() + 2;

			string s2 = getCommaDelimitedString(line, pos);
			pos += s2.size() + 2;

			tmp.at(i) = s2;
			i++;
		}
	}
	return tmp;
}

// Parse line from file into words and put into attributes vector 
vector<string> getAttributes(const vector<string>&columnNames, string line)
{
	int pos = 1, vCtr = 0;
	vector<string> attributes;
	while (pos < line.size() && vCtr < columnNames.size())
	{
		string s1 = getColonDelimitedString(line, pos);
		pos += s1.size() + 2;

		/*if (s1 != columnNames.at(vCtr) && s1 != "hours") // If Error
		{
			cout << line << endl << endl;
			cout << s1 << endl << endl;
			cout << "vs." << endl << endl;
			cout << columnNames.at(vCtr) << endl << endl;
		}*/

		string s2 = getCommaDelimitedString(line, pos);
		if (s2 == "\"miAbIuIln8YfLoo6rf3MyQ\"") return attributes; // Don't include this entry 
		pos += s2.size() + 2;

		attributes.push_back(s2);
		vCtr++;
	}
	return attributes;
}

// Parse business/review/user.json file
Table createJSONTable(vector<string> v, string tableName, string jsonFileName, string fileName, int numDataEntries)
{
	// File IO
	fstream inFile;
	fstream outFile;
	inFile.open(jsonFileName);
	outFile.open(fileName);

	if (!inFile) printf("Error opening file to read!\n");
	if (!outFile) printf("Error opening file to write!\n");

	outFile << tableName << " Database" << endl << endl << endl;
	outFile << tableName << " Table" << endl;
	printToFile(outFile, v); // Print attributes to file

	string line = "";
	int ctr = 0;
	while (getline(inFile, line) && ctr < numDataEntries) // Iterate through file line by line 
	{
		// Strip leading and trailing characters "{ }"
		line.erase(line.begin());
		line.pop_back();

		vector<string> attributes = getAttributes(v, line); // Get attribute vector

		if (jsonFileName == "business.json")
		{
			attributes.pop_back();
			vector<string> tmp = getHours(line);
			attributes.insert(attributes.end(), tmp.begin(), tmp.end());
		}
		if (attributes.size() != 0) printToFile(outFile, attributes); // Print attribute vector to file

		ctr++;
	}
	Database db("My Database");
	db.load(fileName); // Load data into file 

	inFile.close();
	outFile.close();
	return db.getTable(tableName + " Table");
}

int main(int argc, char const *argv[])
{
	// BUSINESS, REVIEW, USER TABLES! 
	/* UNCOMMENT HERE IF PARSING THE JSON FILES DIRECTLY
	vector<string> v{ "business_id", "name", "address", "city", "state", "postal_code", "latitude", "longitude", "stars", "review_count", "is_open", "attributes", "categories", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
	vector<string> vv{ "review_id", "user_id", "business_id", "stars", "useful", "funny", "cool", "text", "date" };
	vector<string> vvv{ "user_id", "name", "review_count", "yelping_since", "useful", "funny", "cool", "elite", "friends", "fans", "average_stars", "compliment_hot", "compliment_more", "compliment_profile", "compliment_cute", "compliment_list", "compliment_note", "compliment_plain", "compliment_cool", "compliment_funny", "compliment_writer", "compliment_photos" };

	Table business = createJSONTable(v, "Business", "business.json", "myFile.txt", 10000);
	Table review = createJSONTable(vv, "Review", "review.json", "myFile1.txt", 10000);
	Table user = createJSONTable(vvv, "User", "user.json", "myFile2.txt", 100);
	
	Database myDatabase;
	myDatabase.createTable(business.getTableName(), business);
	myDatabase.createTable(review.getTableName(), review);
	myDatabase.createTable(user.getTableName(), user);
	*/
	Database myDatabase;
	myDatabase.load("myFileFinal.txt");
	// Now we want to let the user determine certain things about the data in the tables we have created.
	bool eventQuit = true;
	while (eventQuit)
	{
		// Here we will prompt the user for some input:
		cout << "--------------------------------------------------" << endl;
		cout << "------------ Yelp Database Initiated  ------------" << endl;
		cout << "--------------------------------------------------" << endl;
		cout << endl;
		cout << "Please setect one of the following menu options by pressing the correct number" << endl;
		cout << "1) Display information about a user by first name." << endl;
		cout << "2) Display information about a business by business name." << endl;
		cout << "3) Display all the reviews from a user or a business." << endl;
		cout << "4) Show all reviews with a specific number of stars." << endl;
		cout << "10) Exit the Yelp Database" << endl;
		cout << "--------------------------------------------------" << endl;
		// ADD ANY EXTRA OPTIONS HERE
		// Now we ask for the user input
		int userChoice;
		cin >> userChoice;
		// If else block to display different things based on the input:
		if (userChoice == 1)
		{
			// Here we will print all of the data about a certain person.
			// Now we look for the user and print out all of the attribute data about him
			cout << "Please enter the name of the user you would like to find:" << endl;
			string userName;
			cin >> userName;
			cout << "Printing the user data:";
			// Running a query after changing the userName to be the right value.
			string whereInput = "name = ";
			whereInput.append(userName);
			cout << (*myDatabase.Query("*", "User Table", whereInput)) << endl;
		}
		else if (userChoice == 2)
		{
			// Here we accept the name of a business and print all of the corresponding information
			cout << "Please enter the name of the business you would like to find:" << endl;
			string businessName;
			cin >> businessName;
			cout << "Printing the business data:";
			// Running a query after changing the userName to be the right value.
			string whereInput = "name = ";
			whereInput.append(businessName);
			cout << (*myDatabase.Query("*", "Business Table", whereInput)) << endl;
		}
		else if (userChoice == 3)
		{
			// Show the reviews for a user or a business. Because we don't have cross join functionality in Query,
			// we handle it directly
		}
		else if (userChoice == 4)
		{
			// The user wants to directly access all 5 star ratings
			cout << "Please enter the number of stars that you would like to check for:" << endl;
			string starCount;
			cin >> starCount;
			double testval = 0.0;
			try
			{
				testval = stod(starCount);
				if ((testval > 5.0) || (testval <= 0.0))
				{
					cout << "Invalid star request, please try again." << endl;
					continue;
				}
			}
			catch (invalid_argument &e)
			{
				cout << e.what() << endl;
			}
			cout << "Printing all reviews that have the same number of stars provided" << endl;
			string whereInput = "stars = ";
			whereInput = whereInput.append(starCount);
			cout << (*myDatabase.Query("*", "Review Table", whereInput)) << endl;
		}
		else if (userChoice == 10)
		{
			cout << "Thank you for using the Yelp database, please come again!" << endl;
			break;
		}
	}
	return 0;
}
