#ifndef DRAW_H_
#define DRAW_H_

struct App;
struct SDL_Texture;

void prepareScene(struct App* app);
void drawScene(struct App* app);

struct SDL_Texture* loadTexture(struct App* app, char* filename);
void blit(struct App* app, struct SDL_Texture* texture, int x, int y);

#endif
