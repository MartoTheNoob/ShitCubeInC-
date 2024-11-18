#include <math.h>


internal void
draw_line(point s, point e, u32 color) {

    bool inv;

    if (s.x >= e.x && s.y >= e.y) {
        inv = false;
    }
    else if (e.x >= s.x && e.y >= s.y) {
        inv = false;
    }
    else {
        inv = true;
    }

    int sX = s.x;
    int eX = e.x;
    int sY = s.y;
    int eY = e.y;



    if (s.x > e.x && inv == false) {
        eX = s.x;
        sX = e.x;
        eY = s.y;
        sY = e.y;
    }
    else if (s.x < e.x && inv == true) {
        eX = s.x;
        sX = e.x;
        eY = s.y;
        sY = e.y;
    }

    s.x = clamp(0, s.x, render_state.width);
    e.x = clamp(0, e.x, render_state.width);
    s.y = clamp(0, s.y, render_state.height);
    e.y = clamp(0, e.y, render_state.height);

    sX = clamp(0, sX, render_state.width);
    eX = clamp(0, eX, render_state.width);
    sY = clamp(0, sY, render_state.height);
    eY = clamp(0, eY, render_state.height);

    

    int left = 0;

    for (int y = render_state.height - eY; y <= render_state.height - sY; y++)
    {
        if (inv == false) {
            u32* pixel = (u32*)render_state.memory + sX + y * render_state.width;
            float a = (float(e.x - s.x) / float(e.y - s.y));
            left = left + (int(a * 100) % 100);
            for (int x = sX; x <= eX; x++)
            {
                if (x >= eX - a) {
                    *pixel++ = color;
                }
                else {
                    *pixel++;
                }
            }
            if (left >= 100) {
                eX = eX - floor(a) - 1;
                left = left - 100;
            }
            else {
                eX = eX - floor(a);
            }
        }
        else if (inv == true) {
            u32* pixel = (u32*)render_state.memory + eX + y * render_state.width;
            float a = (float(s.x - e.x) / float(e.y - s.y)); 
            left = left + (int(a * 100) % 100);
            for (int x = eX; x <= sX; x++)
            {
                if (x <= eX + a) {
                    *pixel++ = color;
                }
                else {
                    *pixel++;
                }
            }
            if (left >= 100) {
                eX = eX + floor(a) + 1;
                left = left - 100;
            }
            else {
                eX = eX + floor(a);
            }
            
        }
    }
}


