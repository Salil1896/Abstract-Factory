// Created by Salil Mamodiya
// Roll No:  120102053
// Batch: ECE

// Key for the Enchanted game is 'Sherlocked'
//In the Bombed game after blast the health is reduced by 50

#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;
enum Direction { North = 0, South, East, West };  // For movement
enum gametype {Norm=0,enchant,bomb}; ///for type of the maze

//Abstract class for class Room, Door, Wall
class MapSite {
public:
	int wallhealth;
	// pure Virtual Function
	virtual int Enter(int) = 0;
	virtual bool iswall() = 0;
};

//Room class for normal maze
class Room :public MapSite {
private:
	MapSite* sides[4]; // will save pointer to all 4 sides objects.
	int roomNumber;
public:
	Room(){}
	Room(int roomNo) //constructor
	{   roomNumber = roomNo;
		for (int i = 0; i<3; i++) sides[i] = NULL;}

	virtual int findRoomNo() {return roomNumber;}//gives room no.
    virtual MapSite* GetSide(Direction d1){return sides[d1];}// gives object at a particular side
    virtual void SetSide(Direction d1, MapSite* msite){sides[d1] = msite;} // sets object to a particular side
	virtual int Enter(int k){return 0;}//enter the room and return room no.
	virtual void makebomboff(){}
	virtual bool iswall(){return false;}
	
};

//wall class for normal maze
class Wall :public MapSite{
	int wallhealth;
public:
	Wall()
	{wallhealth = 100;}
	virtual void setwallhlth(int value){}
	
	virtual int Enter(int k)
	{cout << "You hit a Wall. You Cannot Enter\n";
	return k;}	//We cannot enter into the wall 
	
	//return true if Mapsize object is wall
	virtual bool iswall()
	{return true;}
	
};


//door class for normal maze
class Door :public MapSite{
private:
	Room* room1; Room* room2;
	bool isOpen;
public:
	Door(){}
	//constructor
	Door(Room* r1, Room *r2)
	{   room1 = r1;
		room2 = r2;
		isOpen = false;}
		
    //return true if Mapsize object is door
	virtual bool iswall()
	{return false;}
    
    //enter the door and return current position of room in the maze game.
	virtual int Enter(int k)
	{   if (isOpen)
		{   cout << "Entered a new room";
			if (k == room1->findRoomNo()) return room2->findRoomNo();
			if (k == room2->findRoomNo()) return room1->findRoomNo();
		}
		else
		{return k;}
	}
	//function to open door if door is closed.
	virtual void OpenDoor()
	{if (isOpen == false) isOpen = true;}
	
	//return the other side room of the door.
	virtual Room* OtherSideFrom(Room *r1)
	{   if (r1 == room1) return room2;
		if (r1 == room2) return room1;}
};


// class Bombedwall which intherit wall for the Bombed maze 
class BombedWall:public Wall{
private:
	int wallhealth; 
    
public:
	BombedWall()
	{wallhealth = 100;}// initial wall health

	bool iswall(){return true;}
	
    void setwallhlth(int value)// function to set wall health after blast.
    {wallhealth = value;}
    
	int Enter(int k) //  we cannot enter the wall
	{  cout << "Wallhealth "<<wallhealth<<"\n";
	   cout << "You hit a Wall. Get well soon\n";
	   return k;}

};

// class RoomWithABomb which intherit Room for the Bombed maze 
class RoomWithABomb:public Room{
private:
	bool isoff; // tell if the bomb is off in the room
	MapSite* sides[4]; //  to save object of all four side of room.
	int roomNumber;

public:
	RoomWithABomb(int roomNo)
	{   isoff = false;
		roomNumber = roomNo;
		for (int i = 0; i <= 3; i++) sides[i] = NULL;}

	void makebomboff()//  this function make blast in Room.
	{   
        if(isoff==false)
		{isoff = true;
		for (int i = 0; i <= 3; i++)
		{
			if (sides[i]->iswall() == true)
			{((BombedWall *)sides[i])->setwallhlth(50);} // reduce the health of all the sides wall.
		}
		cout<<"There is a blast in Room "<<roomNumber<<"\n";
	    }
	}

