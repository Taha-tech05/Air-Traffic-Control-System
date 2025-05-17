#include <iostream>
#include<queue>
#include <chrono>
#include <thread>
#include <iomanip>
#include <vector>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
using namespace std;
using namespace std::chrono;


const int PRIORITY_FOR_CARGO = 3;
sem_t mySemaphore1;
sem_t mySemaphore2;
sem_t mySemaphore3;

int finishCounter=0;

void* simulatorA(void*arg);
void* simulatorB(void*arg);
void* simulatorC(void*arg);


struct Coord {
	int x;
	int y;
};
class Aircraft {
private:
	int aircraftId;
	bool flightOrDeparture; // 1 for flight 0 for departure
	int speed;
	string name;
	string type;
	int altitude;
	//Coord position; // if 0,0 means its landing if its 1,1 it means its in the air
	string direction;
	bool violationStatus;
	int priority;  //greater the number greater the priority
	int priorityMul;

public:
	bool assigned = false;
	// --- Getters ---
	int getAircraftId() const { return aircraftId; }
	bool isFlightOrDeparture() const { return flightOrDeparture; } // "is" prefix for boolean
	int getSpeed() const { return speed; }
	string getType() const { return type; }
	int getAltitude() const { return altitude; }
	string getDirection() const { return direction; }
	bool hasViolation() const { return violationStatus; } // "has" prefix for boolean
	int getPriority() const { return priority; }
	int getPriorityMul() const { return priorityMul; }
	string getName()const { return name; }
	// --- Setters ---
	void setSpeed(int newSpeed) { speed = newSpeed; }
	void setAltitude(int newAltitude) { altitude = newAltitude; }
	void setViolation(bool hasViolation) { violationStatus = hasViolation; }
	void setPriorityMul(int newMul) { priorityMul = newMul; }

	Aircraft(int aircraftId, string name, bool flightOrDeparture, int speed, string type, int altitute, string direction, int priorityMul, int priority = 1 /*Coord position */ ) {
		this->aircraftId = aircraftId;
		this->flightOrDeparture = flightOrDeparture;
		this->name = name;
		this->speed = speed;
		this->type = type;
		this->altitude = altitute;
		this->direction = direction;
		this->priorityMul = priorityMul;
		this->priority = priorityMul;
		this->violationStatus = 0;
	}
	void increasePriority(int customPriority = 0) {
		if (customPriority == 0)
			this->priority += (this->priority * this->priorityMul);
		else
			this->priority += (this->priority * customPriority);
		return;
	}
	void reCalculatePriority() {
		double sum = 0;
		sum = (((100 - this->fuel) / 100.0) * 3);
		sum += this->isEmergency * 4;
		sum += this->isCargo * 2;
		if (sum < 0)
		{
			sum *= -1;
		}
		sum = ceil(sum);
		//	cout << endl << endl << endl << " new priority << " << sum << endl << endl << endl;
		this->priority = sum;
	
		return;
	}
};

struct ComparePriority{
	bool operator()(Aircraft*a1,Aircraft*a2){
		if(a1->getPriority()==a2->getPriority()){
			if(a1->getAltitude()<a2->getAltitude()){
				
			}
			else{
				if(a1->getAltitude()==a2->getAltitude()){
					if(a1->getAircraftId()>a2->getAircraftId()){
						
					}
					else{
						
					}
				}
				else{
					
				}
			}
		}
		return a1->getPriority()>a2->getPriority();
	}
};

class Airline {
private:
	string name;
	string type;
	int numOfAircrafts; // total aircrafts
	int numOfFlights;   // no of flights currently in simulation
	int maxNoOfFlights; // max no of flights there can be
	Aircraft* aircrafts;

public:
	// Getters
	string getName() {
		return name;
	}

	string getType() {
		return type;
	}

	int getNumOfAircrafts() {
		return numOfAircrafts;
	}

	int getNumOfFlights() {
		return numOfFlights;
	}

	int getMaxNoOfFlights() {
		return maxNoOfFlights;
	}

	Aircraft* getAircrafts() {
		return aircrafts;
	}
	//Setterrs 

