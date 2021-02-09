//gcc hello.c -o hello $(pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_image-5 --libs --cflags)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "BILL_HEADER.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#define BILL_WIDTH 710
#define WHITE al_map_rgb(255,255,255)
//#define QUANTITY_SIZE 12
#define COLUMN1_x 1  
#define COLUMN2_x 65 
#define COLUMN3_x 165
#define COLUMN4_x 465
#define COLUMN5_x 535
#define COLUMN6_x 596
//#define COLUMN_y 80
#define print_x1 20
#define print_x2 90

//error check for all inits
void must_init(bool test, char description[]){
    if (test) return;
    printf("Error in initialising %s\n",description);
    exit(1);
}

//layout includes lines,heading,header names,total and gst
void draw_layout(ALLEGRO_FONT *big_font,ALLEGRO_FONT *header_font,ALLEGRO_FONT *items_font,int i,int bill_height,float total, ALLEGRO_CONFIG* cfg){
    char temp_str[10];
    int COLUMN_y = 80 - (al_get_font_line_height(items_font))*i;
    al_draw_text(big_font,WHITE,COLUMN1_x+(BILL_WIDTH-COLUMN1_x-al_get_text_width(big_font,"PROVISIONAL BILL DETAILS"))/2,
    (80-al_get_font_line_height(big_font))/2 - (al_get_font_line_height(items_font))*i,0,"PROVISIONAL BILL DETAILS");
    al_draw_rectangle(COLUMN1_x,COLUMN_y,BILL_WIDTH,COLUMN_y+76,WHITE,1);

    al_draw_rectangle(COLUMN1_x,COLUMN_y,COLUMN2_x,bill_height-1,WHITE,1);
    al_draw_text(header_font,WHITE,COLUMN1_x+5,COLUMN_y+10,0,"Serial");
    al_draw_text(header_font,WHITE,COLUMN1_x+16,COLUMN_y+43,0,"No.");

    al_draw_rectangle(COLUMN2_x,COLUMN_y,COLUMN3_x,bill_height-1,WHITE,1);
    al_draw_text(header_font,WHITE,COLUMN2_x+30,COLUMN_y+10,0,"Item");
    al_draw_text(header_font,WHITE,COLUMN2_x+23,COLUMN_y+43,0,"Code");

    al_draw_rectangle(COLUMN3_x,COLUMN_y,COLUMN4_x,bill_height-1,WHITE,1);
    al_draw_text(header_font,WHITE,COLUMN3_x+98,COLUMN_y+22.5,0,"Item Name");

    al_draw_rectangle(COLUMN4_x,COLUMN_y,COLUMN5_x,bill_height-1,WHITE,1);
    al_draw_text(header_font,WHITE,COLUMN4_x+12,COLUMN_y+26.5,0,"Rate");

    al_draw_rectangle(COLUMN5_x,COLUMN_y,COLUMN6_x,bill_height-1,WHITE,1);
    al_draw_text(header_font,WHITE,COLUMN5_x+6,COLUMN_y+26.5,0,"Qty.");

    al_draw_rectangle(COLUMN6_x,COLUMN_y,BILL_WIDTH,bill_height-1,WHITE,1);
    al_draw_text(header_font,WHITE,COLUMN6_x+34,COLUMN_y+26.5,0,"Price");

    al_draw_rectangle(COLUMN4_x,bill_height-1,BILL_WIDTH,bill_height+50,WHITE,1);
    al_draw_text(header_font,WHITE,COLUMN4_x+(COLUMN6_x-COLUMN4_x-al_get_text_width(header_font,"GST"))/2,
    bill_height+10,0,"GST");
    
    al_draw_text(header_font,WHITE,COLUMN6_x+(BILL_WIDTH-COLUMN6_x-al_get_text_width(header_font,al_get_config_value(cfg,"","GST")))/2,
    bill_height+10,0,al_get_config_value(cfg,"","GST"));

    al_draw_rectangle(COLUMN4_x,bill_height-1,BILL_WIDTH,bill_height+99,WHITE,1);
    al_draw_text(header_font,WHITE,COLUMN4_x+(COLUMN6_x-COLUMN4_x-al_get_text_width(header_font,"Total"))/2,
    bill_height+60,0,"Total");
    sprintf(temp_str,"%.2f",total);
    al_draw_text(header_font,WHITE,COLUMN6_x+(BILL_WIDTH-COLUMN6_x-al_get_text_width(header_font,temp_str))/2,
    bill_height+60,0,temp_str);

    al_draw_text(header_font,WHITE,print_x1,bill_height+45,0,"Print bill:");

    return;
}

