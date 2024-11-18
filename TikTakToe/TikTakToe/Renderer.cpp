#include <math.h>

internal void 
render_background() {
    u32* pixel = (u32*)render_state.memory;
    for (int y = 0; y < render_state.width; y++) {
        for (int x = 0; x < render_state.height; x++) {
            *pixel++ = x;
        }
    }
}

internal void 
clear_screen(u32 color) {
    u32* pixel = (u32*)render_state.memory;
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            *pixel = color;
            pixel++;
        }
    }
}



internal void 
draw_rect_in_pixels(int startX, int startY, int endX, int endY, u32 color) {

    startX = clamp(0, startX, render_state.width);
    endX = clamp(0, endX, render_state.width);
    startY = clamp(0, startY, render_state.height);
    endY = clamp(0, endY, render_state.height);

    //for (int y = render_state.height - endY; y < render_state.height - startY; y++) 
    //{
    //    u32* pixel = (u32*)render_state.memory + startX + y*render_state.width;
    //    for (int x = startX; x < endX; x++) 
    //    {
    //        *pixel++ = color;
    //    }
    //}

    for (int y = render_state.height - endY; y < render_state.height - startY; y++)
    {
        u32* pixel = (u32*)render_state.memory + startX + y * render_state.width;
        for (int x = startX; x < endX; x++)
        {
            if (y == render_state.height - endY || y == render_state.height - startY - 1  || x == startX || x == endX - 1) {
                *pixel++ = color;
            }
            else {
                pixel++;
            }
        }
    }
}

internal void
draw_triangle(int startX, int startY, int endX, int endY, u32 color) {

    startX = clamp(0, startX, render_state.width);
    endX = clamp(0, endX, render_state.width);
    startY = clamp(0, startY, render_state.height);
    endY = clamp(0, endY, render_state.height);

    //for (int y = render_state.height - endY; y < render_state.height - startY; y++)
    //{
    //    u32* pixel = (u32*)render_state.memory + startX + y * render_state.width;
    //    for (int x = startX; x < endX; x++)
    //    {
    //        *pixel++ = color;
    //    }
    //    endX--;
    //}

    for (int y = render_state.height - endY; y < render_state.height - startY; y++)
    {
        u32* pixel = (u32*)render_state.memory + startX + y * render_state.width;
        for (int x = endX; x > startX; x--)
        {
            if (x == endX - 1) {
                *pixel++ = color;
            }
            else {
                *pixel++;
            }
        }startX++;
    }
}

internal void 
draw_rect_proper(int possX, int possY, int sizeX, int sizeY, u32 color) {

    possX = clamp(0, possX, render_state.width);
    sizeX = clamp(0, sizeX, render_state.width);
    possY = clamp(0, possY, render_state.height);
    sizeY = clamp(0, sizeY, render_state.height);

    for (int y = render_state.height - (possY + sizeY); y < render_state.height - possY; y++) {
        u32* pixel = (u32*)render_state.memory + possX + y * render_state.width;
        for (int x = possX; x < possX + sizeX; x++) {
            *pixel++ = color;
        }
    }
}

internal void
draw_rect(int possX, int possY, int sizeX, int sizeY, u32 color) {
    float fpossX = possX;
    //int pX = int((float(possX) / 1920) * render_state.width);
    //int pY = int((float(possY) / 1080) * render_state.height);
    int pX = possX;
    int pY = possY;
    int sY = int((float(sizeY) / 1080) * render_state.height);
    float sX = float(sY)/(float(sizeY) / float(sizeX));
    draw_rect_in_pixels(pX - int(sX / 2), pY - int(sY / 2), pX + int(sX / 2), pY + int(sY / 2), color);
}

internal void
draw_triangle(int possX, int possY, int r, u32 color) {
    int i = 0;
    for (int y = possY - r; y < possY + r; y++) {
        if (y <= y + r) {
            u32* pixel = (u32*)render_state.memory + possX - i + y * render_state.width;
            for (int x = possX - i; x < possX + i; x++) {
                *pixel++ = color;
            }
        }
        i++;
    }
}

internal void
draw_circle(int possX, int possY, int r, u32 color) {
    int i = r;
    for (int y = possY - r; y < possY + r; y++) {
        if (i > -r) {
            float min = (sqrt((r * r) - (i * i)) * -1);
            float max = sqrt((r * r) - (i * i));
            u32* pixel = (u32*)render_state.memory + possX - int(max) + y * render_state.width;
            for (int x = min; x < max; x++) {
                *pixel++ = color;
            }
        }
        i--;
    }
}

//internal void
//draw_cube_fake(int x, int y, int s, u32 color) {
//    draw_rect_in_pixels(x, y, x + s, y + s, color); //1
//    draw_rect_in_pixels(x + s/2, y - s/2, x + s/2 + s, y - s/2 + s, color); //2
//    draw_triangle(x, y - s/2, x + s/2 , y, color); //3
//    draw_triangle(x, y + s/2, x + s/2, y + s, color); //4
//    draw_triangle(x + s, y - s/2, x + s/2 + s, y, color); //5
//    draw_triangle(x + s, y + s/2, x + s/2 + s, y + s, color); //6
//}