	void setNumOfFlights(int newNum) {
		numOfFlights = newNum;
	}

	void setAircrafts(Aircraft* newAircrafts) {
		aircrafts = newAircrafts;
	}

	Airline(string name, string type, int numOfAircrafts, int maxNoOfFlights, int numOfFlights = 0) {
		this->name = name;
		this->type = type;
		this->numOfAircrafts = numOfAircrafts;
		this->numOfFlights = numOfFlights;
		this->maxNoOfFlights = maxNoOfFlights;
		aircrafts = nullptr;
	}
};

struct AVN {
	int avnId;
	int aircraftId;
	string aircraftType;
	string paymentStatus;
	int fineAmount;

};

class Flight_Arrival_Dispatch {
private:
	string direction[4];
	string flightType[4];
	int time[4];
	float emergencyRate[4];
public:
	Flight_Arrival_Dispatch() {
		direction[0] = "North";
		direction[1] = "South";
		direction[2] = "East";
		direction[3] = "West";

		flightType[0] = "International Arrivals";
		flightType[1] = "Domestic Arrivals";
		flightType[2] = "International Departures";
		flightType[3] = "International Departures";

		time[0] = 1 * 60;
		time[1] = 1* 60;
		time[2] = 1 * 60;
		time[3] = 1 * 60;

		emergencyRate[0] = 0.1;
		emergencyRate[1] = 0.05;
		emergencyRate[2] = 0.15;
		emergencyRate[3] = 0.2;
	}

	string getDirection(int i) {
		if (i >= 0 && i <= 3) {
			return direction[i];
		}
		else
			return "Invalid";
	}

	int getTime(string directionName) {
		for (int i = 0;i < 4;i++) {
			if (direction[i] == directionName) {
				return time[i];
			}
		}
		return -1;
	}
	string getType(string directionName) {
		for (int i = 0;i < 4;i++) {
			if (direction[i] == directionName) {
				return flightType[i];
			}
		}
		return "Invalid";
	}
	int getEmerRate(string directionName) {
		for (int i = 0;i < 4;i++) {
			if (direction[i] == directionName) {
				return emergencyRate[i];
			}
		}
		return -1;
	}

};

