#include "simulate.h"
#include <algorithm>

void elevatorVisual(SDL_Renderer* renderer, float& elevatorY, Elevator elev);
void floorVisual(SDL_Renderer* renderer);
void frameVisual(SDL_Renderer* renderer);
void passagerVisual(SDL_Renderer* renderer, int& floor, Floor& flo, vector <Passenger>& waiting);
void buttonVisual(SDL_Renderer* renderer, Button button_e[FLOOR_COUNT][BUTTON_COUNT]);

Building::Building()
{

	for (int floor = 0; floor < FLOOR_COUNT; ++floor)
	{
		floors[floor].position.y = 600 - floor * 150; 
		floors[floor].position.w = 400;
		floors[floor].position.h = 150;
		
		floors[floor].position.x=(floor % 2) * 800;

		for (int button = 0; button < BUTTON_COUNT; ++button) {

			int x = (floor % 2) * 1140 + 20;
			int buttonY = floors[floor].position.y+floors[floor].position.h - (button + 1) * (BUTTON_SIZE + BUTTON_MARGIN);
			floors[floor].buttons[button].position = { x, buttonY, BUTTON_SIZE, BUTTON_SIZE };

			if (button >= floor)
			{
				floors[floor].buttons[button].destFloor = button + 1;
			}
			else
			{
				floors[floor].buttons[button].destFloor = button;

			}
		}
	}

	ele.position = { 420,600,360,150 };
	ele.targetFloor = 0;
	ele.lastMove = 0;
}
void Button::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 172, 172, 172, 255);

	SDL_RenderFillRect(renderer, &position);

}

void Floor::Draw(SDL_Renderer* renderer)
{
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(renderer, position.x, position.y+position.h, position.x+ position.w, position.y + position.h);
	for (Button& b : buttons)
	{
		b.Draw(renderer);
	}
}

void Passenger::Draw(SDL_Renderer* renderer, int x, int y)
{
	SDL_Rect ppl{ x,y - PPL_HEIGHT,PPL_WIDTH,PPL_HEIGHT };

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &ppl);


}

void Elevator::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 217, 2, 16, 255);

	SDL_RenderDrawRect(renderer, &position);

	for (int i = 0; i < passengers.size(); i++)
	{
		
		passengers[i].Draw(renderer, 430 + 40 * i ,position.y+ position.h);

	}


}



void Building::Draw(SDL_Renderer* renderer)
{
	int x = 0;
	int dx = 0;
	for (int i = 0; i < 5; i++)
	{
		floors[i].Draw(renderer);
		if (i % 2 == 0)
		{
			x = floors[i].position.x + floors[i].position.w - PPL_WIDTH - 10;
			dx = -PPL_WIDTH - 10;
		}
		else
		{
			x = floors[i].position.x +10;
			dx = PPL_WIDTH + 10;
		}
		for (Passenger& w : waiting)
		{
			if (w.startFloor == i)
			{
				w.Draw(renderer, x, floors[i].position.y + floors[i].position.h);
				x+=dx;
			}

		}
	}
	ele.Draw(renderer);

	SDL_Rect frame{400,0,400,750};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &frame);



}
void Building::Click(int mouseX, int mouseY)
{
	SDL_Point point = { mouseX, mouseY };

	for (int floor = 0; floor < FLOOR_COUNT; ++floor) {
		for (int button = 0; button < BUTTON_COUNT; ++button)
		{
			if (SDL_PointInRect(&point, &floors[floor].buttons[button].position))
			{

				waiting.push_back(Passenger{floors[floor].buttons[button].destFloor,floor});

			}

		}
	}
}



void Building::Update(int dt)
{


	ele.targetFloor = elevatorTarget();
	if (ele.position.y + ele.position.h == (5- ele.targetFloor)*150)
	{
		
		for(vector<Passenger>::iterator it = ele.passengers.begin(); it != ele.passengers.end(); )
		{
			if (it->destFloor == ele.targetFloor)
			{
				it = ele.passengers.erase(it);
			}
			else
				++it;
		}
		if (ele.passengers.empty())
		{
			for (int j = 0; j < waiting.size(); j++)
			{
				if (ele.targetFloor == waiting[j].startFloor)
				{
					ele.passengers.push_back(waiting[j]);
					waiting.erase(waiting.begin()+j);
					break;
				}
			}
		}

		//<--
		if (ele.passengers.empty())
		{
			ele.lastMove += dt;
			return ;
		}
		else
		{
			int des=floorDir(ele.passengers[0].destFloor);

			for (int j = 0; j < waiting.size() && ele.passengers.size()<8;)
			{
				if (waiting[j].startFloor == ele.targetFloor && floorDir(waiting[j].destFloor) == des)
				{
					ele.passengers.push_back(waiting[j]);
					waiting.erase(waiting.begin() + j);

				}
				else
					j++;
			}

		}
	}
	else
	{
		ele.position.y-=ELE_SPEED* floorDir(ele.targetFloor);
		
		if (floorDir(ele.targetFloor) != 0)
			ele.lastMove = 0;
	}

	
}
int Building::elevatorTarget()
{
	int min = 0;
	if (!ele.passengers.empty())
	{

		min = ele.passengers.front().destFloor;
		for (int i = 1; i < ele.passengers.size(); i++)
		{
			if (floorDist(min) > floorDist(ele.passengers[i].destFloor))
				min = ele.passengers[i].destFloor;

		}
		//[next..min)
		int dir = floorDir(min);
		for (int i = floorNext(dir); i < min; i++)
		{

			if (passengersWait(dir, i))
			{
				min = i;
				break;

			}

		}

	}
	else
	{
		if (!waiting.empty())
		{
			min = waiting[0].startFloor;
		}
		else
		{
			if (ele.lastMove > 5000)
				min = 0;
			else
				min = ele.targetFloor; 
		}
	}
	return min;
}
bool Building::passengersWait(int dir,int floorNumber)
{
	for (int j = 0; j < waiting.size(); j++)
	{
		if ((waiting[j].destFloor - waiting[j].startFloor) * dir >= 0 && floorNumber == waiting[j].startFloor)
		{
			return true;
		}
	}
	return false;
}
int Building::floorNext(int dir)
{
	int next = 0;
	float pom = 5 - (ele.position.y + ele.position.h) / 150;
	if (dir > 0)
		next = ceil(pom);
	else
		next = floor(pom);

	return next;
}
int Building::floorDir(int floorNumber)
{
	int x = ele.position.y - 600 + floorNumber * 150;
	if (x < 0)
		return -1;
	else if (x > 0)
		return 1;
	else
		return 0;
}

int Building::floorDist(int floorNumber)
{
	return
		abs(ele.position.y - 600 + floorNumber * 150);
}
