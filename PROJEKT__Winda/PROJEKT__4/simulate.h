#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <SDL.h>
#include <thread>


using namespace std;

const int ELE_WIDTH = 360;
const int ELE_HEIGHT = 118;
const int ELE_SPEED =3; 
const int FLOOR_COUNT = 5;
const int BUTTON_COUNT = 4;
const int BUTTON_SIZE = 20;
const int BUTTON_MARGIN = 10;
const int PPL_WIDTH = 30;
const int PPL_HEIGHT = 80;
const int WEIGHT = 70;
const int MAX_WEIGHT= 600;



struct Passenger
{
	int destFloor; 
	int startFloor;
	void Draw(SDL_Renderer* renderer,int x,int y);

};

struct Button
{
	SDL_Rect position;
	int destFloor;
	void Draw(SDL_Renderer* renderer);

};

class Floor
{
public:
	SDL_Rect position;

	Button buttons[BUTTON_COUNT]; 
	void Draw(SDL_Renderer* renderer);



};


class Elevator
{
public:
	SDL_Rect position;
	int targetFloor;
	vector <Passenger> passengers;
	int lastMove;
	void Draw(SDL_Renderer* renderer);


};

class Building
{
public:

	Floor floors[FLOOR_COUNT];
	vector <Passenger> waiting;
	Elevator ele;

	void Draw(SDL_Renderer* renderer);
	void Update(int dt);
	void Click(int mouseX, int mouseY);
	int floorNext(int dir); 
	bool passengersWait(int dir, int floorNumber);
	int elevatorTarget();
	int floorDir(int floorNumber);
	int floorDist(int floorNumber);
	Building();
	
};