//Place serial no., item code, item name, quantity
//Calculates rate and subsequent price for each line

void add_line(int current_y,int quantity,int code,int serial,ALLEGRO_FONT* items_font,ALLEGRO_CONFIG* cfg){
    int current_x=20;
    char s_code[10],s_serial[4],s_quantity[5],s_amount[10];
    sprintf(s_code,"%d",code);
    
    sprintf(s_serial,"%d.",serial);
    al_draw_text(items_font,WHITE,current_x,current_y,0,s_serial);
    
    current_x=COLUMN2_x+(COLUMN3_x-COLUMN2_x-al_get_text_width(items_font,s_code))/2;
    al_draw_text(items_font,WHITE,current_x,current_y,0,s_code);
    
    current_x=COLUMN3_x;
    al_draw_text(items_font,WHITE,current_x+20,current_y,0,al_get_config_value(cfg,s_code,"name"));
    
    current_x=COLUMN4_x+(COLUMN5_x-COLUMN4_x-al_get_text_width(items_font,al_get_config_value(cfg,s_code,"price")))/2;
    float rate=price(al_get_config_value(cfg,s_code,"price"));
    al_draw_text(items_font,WHITE,current_x,current_y,0,al_get_config_value(cfg,s_code,"price"));
    
    sprintf(s_quantity,"%d",quantity);
    current_x=COLUMN5_x+(COLUMN6_x-COLUMN5_x-al_get_text_width(items_font,s_quantity))/2;
    al_draw_text(items_font,WHITE,current_x,current_y,0,s_quantity);
    
    float amount=rate*quantity;
    sprintf(s_amount,"%.2f",amount);
    current_x=COLUMN6_x+(BILL_WIDTH-COLUMN6_x-al_get_text_width(items_font,s_amount))/2;
    al_draw_text(items_font,WHITE,current_x,current_y,0,s_amount);
    return;
}

//Calclate the total of all the chosen items
//Make sure the config section exists for a certain code

float total(int quant[],int code[],int len,ALLEGRO_CONFIG* cfg){
    float amount,sum=0;
    int i;
    char s_code[10];
    for (i=0;i<len;i++){
        sprintf(s_code,"%d",code[i]);
        amount=price(al_get_config_value(cfg,s_code,"price"))*quant[i];
        sum+=amount;
    }
    //Needs a GST value in the config file
    sum+=sum*(price(al_get_config_value(cfg,"","GST")))/100;
    return sum;
}

//Changes the string input that al_get_config gives and changes it to a floating point value
float price(const char *s){
    float out=0,decimal=0;
    int i=0,j=0;
    while (*(s+i)!='\0'){
        if (*(s+i)=='.'){
            i++;
            while (*(s+i)!='\0'){
                decimal=decimal*10+ (*(s+i)-'0');
                j++;
                i++;
            }
            break;
        }
        out=out*10+(*(s+i)-'0');
        i++;
    }
    
    return out+decimal/pow(10,j);
}


int *get_code_array(int len,ALLEGRO_CONFIG *config){
    int *code;
    code = malloc(sizeof(int)*len);
    int i = 0;
    ALLEGRO_CONFIG_SECTION **iter;
    al_get_first_config_section(config,iter);
    for (i=0;i<len;i++){
        code[i] = price(al_get_next_config_section(iter));
    }
    return code;
}

int item_number(ALLEGRO_CONFIG *config){
    int i=0;
    ALLEGRO_CONFIG_SECTION **iter;
    al_get_first_config_section(config,iter);
    while (al_get_next_config_section(iter) != NULL){
        i++;
    }
    return i;
}

