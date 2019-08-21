#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Card.h"
#include "TextRenderer.h"
#include <string>
#include <vector>
#include <sstream>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include "common.h"
#include <sstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <assert.h>
#include <string>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

class Player {
public:
	string Name;
	int Place;
	string Position;
	int Cheat;
	int Turn;
	string Score;
	int nameX;
	int nameY;

	Player() {

	}

	Player(string name_, int place_, string position_, int nameX_, int nameY_) {
		Name = name_;
		Place = place_;
		Position = position_;
		Cheat = 0;
		Turn = 0;
		nameX = nameX_;
		nameY = nameY_;
		Score;
	}
};


vector<Card> splitCards(string cardList){
	size_t pos = 0;
	string token;
	int a;
	int b;

	cardList.erase(0, 15);
	cardList.erase(cardList.end() - 2, cardList.end());

	vector<Card> hand;
	int cardPos = 240;
	while ((pos = cardList.find(",(")) != string::npos) {
		token = cardList.substr(0, pos);
		a = stoi(token.substr(1, token.find(',') - 1));
		b = stoi(token.substr(token.find(',') + 1));

		hand.push_back(Card(a, b));
		hand.back().setPosition(cardPos, 500);
		hand.back().flip();
		cardPos += 100;
		cardList.erase(0, pos + 1);
	}
	a = stoi(cardList.substr(1, cardList.find(',') - 1));
	b = stoi(cardList.substr(cardList.find(',') + 1));
	hand.push_back(Card(a, b));
	hand.back().setPosition(cardPos, 500);
	hand.back().flip();
	return hand;
}



