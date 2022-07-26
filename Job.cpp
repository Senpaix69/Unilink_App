#include "Job.h"
int Job::jobID = 1;
Job::Job(const string& title, const string& description, float proposedPrice, float low)
	:Post(title, description)
{
	if (jobID < 10) { this->setID(("JOB00" + to_string(jobID))); }
	else if (jobID < 100) { this->setID(("JOB0" + to_string(jobID))); }
	else { this->setID(("JOB" + to_string(jobID))); }
	jobID++;
	if (proposedPrice < 0) { proposedPrice *= -1; }
	this->ProposedPrice = proposedPrice;
	if (low > 0)
		this->LowestOffer = low;
	else
		this->LowestOffer = this->ProposedPrice;
}
string Job::getPostDetails(void) const
{
	string postDetails = Post::getPostDetails();
	return (postDetails + "\nProposed Price: $" + to_string(this->ProposedPrice) + "\nLowest Offer: $" +
		to_string(this->LowestOffer) + "\n");
}
bool Job::handleReply(MYSQL_Con*& database, const Reply& reply)
{
	if (this->getStatus() && reply.getValue() <= this->LowestOffer)
	{
		for (int i = 0; i < this->reply.size(); i++)
		{
			if (this->reply[i]->getResponderID() == reply.getResponderID())
				return false;
		}
		this->reply.insertAtEnd(new Reply(reply.getPostID(), reply.getValue(),
			reply.getResponderID()));
		this->LowestOffer = reply.getValue();
		if (database)
		{
			database->insert("INSERT INTO `reply` values ('" + this->getpostID() + "','" + reply.getResponderID() + "'," + to_string(reply.getValue()) + ");");
			database->select("UPDATE `jobpost` set lowestOffer = " + to_string(this->LowestOffer) + " where PostID = '" + this->getpostID() + "';");
		}
		return true;
	}
	else { return false; }
}
string Job::getReplyDetails(void) const
{
	string Temp = this->getPostDetails();
	string offerHistory = this->offerHistory(2); //parameter 2 is for ascending order
	if (offerHistory == "") { offerHistory = "\nNo offer Yet"; }
	return (Temp + "-- Offer History --" + offerHistory + "\n");
}

//setters
void Job::setProposedPrice(float proposedPrice) 
{ if (proposedPrice < 0) { proposedPrice *= -1; } this->ProposedPrice = proposedPrice; }
void Job::setLowestOffer(float lowestOffer)
{ if (lowestOffer < 0) { lowestOffer *= -1; } this->LowestOffer = lowestOffer; }
void Job::deletePost(void) { jobID--; }
//getters
float Job::getProposedPrice(void)const { return this->ProposedPrice; }
float Job::getLowestOffer(void)const { return this->LowestOffer; }
