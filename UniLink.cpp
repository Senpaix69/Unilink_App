#include "UniLink.h"
#include <Windows.h>
int UniLink::width = 53;

UniLink::UniLink(const string& server, const string& name, const string& pass)
{
	// connectiong to database
	this->database = new MYSQL_Con(server, name, pass);
	this->sizePost = 0;
}
UniLink::~UniLink()
{
	if (this->database)
		delete database;
	this->clear();
}
void UniLink::read()
{
	string postID = "", title = "", desc = "", creatorID = "", status = "";
	sql::ResultSet* R = nullptr;
	int count = 1;
	while (1)
	{
		if (count == 1)
		{
			if (!this->database->select("SELECT * FROM `post` as P inner join `eventpost` as E on E.PostID = P.PostID;"))
				count = 2;
		}
		if (count == 2)
		{
			if (!this->database->select("SELECT * FROM `post` as P inner join `salepost` as S on S.postID = P.PostID;"))
				count = 3;
		}
		if (count == 3)
		{
			if (!this->database->select("SELECT * FROM `post` as P inner join `jobpost` as J on J.postID = P.PostID;"))
				break;
		}
		R = this->database->getResult();
		while (R && R->next())
		{
			postID = R->getString(1);
			title = R->getString(2);
			desc = R->getString(3);
			creatorID = R->getString(4);
			status = R->getString(5);

			if (count == 1)
			{
				string venue = "", date = "", attendee = "", cap = "";
				venue = R->getString(7);
				date = R->getString(8);
				attendee = R->getString(9);
				cap = R->getString(10);
				this->post.insertAtEnd(new Event(title, desc, venue, date, stoi(cap), stoi(attendee)));
				this->post[this->sizePost]->setCreatorID(creatorID);
				this->post[this->sizePost]->setStatus(status == "Open" ? true : false);
				this->sizePost++;
			}
			if (count == 2)
			{
				string askingP = "", minimumR = "", highestO = "";
				askingP = R->getString(7);
				highestO = R->getString(8);
				minimumR = R->getString(9);
				this->post.insertAtEnd(new Sale(title, desc, stof(askingP), stof(minimumR), stof(highestO)));
				this->post[this->sizePost]->setCreatorID(creatorID);
				this->post[this->sizePost]->setStatus(status == "Open" ? true : false);
				this->sizePost++;
			}
			if (count == 3)
			{
				string proposedOffer = "", lowestOffer = "";
				proposedOffer = R->getString(7);
				lowestOffer = R->getString(8);
				this->post.insertAtEnd(new Job(title, desc, stof(proposedOffer), stof(lowestOffer)));
				this->post[this->sizePost]->setCreatorID(creatorID);
				this->post[this->sizePost]->setStatus(status == "Open" ? true : false);
				this->sizePost++;
			}
		}
		count++;
		if (count == 4)
			break;
	}
	this->database->select("SELECT * FROM `reply`;");
	R = this->database->getResult();
	MYSQL_Con* S = nullptr;
	while (R && R->next())
	{
		string postID = R->getString(1);
		for (int i = 0; i < this->sizePost; i++)
		{
			if (this->post[i]->getpostID() == postID)
			{
				this->post[i]->handleReply(S, Reply(stoi(postID.substr(3, 6)), stof(R->getString(3)), R->getString(2)));
				break;
			}
		}
	}
}
void UniLink::run()
{
	if (this->database->connect())
	{
		this->database->setSchema("unilink");
		int input = 0;
		bool flag = false;
		string ID = "", password = "";
		while (1)
		{
			system("CLS");
		a:
			input = this->menu();
			if (input == 1) // login
			{
				this->centerStringln("Enter username or email: ", this->width + 1); cin >> ID;
				this->centerStringln("Enter password: ", this->width + 1); cin >> password;
				if (!this->login(ID, password))
				{
					this->centerString("Error: Wrong Password or Email", this->width + 1);
					this->wait(3);
				}
				else
				{
					if (!flag)
					{
						this->read();
						flag = true;
					}
					while (1)
					{
						system("CLS");
					b:
						input = this->studentMenu(ID);
						if (input == 1)
						{
							this->createPost(ID, 1);
							goto b;
						}
						else if (input == 2)
						{
							this->createPost(ID, 2);
							goto b;
						}
						else if (input == 3)
						{
							this->createPost(ID, 3);
							goto b;
						}
						else if (input == 4)
						{
							this->replyPost(ID);
							goto b;
						}
						else if (input == 5)
						{
							this->displayPosts(ID, 1); //1 parameter is for Creator posts only
							goto b;
						}
						else if (input == 6)
						{
							this->displayPosts(ID, 0); //0 parameter is for All posts
							goto b;
						}
						else if (input == 7)
						{
							this->closePost(ID, 1); //1 parameter to close post
							goto b;
						}
						else if (input == 8)
						{
							this->closePost(ID, 2); //2 parameter to delete post
							goto b;
						}
						else if (input == 9)
						{
							if(this->deleteProfile(ID))
								flag = false;
							this->wait(3);
							break;
						}
						else if (input == 10) { break; }
						else { this->centerString("  Invalid Input!  ", 66); goto b; }
					}
				}
			}
			else if (input == 2) // registration
			{
			reg:
				this->centerStringln("Enter username or email: ", this->width + 1); cin >> ID;
				this->centerStringln("Enter password: ", this->width + 1); cin >> password; goto check;
			check:
				if (this->registration(ID, password))
					this->centerString("Registered Successfully! ", this->width + 1);
				else
				{
					this->centerString("Error: Username already exists! ", this->width + 1);
					goto reg;
				}
				this->wait(3);
			}
			else if (input == 3)
			{
				system("CLS");
				this->centerString("System Exited! Thanks for using UniLink System", 48);
				break;
			}
			else { this->centerString("  Invalid Input!  ", 66); goto a; }
		}
	}
}

