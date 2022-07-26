#include "Reply.h"

Reply::Reply(int postid, float value, const std::string& responderID)
{
	this->ResponderID = responderID;
	this->PostID = postid;
	if (value < 0) { value *= -1; }
	this->Value = value;
}
Reply& Reply::operator=(const Reply& rep)
{
	this->PostID = rep.getPostID();
	this->ResponderID = rep.getResponderID();
	this->Value = rep.getValue();
	return *this;
}
//setters
void Reply::setPostID(int postid) { this->PostID = postid; }
void Reply::setValue(float value) 
{ if (value < 0) { value *= -1; } this->Value = value; }
void Reply::setResponderID(const std::string& responderid) 
{ this->ResponderID = responderid; }

//getters
int Reply::getPostID(void)const { return this->PostID; }
float Reply::getValue(void)const { return this->Value; }
std::string Reply::getResponderID(void)const
{ return this->ResponderID; }
