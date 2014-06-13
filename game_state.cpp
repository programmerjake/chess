#include "game_state.h"
#include <cmath> // for abs

void GameState::calcEndCondition()
{

}

void GameState::calcStaticEvaluation()
{
    switch(getEndCondition())
    {
    case EndCondition::Lose:
        return -1000;
    case EndCondition::Win:
        return 1000;
    case EndCondition::Tie:
        return 0;
    case EndCondition::Nothing:
        break;
    }
}

bool GameState::isPositionAttackedByPawn(size_t x, size_t y) const
{
    if(player == Player::White)
    {
        if(y > 0)
        {
            if(x > 0)
            {
                if(board[x - 1][y - 1] == PieceType::BlackPawn)
                    return true;
            }
            if(x < BoardSize - 1)
            {
                if(board[x + 1][y - 1] == PieceType::BlackPawn)
                    return true;
            }
        }
    }
    else // then it's black
    {
        if(y < BoardSize - 1)
        {
            if(x > 0)
            {
                if(board[x - 1][y + 1] == PieceType::WhitePawn)
                    return true;
            }
            if(x < BoardSize - 1)
            {
                if(board[x + 1][y + 1] == PieceType::WhitePawn)
                    return true;
            }
        }
    }
    return false;
}

bool GameState::isPositionAttackedByRookOrQueenOnOrthagonals(size_t x, size_t y) const
{
    const PieceType searchForRook = (player == Player::White ? PieceType::BlackRook : PieceType::WhiteRook);
    const PieceType searchForQueen = (player == Player::White ? PieceType::BlackQueen : PieceType::WhiteQueen);
    for(size_t xSearch = x - 1, ySearch = y, i = 0; i < x; i++, xSearch--)
    {
        const PieceType piece = board[xSearch][ySearch];
        if(searchForRook == piece)
            return true;
        if(searchForQueen == piece)
            return true;
        if(PieceType::Empty == piece)
            break;
    }
    for(size_t xSearch = x + 1, ySearch = y; xSearch < BoardSize; xSearch++)
    {
        const PieceType piece = board[xSearch][ySearch];
        if(searchForRook == piece)
            return true;
        if(searchForQueen == piece)
            return true;
        if(PieceType::Empty == piece)
            break;
    }
    for(size_t xSearch = x, ySearch = y - 1, i = 0; i < y; i++, ySearch--)
    {
        const PieceType piece = board[xSearch][ySearch];
        if(searchForRook == piece)
            return true;
        if(searchForQueen == piece)
            return true;
        if(PieceType::Empty == piece)
            break;
    }
    for(size_t xSearch = x, ySearch = y + 1; ySearch < BoardSize; ySearch++)
    {
        const PieceType piece = board[xSearch][ySearch];
        if(searchForRook == piece)
            return true;
        if(searchForQueen == piece)
            return true;
        if(PieceType::Empty == piece)
            break;
    }
    return false;
}

bool GameState::isPositionAttackedByBishopOrQueenOnDiagonals(size_t x, size_t y) const
{
    const PieceType searchForBishop = (player == Player::White ? PieceType::BlackBishop : PieceType::WhiteBishop);
    const PieceType searchForQueen = (player == Player::White ? PieceType::BlackQueen : PieceType::WhiteQueen);
    for(int dx : {-1, 1})
    {
        for(int dy : {-1, 1})
        {
            for(int xSearch = x + dx, ySearch = y + dy; (size_t)xSearch < BoardSize && (size_t)ySearch < BoardSize && xSearch >= 0 && ySearch >= 0; xSearch += dx, ySearch += dy)
            {
                const PieceType piece = board[xSearch][ySearch];
                if(searchForBishop == piece)
                    return true;
                if(searchForQueen == piece)
                    return true;
                if(PieceType::Empty == piece)
                    break;
            }
        }
    }
    return false;
}

bool GameState::isPositionAttackedByKnight(size_t x, size_t y) const
{
    const PieceType searchFor = (player == Player::White ? PieceType::BlackKnight : PieceType::WhiteKnight);
    for(const int dx : {-2, -1, 1, 2})
    {
        const int yStep = 3 - abs(dx);
        for(const int dy : {-yStep, yStep})
        {
            if((int)x < dx || (int)y < dy)
                continue;
            size_t xSearch = x + dx, ySearch = y + dy;
            if(xSearch >= BoardSize || ySearch >= BoardSize)
                continue;
            if(board[xSearch][ySearch] == searchFor)
                return true;
        }
    }
    return false;
}

bool GameState::isPositionAttackedByKing(size_t x, size_t y) const
{
    const PieceType searchFor = (player == Player::White ? PieceType::BlackKing : PieceType::WhiteKing);
    for(const int dx : {-1, 0, 1})
    {
        for(const int dy : {-1, 0, 1})
        {
            if(dx == 0 && dy == 0)
                continue;
            if((int)x < dx || (int)y < dy)
                continue;
            size_t xSearch = x + dx, ySearch = y + dy;
            if(xSearch >= BoardSize || ySearch >= BoardSize)
                continue;
            if(board[xSearch][ySearch] == searchFor)
                return true;
        }
    }
    return false;
}

bool GameState::isPositionAttacked(size_t x, size_t y) const
{
    assert(x < BoardSize && y < BoardSize);
    if(isPositionAttackedByPawn(x, y))
        return true;
    if(isPositionAttackedByRookOrQueenOnOrthagonals(x, y))
        return true;
    if(isPositionAttackedByBishopOrQueenOnDiagonals(x, y))
        return true;
    if(isPositionAttackedByKnight(x, y))
        return true;
    if(isPositionAttackedByKing(x, y))
        return true;
    return false;
}
