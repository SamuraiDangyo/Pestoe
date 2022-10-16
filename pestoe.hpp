/*
Simple Phased Pesto Evaluation written in C++17 language
Copyright (C) 2022 Toni Helminen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

// Header guard

#pragma once

// Headers

#include <algorithm>
#include <iostream>

// Namespace

namespace pestoe {

// Constants

constexpr std::string_view kVersion = "Pestoe 1.1"; // Version
constexpr int kMaxPieces            = (2 * (8 * 1 + 2 * 3 + 2 * 3 + 2 * 5 + 2 * 9 + 2 * 0)); // Max pieces on board (Kings always exist)

// Evaluation phases      ( P, N, B, R, Q, K )
constexpr int kPiece[6] = { 1, 3, 3, 5, 9, 0 };

constexpr int kPestoMaterial[2][6] =
{
 // P   N    B    R    Q     K
  { 82, 337, 365, 477, 1025, 0 }, // MG
  { 94, 281, 297, 512,  936, 0 }  // EG
};

// [Piece][Phase][Square]
constexpr int kPestoPsqt[6][2][64] =
{
  {
    // Pawn (MG)
    {
       0,   0,   0,   0,   0,   0,   0,   0,
     -35,  -1, -20, -23, -15,  24,  38, -22,
     -26,  -4,  -4, -10,   3,   3,  33, -12,
     -27,  -2,  -5,  12,  17,   6,  10, -25,
     -14,  13,   6,  21,  23,  12,  17, -23,
      -6,   7,  26,  31,  65,  56,  25, -20,
      98, 134,  61,  95,  68, 126,  34, -11,
       0,   0,   0,   0,   0,   0,   0,   0
    },
    // Pawn (EG)
    {
       0,   0,   0,   0,   0,   0,   0,   0,
      13,   8,   8,  10,  13,   0,   2,  -7,
       4,   7,  -6,   1,   0,  -5,  -1,  -8,
      13,   9,  -3,  -7,  -7,  -8,   3,  -1,
      32,  24,  13,   5,  -2,   4,  17,  17,
      94, 100,  85,  67,  56,  53,  82,  84,
     178, 173, 158, 134, 147, 132, 165, 187,
       0,   0,   0,   0,   0,   0,   0,   0
    }
  },
  {
    // Knight (MG)
    {
    -105, -21, -58, -33, -17, -28, -19, -23,
     -29, -53, -12,  -3,  -1,  18, -14, -19,
     -23,  -9,  12,  10,  19,  17,  25, -16,
     -13,   4,  16,  13,  28,  19,  21,  -8,
      -9,  17,  19,  53,  37,  69,  18,  22,
     -47,  60,  37,  65,  84, 129,  73,  44,
     -73, -41,  72,  36,  23,  62,   7, -17,
    -167, -89, -34, -49,  61, -97, -15,-107
    },
    // Knight (EG)
    {
     -29, -51, -23, -15, -22, -18, -50, -64,
     -42, -20, -10,  -5,  -2, -20, -23, -44,
     -23,  -3,  -1,  15,  10,  -3, -20, -22,
     -18,  -6,  16,  25,  16,  17,   4, -18,
     -17,   3,  22,  22,  22,  11,   8, -18,
     -24, -20,  10,   9,  -1,  -9, -19, -41,
     -25,  -8, -25,  -2,  -9, -25, -24, -52,
     -58, -38, -13, -28, -31, -27, -63, -99
    }
  },
  {
    // Bishop (MG)
    {
     -33,  -3, -14, -21, -13, -12, -39, -21,
       4,  15,  16,   0,   7,  21,  33,   1,
       0,  15,  15,  15,  14,  27,  18,  10,
      -6,  13,  13,  26,  34,  12,  10,   4,
      -4,   5,  19,  50,  37,  37,   7,  -2,
     -16,  37,  43,  40,  35,  50,  37,  -2,
     -26,  16, -18, -13,  30,  59,  18, -47,
     -29,   4, -82, -37, -25, -42,   7,  -8
   },
    // Bishop (EG)
    {
     -23,  -9, -23,  -5,  -9, -16,  -5, -17,
     -14, -18,  -7,  -1,   4,  -9, -15, -27,
     -12,  -3,   8,  10,  13,   3,  -7, -15,
      -6,   3,  13,  19,   7,  10,  -3,  -9,
      -3,   9,  12,   9,  14,  10,   3,   2,
       2,  -8,   0,  -1,  -2,   6,   0,   4,
      -8,  -4,   7, -12,  -3, -13,  -4, -14,
     -14, -21, -11,  -8,  -7,  -9, -17, -24
    }
  },
  {
    // Rook (MG)
    {
     -19, -13,   1,  17,  16,   7, -37, -26,
     -44, -16, -20,  -9,  -1,  11,  -6, -71,
     -45, -25, -16, -17,   3,   0,  -5, -33,
     -36, -26, -12,  -1,   9,  -7,   6, -23,
     -24, -11,   7,  26,  24,  35,  -8, -20,
      -5,  19,  26,  36,  17,  45,  61,  16,
      27,  32,  58,  62,  80,  67,  26,  44,
      32,  42,  32,  51,  63,   9,  31,  43
    },
    // Rook (EG)
    {
      -9,   2,   3,  -1,  -5, -13,   4, -20,
      -6,  -6,   0,   2,  -9,  -9, -11,  -3,
      -4,   0,  -5,  -1,  -7, -12,  -8, -16,
       3,   5,   8,   4,  -5,  -6,  -8, -11,
       4,   3,  13,   1,   2,   1,  -1,   2,
       7,   7,   7,   5,   4,  -3,  -5,  -3,
      11,  13,  13,  11,  -3,   3,   8,   3,
      13,  10,  18,  15,  12,  12,   8,   5
     }
  },
  {
    // Queen (MG)
    {
      -1, -18,  -9,  10, -15, -25, -31, -50,
     -35,  -8,  11,   2,   8,  15,  -3,   1,
     -14,   2, -11,  -2,  -5,   2,  14,   5,
      -9, -26,  -9, -10,  -2,  -4,   3,  -3,
     -27, -27, -16, -16,  -1,  17,  -2,   1,
     -13, -17,   7,   8,  29,  56,  47,  57,
     -24, -39,  -5,   1, -16,  57,  28,  54,
     -28,   0,  29,  12,  59,  44,  43,  45
   },
    // Queen (EG)
    {
     -33, -28, -22, -43,  -5, -32, -20, -41,
     -22, -23, -30, -16, -16, -23, -36, -32,
     -16, -27,  15,   6,   9,  17,  10,   5,
     -18,  28,  19,  47,  31,  34,  39,  23,
       3,  22,  24,  45,  57,  40,  57,  36,
     -20,   6,   9,  49,  47,  35,  19,   9,
     -17,  20,  32,  41,  58,  25,  30,   0,
      -9,  22,  22,  27,  27,  19,  10,  20
    }
  },
  {
    // King (MG)
    {
     -15,  36,  12, -54,   8, -28,  24,  14,
       1,   7,  -8, -64, -43, -16,   9,   8,
     -14, -14, -22, -46, -44, -30, -15, -27,
     -49,  -1, -27, -39, -46, -44, -33, -51,
     -17, -20, -12, -27, -30, -25, -14, -36,
      -9,  24,   2, -16, -20,   6,  22, -22,
      29,  -1, -20,  -7,  -8,  -4, -38, -29,
     -65,  23,  16, -15, -56, -34,   2,  13
    },
    // King (EG)
    {
     -53, -34, -21, -11, -28, -14, -24, -43,
     -27, -11,   4,  13,  14,   4,  -5, -17,
     -19,  -3,  11,  21,  23,  16,   7,  -9,
     -18,  -4,  21,  24,  27,  23,   9, -11,
      -8,  22,  24,  27,  26,  33,  26,   3,
      10,  17,  23,  15,  20,  45,  44,  13,
     -12,  17,  14,  17,  17,  38,  23,  11,
     -74, -35, -18, -18, -11,  15,   4, -17
    }
  }
};

// Variables

int g_mg = 0, g_eg = 0, g_piece_sum = 0;

// Functions

inline int FlipY(const int sq) { return sq ^ 56; } // Mirror horizontal
void Reset() { g_mg = g_eg = g_piece_sum = 0; }

template<bool wtm>
inline void Pesto(const int piece, const int sq) {
  if constexpr (wtm) {
    g_mg        += kPestoPsqt[piece][0][sq]        + kPestoMaterial[0][piece];
    g_eg        += kPestoPsqt[piece][1][sq]        + kPestoMaterial[1][piece];
    g_piece_sum += kPiece[+piece - 1];
  } else {
    g_mg        -= kPestoPsqt[piece][0][FlipY(sq)] + kPestoMaterial[0][piece];
    g_eg        -= kPestoPsqt[piece][1][FlipY(sq)] + kPestoMaterial[1][piece];
    g_piece_sum += kPiece[-piece - 1];
  }
}

void EvaluatePieces(const std::int8_t *board) {
  for (std::size_t sq = 0; sq < 64; ++sq)
    if (const auto p = board[sq]; p >= +1) Pesto<true >(+p - 1, sq);
    else if (                     p <= -1) Pesto<false>(-p - 1, sq);
}

int CalculateScore() { // 96 phases for HCE
  const float n = static_cast<float>(std::clamp(g_piece_sum, 0, kMaxPieces)) / static_cast<float>(kMaxPieces);
  return static_cast<int>(n * static_cast<float>(g_mg) + (1.0f - n) * static_cast<float>(g_eg));
}

/*
Phased Pesto Evaluation (PPE)
+-+-+ Board encoding +-+-+
+1: White Pawn
+2: White Knight
+3: White Bishop
+4: White Rook
+5: White Queen
+6: White King
 0: Empty
-1: Black Pawn
-2: Black Knight
-3: Black Bishop
-4: Black Rook
-5: Black Queen
-6: Black King
*/
int Evaluate(const std::int8_t *board) {
  Reset();
  EvaluatePieces(board);
  return CalculateScore(); // cp
}
} // namespace pestoe
