#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "gfx2.h"
#include <unistd.h>
// including libraries


// defining macros
#define BALL_AMT 15
#define X_Ball 30
#define Y_Ball 550

// Andre Mayard, project.c
// This program simulates a hoop game with no backboard, the user controlls the power (with some rng) as well as the angle that the ball takes (no rng)


typedef struct {
	float angle;
	int x;
	int y;
	bool made;
} Bball;
// struct containing position of ball, whether it is made or not, and the angle it is launched from.



void placeGoal(int x, int y); // places the goal on the screen
void placeBall(int x, int y, int rad); // places the ball on the screen
int launch(Bball ball, float power, int rad,int x, int y, int w, int h, int makes, int ct); // launches the ball and updates the screen
float placeArrow(int xCursor, int yCursor, int x, int y, int rad, int makes, int ct); // places the arrow that the user controls the direction with 
void printMakes(int makes, int total, int last); // prints the level and other things depending on if it is the last or not

int main()
{
	int wd = 800, ht = 600;
	char c;
	Bball ballArray[BALL_AMT]; // make an array of ball structs
	int ballCt = 0; // set the count of the ball to 0
	int rad = 15;	//radius of ball
	float theta = -M_PI/4; // angle for launch
	int last = 0; // last is only 0 when the levels are all complete

	gfx_open(wd,ht, "Hoop Game");
	gfx_clear_color(115,215,255); // sky blue color for background
	gfx_clear();

	int x_cur, y_cur; // for obtaining where the user's cursor is
	int make, makeCt= 0; // makeCt initialized to 0, make is for the launch function

	while (1) {
		if (ballCt > BALL_AMT-1){ // ballCt is the index, so if it is 15, then it shouldn't go
			last = 1; 
			printMakes(makeCt,ballCt,last);
			usleep(5000000); // gives 5 seconds to read the comments on the game
			break;
		}
		
		ballArray[ballCt].angle = theta; // setting initial conditions for current ball
		ballArray[ballCt].made = false;

		printMakes(makeCt, ballCt+1, last); // ballCt + 1 based on structure of printmakes
		srand(time(NULL)); // randomizing the seed each time through

		int x, y; // generates a place on a grid of where the hoop goes
		x = rand()%500 + 100;// not so far to the right or so far to the left
		y = rand()%400 + 50;// not so high or so low;

		ballArray[ballCt].x = X_Ball; // set the ball to be where the ball is initially
		ballArray[ballCt].y = Y_Ball;

		placeGoal(x,y);
		placeBall(ballArray[ballCt].x, ballArray[ballCt].y, rad);

		gfx_color(0,0,0);
		gfx_line(X_Ball,Y_Ball, ballArray[ballCt].x + rad + 30, ballArray[ballCt].y - rad - 30); // show an arrow 45 degrees (initial angle before input by user)

		while (1) {
			if (ballArray[ballCt].made) break;
			float power; // set power for user to control

			c = gfx_wait();

			if (c == 1) { // user sets where the arrow points to, theta is returned
				x_cur = gfx_xpos();
				y_cur = gfx_ypos();
				theta = placeArrow(x_cur,y_cur, x, y, rad, makeCt, ballCt + 1);
				ballArray[ballCt].angle = theta;

			} else if (c == 'r') { // r for restart, if the user presses r, the level increments by 1 and it counts as a miss
				ballCt++;
				break;
			} else if (c > '0' && c <= '7') { // user chooses number between 1 and 7

				// effectively, 2*userInput*(random number between 1.5 and 2)
				power = 2*(c - '0')*(1.5+ (float)rand()/RAND_MAX*.5); // make the power they set to have rng, it's not always going to be the same shot even if you press the same number and shoot the same way.

				// launch the ball and return 1 if it is made and 0 if it is not (hits the bottom of screen)
				make = launch(ballArray[ballCt], power, rad, x, y, wd, ht, makeCt, ballCt + 1);
					
				if (make > 0) {
					ballArray[ballCt].made = true;
				}
					
				makeCt += make;
				ballCt++;
				break;
			}
		}
		gfx_clear();
	}	




  return 0;
}




void placeGoal(int x, int y)
{
	gfx_color(208, 27, 41); // red rim
	gfx_fill_rectangle(x, y, 80, 10);
	
	gfx_color(255,255,255); // white net
	gfx_line(x,y+10,x+20,y+50);
	gfx_line(x+20,y+50,x+60,y+50);
	gfx_line(x+60,y+50,x+80,y+10);
	
	gfx_line(x+20,y+10,x+20,y+50);
	gfx_line(x+40,y+10,x+40,y+50);
	gfx_line(x+60,y+10,x+60,y+50);

}


