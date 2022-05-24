#! /usr/bin/env python

# sudo apt install python3-pip
# sudo pip install pygame
# https://www.pygame.org/docs/tut/PygameIntro.htmlo

import sys, pygame
pygame.init()

size = width, height = 320, 240
size = width, height = 1024, 768
speed = [2, 2]
black = 0, 0, 0

screen = pygame.display.set_mode(size)

ball = pygame.image.load("intro_ball.gif")
ballrect = ball.get_rect()

while 1:
    ms = 5
    pygame.time.wait(ms)
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()

    ballrect = ballrect.move(speed)
    if ballrect.left < 0 or ballrect.right > width:
        speed[0] = -speed[0]
    if ballrect.top < 0 or ballrect.bottom > height:
        speed[1] = -speed[1]

    screen.fill(black)
    screen.blit(ball, ballrect)
    pygame.display.flip()

