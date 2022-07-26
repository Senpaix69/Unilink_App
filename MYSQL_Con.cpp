#include "MYSQL_Con.h"

MYSQL_Con::MYSQL_Con(const string& server, const string& username, const string& password)
	:SERVER(server), USERNAME(username), PASSWORD(password)
{
}

MYSQL_Con::~MYSQL_Con()
{
	if (this->con)
	{
		if(!this->con->isClosed())
			this->con->close();
		delete this->con;
	}
	if (this->stmt)
		delete this->stmt;
	if (this->pstmt)
		delete this->pstmt;
	if (this->result)
		delete this->result;
}

bool MYSQL_Con::connect()
{
	try {
		this->driver = get_driver_instance();
		this->con = this->driver->connect(this->SERVER, this->USERNAME, this->PASSWORD);
		return true;
	}
	catch (sql::SQLException e) {
		cout << "Could not connect to server. Error message: " << e.what() << endl;
		return false;
	}
}

void MYSQL_Con::setSchema(const string& schema)
{
	this->con->setSchema(schema);
}

bool MYSQL_Con::insert(const string& statement)
{
	try {
		this->pstmt = this->con->prepareStatement(statement);
		this->pstmt->execute();
		return true;
	}
	catch (sql::SQLException e) {
		cout << e.what() << endl;
		return false;
	}
}

bool MYSQL_Con::select(const string& statement)
{
	try {
		this->pstmt = this->con->prepareStatement(statement);
		this->result = this->pstmt->executeQuery();
		return true;
	}
	catch (sql::SQLException e) {
		cout << e.what() << endl;
		return false;
	}
}

sql::ResultSet* MYSQL_Con::getResult()
{
	return this->result;
}
