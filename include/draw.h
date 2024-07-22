#ifndef DRAW_H_
#define DRAW_H_

struct SDL_Texture;

void prepareScene();
void drawScene();

struct SDL_Texture* loadTexture(char* filename);
void blit(struct SDL_Texture* texture, int x, int y);

#endif
