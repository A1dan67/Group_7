import pygame
import sys
import random

# --- 1. INITIALIZATION ---
pygame.init()
pygame.font.init()

screen = pygame.display.set_mode((800, 600))
pygame.display.set_caption("Self Driving Matatu Sim")
clock = pygame.time.Clock()
game_font = pygame.font.SysFont('Arial', 50, bold=True)

# --- 2. GAME VARIABLES ---
matatu_x = 375
matatu_y = 500
obstacle_speed = 7
line_y = 0
obstacles = []

SPAWN_EVENT = pygame.USEREVENT + 1
pygame.time.set_timer(SPAWN_EVENT, 800)

# --- 3. THE MAIN GAME LOOP ---
running = True
while running:

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        if event.type == SPAWN_EVENT:
            new_x = random.randint(50, 700)
            new_y = -100
            obstacles.append(pygame.Rect(new_x, new_y, 50, 100))

    # STEP B: Game Logic
    evading = False

    keys = pygame.key.get_pressed()

    # Braking Logic: If DOWN is pressed, the road slows down!
    if keys[pygame.K_DOWN]:
        obstacle_speed = 3
    else:
        obstacle_speed = 7

    # Manual Steering: If the user presses LEFT or RIGHT, it overrides the AI
    manual_override = False
    if keys[pygame.K_LEFT] and matatu_x > 0:
        matatu_x -= 6
        manual_override = True
    if keys[pygame.K_RIGHT] and matatu_x < 750:
        matatu_x += 6
        manual_override = True