	int findRoomNo() {return roomNumber;}

	MapSite* GetSide(Direction d1){return sides[d1];}
	bool iswall() {return false;}
	int enter(int k){return 0;}
	void SetSide(Direction d1, MapSite* mst){sides[d1] = mst;}

};

// class Enchanted which intherit Room for the Enchanted maze game. 
class EnchantedRoom:public Room
{
    private:
	  int roomNumber;
	  MapSite* sides[4];
      string doorkey; // save doorkey for tht room
    public:
		EnchantedRoom(int roomNo, string s)
		{doorkey.assign(s);
		 roomNumber = roomNo;
		 for (int i = 0; i<3; i++) sides[i] = NULL;
		}
		int findRoomNo()
		{return roomNumber;}
        
		MapSite* GetSide(Direction d1)
		{return sides[d1];}

		void SetSide(Direction d1, MapSite* msite){sides[d1] = msite;}
		int Enter(int k){return 0;}
		bool iswall(){return false;}
};

// class DoorNeedingSpell which intherit Door for the Enchanted maze game. 
class DoorNeedingSpell:public Door{
    
    private:
	Room* room1; Room* room2;
	bool isOpen;
	string key;
    public:
	DoorNeedingSpell(Room* r1, Room *r2,string key1)
	{   room1 = r1;
		room2 = r2;
		isOpen = false;
		key.assign(key1);}
		
	void OpenDoor(string s1) // open door if input string matches with Key.
	{   if (isOpen == false && s1.compare(key)==0) isOpen = true;
		if (s1.compare(key) != 0) {cout << "Your key is wrong.\n";
		  cout<<"You are still in Room ";
		;}
	}
	int Enter(int k)
	{
		if (isOpen)
		{   cout << "Entered a new Room ";
			if (k == room1->findRoomNo()) return room2->findRoomNo();
			if (k == room2->findRoomNo()) return room1->findRoomNo();}
		else {return k;}

	}
	void LockDoor()
	{isOpen = false;} // after opening door make the door lock
	
	void OpenDoor()
	{if (isOpen == false) isOpen = true;}

	Room* OtherSideFrom(Room *r1)
	{   if (r1 == room1) return room2;
		if (r1 == room2) return room1;}
	
	bool iswall() {return false;}

};


// class Maze 
class Maze{

private:
	map<int, Room*> mazeroom; // in Maze it maps room number to room pointer.
	int roomposition;
	int gametype; // three types of mazes normal, enchanted, and bombed.

public:
	Maze()
	{roomposition = 0;}
	
	void setroompos(int roomid)// function to set current room position
	{roomposition = roomid;}
	
	void setgametype(int type){gametype = type;}
	void AddRoom(Room *r){mazeroom[r->findRoomNo()] = r;}

	Room* RoomNo(int n)
	{return mazeroom[n];}
	
	void blast()
	{mazeroom[roomposition]->makebomboff();}
	
	void walk(Direction d1)// function to move in a maze
	{
	    
		MapSite *side = mazeroom[roomposition]->GetSide(d1);
		int k = side->Enter(roomposition);
		
		if(k!= 0 && k!=roomposition)
		{roomposition = k;
		 cout << " " << roomposition << "\n";
		}
        else if (k == roomposition)
		{
			if (side->iswall() == false)
			{
				string s1;
				cout << "\nYou have Door this side.press key to open Door.\n";
				cout<<"For Enchanted:Original key     For Normal And Bombed:Any key   ";
				cin >>s1;
				if(gametype == enchant)
				{
				  ((DoorNeedingSpell *)side)->OpenDoor(s1);
				   roomposition = side->Enter(roomposition);
				   ((DoorNeedingSpell *)side)->LockDoor();
				}
				else{
				((Door *)side)->OpenDoor();
				roomposition = side->Enter(roomposition);
			     }
				 cout << " " << roomposition << "\n";
			}
		}
	}
	
};

//mazefactory which creates normal mazes
class MazeFactory {

public:
	MazeFactory()
	{}
	virtual Maze* MakeMaze() 
	{return new Maze;}

	virtual Wall* MakeWall() 
	{return new Wall;}

