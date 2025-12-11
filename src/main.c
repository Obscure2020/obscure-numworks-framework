#include <eadk.h>
#include "sprites.h"
// #include <stdlib.h>

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Bounce001";
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

sprite_instance_t guy = {0, 0, 1, 1, &electrode};

void update_screen(){
    for(int y=0; y<EADK_SCREEN_HEIGHT; y++){
        for(int x=0; x<EADK_SCREEN_WIDTH; x++){
            row_buffer[x] = background_color;
        }
        int height = (int) guy.sprite_ptr->height;
        int width = (int) guy.sprite_ptr->width;
        int row_offset = y - guy.y;
        if((row_offset >= 0) && (row_offset < height)){
            for(int x=0; x<width; x++){
                eadk_color_t pixel = (guy.sprite_ptr->pixels)[row_offset * width + x];
                if(pixel != guy.sprite_ptr->back_color){
                    int out_x = guy.x + x;
                    if((out_x >= 0) && (out_x < EADK_SCREEN_WIDTH)){
                        row_buffer[out_x] = pixel;
                    }
                }
            }
        }
        eadk_display_push_rect((eadk_rect_t){0, y, EADK_SCREEN_WIDTH, 1}, row_buffer);
    }
}

void process_motion(){
    guy.x += guy.vel_x;
    guy.y += guy.vel_y;
}

int main(int argc, char * argv[]) {
    while(true){
        update_screen();
        eadk_timing_msleep(20);
        process_motion();
    }
}
