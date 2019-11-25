#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include "pch.h"

class Database
{
public:
	Database() {};
	~Database() {};
	Database(std::string name) : databaseName(name) {};

	// Database Functions
	void createTable(std::string tableName, Table t);
	void drop(std::string tableName); // Deletes a table from the database, given its name 
	void save(std::string filename);
	void load(std::string filename);

	// Getters and Setters
	std::string getDatabaseName() const { return databaseName; } // Returns database name 
	Table getTable(std::string tableName) const; // Returns a table via table name
	std::vector<Table> getTables() const; // Returns list of all tables
	std::vector<std::string> getTableNames() const; // Returns list of all table names
	void setDatabaseName(std::string aDatabaseName) { databaseName = aDatabaseName; } // Sets database name 

	// Query function 
	Table* Query(std::string selectInput, std::string fromInput, std::string whereInput);
	std::vector<std::string> parseSelectCommand(std::string s); // Helper function!

	// Extra credit!
	void merge(Database d); // Merges 2 databases
	Database(const Database &obj);  // Copy constructor

private:
	std::string databaseName;
	std::map<std::string, Table> database; // Table name, Table object 
	std::string simpleExpression(std::string simpleExpr);
	bool booleanEvaluater(std::string boolExpression);

	// Print Database (for debugging)
	friend std::ostream&operator<<(std::ostream&os, const Database& d)
	{
		os << d.getDatabaseName() << std::endl << std::endl;
		for (auto elem : d.database)
		{
			os << elem.second << std::endl;
		}
		return os;
	}
};

#endif 