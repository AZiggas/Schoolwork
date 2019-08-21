#pragma once
#include "SDL.h"
#include "Metrics.h"
#include <string>
#include <vector>

class TextRenderer{
    public:
    SDL_Surface* texture;
    std::vector<SDL_Rect> metrics;
    
    TextRenderer(){
        int RED=0x0000ff;
        int GREEN=0x00ff00;
        int BLUE=0xff0000;
        int ALPHA=0xff000000;
 
        this->texture = SDL_LoadBMP("font.bmp");
        SDL_PixelFormat pixf;
        pixf.format = SDL_PIXELFORMAT_ABGR8888; 
        pixf.palette = NULL;
        pixf.BitsPerPixel = 32;
        pixf.BytesPerPixel = 4;
        pixf.Rmask = RED;
        pixf.Gmask = GREEN;
        pixf.Bmask = BLUE;
        pixf.Amask = ALPHA;
        this->texture = SDL_ConvertSurface(this->texture,&pixf,0);
        SDL_SetColorKey(this->texture,1,0xff010101);

        this->metrics.resize(256);
        int msize = sizeof(Metrics)/sizeof(Metrics[0]);
        for(int i=0;i<msize;i+=5){
            int ascii=Metrics[i];
            int x=Metrics[i+1];
            int y=Metrics[i+2];
            int w=Metrics[i+3];
            int h=Metrics[i+4];
            SDL_Rect r;
            r.x=x;
            r.y=y;
            r.w=w;
            r.h=h;
            this->metrics[ascii] = r;
        }
    }
    
    void draw(SDL_Surface* win,std::string txt,  int x, int y, int color[]){
        int r=color[0];
        int g=color[1];
        int b=color[2];
        SDL_SetSurfaceColorMod(this->texture,r,g,b);
        
        int ox=x;
        
        SDL_Rect dr;
        dr.y=y;
        
        for(unsigned i=0;i<txt.length();++i){
            char c=txt[i];
            if(c == '\n'){
                x=ox;
                y += this->metrics[32].h;
                dr.y=y;
            }
            else {
                SDL_Rect* sr = &(this->metrics[c]);
                dr.x=x;
                dr.w=sr->w;
                dr.h=sr->h;
                SDL_BlitSurface(this->texture, sr, win, &dr);
                x += sr->w;
            }
        }
    }
};