internal void 
draw_cube(line fond, int horizon_ratio, float rX, float rY, float rZ){

    // d = diagnal
    float d = sqrtf(2 * pow((fond.e.y - fond.s.y), 2));
    //ps = perspective size
    float ps = 0;
    if (rX > 45) {
        ps = (90 - rX) * ((d - (fond.e.y - fond.s.y)) / 45) + (fond.e.y - fond.s.y);
    }
    else {
        ps = rX * ((d - (fond.e.y - fond.s.y)) / 45) + (fond.e.y - fond.s.y);
    }
    float ratioR = 90 - rX;
    float ratioL = rX;

    float psL = ps * (ratioL / 90);
    float psR = ps * (ratioR / 90);

    point hrzL;
    hrzL.x = (((fond.e.y - fond.s.y) * (rX * (float(horizon_ratio) / 90)) * -1) + (fond.s.x + fond.e.x) / 2 - rZ * (rX * (float(horizon_ratio) / 90)));
    hrzL.y = (fond.e.y - fond.s.y) / 2 + fond.s.y + rY - rZ * (rX * (float(horizon_ratio) / 90));
    //hrzL.y = 200;

    point hrzR;
    hrzR.x = (((fond.e.y - fond.s.y) * (horizon_ratio - (rX * (float(horizon_ratio) / 90)))) * 1) + (fond.s.x + fond.e.x) / 2 + rZ * (horizon_ratio - (rX * (float(horizon_ratio) / 90)));
    hrzR.y = (fond.e.y - fond.s.y) / 2 + fond.s.y + rY + rZ * (horizon_ratio - (rX * (float(horizon_ratio) / 90)));
    //hrzR.y = 200;

    point hrzMid;
    hrzMid.x = fond.s.x;
    hrzMid.y = hrzL.y;

    float maxL = hrzMid.x - hrzL.x;
    float maxR = hrzR.x - hrzMid.x;

    int sUL = (((hrzMid.y - fond.s.y) / maxL) * psL);
    int sLL = (((hrzMid.y - fond.e.y) / maxL) * psL);
    int sUR = (((hrzMid.y - fond.s.y) / maxR) * psR);
    int sLR = (((hrzMid.y - fond.e.y) / maxR) * psR);

    point ptUL;
    ptUL.x = fond.s.x - (floor(psL));
    ptUL.y = fond.s.y + sUL;

    point ptLL;
    ptLL.x = fond.s.x - (floor(psL));
    ptLL.y = fond.e.y + sLL;

    point ptUR;
    ptUR.x = fond.s.x + floor(psR);
    ptUR.y = fond.s.y + sUR;

    point ptLR;
    ptLR.x = fond.s.x + floor(psR);
    ptLR.y = fond.e.y + sLR;

    point ptUU;
    if (rY > 0) {
        ptUU.y = fond.e.y;
        ptUU.x = fond.e.x;
    }
    else {
        ptUU.y = fond.s.y;
        ptUU.x = fond.s.x;
    }


    if (rY <= 0) {

        for (int i = 1; i < ptUL.y - hrzR.y; i++) {


            float ratioLeft = ptUL.x + ((float(hrzR.x - ptUL.x) / float(ptUL.y - hrzR.y)) * i);
            float ratioRight = ptUR.x - ((float(ptUR.x - hrzL.x) / float(ptUR.y - hrzL.y)) * i);

            if (ratioLeft >= ratioRight) {
                ptUU.x = ratioLeft;
                ptUU.y = ptUR.y - i;
                goto escape;
            }
        }
    }
    else {
        for (int i = hrzR.y - ptLL.y; i > 1; i--) {


            float ratioLeft = ptLL.x + ((float(hrzR.x - ptLL.x) / float(hrzR.y - ptLL.y)) * i);
            float ratioRight = ptLR.x - ((float(ptLR.x - hrzL.x) / float(hrzL.y - ptLR.y)) * i);

            if (ratioLeft <= ratioRight) {
                ptUU.x = ratioLeft;
                ptUU.y = ptLR.y + i;
                goto escape;
            }
        }
    }

escape:

    clear_screen(0x000000);
    draw_line(fond.s, fond.e, 0xffffff);
    draw_line(fond.s, ptUL, 0xffffff);
    draw_line(fond.e, ptLL, 0xffffff);
    draw_line(fond.s, ptUR, 0xffffff);
    draw_line(fond.e, ptLR, 0xffffff);

    //draw_line(fond.s, hrzL,  0xffffff);
    //draw_line(fond.e, hrzL,  0xffffff);
    //draw_line(fond.s, hrzR,  0xffffff);
    //draw_line(fond.e, hrzR,  0xffffff);

    if (rY < ((fond.e.y - fond.s.y) - fond.s.y) / 2) {
        draw_line(ptUL, ptUU, 0xffffff);
        draw_line(ptUR, ptUU, 0xffffff);
    }
    else if (rY > (fond.e.y - fond.s.y) / 2) {
        draw_line(ptLL, ptUU, 0xffffff);
        draw_line(ptLR, ptUU, 0xffffff);
    }

    //draw_line(hrzL, hrzR, 0xffffff);
    //draw_line(hrzMid, fond.s, true, 0xffffff);

    draw_line(ptUL, ptLL, 0xffffff);
    draw_line(ptUR, ptLR, 0xffffff);
}



