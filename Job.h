#pragma once
#include "Post.h"
class Job:public Post
{
	float ProposedPrice;
	float LowestOffer;
protected:
	static int jobID;
public:
	Job(const string& title="", const string& description="", float proposedPrice=0.0f, float low = 0.f);
	virtual string getPostDetails(void)const;
	virtual bool handleReply(MYSQL_Con*& database, const Reply& reply);
	virtual string getReplyDetails(void)const;

	//setters
	void setProposedPrice(float proposedPrice);
	void setLowestOffer(float lowestOffer);
	virtual void deletePost(void);
	//getters
	float getProposedPrice(void)const;
	float getLowestOffer(void)const;
};

