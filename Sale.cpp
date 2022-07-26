#include "Sale.h"
int Sale::saleID = 1;
Sale::Sale(const string& title, const string& description, float askingPrice, float minimumRaise, float highestO)
	:Post(title, description)
{
	if (highestO > 0)
		this->HighestOffer = highestO;
	else
		this->HighestOffer = minimumRaise;
	if (saleID < 10) { this->setID(("SAL00" + to_string(saleID))); }
	else if (saleID < 100) { this->setID(("SAL0" + to_string(saleID))); }
	else { this->setID(("SAL" + to_string(saleID))); }
	
	if (askingPrice < 0) { askingPrice *= -1; }
	this->AskingPrice = askingPrice;
	if (minimumRaise < 0) { minimumRaise *= -1; }
	this->MinimumRaise = minimumRaise;
	saleID++;
}

string Sale::getPostDetails(void) const
{
	string postDetails = Post::getPostDetails(), temp = "";
	if (this->HighestOffer != 0) { temp = "$" + to_string(this->HighestOffer); }
	else { temp = "No Offer"; }
	return (postDetails + "\nMinimum Raise: $" + to_string(this->MinimumRaise) + "\nHighest Offer: " + temp + "\n");
}
bool Sale::handleReply(MYSQL_Con*& database, const Reply& reply)
{
	if (this->getStatus() && reply.getValue() > this->HighestOffer)
	{
		for (int i = 0; i < this->reply.size(); i++)
		{
			if (this->reply[i]->getResponderID() == reply.getResponderID())
				return false;
		}
		this->HighestOffer = reply.getValue();
		if(database)
			database->select("UPDATE `salepost` set highesOffer = " + to_string(this->HighestOffer) + " where PostID = '" + this->getpostID() + "';");
		this->reply.insertAtEnd(new Reply(reply.getPostID(), reply.getValue(),
			reply.getResponderID()));
		if(database)
			database->insert("INSERT INTO `reply` values ('" + this->getpostID() + "','" + reply.getResponderID() + "'," + to_string(reply.getValue()) + ");");
		if (this->HighestOffer >= this->AskingPrice)
		{
			this->setStatus(false);
			if (database)
				database->select("UPDATE `post` set Status = 'Close' where PostID = '" + this->getpostID() + "';");
		}
		return true;
	}
	else { return false; }
}
string Sale::getReplyDetails(void) const
{
	string Temp = this->getPostDetails();
	string offerHistory = this->offerHistory(1); //parameter 1 is for descending order
	if (offerHistory == "") { offerHistory = "\nNo offer Yet"; }
	return (Temp + "-- Offer History --" + offerHistory + "\n");
}

//setters
void Sale::setAskingPrice(float askingPrice) 
{ if (askingPrice < 0) { askingPrice *= -1; } this->AskingPrice = askingPrice; }
void Sale::setHighestOffet(float highestOffer) 
{ if (highestOffer < 0) { highestOffer *= -1; } this->HighestOffer = highestOffer; }
void Sale::setMinimumRaise(float minimumRaise) 
{ if (minimumRaise < 0) { minimumRaise *= -1; } this->MinimumRaise = minimumRaise; }
void Sale::deletePost(void) { saleID--; }
//getters
float Sale::getAskingPrice(void)const { return this->AskingPrice; }
float Sale::setHighestOffet(void)const { return this->HighestOffer; }
float Sale::getMinimumRaise(void)const { return this->MinimumRaise; }