	virtual Room* MakeRoom(int n) 
	{return new Room(n);}

	virtual Door* MakeDoor(Room* r1, Room* r2)
	{return new Door(r1, r2);}
};

//EnchantedMazefactory 
class EnchantedMazeFactory:public MazeFactory {

private:
	string s1;
	MapSite* sides[4];
	int roomNumber;

public:
    EnchantedMazeFactory()
	{s1.assign("Sherlocked");}
//    //key of enchanted
	virtual Room* MakeRoom(int n)
	{
	return new EnchantedRoom(n, CastSpell());}
    virtual Door* MakeDoor(Room* r1, Room* r2) 
	{return new DoorNeedingSpell(r1, r2, s1);}

    protected:
	string CastSpell()
	{return s1;}


};

// Bombed Maze factory
class BombedMazeFactory:public MazeFactory {
public:
	BombedMazeFactory(){}
	
	Room* MakeRoom(int n)
	{new RoomWithABomb(n);}

	Wall* MakeWall()
	{return new BombedWall;}

};

//function to created all types of mazes 
Maze* CreateMaze(MazeFactory& factory,int type) {

	Maze* aMaze = factory.MakeMaze();
	aMaze->setgametype(type);
	Room* r1 = factory.MakeRoom(1);
	Room* r2 = factory.MakeRoom(2);
	Door* aDoor = factory.MakeDoor(r1, r2);

	aMaze->AddRoom(r1);
	aMaze->AddRoom(r2);
	r1->SetSide(North, factory.MakeWall());
	r1->SetSide(East, aDoor);
	r1->SetSide(South, factory.MakeWall());
	r1->SetSide(West, factory.MakeWall());
	
	r2->SetSide(North, factory.MakeWall());
	r2->SetSide(East, factory.MakeWall());
	r2->SetSide(South, factory.MakeWall());
	r2->SetSide(West, aDoor);
	return aMaze;
}

int main(void)
{
   
    MazeFactory factory;
	Maze* game1 = CreateMaze(factory,Norm);
	game1->setroompos(1);
	cout<<"Game1\n";
	cout << "Normal Maze Game: Select walk n:north, s:south, e:east, w:west, q:quit" << endl;

    char ch = 'q';
	do
	{
		cout << "Select walk direction:  ";
		cin >> ch;
		switch (ch)
		{
		case 'n':
			game1->walk(North);
			break;
		case 's':
			game1->walk(South);
			break;
		case 'e':
			game1->walk(East);
			break;
		case 'w':
			game1->walk(West);
			break;
		}
	} while (ch != 'q');
	
	// Create Enchanted Maze
	cout<<"\nGame2\n";
	EnchantedMazeFactory factory1;
	Maze* game2 = CreateMaze(factory1,enchant);
	game2->setroompos(1);
	cout << "Enchanted Maze Game: Select walk n:north, s:south, e:east, w:west, q:quit" << endl;
	char ch1 = 'q';
	do
	{
		cout << "Select walk direction:  ";
		cin >> ch1;
		switch (ch1)
		{
		case 'n':
			game2->walk(North);
			break;
		case 's':
			game2->walk(South);
			break;
		case 'e':
			game2->walk(East);
			break;
		case 'w':
			game2->walk(West);
			break;
		}
	} while (ch1 != 'q');
    
    
    ///Bombed maze game
	BombedMazeFactory factory2;
	Maze* game3 = CreateMaze(factory2,bomb);
	game3->setroompos(1);
	cout<<"\nGame3\n";
	cout << "Bombed Maze Game: Select walk n:north, s:south, e:east, w:west, q:quit" << endl;
	char ch3 = 'q';
	int count = 0;
	do
	{
		count++;
		if(count%3==0) game3->blast();
		cout << "Select walk direction : ";
		cin >> ch3;
		switch (ch3)
		{
		case 'n':
			game3->walk(North);
			break;
		case 's':
			game3->walk(South);
			break;
		case 'e':
			game3->walk(East);
			break;
		case 'w':
			game3->walk(West);
			break;
		}
	} while (ch3 != 'q');
	return 0;
}

