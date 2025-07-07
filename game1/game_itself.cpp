#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 150, ball_dp_y, ball_half_size = 1;

int player_1_score, player_2_score;
int border_color = 0x006b6b;
int arena_color = 0x40e0d0;
int paddle_color;
int current_level = 1;
global_variable int max_level = 5;

void update_colors(int level) {
    switch (level % 5) {
    case 0: 
        border_color = 0x8b0000;
        arena_color = 0xff6347;
        break;

    case 1: 
        border_color = 0x006b6b;
        arena_color = 0x40e0d0;
        break;

    case 2:
        border_color = 0x003366;
        arena_color = 0x66ccff;
        break;

    case 3: 
        border_color = 0x4b0082;
        arena_color = 0x9370db;
        break;

    case 4: 
        border_color = 0x8b1c62;
        arena_color = 0xf28ca7;
        break;
    }
}

void update_paddle_colors(int level) {
    switch (level % 5) {
    case 0: 
        paddle_color = 0x8b0000; 
        break;

    case 1:
        paddle_color = 0xdcdcdc ; 
        break;

    case 2: 
        paddle_color = 0x2a1f81;
        break;

    case 3: 
        paddle_color = 0xf2f99f; 
        break;

    case 4: 
        paddle_color = 0x5f0f40;
        break;
    }
}

internal void simulate_player(float *p, float *dp, float ddp, float dt) {
    ddp -= *dp * 10.f;

    *p = *p + *dp * dt + ddp * dt * dt * .5f;
    *dp = *dp + ddp * dt;

    if (*p + player_half_size_y > arena_half_size_y) {
        *p = arena_half_size_y - player_half_size_y;
        *dp = 0;
    }
    else if (*p - player_half_size_y < -arena_half_size_y) {
        *p = -arena_half_size_y + player_half_size_y;
        *dp = 0;
    }
}

internal bool aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y) {
    return(p1x + hs1x > p2x - hs2x &&
        p1x - hs1x < p2x + hs2x &&
        p1y + hs1y > p2y - hs2y &&
        p1y + hs1y < p2y + hs2y);
}

enum Gamemode {
    GM_MENU,
    GM_GAMEPLAY,
    GM_LEVEL,
    GM_GAME_OVER,
};

Gamemode current_gamemode;
int hot_button; 
bool enemy_is_ai;