bool UniLink::login(const string& ID, const string& Pass)
{
	if (this->database->select("SELECT * FROM `admin` where username='" + ID + "' AND password='" + Pass + "';"))
	{
		sql::ResultSet* result = this->database->getResult();
		if (result->next())
		{
			string id = result->getString(1), pass = result->getString(2);
			if (id == ID && Pass == pass)
				return true;
		}
	}
	return false;
}

bool UniLink::registration(const string& ID, const string& Pass)
{
	if (this->database->insert("INSERT INTO `admin` values('" + ID + "','" + Pass + "');"))
		return true;
	return false;
}

void UniLink::clear()
{
	while (!this->post.isEmpty())
		this->post.deleteFromFront();
	this->sizePost = 0;
}

bool UniLink::deleteProfile(const string& ID)
{
	char in = {};
	this->centerStringln("Do you really want to delete? y/n : ", width + 1);
	cin >> in;
	if (in == 'y' || in == 'Y')
	{
		this->database->select("DELETE FROM `admin` where username = '" + ID + "';");
		this->database->select("DELETE FROM `eventpost` where PostID in (SELECT PostID from `post` where AdminID='" + ID + "');");
		this->database->select("DELETE FROM `salepost` where postID in (SELECT PostID from `post` where AdminID='" + ID + "');");
		this->database->select("DELETE FROM `jobpost` where postID in (SELECT PostID from `post` where AdminID='" + ID + "');");
		if (this->database->select("SELECT postID from `reply` where responderID='" + ID + "';"))
		{
			sql::ResultSet* r = this->database->getResult();
			r->next();
			if (r->getString(1).substr(0, 3) == "EVE")
			{
				this->database->select("SELECT Attendee, postID from `eventpost` where postID ='" + r->getString(1) + "';");
				r = this->database->getResult();
				r->next();
				int Attendee = stoi(r->getString(1));
				this->database->select("UPDATE `eventpost` set Attendee =" + to_string(--Attendee) + " where postID ='" + r->getString(2) + "';");
			}
		}
		this->database->select("DELETE FROM `reply` where responderID='" + ID + "';");
		this->database->select("DELETE FROM `post` where AdminID = '" + ID + "';");
		this->centerString("Profile Deleted Successfully", width + 1);
		this->clear();
		return true;
	}
	this->centerString("Profile Deletion Declined", width + 1);
	return false;
}

void UniLink::wait(int timer)
{
	this->centerStringln("Please Wait!   ", width + 1);
	for (int i = timer; i > 0; i--)
	{
		if (i == timer)
			this->centerStringln(to_string(i).c_str());
		else
		{
			for (int i = 0; i < 2; i++)
			{
				Sleep(500);
				this->centerStringln(".");
			}
			Sleep(500);
			this->centerStringln((to_string(i)).c_str());
		}
	}
}

