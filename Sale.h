#pragma once
#include "Post.h"
class Sale:public Post
{
	float AskingPrice;
	float HighestOffer;
	float MinimumRaise;
protected:
	static int saleID;
public:
	Sale(const string& title = "", const string& description = "", float askingPrice = 0.0f,
		float minimumRaise = 0.0f, float highestO = 0.f);
	virtual string getPostDetails(void)const;
	virtual bool handleReply(MYSQL_Con*& database, const Reply& reply);
	virtual string getReplyDetails(void)const;

	//setters
	void setAskingPrice(float askingPrice);
	void setHighestOffet(float highestOffer);
	void setMinimumRaise(float minimumRaise);
	virtual void deletePost(void);
	//getters
	float getAskingPrice(void)const;
	float setHighestOffet(void)const;
	float getMinimumRaise(void)const;

};

