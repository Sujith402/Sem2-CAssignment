#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

float price(const char *s);
void must_init(bool test, char description[]);
void draw_layout(ALLEGRO_FONT *big_font,ALLEGRO_FONT *header_font,ALLEGRO_FONT *items_font,int i,int bill_height,float total, ALLEGRO_CONFIG* cfg);
float total(int quant[],int code[],int len,ALLEGRO_CONFIG* cfg);
float price(const char *s);
void bill_preview();
int *get_code_array(int len,ALLEGRO_CONFIG *config);