#include "Event.h"

int Event::eveID = 1;
Event::Event(const string& title, const string& description, const string& date,
	const string& venue, int capacity, int attendee)
	:Post(title, description), Venue(venue), Date(date)
{
	this->AttendeeCount = attendee;
	if (eveID < 10) { this->setID(("EVE00" + to_string(eveID))); }
	else if (eveID < 100) { this->setID(("EVE0" + to_string(eveID))); }
	else{ this->setID(("EVE" + to_string(eveID))); }
	if (capacity < 0) { capacity *= -1; }
	this->Capacity = capacity;
	eveID++;
}
string Event::getPostDetails(void)const
{
	string postDetails = Post::getPostDetails();
	return (postDetails + "\nVenue: " + this->Venue + "\nDate: " + this->Date + "\nCapacity: " +
		to_string(this->Capacity) + "\nAttendee: " + to_string(this->AttendeeCount) + "\n");
}
bool Event::handleReply(MYSQL_Con*& database, const Reply& reply)
{
	if (this->getStatus() && reply.getValue() == 1)
	{
		for (int i = 0; i < this->reply.size(); i++)
		{
			if (this->reply[i]->getResponderID() == reply.getResponderID())
				return false;
		}
		this->reply.insertAtEnd(new Reply(reply.getPostID(), reply.getValue(),
			reply.getResponderID()));
		if(database)
			this->AttendeeCount++;
		if (this->AttendeeCount == this->Capacity)
		{ 
			this->setStatus(false); 
			if (database)
				database->select("UPDATE `post` set Status = 'Close' where PostID = '" + this->getpostID() + "';");
		}
		if (database)
		{
			database->insert("INSERT INTO `reply` values ('" + this->getpostID() + "','" + reply.getResponderID() + "'," + to_string(reply.getValue()) + ");");
			database->select("UPDATE `eventpost` set Attendee = " + to_string(this->AttendeeCount) + " where PostID = '" + this->getpostID() + "';");
		}
		return true;
	}
	else { return false; }
}
string Event::getReplyDetails(void) const
{
	string list = "";
	if (this->AttendeeCount == 0) { list = "Empty!"; }
	else
	{
		int s = this->reply.size();
		for (int i = 0; i < s; i++)
		{
			list += this->reply[i]->getResponderID();
			if (i != s - 1) { list += ", "; }
		}
	}
	list = this->getPostDetails() + "Attendee List: " + list + "\n";
	return list;
}

//setters
void Event::setVenue(const string& venue) { this->Venue = venue; }
void Event::setDate(const string& date) { this->Date = date; }
void Event::setCapacity(int capacity) 
{ if (capacity < 0) { capacity *= -1; } this->Capacity = capacity; }
void Event::setAttendeeCount(int attendeeCount) 
{ if (attendeeCount < 0) { attendeeCount *= -1; }this->AttendeeCount = attendeeCount; }

void Event::deletePost(void) { eveID--; }

//getters
string Event::getVenue(void)const { return this->Venue; }
string Event::getDate(void)const { return this->Date; }
int Event::getCapacity(void)const { return this->Capacity; }
int Event::getAttendeeCount(void)const { return this->AttendeeCount; }