void UniLink::createPost(const string& creatorID, int flag)
{
	system("CLS");
	if (flag == 1) { this->displayPostBanner(1); }
	else if (flag == 2) { this->displayPostBanner(2); }
	else { this->displayPostBanner(3); }
	string title = "", description = "";
	cin.ignore();
a:
	this->centerStringln("Enter Title: ", width + 1); getline(cin, title);
	if (title == "") { this->centerString("Note: Empty Title/Description are not allowed", width + 1); goto a; }
b:
	this->centerStringln("Enter Description: ", width + 1); getline(cin, description);
	if (description == "") { this->centerString("Note: Empty Title/Description are not allowed", width + 1); goto b; }
	string postid = "";
	if (flag == 1) //for event post creation
	{
		string venue = "", date = "";
		int capacity = 0;
		this->centerStringln("Enter Venue of Event: ", width + 1); getline(cin, venue);
		this->centerStringln("Enter Date for Event (0000-00-00): ", width + 1); getline(cin, date);
		this->centerStringln("Enter Capacity for Attendees: ", width + 1); cin >> capacity;

		this->post.insertAtEnd(new Event(title, description, venue, date, capacity));
		this->sizePost++;
		this->centerString("|----------------------------------------------------|", width);
		this->centerStringln("Your event has been created with ID:", width + 5);
		postid = this->post[this->sizePost - 1]->getpostID();
		this->centerString(postid.c_str(), 1);
		this->database->insert("INSERT INTO `eventpost` values('" + postid + "','" + venue + "','" +
			date + "',0," + to_string(capacity) + ");");
	}
	else if (flag == 2) //for sale post creation
	{
		float askingPrice = 0.0f, minimumRaise = 0.0f;
		this->centerStringln("Enter Asking_Price: ", width + 1); cin >> askingPrice;
		this->centerStringln("Enter Minimum_Raise: ", width + 1); cin >> minimumRaise;

		this->post.insertAtEnd(new Sale(title, description, askingPrice, minimumRaise));
		this->sizePost++;
		this->centerString("|----------------------------------------------------|", width);
		this->centerStringln("Your sale has been created with ID:", width + 5);
		postid = this->post[this->sizePost - 1]->getpostID();
		this->centerString(postid.c_str(), 1);
		this->database->insert("INSERT INTO `salepost` values('" + postid + "'," + to_string(askingPrice) + "," +
			to_string(minimumRaise) + "," + to_string(minimumRaise) + ");");
	}
	else
	{
		float ProposedPrice = 0.0f;
		this->centerStringln("Enter Proposed Price: ", width + 1); cin >> ProposedPrice;

		this->post.insertAtEnd(new Job(title, description, ProposedPrice));
		this->sizePost++;
		this->centerString("|----------------------------------------------------|", width);
		this->centerStringln("Your job has been created with ID:", width + 5);
		postid = this->post[this->sizePost - 1]->getpostID();
		this->centerString(postid.c_str(), 1);
		this->database->insert("INSERT INTO `jobpost` values('" + postid + "'," + to_string(ProposedPrice) + "," +
			to_string(ProposedPrice) + ");");
	}
	this->post[this->sizePost - 1]->setCreatorID(creatorID);
	this->database->insert("INSERT INTO `post` values('" + postid + "','" + title + "','" + description + "','" +
		creatorID + "','Open');");
}
void UniLink::closePost(const string& creatorID, int flag)
{
	system("CLS");
	if (flag == 1) { this->displayPostBanner(6); }
	else { this->displayPostBanner(7); }
	if (this->sizePost != 0)
	{
		string postID = "";
		this->centerStringln("Enter Post_ID: ", width + 1); cin >> postID;

		char in = {}; bool isFound = false; int tempflag = 0;
		for (int i = 0; i < this->sizePost; i++)
		{
			if (postID == this->post[i]->getpostID())
			{
				this->displayPostBanner(8);
				this->displayPost(postID);
				if (this->post[i]->getCreatorID() != creatorID)
				{
					if (flag == 1)
						this->centerString("|         You can't close someone else post!         |", width);
					else
						this->centerString("|        You can't delete someone else post!         |", width);
					return;
				}
				if (flag == 1)
					this->centerStringln("Do you want to close? (Press Y : Confirm) : ", width + 5);
				else { this->centerStringln("Do you want to delete? (Press Y : Confirm) : ", width + 5); }
				cin >> in;
				if ((in == 'y' || in == 'Y') && flag == 1)
				{
					this->post[i]->setStatus(false);
					this->database->select("UPDATE `post` set Status = 'Close' where PostID = '" + postID + "';");
					isFound = true;
					break;
				}
				else if ((in == 'y' || in == 'Y') && flag == 2)
				{
					this->post.deleteFromPosition(i);
					this->database->select("DELETE FROM `eventpost` where PostID ='" + postID + "';");
					this->database->select("DELETE FROM `salepost` where postID ='" + postID + "';");
					this->database->select("DELETE FROM `jobpost` where postID ='" + postID + "';");
					this->database->select("DELETE FROM `reply` where postID ='" + postID + "';");
					this->database->select("DELETE FROM `post` where PostID = '" + postID + "';");
					this->sizePost--;
					isFound = true;
					break;
				}
				else
				{
					this->centerString("|                     Declined!                      |", width);
					tempflag = 1;
				}
			}
		}
		if (isFound && flag == 1) { this->centerString("|             Post Closed Successfully!              |", width); }
		else if (isFound && flag == 2) { this->centerString("|             Post Deleted Successfully!             |", width); }
		else if (tempflag == 0) { this->centerString("|                    Post not found!                 |", width); }
	}
	else { this->centerString("|            There's no Post created Yet!            |", width); }
}
bool UniLink::replyPost(const string& creatorID)
{
	system("CLS");
	this->displayPostBanner(4);
	string ID = "";
	int flag = 0;
	if (this->sizePost != 0) //if theres some posts
	{
		this->centerStringln("Enter Post_ID: ", width + 16); cin >> ID;
		bool isFound = false;
		for (int i = 0; i < this->sizePost; i++)
		{
			if (this->post[i]->getpostID() == ID)
			{
				if (this->post[i]->getCreatorID() == creatorID)
				{
					isFound = true;
					flag = 2;
				}
				if (flag != 2)
				{
					this->displayPostBanner(8);
					this->displayPost(ID);
					float value = 0;
					int postID = 0;
					string IDnum = ID.substr(3, 6), IDtype = ID.substr(0, 3);
					if (this->post[i]->getStatus())
					{
						if (IDtype == "EVE")
						{
							this->centerStringln("Enter 1 if you want to join: ", width + 10); cin >> value;
						}
						else
						{
							this->centerStringln("Enter Your Offer: ", width + 1); cin >> value;
						}
						postID = stoi(IDnum);
						Reply R(postID, value, creatorID);
						bool isAccept = this->post[i]->handleReply(this->database, R);
						if (isAccept)
						{
							this->centerString("|               Reply Added Successfully!            |", width);
						}
						else
						{
							this->centerString("|                      Declined!                     |", width);
						}
						isFound = true;
					}
					else { this->centerString("|                  The Post is closed!               |", width); flag = 1; }
				}
			}
		}
		if (isFound && flag == 2)
		{
			this->displayPostBanner(8);
			this->displayPost(ID);
			this->centerString("|          You can't reply to your own post!         |", width);
		}
		if (!isFound && flag == 0) { this->centerString("|                    Post Not Found!                 |", width); }
	}
	else { this->centerString("|            There's no Post created Yet!            |", width); }
	return 0;
}
void UniLink::displayPosts(const string& CreatorID, int flag) const
{
	system("CLS");
	if (flag == 0) { this->displayPostBanner(5); }
	else { this->displayPostBanner(9); }
	int myPost = 0, allPost = 0;
	if (this->sizePost != 0)
	{
		for (int i = 0; i < this->sizePost; i++)
		{
			if (CreatorID == this->post[i]->getCreatorID() && flag == 1 || flag == 0)
			{
				string temp = "", line = "";
				if (flag == 1)
				{
					temp = this->post[i]->getReplyDetails(); //get reply details with post details
					myPost = 1;
				}
				else { temp = this->post[i]->getPostDetails(); allPost = 1; } //get only post details

				//printing with centered string
				int start = 0, len = 0;
				for (int j = 0; temp[j] != '\0'; j++)
				{
					len++;
					if (temp[j] == '\n')
					{
						line = temp.substr(start, len - 1);
						start = j + 1;
						len = 0;
						this->centerString(line.c_str(), width + 1);
					}
				}
				this->centerString("|----------------------------------------------------|", width);
			}
		}
	}
	if((myPost == 0 && allPost == 0) || this->sizePost == 0)
		this->centerString("|            There's no Post created Yet!            |", width);
}
void UniLink::displayPost(const string& postID) const
{
	for (int i = 0; i < this->sizePost; i++)
	{
		if (postID == this->post[i]->getpostID())
		{
			string temp = "", line = "";
			temp = this->post[i]->getPostDetails(); //get only post details

			//printing with centered string
			int start = 0, len = 0;
			for (int j = 0; temp[j] != '\0'; j++)
			{
				len++;
				if (temp[j] == '\n')
				{
					line = temp.substr(start, len - 1);
					start = j + 1;
					len = 0;
					this->centerString(line.c_str(), width + 1);
				}
			}
			this->centerString("|----------------------------------------------------|", width);
		}
	}
}