class ATC {

private:
bool runwayAAvailabe;
bool runwayBAvailabe;
bool runwayCAvailabe;
int noOfTotalFlights;
AVN* violations;
Airline** airlines;
Flight_Arrival_Dispatch flightdata;
pthread_t tid1;
pthread_t tid2;
pthread_t tid3;

public:
	vector<Aircraft*>flights;
	priority_queue<Aircraft*,vector<Aircraft*>,ComparePriority>runwayA;
	priority_queue<Aircraft*,vector<Aircraft*>,ComparePriority>runwayB;
	priority_queue<Aircraft*,vector<Aircraft*>,ComparePriority>runwayC;
	ATC() {
		noOfTotalFlights = 6;
		airlines = new Airline * [6];
		airlines[0] = new Airline("PIA", "Commercial", 6, 4);
		airlines[1] = new Airline("AirBlue", "Commercial", 4, 4);
		airlines[2] = new Airline("FedEx", "Cargo", 3, 2);
		airlines[3] = new Airline("Blue Dart", "Cargo", 2, 2);
		airlines[4] = new Airline("Pakistan Airforce", "Military", 2, 1);
		airlines[5] = new Airline("AghaKhan Air Ambulance", "Medical", 2, 1);
		srand(time(0));

		int id = 1;
		bool flightOrDeparture = rand() % 2;
		int speed = flightOrDeparture * 700;//before landing     ////  //0 + (flightOrDeparture * (rand() % 200 + 400);// if flight then assign from 400-600 else 0 for departure;
		int altitude = flightOrDeparture?810:10; //metres dont know if required
		int chooseAirline = rand() % 2 + 2;
		int chooseDirection = 0;
		if (flightOrDeparture == 0) {
			chooseDirection = rand() % 2 + 2;
		}
		else {
			chooseDirection = rand() % 2;
		}
		string direction = flightdata.getDirection(chooseDirection);
		int priorityMul = PRIORITY_FOR_CARGO;
		string type = "Cargo";

		Aircraft*aircraft=new Aircraft(id++, airlines[chooseAirline]->getName(), flightOrDeparture, speed, type, altitude, direction, priorityMul); // 1 cargo
		flights.push_back(aircraft);
		airlines[chooseAirline]->setNumOfFlights(1);


		for (int i = 0;i < noOfTotalFlights - 1;i++) {
			flightOrDeparture = rand() % 2;
			speed = flightOrDeparture * 700;//before landing     ////  //0 + (flightOrDeparture * (rand() % 200 + 400);// if flight then assign from 400-600 else 0 for departure;
			altitude = flightOrDeparture?810:10; 
			chooseAirline = rand() % 6 ;
			if (flightOrDeparture == 0) {
				chooseDirection = rand() % 2+2;
			}
			else {
				chooseDirection = rand() % 2;
			}
			direction = flightdata.getDirection(chooseDirection);
			priorityMul = 1;

			while (airlines[chooseAirline]->getNumOfFlights() >= airlines[chooseAirline]->getMaxNoOfFlights()) // of all flights used
			{
				chooseAirline = rand() % 6;
			}
			Aircraft*aircraft=new Aircraft(id++, airlines[chooseAirline]->getName(), flightOrDeparture, speed, airlines[chooseAirline]->getType(), altitude, direction, priorityMul);
			flights.push_back(aircraft);
			airlines[chooseAirline]->setNumOfFlights(airlines[chooseAirline]->getNumOfFlights() + 1); //increment flights used

		}


	}

	
	void runwaysSimulator(){
	
		pthread_create(&tid1,NULL,simulatorA,this);
		pthread_create(&tid2,NULL,simulatorB,this);
		pthread_create(&tid3,NULL,simulatorC,this);

	}
	void joinThreads() {
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
        pthread_join(tid3, NULL);
    }
	void simulate(const int& timeInSeconds) {
		for (int i = 0;i < flights.size();i++) {
			if (timeInSeconds % flightdata.getTime(flights[i]->getDirection()) == 0) {
				if (flights[i]->getType() == "Cargo") {
					cout<<"[ENTRY] A new plane arrived : "<<flights[i]->getName()<<" "<<flights[i]->getAircraftId()<<endl;
					runwayC.push(flights[i]);
					flights.erase(flights.begin() + i);
				}
				else if (flights[i]->getDirection() == "North" || flights[i]->getDirection() == "South") {
					cout<<"[ENTRY] A new plane arrived : "<<flights[i]->getName()<<" "<<flights[i]->getAircraftId()<<endl;
					runwayA.push(flights[i]);
					flights.erase(flights.begin() + i);
				}
				else if (flights[i]->getDirection() == "East" || flights[i]->getDirection() == "West") {
					cout<<"[ENTRY] A new plane arrived : "<<flights[i]->getName()<<" "<<flights[i]->getAircraftId()<<endl;
					runwayB.push(flights[i]);
					flights.erase(flights.begin() + i);
				}

			}
		}
	}
	
	~ATC() {
		for (int i = 0;i < 6;i++) {
			delete airlines[i];
		}
		delete[]airlines;
	}

	void initialise() {

	}

};