internal void simulate_game(Input* input, float dt) {
    draw_rect(0, 0, arena_half_size_x, arena_half_size_y, arena_color);
    draw_arena_borders(arena_half_size_x, arena_half_size_y, border_color);

    if (current_gamemode == GM_GAMEPLAY) {

        float player_1_ddp = 0.f; // units per second
        if (!enemy_is_ai) {
            if (is_down(BUTTON_UP)) player_1_ddp += 2000;
            if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;
        }
        else {

            //if (ball_p_y > player_1_p+2.f) player_1_ddp += 1300;
            //if (ball_p_y < player_1_p-2.f) player_1_ddp -= 1300;
            player_1_ddp = (ball_p_y - player_1_p) * 100;
            if (player_1_ddp > 1300) player_1_ddp = 1300;
            if (player_1_ddp < -1300) player_1_ddp = -1300;
        }

        float player_2_ddp = 0.f; // units per second
        if (is_down(BUTTON_W)) player_2_ddp += 2000;
        if (is_down(BUTTON_S)) player_2_ddp -= 2000;

        simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
        simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

        // simulate ball
        {
            ball_p_x += ball_dp_x * dt;
            ball_p_y += ball_dp_y * dt;

            if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y)) {
                ball_p_x = 80 - player_half_size_x - ball_half_size;
                ball_dp_x *= -1;
                ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * .75f;
            }
            else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y)) {
                ball_p_x = -80 + player_half_size_x + ball_half_size;
                ball_dp_x *= -1;
                ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * .75f;
            }

            if (ball_p_y + ball_half_size > arena_half_size_y) {
                ball_p_y = arena_half_size_y - ball_half_size;
                ball_dp_y *= -1;
            }
            else if (ball_p_y - ball_half_size < -arena_half_size_y) {
                ball_p_y = -arena_half_size_y + ball_half_size;
                ball_dp_y *= -1;
            }

            if (ball_p_x + ball_half_size > arena_half_size_x) {
                ball_dp_x *= -1;
                ball_dp_y = 0;
                ball_p_x = 0;
                ball_p_y = 0;
                player_1_score++;
            }
            else if (ball_p_x + ball_half_size < -arena_half_size_x) {
                ball_dp_x *= -1;
                ball_dp_y = 0;
                ball_p_x = 0;
                ball_p_y = 0;
                player_2_score++;
            }

        }

        draw_number(player_1_score, -10, 40, 1.f, 0xffffff);
        draw_number(player_2_score, 10, 40, 1.f, 0xffffff);

        // rendering
        draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);

        draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, paddle_color);
        draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, paddle_color);

        if ((player_1_score == 5) || (player_2_score == 5)) { // Тут умова завершення рівня
            if (current_level < max_level) {
                current_level++;
                player_1_score = 0;
                player_2_score = 0;
                current_gamemode = GM_LEVEL; // Повертаємося до екрану рівня
            }
            else {
                player_1_score = 0;
                player_2_score = 0;
                current_level = 1;
                current_gamemode = GM_GAME_OVER;
            }
        }
    }
    else if(current_gamemode == GM_MENU){

        if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
            hot_button = !hot_button;
        }

        if (pressed(BUTTON_ENTER)) {
            if (current_gamemode == GM_MENU) {
                current_gamemode = GM_LEVEL; // Спочатку показуємо екран рівня
            }
            else if (current_gamemode == GM_LEVEL) {
                current_gamemode = GM_GAMEPLAY; // Починаємо гру після екрану рівня
                enemy_is_ai = hot_button ? 0 : 1;

            }
        }

        static float time = 0;
        time += delta_time * 5; // Змінюй швидкість ефекту тут

        float scale_single = (hot_button == 0) ? 1.0 + 0.1 * sin(time) : 1.0;
        float scale_couple = (hot_button == 1) ? 1.0 + 0.1 * sin(time) : 1.0;

        if (hot_button == 0) {
            draw_text("SINGLE PLAYER", -75, -1, scale_single, 0x5f0f40);
            draw_text("COUPLE MODE", 10, -1, scale_couple, 0xffffff);
        }
        else {
            draw_text("SINGLE PLAYER", -75, -1, scale_single, 0xffffff);
            draw_text("COUPLE MODE", 10, -1, scale_couple, 0x5f0f40);
        }

        draw_text("PRESS ENTER TO START", -55, -30, 1, 0xffffff);
        draw_text("LOVELY PING PONG", -63, 30, 1.4, 0xffffff);


    }
    else if (current_gamemode == GM_LEVEL) {

        update_colors(current_level);
        update_paddle_colors(current_level);
        draw_text("LEVEL", -30, 10, 2, 0xffffff);
        draw_number(current_level, 35, 4, 3, 0xffffff);
        draw_text("PRESS ENTER TO START", -55, -30, 1, 0xffffff);

        if (pressed(BUTTON_ENTER)) {
            current_gamemode = GM_GAMEPLAY; // Починаємо гру після екрану рівня
            enemy_is_ai = hot_button ? 0 : 1;
        }
    }
    else if (current_gamemode == GM_GAME_OVER) {

        draw_text("GAME OVER", -50, 10, 2, 0xffffff);
        draw_text("YOU GOT YOUR TRUE LOVE STORY", -83, -30, 1, 0xffffff);

        if (pressed(BUTTON_ENTER)) {
            current_gamemode = GM_MENU;
        }
    }
}