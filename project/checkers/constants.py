import pygame

WIDTH, HEIGHT = 800, 800
ROWS, COLS = 8, 8
SQUARE_SIZE = WIDTH//COLS

BG_COLOR_WHITE = (245, 245, 245)
BG_COLOR_GREEN = (120, 200, 120)
BG_COLOR_YELLOW = (255, 255, 100)

PIECE_COLOR_WHITE = (255, 255, 255)
PIECE_COLOR_BLACK = (0, 0, 0)

CROWN = pygame.transform.scale(pygame.image.load('assets/crown.png'), (45, 25))
