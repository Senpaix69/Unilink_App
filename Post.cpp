#include "Post.h"
Post::Post(const string& title, const string& description)
{
	this->ID = "";
	this->CreatorID = "";
	this->Title = title;
	this->Description = description;
	this->Status = true;
}
Post::~Post()
{
	if(!this->reply.isEmpty())
		this->reply.~CircularDoubleLinkedList();
}
string Post::getPostDetails(void)const
{
	string status = "";
	if (Status == true) { status = "Open!"; }
	else { status = "Closed!"; }
	return ("Post ID: "+this->ID+ "\nPost Title: "+this->Title + 
		"\nDescription: "+this->Description+"\nCreator ID: "+this->CreatorID+
		"\nStatus: "+ status);
}

string Post::offerHistory(int flag) const
{
	//saving in string
	string offerHistory = "", dollar = "$";
	int s = this->reply.size();
	for (int i = 0; i < s; i++)
	{
		string resID = (this->reply[i]->getResponderID() + ": ");
		offerHistory += ("\n" + resID + dollar + to_string(this->reply[i]->getValue()));
	}
	return offerHistory;
}

//setters
void Post::setCreatorID(const string& id) { this->CreatorID = id; }
void Post::setID(const string& id) { this->ID = id; }
void Post::setTitle(const string& title) { this->Title = title; }
void Post::setDescription(const string& description)
{ this->Description = description; }
void Post::setStatus(bool status) { this->Status = status; }
//getters
string Post::getCreatorID(void) const { return this->CreatorID; }
string Post::getpostID(void)const { return this->ID; }
string Post::getTitle(void)const { return this->Title; }
string Post::getDescription(void)const { return this->Description; }
bool Post::getStatus(void)const { return this->Status; }