void placeBall(int x, int y, int rad)
{
	gfx_color(228,112,65); // basketball orange
	gfx_fill_circle(x,y,rad); // gfx2 function
}

float placeArrow(int xCursor, int yCursor, int x, int y, int rad, int makes, int ct)
{
	
	gfx_clear(); // clear for the arrow already placed to go away

	placeGoal(x,y); // placing the other stuff that has to be there
	placeBall(X_Ball,Y_Ball,rad);
	printMakes(makes,ct, 0);

	float theta;
	gfx_color(0,0,0);
	gfx_line(X_Ball,Y_Ball, xCursor, yCursor); // ball to cursor line placement
	

	theta = atan2(yCursor-Y_Ball,xCursor - X_Ball); // atan2 accounts for all quadrants
	if (theta < 0) {
		theta += 2*M_PI; // be in the range of 0 and 2PI
	}

	return theta;
}


int launch(Bball ball, float power, int rad, int xGoal, int yGoal, int wd, int ht, int makes, int ct)
{
	float x = ball.x, y = ball.y; //setting initial ball placement
	float t = 0.015; // setting initial time

	float dx = power*cos(ball.angle); //velos
	float dy = power*sin(ball.angle);

	gfx_clear();

	while (1) {
		printMakes(makes, ct, 0); // keep levels on the screen
		
		if (ball.made) {
			return 1;
		}	

		x += dx; // change in x
			
		dy += 4.9*t; // increments dy by acc due to gravity
		y += dy; // increments y by dy

		placeBall(x,y,rad); // place the ball where x and y are now
		placeGoal(xGoal,yGoal); // place the goal where it has been
	
		gfx_flush(); // flush to the screen
		usleep(30000);
	

		// for the y value, the center of the ball has to be within the rim
		// for the x comparison,  both sides of the ball have to be within the rim
		if ((y + rad >= yGoal + 10) && (y - rad <= yGoal)&& (x-rad >= xGoal) && (x + rad <= xGoal + 80) && (dy > 0)) {
			ball.made = true;
			return 1;
		} 
		
		// for bouncing
		if (x + rad >= wd) {
			x = wd - rad - 1;
			dx = -dx;
		} else if (x - rad <= 0) {
			x = rad + 1;
			dx = -dx;
		}
	
		// if it hits the ceiling, it bounces, if it hits the floor its not a make
		if (y - rad <= 0) {
			y = rad + 1;
			dy = -dy;
		} else if ( y + rad >= ht) {
			ball.made = false;
			return 0;
		}
		t += 0.005;
		gfx_clear();
	}

}


void printMakes(int makes, int total, int last)
{
	gfx_color(228,112,65); // bball color
	char text[50];
	char firstGo[1000];
	gfx_rectangle(0,0,250,30);

	// first statement does not go through for the last call (this is for adjusting the fact that the level is +1 of the total count until the last one is all the way through
	if (total != 15 || last != 1) {
		 sprintf(text, "Level %d - Current: %d/%d - %g%%", total, makes, total - 1, (float)makes/((float)(total-1))*100);
	} else {
		 sprintf(text, "Level %d - Current: %d/%d - %g%%", total, makes, total, (float)makes/((float)(total))*100);
	}		

	// if it's the users first shot
	if (total == 1) {
		sprintf(firstGo, "Welcome to the Hoop Game!");
		gfx_text(300,10,firstGo);
		gfx_text(300,25, "The hoop is randomly placed level to level");
		gfx_text(300,40, "It is your job to choose your direction by");
		gfx_text(300,55, "clicking where you want the ball to go.");
		gfx_text(300,70,"Choose a number 1-7 to determine the power!");
		gfx_text(300, 85, "You get 15 tries!");
		gfx_text(300,100,"Be careful, gravity and collisions with borders are in effect!");
		gfx_text(300,115, "Get the ball fully in the hoop!");

	// for after the last shot, gives the user feedback on how they did
	} else if ( total == 15 && last == 1) {
		if (2*makes >= total) {
			gfx_text(300,10,"Good Job! You got higher than 50%! Genuinely insane work. Touch grass.");
		} else if (makes == 0) {
			gfx_text(300,10,"I don't know what to say. Maybe next time no goose egg?");
		} else if (makes*3 >= total) {
			gfx_text(300,10, "Decent work that I respect.");
		} else {
			gfx_text(300,10, "Meh. Anyone could get that score.");
		}
	}
	// gives results
	gfx_text(2,15, text);
	if (total == 15) gfx_flush(); // flushes if it is the last one so the user can see
	
}
