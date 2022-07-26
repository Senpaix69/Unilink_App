#pragma once
#include "Post.h"
class Event:public Post
{
	string Venue;
	string Date;
	int Capacity;
	int AttendeeCount;
protected:
	static int eveID;
public:
	Event(const string& title = "", const string& description = "", const string& venue = "",
		const string& date = "", int capacity = 0, int attendee = 0);
	virtual string getPostDetails(void)const;
	virtual bool handleReply(MYSQL_Con*& database, const Reply& reply);
	virtual string getReplyDetails(void)const;

	//setters
	void setVenue(const string& venue);
	void setDate(const string& date);
	void setCapacity(int capacity);
	void setAttendeeCount(int attendeeCount);
	virtual void deletePost(void);
	//getters
	string getVenue(void)const;
	string getDate(void)const;
	int getCapacity(void)const;
	int getAttendeeCount(void)const;
};