//menus
int UniLink::menu()
{
	int input = 0;
	this->centerString(" ____________________________________________________ ", width);
	this->centerString("|****************** UniLink System ******************|", width);
	this->centerString("| 1. Login                                           |", width);
	this->centerString("| 2. Registration                                    |", width);
	this->centerString("| 3. Quit                                            |", width);
	this->centerString("|____________________________________________________|", width);
	this->centerStringln("Enter Choice: ", width + 1);
	cin >> input;
	return input;
}
int UniLink::studentMenu(const string& id)
{
	//student greeting reference
	string line = " ____________________________ ";
	this->centerString(line.c_str(), width + 12);
	int lenLine = unsigned int(strlen(line.c_str()));
	string greetings = "Welcome " + id;
	int len = unsigned int(strlen(greetings.c_str()));
	this->centerStringln("| ", width + 12);
	int startline = (lenLine / 2) - (len / 2) - 2, endline = startline - 1;
	this->centerStringln(greetings.c_str(), startline);
	this->centerStringln(" |", endline);
	cout << endl;

	//student menu reference
	int input = 0;
	this->centerString("|____________________________________________________|", width);
	this->centerString("|******************* Student Menu *******************|", width);
	this->centerString("| 1. New Event Post                                  |", width);
	this->centerString("| 2. New Sale Post                                   |", width);
	this->centerString("| 3. New Job Post                                    |", width);
	this->centerString("| 4. Reply to Post                                   |", width);
	this->centerString("| 5. Display My Posts                                |", width);
	this->centerString("| 6. Display All Post                                |", width);
	this->centerString("| 7. Close Post                                      |", width);
	this->centerString("| 8. Delete Post                                     |", width);
	this->centerString("| 9. Delete Profile                                  |", width);
	this->centerString("|10. Log-Out                                         |", width);
	this->centerString("|____________________________________________________|", width);
	this->centerStringln("Enter Choice: ", width + 1);
	cin >> input;
	return input;
}