int main(int argc, char* argv[])
{
#ifdef WIN32
	WORD wsVersion;     //version we want
	WSADATA dat;        //data for winsock
	int rv;
	wsVersion = MAKEWORD(2, 2); //we want version 2.2
	rv = WSAStartup(wsVersion, &dat);
	if (rv)
		exit(1);
	if (LOBYTE(dat.wVersion) != 2 || HIBYTE(dat.wVersion) != 2) {
		WSACleanup();
		exit(1);
	}
#endif

	int s = socket(AF_INET, SOCK_STREAM, 0);

	if (s < 0)
		return 1;
	hostent* he = gethostbyname("206.21.94.117");
	if (!he)
		return 1;

	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(7654);
	memcpy(&sa.sin_addr.s_addr, he->h_addr, sizeof(sa.sin_addr));

	if (connect(s, (sockaddr*)&sa, sizeof(sa)) < 0) {
		perror("foolish");
		return 1;
	}

	string nameInput = argv[1];
	std::string name = "Login: " + nameInput + "\n";
	sendall(s, name.c_str(), name.length());

	string cards;
	string position;
	int i = 0;
	while (i < 2) {
		if (i == 0) {
			cards = readline(s);
		}
		if (i == 1) {
			position = readline(s);
		}
		i++;
	}

	cout << cards << endl;
	cout << position << endl;

	position.erase(0, 10);
	position.erase(position.end() - 1, position.end());

	size_t pos = 0;
	string token;

	vector<Card> hand = splitCards(cards);


	cout << position << endl;

	string people = position.substr(1, position.find(')') - 1);
	position.erase(0, position.find(')'));

	cout << position << endl;
	int place = stoi(position.substr(position.find(' ') + 1));
	cout << place << endl;

	vector<string> names;
	while ((pos = people.find(',')) != string::npos) {
		token = people.substr(1, pos - 2);
		names.push_back(token);
		//cout << token << endl;
		people.erase(0, pos + 1);
	}
	token = people.substr(1, people.length() - 2);
	names.push_back(token);
	//cout << token << endl;

	//Visuals and stuff
	SDL_Init(SDL_INIT_VIDEO);

	string myname = argv[1];
	int rPlay;
	int tPlay;
	int lPlay;
	//Enable for 4 players.
	vector<Player> players;

	Player bottom;
	Player right;
	Player top;
	Player left;
	if (place == 0){
		bottom = Player(names.at(0), 0, "bottom", 565, 650);
		right = Player(names.at(1), 1, "right", 1000, 388);
		top = Player(names.at(2), 2, "top", 565, 125);
		left = Player(names.at(3), 3, "left", 250, 388);

		players.push_back(bottom);
		players.push_back(right);
		players.push_back(top);
		players.push_back(left);

		rPlay = 1;
		tPlay = 2;
		lPlay = 3;
	}
	else if (place == 1) {
		bottom = Player(names.at(1), 1, "bottom", 565, 650);
		right = Player(names.at(2), 2, "right", 1000, 388);
		top = Player(names.at(3), 3, "top", 565, 125);
		left = Player(names.at(0), 0, "left", 250, 388);
		
		players.push_back(left);
		players.push_back(bottom);
		players.push_back(right);
		players.push_back(top);

		rPlay = 2;
		tPlay = 3;
		lPlay = 0;
	}
	else if (place == 2) {
		bottom = Player(names.at(2), 2, "bottom", 565, 650);
		right = Player(names.at(3), 3, "right", 1000, 388);
		top = Player(names.at(0), 0, "top", 565, 125);
		left = Player(names.at(1), 1, "left", 250, 388);
				
		players.push_back(top);
		players.push_back(left);
		players.push_back(bottom);
		players.push_back(right);

		rPlay = 3;
		tPlay = 0;
		lPlay = 1;
	}
	else if (place == 3) {
		bottom = Player(names.at(3), 3, "bottom", 565, 650);
		right = Player(names.at(0), 0, "right", 1000, 388);
		top = Player(names.at(1), 1, "top", 565, 125);
		left = Player(names.at(2), 2, "left", 250, 388);

		players.push_back(right);
		players.push_back(top);
		players.push_back(left);
		players.push_back(bottom);

		rPlay = 0;
		tPlay = 1;
		lPlay = 2;
	}

	//cout << "Left: " << lPlay << " Right: " << rPlay << " Top: " << tPlay << endl;

	TextRenderer tr;
	SDL_Window* win = SDL_CreateWindow("ETEC", 10, 10, 1280, 720, 0);
	SDL_Surface* surf = SDL_GetWindowSurface(win);

	SDL_Event ev;
	int turnFlag = 0;
	int endFlag = 0;
	int winFlag = 0;
	string score1;
	string score2;
	string score3;
	string score4;

	vector<Card> playedCards;
	while (true) {
		fd_set r;
		FD_ZERO(&r);
		FD_SET(s, &r);
		struct timeval to;
		to.tv_sec = 0;
		to.tv_usec = 0;
		//timeout = instaneous
		select(s + 1, &r, NULL, NULL, &to);
		string read;
		vector<string> text;
		vector<Card> oldHand;
		if (FD_ISSET(s, &r)) {
			//read the stuff from socket.
			read = readline(s);
			cout << read << endl;
			text = split(read, " ", 5);
			//CARD? : When It's my turn
			//PLAY (Player #) (RANK #) (SUIT #)
			//Will also send that when it I play the a card.

			if (text.at(0) == "CARD?") {
				//My Turn
				turnFlag = 1;
				players.at(place).Turn = 1;

			}
			else if (text.at(0) == "PLAY") {
				//Someone Played

				int a = atoi(text.at(2).c_str());
				int b = atoi(text.at(3).c_str());

				int checkPos = atoi(text.at(1).c_str());
				if (checkPos != place){
					playedCards.push_back(Card(a, b));
					cout << "A: " << a << "B: " << b << "PLACE: " << checkPos << endl;
					if (checkPos == lPlay) {
						playedCards.back().setPosition(-100, 260);
						playedCards.back().flip();
						playedCards.back().moveTo(480, 260, 8);
					}
					else if (checkPos == tPlay) {
						playedCards.back().setPosition(640, -100);
						playedCards.back().flip();
						playedCards.back().moveTo(640, 150, 8);
					}
					else if (checkPos == rPlay) {
						playedCards.back().setPosition(1380, 260);
						playedCards.back().flip();
						playedCards.back().moveTo(800, 260, 8);
					}
					else {
						cout << "SOMETHING IS WRONG WITH POSITION!" << endl;
					}
				}
			}
			else if (text.at(0) == "SCORE") {
				players.at(0).Score = text.at(1);
				players.at(1).Score = text.at(2);
				players.at(2).Score = text.at(3);
				players.at(3).Score = text.at(4);

				if (endFlag) {
					cout << "SCORES: " << endl;
					cout << players.at(0).Name << ": " << players.at(0).Score << endl;
					cout << players.at(1).Name << ": " << players.at(1).Score << endl;
					cout << players.at(2).Name << ": " << players.at(2).Score << endl;
					cout << players.at(3).Name << ": " << players.at(3).Score << endl;
					break;

				}
			}
			else if (text.at(0) == "WIN") {
				//So and so wins the round
				//WIN #
				int x;
				int y;
				if (players.at(atoi(text.at(1).c_str())).Position == "bottom") {
					x = 640;
					y = 820;
				}
				else if (players.at(atoi(text.at(1).c_str())).Position == "right") {
					x = 1380;
					y = 360;
				}
				else if (players.at(atoi(text.at(1).c_str())).Position == "top") {
					x = 640;
					y = -150;
				}
				else if (players.at(atoi(text.at(1).c_str())).Position == "left") {
					x = -100;
					y = 360;
				}

				for (unsigned j = 0; j < playedCards.size(); ++j) {
					Card& ca = playedCards[j];
					ca.moveTo(x, y, 10);
				}

				


			}
			else if (text.at(0) == "END") {
				//Expect A Score Call
				//Game Ends, Print out player scores, exit program
				endFlag = 1;
			}
			else if (text.at(0) == "Card_Details:") {
				hand.clear();
				playedCards.clear();


				//This below is causing crashes
				oldHand = hand;
				hand = splitCards(read);
			}
			else if (text.at(0) == "CHEAT") {
				players.at(atoi(text.at(1).c_str())).Cheat = 1;

				for (unsigned i = 0; i < hand.size(); ++i) {
					Card& c = hand[i];
					c.fadeOut();
				}
				for (unsigned j = 0; j < playedCards.size(); ++j) {
					Card& ca = playedCards[j];
					ca.fadeOut();
				}


			}
		}
		while (true) {
			int rv = SDL_PollEvent(&ev);
			if (rv == 0)
				break;
			if (ev.type == SDL_QUIT) {
				return 0;
			}
			else if (ev.type == SDL_MOUSEMOTION) {
			}
			else if (ev.type == SDL_MOUSEBUTTONDOWN) {
				int x = ev.button.x;
				int y = ev.button.y;
				Card* cc = NULL;
				for (unsigned i = 0; i < hand.size(); ++i) {
					Card* c = &(hand[i]);
					if (c && c->containsPoint(x, y)) {
						cc = c;

						//moveto this card to send this to center and tell server that you have played.
						//set played flag.
						//
						if(turnFlag == 1) {
							ostringstream send;
							send << "PLAY " << place << " " << hand[i].rank << " " << hand[i].suit << endl;
							string out = send.str();
							//cout << out << endl;
							sendall(s, out.c_str(), out.length());

							hand[i].moveTo(640, 350, 8);
							playedCards.push_back(hand[i]);
							hand.erase(hand.begin() + i);

							players.at(place).Turn = 0;
							turnFlag = 0;
						}
						break;
					}
				}
				if (cc) {
					cout << cc->toString() << "\n";
				}
			}
			else if (ev.type == SDL_MOUSEBUTTONUP) {
			}
		}

		SDL_FillRect(surf, NULL, 0xff109020);

		for (unsigned i = 0; i < hand.size(); ++i) {
			Card& c = hand[i];
			c.update();
			c.draw(surf);
		}
		for (unsigned j = 0; j < playedCards.size(); ++j) {
			Card& ca = playedCards[j];
			ca.update();
			ca.draw(surf);
		}

		int color1[] = { 0,0,0,255 };
		int color2[] = { 0, 200, 200, 255 };
		int color3[] = { 200, 0, 0, 0 };
		//Enable all of these for 4 players.

		int scorePos = 100;
		for (Player p : players) {
			if (p.Turn) {
				tr.draw(surf, p.Name, p.nameX, p.nameY, color2);
			}
			else if (p.Cheat) {
				tr.draw(surf, p.Name, p.nameX, p.nameY, color3);
			}
			else {
				tr.draw(surf, p.Name, p.nameX, p.nameY, color1);
			}

			tr.draw(surf, p.Position + ": " + p.Score, 100, scorePos, color1);
			scorePos = scorePos + 50;
		}


		SDL_UpdateWindowSurface(win);
		SDL_Delay(20);
	}


	//cleanup
	closesocket(s);
	WSACleanup();

	Sleep(1000);
	return 1;
}

SDL_Surface* Card::texture;
SDL_Surface* Card::cardback;

