#define is_down(b) input->buttons[b].is_down
#define is_pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define is_released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float rX = 45.f;
float rY = 200.f;
float rZ = 0.f;
int horizon_ratio = 4;

internal void
simulate_game(Input* input, float dt) {

    if (is_down(BUTTON_LEFT)) rX += 0.1;
    if (is_down(BUTTON_RIGHT)) rX -= 0.1;

    if (is_down(BUTTON_DOWN)) rY += 1;
    if (is_down(BUTTON_UP)) rY -= 1;

    if (rX < 1) {
        rX = 89;
    }
    else if (rX > 89) {
        rX = 1;
    }

    //if (is_down(BUTTON_W)) rZ += 0.1;
    //if (is_down(BUTTON_S)) rZ -= 0.1;

    line fond;
    fond.s.x = render_state.width / 2 + rZ;
    fond.s.y = 500 + rZ;
    fond.e.x = render_state.width / 2 - rZ;
    fond.e.y = 800 - rZ;

    draw_cube(fond, horizon_ratio, rX, rY, rZ);
}