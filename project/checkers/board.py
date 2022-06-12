import pygame

from .piece import Piece
from .constants import BG_COLOR_GREEN, BG_COLOR_WHITE, BG_COLOR_YELLOW, PIECE_COLOR_BLACK, PIECE_COLOR_WHITE, ROWS, COLS, SQUARE_SIZE

class Board:
    def __init__(self):
        self.board = []
        self.white_left = self.black_left = 12
        self.white_kings = self.black_kings = 0
        self.create_board()

    def draw_squares(self, win):
        win.fill(BG_COLOR_GREEN)
        for row in range(ROWS):
            for col in range(row % 2, COLS, 2):
                pygame.draw.rect(win, BG_COLOR_WHITE, (row*SQUARE_SIZE, col*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE))

    def evaluate(self):
        return self.white_left - self.black_left + (self.white_kings * 0.2 - self.black_kings * 0.2)

    def move(self, piece, row, col):
        self.board[piece.row][piece.col], self.board[row][col] = self.board[row][col], self.board[piece.row][piece.col]
        piece.move(row, col)

        if row == ROWS - 1 or row == 0:
            piece.make_king()
            if piece.color == PIECE_COLOR_WHITE:
                self.white_kings +=1
            else:
                self.black_kings += 1

    def get_piece(self, row, col):
        return self.board[row][col]

    def get_all_pieces(self, color):
        pieces = []
        for row in self.board:
            for piece in row:
                if piece != 0 and piece.color == color:
                    pieces.append(piece)
        return pieces

    def create_board(self):
        for row in range(ROWS):
            self.board.append([])
            for col in range(COLS):
                if col % 2 == ((row + 1) % 2):
                    if row < 3:
                        self.board[row].append(Piece(row, col, PIECE_COLOR_WHITE))
                    elif row > 4:
                        self.board[row].append(Piece(row, col, PIECE_COLOR_BLACK))
                    else:
                        self.board[row].append(0)
                else:
                    self.board[row].append(0)

    def draw(self, win):
        self.draw_squares(win)
        for row in range(ROWS):
            for col in range(COLS):
                piece = self.board[row][col]
                if piece != 0:
                    piece.draw(win)

    def remove(self, piece):
        self.board[piece.row][piece.col] = 0
        if piece != 0:
            if piece.color == PIECE_COLOR_BLACK:
                self.black_left -= 1
            else:
                self.white_left -= 1

    def winner(self):
        if self.black_left <= 0:
            return PIECE_COLOR_WHITE
        elif self.white_left <= 0:
            return PIECE_COLOR_BLACK
        
        return None

    def get_valid_moves(self, piece):
        moves = {}
        left = piece.col - 1
        right = piece.col + 1
        row = piece.row

        if piece.color == PIECE_COLOR_BLACK or piece.king:
            if left > 0 and row > 1 and self.board[row-1][left] != 0 and self.board[row-2][left-1] == 0 and all(x == y for x, y in zip(self.board[row-1][left].color, PIECE_COLOR_WHITE)):
                moves.update({(row-2, left-1): self.board[row-1][left]})
                return moves

            if right < COLS-1 and row > 1 and self.board[row-1][right] != 0 and self.board[row-2][right+1] == 0 and all(x == y for x, y in zip(self.board[row-1][right].color, PIECE_COLOR_WHITE)):
                moves.update({(row-2, right+1): self.board[row-1][right]})
                return moves

            if left >= 0 and row > 0 and self.board[row-1][left] == 0:
                moves.update({(row-1, left): 0})
                
            if right < COLS and row > 0 and self.board[row-1][right] == 0:
                moves.update({(row-1, right): 0})

        if piece.color == PIECE_COLOR_WHITE or piece.king:
            if left > 0 and row < ROWS-2 and self.board[row+1][left] != 0 and self.board[row+2][left-1] == 0 and all(x == y for x, y in zip(self.board[row+1][left].color, PIECE_COLOR_BLACK)):
                moves.update({(row+2, left-1): self.board[row+1][left]})
                return moves

            if right < COLS-1 and row < ROWS-2 and self.board[row+1][right] != 0 and self.board[row+2][right+1] == 0 and all(x == y for x, y in zip(self.board[row+1][right].color, PIECE_COLOR_BLACK)):
                moves.update({(row+2, right+1): self.board[row+1][right]})
                return moves

            if left >= 0 and row < ROWS-1 and self.board[row+1][left] == 0:
                moves.update({(row+1, left): 0})
                
            if right < COLS and row < ROWS-1 and self.board[row+1][right] == 0:
                moves.update({(row+1, right): 0})

        return moves