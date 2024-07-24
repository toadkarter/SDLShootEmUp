#ifndef DRAW_H_
#define DRAW_H_

struct SDL_Texture;

void prepareScene();
void drawScene();

struct SDL_Texture* loadTextureFromFilePath(char* filepath);
struct SDL_Texture* loadTextureFromFileName(char* filename);
void blit(struct SDL_Texture* texture, int x, int y);

#endif
