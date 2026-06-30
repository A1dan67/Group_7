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

    # STEP B: Game Logic - UPGRADED AI BRAIN
    evading = False

    for obs in obstacles:
        # THE FIX: We calibrated the sensors!
        # The AI now looks 250 pixels AHEAD of the car (matatu_y - 250)
        # and stops worrying about cars once they are 80 pixels behind it.
        danger_zone = obs.y > matatu_y - 250 and obs.y < matatu_y + 80
        in_same_lane = abs(matatu_x - obs.x) < 65

        if danger_zone and in_same_lane:
            evading = True

            if obs.x >= matatu_x:
                if matatu_x > 0:
                    matatu_x -= 8
            else:
                if matatu_x < 750:
                    matatu_x += 8
            break

    if not evading:
        if abs(matatu_x - 375) < 4:
            matatu_x = 375
        elif matatu_x < 375:
            matatu_x += 3
        elif matatu_x > 375:
            matatu_x -= 3

    for obs in obstacles:
        obs.y += obstacle_speed

    obstacles = [obs for obs in obstacles if obs.y < 600]

    matatu_hitbox = pygame.Rect(matatu_x, matatu_y, 50, 100)

    crashed = False
    for obs in obstacles:
        if matatu_hitbox.colliderect(obs):
            crashed = True
            break

    if crashed:
        fail_text = game_font.render("CRASHED TRY AGAIN.", True, (255, 255, 255))
        screen.blit(fail_text, (80, 250))
        pygame.display.flip()
        pygame.time.delay(2000)

        matatu_x = 375
        obstacles.clear()
        continue

        # STEP C: Rendering
    screen.fill((50, 50, 50))

    for obs in obstacles:
        pygame.draw.rect(screen, (255, 0, 0), obs)

    pygame.draw.rect(screen, (255, 204, 0), matatu_hitbox)

    pygame.display.flip()
    clock.tick(60)

# --- 4. QUIT CLEANLY ---
pygame.quit()
sys.exit()
