#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_timer.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define WIDTH 500
#define HEIGHT 800

int noclipf = 0;

int checkcollision(SDL_Rect a, SDL_Rect b){
 if (!noclipf) {
 
 
  return (a.x < b.x + b.w &&
          a.x + a.w > b.x &&
          a.y < b.y + b.h &&
          a.y + a.h > b.y);
  }else {
  return 0;
  }
}


int main(int argc, char *argv[])
{

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL init didnt worked: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }
  
  if (!IMG_Init(IMG_INIT_PNG)) {
  printf("failed to load IMG: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }
  
  
  // On The Lowes window creation

  SDL_Window* window = SDL_CreateWindow("Flappy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Window* devtool = SDL_CreateWindow("devtools", 100, 100, 300, 300, SDL_WINDOW_SHOWN);
  SDL_Renderer* toolsrender = SDL_CreateRenderer(devtool, -1, SDL_RENDERER_ACCELERATED);
  SDL_Renderer* mainrender = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


  SDL_Surface* SkySurf = IMG_Load("../assets/sky.png");
  if (!SkySurf) {
  printf("Step brother I am stuck here!: %s\n", SDL_GetError());
  SDL_Quit();
  }
  SDL_Texture* SkyTexture = SDL_CreateTextureFromSurface(mainrender, SkySurf);
  if (!SkyTexture) {
  printf("Step brother I am stuck here! %s\n", SDL_GetError());
  }
  SDL_Surface* playerskin = IMG_Load("../assets/player.png");
  SDL_Texture* playertexture = SDL_CreateTextureFromSurface(mainrender, playerskin);
 
  SDL_Surface* playerdownsurface = IMG_Load("../assets/pipedown.png");
  SDL_Texture* playerdowntexture = SDL_CreateTextureFromSurface(mainrender, playerdownsurface);
  
 
  SDL_Surface* playertopsurface = IMG_Load("../assets/pipetop.png");
  SDL_Texture* playertoptexture = SDL_CreateTextureFromSurface(mainrender, playertopsurface);
  
  // Shame on me for this but this is variables that store player pipes and etc. on the top Sky variables

  SDL_Rect sky = { 0, 0, WIDTH, HEIGHT};
  SDL_Rect sky2 = {WIDTH, 0, WIDTH, HEIGHT};
  SDL_Rect noclip = {0, 0, 300, 50};
  SDL_Rect noclipbutton = {10, 10, 30, 30};

  SDL_Rect pipedown = {WIDTH, HEIGHT/2, 100, 1000};
  SDL_Rect pipetop = {WIDTH, 0, 100, 1000};
  SDL_Rect pipedown2 = {WIDTH + 200, HEIGHT/2, 100, 1000};
  SDL_Rect pipetop2 = {WIDTH + 200, 0, 100, 1000};
  SDL_Rect pipedown3 = {WIDTH + 450, HEIGHT/2, 100, 1000};
  SDL_Rect pipetop3 = {WIDTH + 450, 0, 100, 1000};
  
  SDL_Rect player = {WIDTH/2 - 16, HEIGHT/2 - 16, 32, 32};
  
  float player_velocity = 0.0f;
  float gravity = 0.0f;

  float gamespeed;

  int gameloop = 1;
  int restartmenu = 0;
  SDL_Event event;
  int canjump = 1;


  Uint32 lastTime  = SDL_GetTicks();
  float deltatime;

// This is important as we know it will randomize all pipes height  

  srand(time(NULL));
   pipedown.y = (HEIGHT / 2) + (rand() % ((HEIGHT - 100) - ( HEIGHT / 2))); 
   pipetop.y = pipedown.y  - 1200;
   pipedown2.y = (HEIGHT / 2) + (rand() % ((HEIGHT - 100) - ( HEIGHT / 2))); 
   pipetop2.y = pipedown2.y  - 1200;
   pipedown3.y = (HEIGHT / 2) + (rand() % ((HEIGHT - 100) - ( HEIGHT / 2))); 
   pipetop3.y = pipedown3.y  - 1200;

  while (gameloop) {
        float currentTime = SDL_GetTicks();
        deltatime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
    
        const Uint8* keyboardstate = SDL_GetKeyboardState(NULL);
    
        // This is for starter part and is important for starter pause
        // also on lower it is for player inputs
    
        if (keyboardstate[SDL_SCANCODE_SPACE]) {
        gamespeed = 200.0f;
        gravity = 1200.0f;
        }
    
        player_velocity += gravity * deltatime;
        player.y += (int)(player_velocity * deltatime);
        if (player_velocity > 200) player_velocity = 200;
    
        // As you see it is for keyboard events
    
          while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
              gameloop = 0;
             }
         if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE && canjump) {
              player_velocity = -400;
              canjump = 0;
            }
          if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
          canjump = 1;
            }
          if (event.type == SDL_MOUSEBUTTONDOWN) {
          int x, y;
          SDL_GetMouseState(&x, &y);
            if (!noclipf && x >= noclipbutton.x && x <= noclipbutton.x + noclipbutton.w && y >= noclipbutton.y && y <= noclipbutton.y + noclipbutton.h) {
              noclipf = 1;
            }else if (noclipf && x >= noclipbutton.x && x <= noclipbutton.x + noclipbutton.w && y >= noclipbutton.y && y <= noclipbutton.y + noclipbutton.h) {
              noclipf = 0;
            }
          }
          }
    
        // as you see it is for collision check
    
        if (checkcollision(player, pipedown)) {
        gameloop = 0;
        }
        if (checkcollision(player, pipetop)) {
        gameloop = 0;
        }
        if (checkcollision(player, pipedown2)) {
        gameloop = 0;
        }
        if (checkcollision(player, pipetop2)) {
        gameloop = 0;
        }
        if (checkcollision(player, pipedown3)) {
        gameloop = 0;
        }
        if (checkcollision(player, pipetop3)) {
        if(!noclipf) gameloop = 0;
        }
        if (player.y + player.h > HEIGHT) {
          if (!noclipf) gameloop = 0;
        player.y = HEIGHT - player.h;
        }
        if (player.y < 0) {
        if(!noclipf) gameloop = 0;
        player.y = 0;
        }   
    // This is a sky & pipes animation and movement
    
        sky.x -=(int)((gamespeed * deltatime)*1.2);
        sky2.x -=(int)((gamespeed * deltatime)*1.2);
        if (sky.x <= -WIDTH) {
          sky.x = sky2.x + WIDTH;
        }
    
        if (sky2.x <= -WIDTH) {
          sky2.x = sky.x + WIDTH;
        }
        pipedown.x -= (int)(gamespeed * deltatime);
        if (pipedown.x <= -100) {
          pipedown.x = WIDTH + 100;
          pipedown.y = (HEIGHT / 2) + (rand() % ((HEIGHT - 100) - (HEIGHT / 2))); 
        }
    
        pipetop.x -= (int)(gamespeed * deltatime);
        if (pipetop.x <= -100) {
          pipetop.x = WIDTH + 100;
          pipetop.y = pipedown.y - 1200; 
        }
        pipedown2.x -= (int)(gamespeed * deltatime);
        if (pipedown2.x <= -100) {
          pipedown2.x = WIDTH + 100;
          pipedown2.y = (HEIGHT / 2) + (rand() % ((HEIGHT - 100) - (HEIGHT / 2))); 
        }
    
        pipetop2.x -= (int)(gamespeed * deltatime);
        if (pipetop2.x <= -100) {
          pipetop2.x = WIDTH + 100;
          pipetop2.y = pipedown2.y - 1200; 
        }
        pipedown3.x -= (int)(gamespeed * deltatime);
        if (pipedown3.x <= -100) {
          pipedown3.x = WIDTH + 100;
          pipedown3.y = (HEIGHT / 2) + (rand() % ((HEIGHT - 100) - (HEIGHT / 2))); 
        }
    
        pipetop3.x -= (int)(gamespeed * deltatime);
        if (pipetop3.x <= -100) {
          pipetop3.x = WIDTH + 100;
          pipetop3.y = pipedown3.y - 1200; 
        }
     
    
    
        if (keyboardstate[SDL_SCANCODE_ESCAPE]) {
        gameloop = 0;
        }
        
        // This is all tool renderers it is not for game
    
        SDL_SetRenderDrawColor(toolsrender, 50, 50, 50, 255);
        SDL_RenderClear(toolsrender);
    
        SDL_SetRenderDrawColor(toolsrender, 100, 100, 100, 255);
        SDL_RenderFillRect(toolsrender, &noclip);
        
        if (noclipf) {
        SDL_SetRenderDrawColor(toolsrender, 0, 200, 255, 255);
        SDL_RenderFillRect(toolsrender, &noclipbutton);
        }else {
           SDL_SetRenderDrawColor(toolsrender, 255, 255, 255, 255);
        SDL_RenderFillRect(toolsrender, &noclipbutton);
        }
    
    
    // This is Rendering part dont touch anything if you are shitty UE user!
    // This is sky renders
    
        SDL_SetRenderDrawColor(mainrender, 50, 50, 50, 255);
        SDL_RenderClear(mainrender);
      
        SDL_SetRenderDrawColor(mainrender, 0, 100, 255, 255);
        SDL_RenderFillRect(mainrender, &sky);
    
        SDL_RenderCopy(mainrender, SkyTexture, NULL, &sky2);
        SDL_RenderCopy(mainrender, SkyTexture, NULL, &sky);
    
        // Pipe renderes
        
        SDL_RenderCopy(mainrender, playerdowntexture, NULL, &pipedown);
        
        SDL_RenderCopy(mainrender, playertoptexture, NULL, &pipetop);
        
        SDL_RenderCopy(mainrender, playerdowntexture, NULL, &pipedown2);
        
        SDL_RenderCopy(mainrender, playertoptexture, NULL, &pipetop2);
        
        SDL_RenderCopy(mainrender, playerdowntexture, NULL, &pipedown3);
        
        SDL_RenderCopy(mainrender, playertoptexture, NULL, &pipetop3);
    
        SDL_RenderCopy(mainrender, playertexture, NULL, &player);
    
        
        Uint32 framerate = SDL_GetTicks() - lastTime;
        if (framerate < 1000 / 60) {
        SDL_Delay((1000 / 60) - framerate);
        }
    
        SDL_RenderPresent(mainrender);
        SDL_RenderPresent(toolsrender);
    }

  SDL_FreeSurface(SkySurf);
  SDL_FreeSurface(playerskin);
  SDL_FreeSurface(playerdownsurface);
  SDL_FreeSurface(playertopsurface);
  SDL_DestroyTexture(SkyTexture);
  SDL_DestroyTexture(playertexture);
  SDL_DestroyTexture(playerdowntexture);
  SDL_DestroyTexture(playertoptexture);
  SDL_DestroyRenderer(mainrender);
  SDL_DestroyRenderer(toolsrender);
  SDL_DestroyWindow(window);
  SDL_DestroyWindow(devtool);
  IMG_Quit();
  SDL_Quit();

  printf("Programm ended succesfully! \n");
  return EXIT_SUCCESS;
}
