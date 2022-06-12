import pygame

from .board import Board
from .constants import BG_COLOR_YELLOW, PIECE_COLOR_WHITE, PIECE_COLOR_BLACK, SQUARE_SIZE

class Game:
    def __init__(self, win):
        self._init()
        self.win = win

    def update(self):
        self.board.draw(self.win)
        self.draw_valid_moves(self.valid_moves)
        pygame.display.update()

    def _init(self):
        self.selected = None
        self.board = Board()
        self.turn = PIECE_COLOR_BLACK
        self.valid_moves = {}
        self.skipped = False

    def winner(self):
        return self.board.winner()

    def reset(self):
        self._init()

    def select(self, row, col):
        if self.selected:
            result = self._move(row, col)
            if not result:
                self.selected = None
                self.select(row, col)
        
        piece = self.board.get_piece(row, col)
        if piece != 0 and piece.color == self.turn:
            self.selected = piece
            self.valid_moves = self.board.get_valid_moves(piece)
            return True

        return False

    def _move(self, row, col):
        piece = self.board.get_piece(row, col)
        if self.selected and piece == 0 and (row, col) in self.valid_moves:
            self.board.move(self.selected, row, col)
            if self.valid_moves[(row, col)] != 0:
                self.board.remove(self.valid_moves[(row, col)])

                if self.board.get_valid_moves(self.valid_moves[(row, col)]):
                    self.change_turn()
            self.change_turn()
        else:
            return False
        
        return True

    def draw_valid_moves(self, moves):
        for move in moves.keys():
            row, col = move
            pygame.draw.rect(self.win, BG_COLOR_YELLOW, (col * SQUARE_SIZE + 1, row * SQUARE_SIZE + 1, SQUARE_SIZE - 2, SQUARE_SIZE - 2))

    def change_turn(self):
        self.valid_moves = {}
        if self.turn == PIECE_COLOR_BLACK:
            self.turn = PIECE_COLOR_WHITE
        else:
            self.turn = PIECE_COLOR_BLACK

    def get_board(self):
        return self.board

    def ai_move(self, board):
        self.board = board
        self.change_turn()