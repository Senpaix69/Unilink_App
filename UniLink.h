#pragma once
#include "Post.h"
#include "Event.h"
#include "Reply.h"
#include "Sale.h"
#include "Job.h"
class UniLink
{
	MYSQL_Con* database;
	CircularDoubleLinkedList<Post*> post;
	int sizePost;
public:
	UniLink(const string& server, const string& name, const string& pass);
	~UniLink();
	void read();
	void run();
	bool login(const string& ID, const string& Pass);
	bool registration(const string& ID, const string& Pass);
	void clear();
	bool deleteProfile(const string& ID);
	void wait(int timer);
	
	//menu
	int menu(void);
	int studentMenu(const string& id);

	//func for programmer ease
	void displayPosts(const string& CreatorID, int flag = 0)const; // 0- all posts and 1- My posts
	void displayPost(const string& postID)const; 
	void createPost(const string& creatorID, int flag = 0); //1- Event Post , 2- Sale Post, 3- Job Post
	void closePost(const string& creatorID, int flag = 0); //using this func to close or delete the post
	bool replyPost(const string& creatorID); // ease Reply with all rules posiible inputs

	//for decorating center strings
	static int width; //to set the width of all console outputs
	void centerString(const char* s, int wid = 0)const; //to print the string with line break
	void centerStringln(const char* s, int wid = 0)const; //to print string without line break
	void displayPostBanner(int flag)const; //to display all post banners with respect to flag input
};