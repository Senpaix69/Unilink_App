#pragma once
#include <string>
class Reply
{
	int PostID;
	float Value;
	std::string ResponderID;
public:
	Reply(int postid = 0, float value = 0, const std::string& responderID = "");
	Reply& operator=(const Reply& rep);

	//setters
	void setPostID(int postid);
	void setValue(float value);
	void setResponderID(const std::string& responderid);
	//getters
	int getPostID(void)const;
	float getValue(void)const;
	std::string getResponderID(void)const;
};