void* arrival(void*arg) {
	Aircraft*plane=(Aircraft*)arg;
	while(true){
			//when any ground violation or plane has landed/departured then lock is free and thread ends.
	cout << endl << endl;
	int holding = 800;
	int approach = 600;
	int landing = 400;
	int taxi = 20;
	int atGate = 10;
	cout << "RunWay A" << endl;
	cout << "Arrival:from " << plane->getDirection() << endl;
	cout << "Aircraft:" << plane->getAircraftId() <<" "<< plane->getName() <<" arriving"<< endl;
	cout << "Altitude:" << plane->getAltitude() << endl;
	int speed = 0;
	if (plane->getAltitude() ==holding+10) {
		cout << "Entering Holding Phase" << endl;
		int speed = rand() % 20 + 550;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed > 600) {
			cout << "Speed Violation:exceeds 600km/h" << endl;
			plane->setViolation(true);
		}
	}
	else if (plane->getAltitude() == approach+10) {
		cout << "Entering Approach Phase" << endl;
		int speed = rand() % 20 + 285;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed > 290) {
			cout << "Speed Violation:exceeds 290km/h" << endl;
			plane->setViolation(true);
		}
	}
	else if(plane->getAltitude() ==landing+10) {
		int speed = rand() % 20 + 235;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed <240) {
			cout << "Speed Violation:below 240km/h" << endl;
			plane->setViolation(true);
		}

	}
	else if (plane->getAltitude() == landing) {
		cout << "Entering Landing Phase" << endl;
		int speed = rand() % 20 + 225;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed > 240) {
			cout << "Speed Violation:exceeds 240km/h" << endl;
			plane->setViolation(true);
		}

	}
	else if (plane->getAltitude() ==taxi+10) {
		cout<<"Entering taxi phase"<<endl;
		int faults = rand() % 10;
		if (faults == 1) {
			cout << "Ground fault occur of aircraft:" << plane->getAircraftId() << endl;
			sem_post(&mySemaphore1);
			break;
		}
		int speed = rand() % 20 + 25;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed <30) {
			cout << "Speed Violation:below 30km/h" << endl;
			plane->setViolation(true);
		}

	}
	else if (plane->getAltitude() == atGate) {
		int faults = rand() % 10;
		if (faults == 1) {
			cout << "Ground fault occur of aircraft:" << plane->getAircraftId() << endl;
			sem_post(&mySemaphore1);
			break;
		}
		cout << "Entering Gate Phase" << endl;
		int speed = rand() % 15 + 1;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed > 10) {
			cout << "Speed Violation:exceeds 10km/h" << endl;
			plane->setViolation(true);
		}

	}
	else if (plane->getAltitude()==0) {
		cout << "Plane landed" << endl;
		sem_post(&mySemaphore1);
		break;
	}
	plane->setAltitude(plane->getAltitude() - 10);
	//here decrease fuel

	sleep(1);
}
pthread_exit(0);
	
}

