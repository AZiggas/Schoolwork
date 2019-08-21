
#include "SDL.h"
#include <sstream>
#include <string>
#include <assert.h>

class Card {
public:
	static const int WIDTH = 97;
	static const int HEIGHT = 129;
	static SDL_Surface* texture;
	static SDL_Surface* cardback;
	static const int FACE_DOWN = 0;
	static const int FACE_UP = 1;
	static const int TURNING_UP = 2;
	static const int TURNING_DOWN = 3;
	static const int FADING_OUT = 4;
	static const int DIAMONDS = 0;
	static const int HEARTS = 1;
	static const int SPADES = 2;
	static const int CLUBS = 3;
	static const int RED = 0x0000ff;
	static const int GREEN = 0x00ff00;
	static const int BLUE = 0xff0000;
	static const int ALPHA = 0xff000000;

	int rank, suit;
	int x, y, state;
	int progress;
	int movespeed;
	int destination[2];
	SDL_Surface* spr;

	Card* clone() {
		Card* c = new Card(rank, suit);
		*c = *this;
		return c;
	}

	~Card() {
		if (spr)
			SDL_FreeSurface(spr);
		spr = 0;
	}

	Card(const Card& c) {
		spr = 0;
		*this = c;
	}

	Card(int rank, int suit) {
		spr = 0;
		init(rank, suit, 0, 0, Card::FACE_DOWN, 0, 0, 0, 0);
	}

	void operator=(const Card& c) {
		init(c.rank, c.suit, c.x, c.y, c.state, c.progress, c.movespeed, c.destination[0],
			c.destination[1]);
	}

	void init(int rank, int suit,
		int x, int y, int state, int progress,
		int movespeed, int destx,
		int desty) {

		if (spr)
			SDL_FreeSurface(spr);
		spr = 0;

		this->rank = rank;
		this->suit = suit;
		this->x = x;
		this->y = y;
		this->state = state;
		this->progress = progress;
		this->movespeed = movespeed;
		this->destination[0] = destx;
		this->destination[1] = desty;

		if (Card::texture == NULL) {
			Card::texture = SDL_LoadBMP("allcards.bmp");
			assert(Card::texture);
			SDL_PixelFormat pixf;
			pixf.format = SDL_PIXELFORMAT_ABGR8888;
			pixf.palette = NULL;
			pixf.BitsPerPixel = 32;
			pixf.BytesPerPixel = 4;
			pixf.Rmask = Card::RED;
			pixf.Gmask = Card::GREEN;
			pixf.Bmask = Card::BLUE;
			pixf.Amask = Card::ALPHA;
			Card::texture = SDL_ConvertSurface(Card::texture, &pixf, 0);
			SDL_SetColorKey(Card::texture, 1, 0x010101);

			Card::cardback = SDL_CreateRGBSurface(0, Card::WIDTH, Card::HEIGHT,
				32, Card::RED, Card::GREEN, Card::BLUE, Card::ALPHA);
			SDL_Rect r;
			r.x = 4 * Card::WIDTH;
			r.y = 4 * Card::HEIGHT;
			r.w = Card::WIDTH;
			r.h = Card::HEIGHT;
			SDL_BlitSurface(Card::texture, &r, Card::cardback, NULL);
		}

		this->spr = SDL_CreateRGBSurface(0, Card::WIDTH, Card::HEIGHT, 32,
			Card::RED, Card::GREEN, Card::BLUE, Card::ALPHA);
		SDL_Rect r;
		r.x = ((rank == 14) ? 1 : ((rank - 1)*Card::WIDTH));
		r.y = suit*Card::HEIGHT;
		r.w = Card::WIDTH;
		r.h = Card::HEIGHT;
		SDL_BlitSurface(Card::texture, &r, this->spr, NULL);
	}

	std::string toString() {
		std::ostringstream oss;
		oss << "{" << rank << " " << suit << "}";
		return oss.str();
	}

	void setState(int state) {
		this->state = state;
	}


	void setProgress(int p) {
		this->progress = p;
	}

	void setPosition(int x, int y) {
		this->x = x;
		this->y = y;
	}


	void moveTo(int x, int y, int speed = 4) {
		movespeed = speed;
		destination[0] = x;
		destination[1] = y;
	}

	void update() {
		if (movespeed != 0) {
			int dx = destination[0] - x;
			int dy = destination[1] - y;
			if (dx == 0 && dy == 0)
				movespeed = 0;
			if (dx < -movespeed)
				dx = -movespeed;
			if (dy < -movespeed)
				dy = -movespeed;
			if (dx > movespeed)
				dx = movespeed;
			if (dy > movespeed)
				dy = movespeed;
			x += dx;
			y += dy;
		}


		if (this->state == Card::TURNING_DOWN || this->state == Card::TURNING_UP) {
			this->progress += 10;
			if (this->progress >= 100) {
				this->progress = 0;
				if (this->state == Card::TURNING_UP) {
					this->state = Card::FACE_UP;
				}
				else {
					this->state = Card::FACE_DOWN;
				}
			}
		}
		else if (this->state == Card::FADING_OUT) {
			this->progress += 10;
			if (this->progress >= 100) {
				this->progress = 100;
			}
		}
	}

	bool isFaceUp() {
		return (this->state == Card::FACE_UP || this->state == Card::TURNING_UP);
	}

	bool containsPoint(int x, int y) {
		if (this->state == Card::FADING_OUT) {
			return false;
		}
		if (x >= this->x && y >= this->y && x <= this->x + Card::WIDTH && y <= this->y + Card::HEIGHT) {
			return true;
		}
		else {
			return false;
		}
	}

	void flip() {
		if (this->state == Card::FACE_DOWN) {
			this->state = Card::TURNING_UP;
			this->progress = 0;
		}
		else if (this->state == Card::FACE_UP) {
			this->state = Card::TURNING_DOWN;
			this->progress = 0;
		}
		else if (this->state == Card::TURNING_UP) {
			this->state = Card::TURNING_DOWN;
			this->progress = 100 - this->progress;
		}
		else if (this->state == Card::TURNING_DOWN) {
			this->state = Card::TURNING_UP;
			this->progress = 100 - this->progress;
		}
	}

	void fadeOut() {
		this->state = Card::FADING_OUT;
		this->progress = 0;
	}

	void draw(SDL_Surface* win) {
		float sx = 1.0;
		float alpha = 1.0;
		int x = this->x;
		int y = this->y;
		SDL_Surface* s;

		if (this->state == Card::FADING_OUT) {
			s = this->spr;
			alpha = (1.0 - this->progress / 100.0);
		}
		else {
			if (this->state == Card::FACE_DOWN || (this->state == Card::TURNING_UP && this->progress < 50) || (this->state == Card::TURNING_DOWN && this->progress > 50)) {
				s = Card::cardback;
			}
			else {
				s = this->spr;
			}

			if (this->progress < 50) {
				sx = (50 - this->progress) / 50.0;
			}
			else {
				sx = (this->progress - 50) / 50.0;
			}
		}

		if (alpha == 0.0) {
			return;
		}

		SDL_SetSurfaceAlphaMod(s, (alpha * 255));

		SDL_Rect r;
		r.x = x;
		r.y = y;
		r.w = (sx * Card::WIDTH);
		r.h = Card::HEIGHT;
		if (sx != 1.0) {
			r.x += (Card::WIDTH / 2 - sx*Card::WIDTH / 2);
			SDL_BlitScaled(s, NULL, win, &r);
		}
		else {
			SDL_BlitSurface(s, NULL, win, &r);
		}
	}
};

