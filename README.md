# Lovely Ping Pong
Lovely Ping Pong is a colorful retro-inspired 2D arcade game made in C++. It combines the classic feel of Pong with a dynamic level system, simple AI, and playful pixel graphics.

# Features
🏓 Classic Pong gameplay with smooth physics

🎨 Color-changing themes for each level

🧠 Single Player Mode – play against a simple AI

👯 Couple Mode – play with a friend on the same keyboard

📈 Level progression – beat 5 levels to win your love story

💔 Game Over screen with a sweet ending message

# Game Logic
The game uses simple AABB collision for ball-paddle and ball-wall interactions.

Paddle motion includes friction and acceleration (simulated with velocity + acceleration).

AI tracks the ball with basic proportional control.

Colors for arena, borders, and paddles change with each level.

The game consists of 4 main gamemodes:

GM_MENU: choose between single player and couple mode

GM_LEVEL: shows level info and loads new theme

GM_GAMEPLAY: core loop with player input and scoring

GM_GAME_OVER: final screen after level 5

# Controls
Player 1 (right side):

W – move up

S – move down

Player 2 (left side / AI or human):

Arrow Up – move up

Arrow Down – move down

Global:

Enter – confirm / start

Left / Right – switch mode in menu

# Technologies used
Written in C/C++

Custom game loop and physics

Simple graphics rendering using primitives (draw_rect, draw_text, etc.)

No external libraries used

# Screenshots
![image](https://github.com/user-attachments/assets/afa1ab05-efdf-4a01-80b5-9df77168bf22)
![image](https://github.com/user-attachments/assets/687fd091-d7aa-45fe-bcf8-e00ffed63a46)
![image](https://github.com/user-attachments/assets/ca78b0cf-4b68-40a4-ad15-580c88caf98d)
![image](https://github.com/user-attachments/assets/8aae218d-736a-4e26-8427-59e79cb956d9)
![image](https://github.com/user-attachments/assets/43d2d346-41d2-4baa-b12a-11204365b4c7)

# Note
This game was created as a learning project to explore basic game programming, AI behavior, and level-based progression.