//decorator methods
void UniLink::centerString(const char* s, int wid) const
{
	if (wid == 0) { wid = 80; }
	for (int i = 0; i < wid; i++) { cout << " "; }
	cout << s << endl;
}
void UniLink::centerStringln(const char* s, int wid) const
{
	for (int i = 0; i < wid; i++) { cout << " "; }
	cout << s;
}

void UniLink::displayPostBanner(int flag) const
{
	if (flag == 1)
	{
		this->centerString(" ____________________________________________________ ", width);
		this->centerString("|                  ** EVENT POST **                  |", width);
		this->centerString("|____________________________________________________|", width);
	}
	else if (flag == 2)
	{
		this->centerString(" ____________________________________________________ ", width);
		this->centerString("|                  ** SALE POST **                   |", width);
		this->centerString("|____________________________________________________|", width);
	}
	else if (flag == 3)
	{
		this->centerString(" ____________________________________________________ ", width);
		this->centerString("|                   ** JOB POST **                   |", width);
		this->centerString("|____________________________________________________|", width);
	}
	else if (flag == 4)
	{
		this->centerString(" ____________________________________________________ ", width);
		this->centerString("|               ** Reply To A Post **                |", width);
		this->centerString("|____________________________________________________|", width);

	}
	else if (flag == 5)
	{
		this->centerString(" ____________________________________________________ ", width);
		this->centerString("|                   ** ALL POSTS **                  |", width);
		this->centerString("|----------------------------------------------------|", width);
	}
	else if (flag == 6)
	{
		this->centerString(" ____________________________________________________ ", width);
		this->centerString("|                  ** Close POST **                  |", width);
		this->centerString("|----------------------------------------------------|", width);
	}
	else if (flag == 7)
	{
		this->centerString(" ____________________________________________________ ", width);
		this->centerString("|                  ** Delete POST **                 |", width);
		this->centerString("|----------------------------------------------------|", width);
	}
	else if (flag == 8)
	{
		this->centerString("|                     Post Found                     |", width);
		this->centerString("|----------------------------------------------------|", width);
	}
	else if (flag == 9)
	{
		this->centerString(" ____________________________________________________ ", width);
		this->centerString("|                    ** MY POSTS **                  |", width);
		this->centerString("|----------------------------------------------------|", width);
	}
}