//All tasks in this function. Call this in the main function which has all other functions
void bill_preview(){

    must_init(al_init(),"bill_allegro");
    must_init(al_install_keyboard(),"bill_keyboard");
    must_init(al_install_mouse(),"bill_mouse");
    must_init(al_init_primitives_addon(),"bill_primitives_addon");
    must_init(al_init_font_addon(),"font_addon");
    must_init(al_init_ttf_addon(),"ttf_addon");
    must_init(al_init_image_addon(),"image_addon");
    
    ALLEGRO_CONFIG* cfg=al_load_config_file("list2.cfg");

    ALLEGRO_TIMER *timer= al_create_timer(1.0/60.0);
    must_init(timer,"bill_timer");

    ALLEGRO_FONT *big_font=al_load_font("GeosansLight-Oblique.ttf",40,0);
    must_init(big_font,"big_font");

    ALLEGRO_FONT *header_font=al_load_font("GeosansLight-Oblique.ttf",25,0);
    must_init(header_font,"header_font");

    ALLEGRO_FONT *items_font=al_load_font("GeosansLight.ttf",23,0);
    must_init(items_font,"item_font");
    
    ALLEGRO_EVENT_QUEUE* queue=al_create_event_queue();
    must_init(queue,"bill_queue");
    
    ALLEGRO_BITMAP* print=al_load_bitmap("print_image_final.png");

    //initial calculations before the allegro display is initialised
    int QUANTITY_SIZE = item_number(cfg);
    int item_font_height = al_get_font_line_height(items_font);
    int quant[30] = {1,2,1,2,3,4,5,3,1,5,8,3,1,1,1,1,1,1};
    int *code = get_code_array(QUANTITY_SIZE,cfg);
    int x,y,z=0,x_min,y_min;
    int i,j=0;
    float sum=total(quant,code,QUANTITY_SIZE,cfg);

    for (i=0;i<QUANTITY_SIZE;i++){
        
        if (quant[i]!=0)
            j++;
    }
    int bill_height=80+85+(item_font_height+5)*j;
    int bottom;
    printf("%d\n",bill_height);
    ALLEGRO_DISPLAY *display2;
    if (bill_height <= 800){
        display2= al_create_display(BILL_WIDTH,bill_height+100);
        bottom = bill_height +100;
    }
    else{
        display2= al_create_display(BILL_WIDTH,900);
        bottom = 900;
    }
    must_init(display2,"bill_display");
    al_set_window_title(display2,"Provisional Bill Details");

    al_register_event_source(queue,al_get_keyboard_event_source());
    al_register_event_source(queue,al_get_mouse_event_source());
    al_register_event_source(queue,al_get_display_event_source(display2));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done=false;
    bool redraw=true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while (1){
        al_wait_for_event(queue, &event);

        switch (event.type){
            case ALLEGRO_EVENT_TIMER:
                redraw=true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    done=true;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done=true;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                x=event.mouse.x;
                y=event.mouse.y;
                x_min=print_x1+al_get_text_width(header_font,"Print bill: ");
                y_min=bill_height-z*item_font_height+(120-al_get_bitmap_height(print))/2;
                if (x>=x_min && x<=x_min+40 && y>=y_min && y<y_min+36){
                    printf("No error in execution\n");
                    done=true;
                    //add nikitha's function here
                }
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
                if ((event.mouse.dz>0 && bottom <= bill_height +100 ) || (event.mouse.dz<0 && z>0)){
                    z += event.mouse.dz;
                    printf("%d  %d\n",z,event.mouse.dz);
                    bottom += event.mouse.dz * item_font_height ;
                }
                
        }

        if (redraw == true && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0,0,0));
            draw_layout(big_font,header_font,items_font,z,bill_height-z*item_font_height,sum,cfg);
            //add details of the quantity list based on sohan and neha's array/file
            for (i=0,j=0;i<QUANTITY_SIZE;i++){
                if (quant[i]!=0){
                add_line(80+85+(item_font_height+5)*(j)-item_font_height * z,quant[i],code[i],j+1,items_font,cfg);
                j++;
                }
            }
            al_draw_bitmap(print,print_x1+al_get_text_width(header_font,"Print bill: "),bill_height+(120-al_get_bitmap_height(print))/2 - z*item_font_height,0);
        }

        if (done == true){
            break;
        }

        al_flip_display();
        redraw=false;
    }
    al_destroy_font(header_font);
    al_destroy_display(display2);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}
