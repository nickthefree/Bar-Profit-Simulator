
#include <bits/stdc++.h> 
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Event {
 	public:
 	// constructor requires time of event
	Event (unsigned int t) : time(t) { }
 	// time is a public data field
 	unsigned int time;
 	// execute event by invoking this method
 	virtual void processEvent( ) { }
};

class OrderEvent : public Event {
 	public: OrderEvent (unsigned int time, unsigned int gs)
 			: Event(time), groupSize(gs) { }
 		virtual void processEvent( );
 	protected:
 		unsigned int groupSize;
};


class ArriveEvent : public Event {
 	public: ArriveEvent (unsigned int time, unsigned int gs)
 			: Event(time), groupSize(gs) { }
 		virtual void processEvent ( );
 	protected:
 		unsigned int groupSize;
}; 

class LeaveEvent : public Event {
 	public:
 		LeaveEvent (unsigned int time, unsigned int gs)
 			: Event(time), groupSize(gs) { }
 			virtual void processEvent ( );
 	protected:
 		unsigned int groupSize;
};

class eventComparison {
 	public:
 	bool operator ( ) (Event * left, Event * right){
 		return left->time > right->time;
	}
};

class randomInteger {
	public:
		unsigned int operator ( ) (unsigned int);
} randomizer;


unsigned int randomInteger::operator ( ) (unsigned int max)
{
	// rand return random integer
	// convert to unsigned to make positive
	// take remainder to put in range
	unsigned int rval = rand( );
	return rval % max;
}


unsigned int randBetween(int low, int high) {
	return low + randomizer(high - low);//returns value between "low" and "high" inputs
}


class SoftwareGurusBar {
 	public:
 	// try with 50 chairs, then try with 40, 60, ...
 	// in order to find out “optimal” profit prospects
 	SoftwareGurusBar( )
 		: freeChairs(49), profit(0.0) { }
 	bool canSeat(unsigned int numberOfPeople); // slide 12
 	void order(unsigned int beerType); // slide 13
 	void leave(unsigned int numberOfPeople); // slide 13

 	unsigned int freeChairs;
 	double profit;
};


class SimulationFramework {
 	public:
 		SimulationFramework ( ) : eventQueue( ), currentTime(0) { }
 		void scheduleEvent (Event * newEvent)
 		{
 			// insert newEvent into eventQueue (Priority Queue)
 			// Priority Queue is based on MinHeap using newEvent’s time
 			eventQueue.push (newEvent);
 		}
 		void run( );
 		int weightedProbability();
 		unsigned int currentTime;
	protected:
 		priority_queue <Event, vector<Event*>, eventComparison> eventQueue;//Add pointer to first event if DOESNT WORK
};

SoftwareGurusBar theBar;

SimulationFramework theSimulation;

bool SoftwareGurusBar::canSeat (unsigned int numberOfPeople)
{
	// if sufficient room, then seat customers
	cout << "Time: " << theSimulation.currentTime;
	cout << " Group of " << numberOfPeople << " customers arrives";
	if (numberOfPeople < freeChairs) {
 			cout << " Group is seated" << endl;
 			freeChairs -= numberOfPeople;
 			return true;
 		}
	else {
 			cout << " No room, group leaves" << endl;
 			return false;
 		}
}

void SoftwareGurusBar::order (unsigned int beerType)
{
	// serve beer
	cout << "Time: " << theSimulation.currentTime;
	cout << " serviced order for " << beerType << endl;
	// update profit based on this beerType
	// add your code here ...MORE?
	profit += beerType;	
	profit++;
}

void SoftwareGurusBar::leave (unsigned int numberOfPeople)
{
	// people leave, free up chairs
	cout << "Time: " << theSimulation.currentTime;
	cout << " group of size " << numberOfPeople << " leaves" << endl;
	freeChairs += numberOfPeople;
} 

void ArriveEvent::processEvent( )
{
 	if (theBar.canSeat(groupSize))
 	// place an order within 2 & 10 minutes
 		theSimulation.scheduleEvent (
 			new OrderEvent(theSimulation.currentTime + randBetween(2,10),
 							groupSize));
} 

void OrderEvent::processEvent( )
{
	int temp = 0;
 	// each member of the group orders a beer (type 1,2,3)
 	for (int i = 0; i < groupSize; i++){
 		temp = randBetween(1,100);
 		if(temp <= 15){
 			theBar.order(1);//15% chance of ordering local beer
 		}
 		else if(temp <= 75){
 			theBar.order(2);//60% chance of ordering imported beer
 		}
 		else{
 			theBar.order(3);//25% chance of ordering special beer
 		}
 	}
 	int t = theSimulation.currentTime + randBetween(15,35);
 	// schedule a LeaveEvent for this group of drinkers
 	// all the group leaves together
 	// add your code here ...
 	theSimulation.scheduleEvent(
	 	new LeaveEvent(theSimulation.currentTime + randBetween(30,60), groupSize));
 	//use framework from: ArriveEvent::processEvent() in the above code
};


void LeaveEvent::processEvent ( )
{
	theBar.leave(groupSize);
}

void SimulationFramework::run( )
{
 	// execute events until event queue becomes empty
 	while (!eventQueue.empty( )) {
 		// copy & remove min-priority element (min time) from eventQueue
 		cout << "1" << endl;
 		Event * nextEvent = eventQueue.top( );
 		eventQueue.pop( );
 		cout << "2" << endl;

 		// update simulation’s current time
 		currentTime = nextEvent->time;
 		
 		// process nextEvent
 		nextEvent->processEvent( ); // what do you see here???
 		
 		// cleanup nextEvent object
 		delete nextEvent;
 	}
} 

int main( ){
	// load priority queue with initial Arrive Events then run simulation
	unsigned int t = 0;
	// load 4 hours (240 minutes) of Arrive Events
	while (t < 240) {
		// new group every 2-5 minutes
 		t += randBetween(2,5);
 		// group size ranges from 1 to 5
 		theSimulation.scheduleEvent(new ArriveEvent(t, randBetween(1,5)));
 	}
 	// then run simulation and print profits
 	theSimulation.run( );
 	cout << "Total profits " << theBar.profit << endl;
 	
 	return 0;
}

/*NOTES OF SIMULATION:
	It appears that any amount of chairs above 49 has no impact on the bars profits, 
	regardless of whether or not the distribution of beers is uniform. Thus, I suggest
	the bar not waste resources on extra chairs, and only have enough capital to invest
	in exactly 49 chairs.
*/