void* departure(void*arg) {
	Aircraft*plane=(Aircraft*)arg;
	while(true){
	//when any ground violation or plane has landed/departured then lock is free and thread ends.
	cout << endl << endl;
	int departure = 800;
	int climb = 600;
	int takeoffRoll = 400;
	int taxi = 20;
	int atGate = 10;
	cout << "RunWay B" << endl;
	cout << "Departure from:" << plane->getDirection() << endl;
	cout << "Aircraft:" << plane->getAircraftId() << " " << plane->getName() << " departuring" << endl;
	cout << "Altitude:" << plane->getAltitude() << endl;
	if (plane->getAltitude() == departure+10) {
		cout << "Departured" << endl;
		sem_post(&mySemaphore2);
		break;
	}
	else if (plane->getAltitude() == departure - 10) {
		cout << "Entering Departure Phase" << endl;
		int speed = rand() % 20 + 795;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed < 800) {
			cout << "Speed Violation:below 800km/h" << endl;
			plane->setViolation(true);
		}
	}
	else if (plane->getAltitude() == departure) {
		int speed = rand() % 60 + 850;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed > 900) {
			cout << "Speed Violation:exceeds 900km/h" << endl;
			plane->setViolation(true);
		}
	}
	else if (plane->getAltitude() == climb ) {
		cout << "Entering Climb Phase" << endl;
		int speed = rand() % 70 + 400;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed < 463) {
			cout << "Speed Violation:exceeds 463km/h" << endl;
			plane->setViolation(true);
		}

	}
	else if (plane->getAltitude() == takeoffRoll) {
		cout << "Entering takeof Roll" << endl;
		int speed = rand() % 100 + 200;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed > 290) {
			cout << "Speed Violation:exceeds 290km/h" << endl;
			plane->setViolation(true);
		}

	}
	else if (plane->getAltitude() == taxi) {
		int faults = rand() % 10;
		if (faults == 1) {
			cout << "Ground fault occur of aircraft:" << plane->getAircraftId() << endl;
			sem_post(&mySemaphore2);
			break;
		}
		int speed = rand() % 20 + 15;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed > 30) {
			cout << "Speed Violation:exceeds 30km/h" << endl;
			plane->setViolation(true);
		}

	}
	else if (plane->getAltitude() == atGate) {
		int faults = rand() % 10;
		if (faults == 1) {
			cout << "Ground fault occur of aircraft:" << plane->getAircraftId() << endl;
			sem_post(&mySemaphore2);
			break;
		}
		cout << "Entering Gate Phase" << endl;
		int speed = rand() % 15 + 1;
		plane->setSpeed(speed);
		cout << " Speed:" << plane->getSpeed() << endl;
		if (speed > 10) {
			cout << "Speed Violation:exceeds 10km/h" << endl;
			plane->setViolation(true);
		}

	}
	plane->setAltitude(plane->getAltitude() + 10);
	//here decrease fuel
	sleep(1);
}
	pthread_exit(0);
}
void* cargo(void*arg) {
	Aircraft*plane=(Aircraft*)arg;
	while(true){
	//when any ground violation or plane has landed/departured then lock is free and thread ends.
	cout << endl << endl;
	cout << "RunWay C" << endl;
	if (plane->getDirection() == "North" || plane->getDirection() == "South") {
		int holding = 800;
		int approach = 600;
		int landing = 400;
		int taxi = 20;
		int atGate = 10;
		cout << "Arrival:from " << plane->getDirection() << endl;
		cout << "Aircraft:" << plane->getAircraftId() << " " << plane->getName() << " arriving" << endl;
		cout << "Altitude:" << plane->getAltitude() << endl;
		int speed = 0;
		if (plane->getAltitude() == holding + 10) {
			cout << "Entering Holding Phase" << endl;
			int speed = rand() % 20 + 550;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed > 600) {
				cout << "Speed Violation:exceeds 600km/h" << endl;
				plane->setViolation(true);
			}
		}
		else if (plane->getAltitude() == approach + 10) {
			cout << "Entering Approach Phase" << endl;
			int speed = rand() % 20 + 285;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed > 290) {
				cout << "Speed Violation:exceeds 290km/h" << endl;
				plane->setViolation(true);
			}
		}
		else if (plane->getAltitude() == landing + 10) {
			int speed = rand() % 20 + 235;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed < 240) {
				cout << "Speed Violation:below 240km/h" << endl;
				plane->setViolation(true);
			}

		}
		else if (plane->getAltitude() == landing) {
			cout << "Entering Landing Phase" << endl;
			int speed = rand() % 20 + 225;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed > 240) {
				cout << "Speed Violation:exceeds 240km/h" << endl;
				plane->setViolation(true);
			}

		}
		else if (plane->getAltitude() == taxi + 10) {
			int faults = rand() % 10;
			if (faults == 1) {
				cout << "Ground fault occur of aircraft:" << plane->getAircraftId() << endl;	
				sem_post(&mySemaphore3);
				break;					
			}
			int speed = rand() % 20 + 25;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed < 30) {
				cout << "Speed Violation:below 30km/h" << endl;
				plane->setViolation(true);
			}

		}
		else if (plane->getAltitude() == atGate) {
			int faults = rand() % 10;
			if (faults == 1) {
				cout << "Ground fault occur of aircraft:" << plane->getAircraftId() << endl;
				sem_post(&mySemaphore3);
				break;	
			}
			cout << "Entering Gate Phase" << endl;
			int speed = rand() % 15 + 1;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed > 10) {
				cout << "Speed Violation:exceeds 10km/h" << endl;
				plane->setViolation(true);
			}

		}
		else if (plane->getAltitude() == 0) {
			cout << "Plane landed" << endl;
			sem_post(&mySemaphore3);
			break;	
		}
		plane->setAltitude(plane->getAltitude() - 10);
					//here decrease fuel
	}
	else if (plane->getDirection() == "East" || plane->getDirection() == "West") {
		int departure = 800;
		int climb = 600;
		int takeoffRoll = 400;
		int taxi = 20;
		int atGate = 10;
		cout << "Departure from:" << plane->getDirection() << endl;
		cout << "Aircraft:" << plane->getAircraftId() << " " << plane->getName() << " departuring" << endl;
		cout << "Altitude:" << plane->getAltitude() << endl;
		if (plane->getAltitude() == departure+10) {
			cout << "Departured" << endl;
			sem_post(&mySemaphore3);
			break;	
		}
		else if (plane->getAltitude() == departure - 10) {
			cout << "Entering Departure Phase" << endl;
			int speed = rand() % 20 + 795;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed < 800) {
				cout << "Speed Violation:below 800km/h" << endl;
				plane->setViolation(true);
			}
		}
		else if (plane->getAltitude() == departure) {
			int speed = rand() % 60 + 850;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed > 900) {
				cout << "Speed Violation:exceeds 900km/h" << endl;
				plane->setViolation(true);
			}
		}
		else if (plane->getAltitude() == climb) {
			cout << "Entering Climb Phase" << endl;
			int speed = rand() % 70 + 400;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed < 463) {
				cout << "Speed Violation:exceeds 463km/h" << endl;
				plane->setViolation(true);
			}

		}
		else if (plane->getAltitude() == takeoffRoll) {
			cout << "Entering takeof Roll" << endl;
			int speed = rand() % 100 + 200;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed > 290) {
				cout << "Speed Violation:exceeds 290km/h" << endl;
				plane->setViolation(true);
			}

		}
		else if (plane->getAltitude() == taxi) {
			int faults = rand() % 10;
			if (faults == 1) {
				cout << "Ground fault occur of aircraft:" << plane->getAircraftId() << endl;
				sem_post(&mySemaphore3);
				break;	
			}
			int speed = rand() % 20 + 15;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed > 30) {
				cout << "Speed Violation:exceeds 30km/h" << endl;
				plane->setViolation(true);
			}

		}
		else if (plane->getAltitude() == atGate) {
			int faults = rand() % 10;
			if (faults == 1) {
				cout << "Ground fault occur of aircraft:" << plane->getAircraftId() << endl;
				sem_post(&mySemaphore3);
				break;	
			}
			cout << "Entering Gate Phase" << endl;
			int speed = rand() % 15 + 1;
			plane->setSpeed(speed);
			cout << " Speed:" << plane->getSpeed() << endl;
			if (speed > 10) {
				cout << "Speed Violation:exceeds 10km/h" << endl;
				plane->setViolation(true);
			}

		}
		plane->setAltitude(plane->getAltitude() + 10);
		//here decrease fuel
		sleep(1);

	}
}
pthread_exit(0);
}

