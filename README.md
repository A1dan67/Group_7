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

    # Optional: Tell your AI to only run if the user ISN'T steering manually!
    # You can wrap your AI loop in: if not manual_override:

    for obs in obstacles:
        # The AI looks 250 pixels AHEAD of the car (matatu_y - 250)
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


    # Move the lines down the screen at the same speed as the obstacles
    line_y += obstacle_speed
    if line_y > 100:  # Reset the line position so it loops seamlessly
        line_y = 0

    # Draw a dashed white line down the middle of the screen
    for i in range(-100, 600, 100):
        pygame.draw.rect(screen, (255, 255, 255), (395, i + line_y, 10, 50))

    for obs in obstacles:
        pygame.draw.rect(screen, (255, 0, 0), obs)


        # 1. The Main Yellow Body
        pygame.draw.rect(screen, (255, 204, 0), matatu_hitbox, border_radius=8)

        # 2. A Classic Green Nairobi Stripe across the bottom
        pygame.draw.rect(screen, (0, 150, 0), (matatu_x, matatu_y + 70, 50, 15))

        # 3. The Front Windshield (Dark Grey)
        pygame.draw.rect(screen, (40, 40, 40), (matatu_x + 5, matatu_y + 10, 40, 20), border_radius=3)

        # 4. Two Bright Headlights
        pygame.draw.rect(screen, (255, 255, 200), (matatu_x + 5, matatu_y + 5, 12, 6), border_radius=2)
        pygame.draw.rect(screen, (255, 255, 200), (matatu_x + 33, matatu_y + 5, 12, 6), border_radius=2)

    pygame.display.flip()
    clock.tick(60)

# --- 4. QUIT CLEANLY ---
pygame.quit()
sys.exit()
