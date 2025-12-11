#include <eadk.h>
#include "sprites.h"

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Bounce002";
const uint32_t eadk_api_level  __attribute__((section(".rodata.eadk_api_level"))) = 0;

eadk_color_t row_buffer[EADK_SCREEN_WIDTH];
const eadk_color_t background_color = 0x9FFF;

typedef struct {
    int x;
    int y;
    int vel_x;
    int vel_y;
    const sprite_t *sprite_ptr;
} sprite_instance_t;

sprite_instance_t dudes[] = {(sprite_instance_t){0, 0, 1, 1, &electrode}, (sprite_instance_t){50, 10, 2, 1, &onix}};
#define NUM_OF_DUDES 2

void update_screen(){
    for(int y=0; y<EADK_SCREEN_HEIGHT; y++){
        for(int x=0; x<EADK_SCREEN_WIDTH; x++){
            row_buffer[x] = background_color;
        }
        for(int i=0; i<NUM_OF_DUDES; i++){
            int height = (int) dudes[i].sprite_ptr->height;
            int width = (int) dudes[i].sprite_ptr->width;
            int row_offset = y - dudes[i].y;
            if((row_offset >= 0) && (row_offset < height)){
                for(int x=0; x<width; x++){
                    eadk_color_t pixel = (dudes[i].sprite_ptr->pixels)[row_offset * width + x];
                    if(pixel != dudes[i].sprite_ptr->back_color){
                        int out_x = dudes[i].x + x;
                        if((out_x >= 0) && (out_x < EADK_SCREEN_WIDTH)){
                            row_buffer[out_x] = pixel;
                        }
                    }
                }
            }
        }
        eadk_display_push_rect((eadk_rect_t){0, y, EADK_SCREEN_WIDTH, 1}, row_buffer);
    }
}

void process_motion(){
    for(int i=0; i<NUM_OF_DUDES; i++){
        dudes[i].x += dudes[i].vel_x;
        dudes[i].y += dudes[i].vel_y;
        if((dudes[i].x <= 0) || ((dudes[i].x + dudes[i].sprite_ptr->width) >= EADK_SCREEN_WIDTH)){
            dudes[i].vel_x *= -1;
        }
        if((dudes[i].y <= 0) || ((dudes[i].y + dudes[i].sprite_ptr->height) >= EADK_SCREEN_HEIGHT)){
            dudes[i].vel_y *= -1;
        }
    }
}

int main(int argc, char * argv[]) {
    while(true){
        eadk_keyboard_state_t keyboard = eadk_keyboard_scan();
        if (eadk_keyboard_key_down(keyboard, eadk_key_back)) {
            break;
        }
        update_screen();
        eadk_timing_msleep(10);
        process_motion();
    }
}