//SIMULATOR FOR RUNWAYA

void* simulatorA(void* arg) {
    ATC* atc = (ATC*)arg;
	pthread_t tid[atc->flights.size()];
	int i=0;
	bool check=false;
    while (!atc->flights.empty()||!atc->runwayA.empty()) {
				//when the flights array is empty and runway is empty then come out of loop
		if (!atc->runwayA.empty()) {
			//try to acquie lock if not then conitnue with the loop 
			if (sem_trywait(&mySemaphore1) == 0) {
				//if lock is acquired then pop the front of queue and start with the process
				check=true;
                Aircraft* plane = atc->runwayA.top();
                atc->runwayA.pop();
                pthread_create(&tid[i++],NULL,arrival,plane);

            }
			else{
				if(atc->runwayA.empty()!=0){
					if(check){
						cout << "Aircraft:" << atc->runwayA.top()->getAircraftId() << " " << atc->runwayA.top()->getName() << "waiting for RunWay A" << endl;
						check=false;
					}
				}
			}
        }
		else{
			//here decrease fuel
		    //increase their priority as fuel is decreasing

		}
	}	
	for(int j=0;j<i;j++){
		pthread_join(tid[j],NULL);
	}
	finishCounter++;

    pthread_exit(NULL);
}
//SIMULATOR FOR RUNWAYb

