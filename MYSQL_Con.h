#pragma once
#include <iostream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace std;

class MYSQL_Con
{
	const string SERVER;
	const string USERNAME;
	const string PASSWORD;
protected:
	sql::Driver* driver = nullptr;
	sql::Connection* con = nullptr;
	sql::Statement* stmt = nullptr;
	sql::PreparedStatement* pstmt = nullptr;
	sql::ResultSet* result = nullptr;
public:
	MYSQL_Con(const string& server, const string& username, const string& password);
	~MYSQL_Con();

	bool connect();
	void setSchema(const string& schema);
	bool insert(const string& statement);
	bool select(const string& statement);
	sql::ResultSet* getResult();
};

