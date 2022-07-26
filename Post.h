#pragma once
#include "Reply.h"
#include "CircularDoubleLinkedList.h"
#include "MYSQL_Con.h"
class Post
{
	string ID;
	string Title;
	string Description;
	string CreatorID;
	bool Status;
protected:
	CircularDoubleLinkedList<Reply*> reply;
	Post(const string& title = "", const string& description = "");
public:
	~Post(void);
	virtual string getPostDetails(void)const;
	virtual bool handleReply(MYSQL_Con*& database, const Reply& reply) = 0;
	virtual string getReplyDetails(void)const = 0;
	virtual void deletePost(void) = 0;
	string offerHistory(int flag)const; //for job/sale classes

	//setters
	void setCreatorID(const string& id);
	void setID(const string& id);
	void setTitle(const string& title);
	void setDescription(const string& description);
	void setStatus(bool status);
	//getters
	string getCreatorID(void)const;
	string getpostID(void)const;
	string getTitle(void)const;
	string getDescription(void)const;
	bool getStatus(void)const;
};