void* simulatorB(void* arg) {
    ATC* atc = (ATC*)arg;
	pthread_t tid[atc->flights.size()];
	int i=0;
	bool check=false;
    while (!atc->flights.empty()||!atc->runwayB.empty()) {
		//when the flights array is empty and runway is empty then come out of loop
        if (!atc->runwayB.empty()) {
			if (sem_trywait(&mySemaphore2) == 0) {
								//if lock is acquired then pop the front of queue and start with the process
				check=true;
				Aircraft* plane = atc->runwayB.top();
                atc->runwayB.pop();
                pthread_create(&tid[i++],NULL,departure,plane);

            }
			else{
				if(atc->runwayB.empty()!=0){
					if(check){
						cout << "Aircraft:" << atc->runwayB.top()->getAircraftId() << " " << atc->runwayB.top()->getName() << "waiting for RunWay B" << endl;
						check=false;
					}
				}
			}
        }
		else{
			//here decrease fuel
							//increase their priority as fuel is decreasing

		}
    }
	for(int j=0;j<i;j++){
		pthread_join(tid[j],NULL);
	}
	finishCounter++;
    pthread_exit(NULL);
}
//SIMULATOR FOR RUNWAYc

void* simulatorC(void* arg) {
    ATC* atc = (ATC*)arg;
	pthread_t tid[atc->flights.size()];
	int i=0;
	bool check=false;
    while (!atc->flights.empty()||!atc->runwayC.empty()) {
				//when the flights array is empty and runway is empty then come out of loop
		if (!atc->runwayC.empty()) {
			if (sem_trywait(&mySemaphore3) == 0) {
								//if lock is acquired then pop the front of queue and start with the process
                check=true;
				Aircraft* plane = atc->runwayC.top();
                atc->runwayC.pop();
                pthread_create(&tid[i++],NULL,cargo,plane);

            }
			else{
				if(atc->runwayC.empty()!=0){
					if(check){
						cout << "Aircraft:" << atc->runwayC.top()->getAircraftId() << " " << atc->runwayC.top()->getName() << "waiting for RunWay C" << endl;
						check=false;
					}
				}
			}
        }
    }
	for(int j=0;j<i;j++){
		pthread_join(tid[j],NULL);
	}
	finishCounter++;

    pthread_exit(NULL);
}
//Functions 
milliseconds getSimulationTime(steady_clock::time_point startTime, int& simHours, int& simMins, int& simSecs) {
	// Get elapsed real time in milliseconds
	steady_clock::time_point currentTime = steady_clock::now();
	milliseconds realElapsed = duration_cast<milliseconds>(currentTime - startTime);

	// Convert to 5X simulation time (1 real sec = 5 sim secs)
	int totalSimSeconds = static_cast<int>(realElapsed.count() * 5 / 1000);

	// Break down into HH:MM:SS
	simHours = totalSimSeconds / 3600;
	simMins = (totalSimSeconds % 3600) / 60;
	simSecs = totalSimSeconds % 60;
	return realElapsed;
}

// Function to display time
void displayTime(int hours, int mins, int secs) {
	cout << "\rSimulation Time: "
		<< setfill('0') << setw(2) << hours << ":"
		<< setw(2) << mins << ":"
		<< setw(2) << secs << flush;
}


int main() {
	ATC atc;
	// Initialize with explicit types
	steady_clock::time_point startTime = steady_clock::now();
	int simHours = 0, simMins = 0, simSecs = 0;
	sem_init(&mySemaphore1, 0, 1); 
	sem_init(&mySemaphore2, 0, 1); 
	sem_init(&mySemaphore3, 0, 1); 
	atc.runwaysSimulator();
	double timeInSeconds=100;//in seconds
	//when all threads of runway finish then loop is ended
	while (finishCounter<3) {  // Run for up to 1 sim hour
		// Update time
		seconds sec = duration_cast<seconds>(getSimulationTime(startTime, simHours, simMins, simSecs));  // Truncates to whole seconds
		timeInSeconds+=1;
		cout << timeInSeconds << endl;
		atc.simulate(timeInSeconds);
		this_thread::sleep_for(milliseconds(100));  // Update every 100ms
	}
	atc.joinThreads();
	cout << "\nSimulation ended.\n";
	return 0;
}