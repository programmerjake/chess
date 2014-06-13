#include "game_state.h"
#include <cmath> // for abs

using namespace std;

bool GameState::isKingAttacked(Player side) const
{
    bool kingAttacked = false, kingFound = false;
    const PieceType king = (side == Player::White ? PieceType::WhiteKing : PieceType::BlackKing);
    for(size_t x = 0; x < BoardSize; x++)
    {
        for(size_t y = 0; y < BoardSize; y++)
        {
            const PieceType piece = board[x][y];
            if(piece == king)
            {
                kingFound = true;
                if(isPositionAttacked(x, y, side))
                    kingAttacked = true;
            }
        }
    }
    return !kingFound || kingAttacked;
}

void GameState::calcEndCondition()
{
    endCondition = EndCondition::Nothing;
    if(isKingAttacked())
        endCondition = EndCondition::Lose;
    else if(getValidMoves(*this).empty())
        endCondition = EndCondition::Tie;
    endConditionSet = true;
}

void GameState::calcStaticEvaluation()
{
    switch(getEndCondition())
    {
    case EndCondition::Lose:
        staticEvaluation = -1000;
        staticEvaluationSet = true;
        return;
    case EndCondition::Win:
        staticEvaluation = 1000;
        staticEvaluationSet = true;
        return;
    case EndCondition::Tie:
        staticEvaluation = 0;
        staticEvaluationSet = true;
        return;
    case EndCondition::Nothing:
        break;
    }
    assert(false);
#error finish
    staticEvaluationSet = true;
}

bool GameState::isPositionAttackedByPawn(size_t x, size_t y, Player side) const
{
    if(side == Player::White)
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

bool GameState::isPositionAttackedByRookOrQueenOnOrthagonals(size_t x, size_t y, Player side) const
{
    const PieceType searchForRook = (side == Player::White ? PieceType::BlackRook : PieceType::WhiteRook);
    const PieceType searchForQueen = (side == Player::White ? PieceType::BlackQueen : PieceType::WhiteQueen);
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

bool GameState::isPositionAttackedByBishopOrQueenOnDiagonals(size_t x, size_t y, Player side) const
{
    const PieceType searchForBishop = (side == Player::White ? PieceType::BlackBishop : PieceType::WhiteBishop);
    const PieceType searchForQueen = (side == Player::White ? PieceType::BlackQueen : PieceType::WhiteQueen);
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

bool GameState::isPositionAttackedByKnight(size_t x, size_t y, Player side) const
{
    const PieceType searchFor = (side == Player::White ? PieceType::BlackKnight : PieceType::WhiteKnight);
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

bool GameState::isPositionAttackedByKing(size_t x, size_t y, Player side) const
{
    const PieceType searchFor = (side == Player::White ? PieceType::BlackKing : PieceType::WhiteKing);
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

bool GameState::isPositionAttacked(size_t x, size_t y, Player side) const
{
    assert(x < BoardSize && y < BoardSize);
    if(isPositionAttackedByPawn(x, y, side))
        return true;
    if(isPositionAttackedByRookOrQueenOnOrthagonals(x, y, side))
        return true;
    if(isPositionAttackedByBishopOrQueenOnDiagonals(x, y, side))
        return true;
    if(isPositionAttackedByKnight(x, y, side))
        return true;
    if(isPositionAttackedByKing(x, y, side))
        return true;
    return false;
}

namespace
{
void addPawnMove(vector<GameStateMove> & moves, GameStateMove m, Player player)
{
    const size_t queeningRow = (player == Player::White ? BoardSize - 1 : 0);
    if(m.endY == queeningRow)
    {
        for(PieceType piece : {PieceType::WhiteBishop, PieceType::WhiteKnight, PieceType::WhiteQueen, PieceType::WhiteRook})
        {
            piece = setPieceColor(piece, player);
            m.promoteToType = piece;
            moves.push_back(m);
        }
    }
    else
        moves.push_back(m);
}

void addPawnMoves(vector<GameStateMove> & moves, GameState gs)
{
    PieceType pawn = (gs.player == Player::White ? PieceType::WhitePawn : PieceType::BlackPawn);
    int yMoveDir = (gs.player == Player::White ? 1 : -1);
    size_t startRow = (gs.player == Player::White ? 1 : BoardSize - 2);
    //size_t captureEnpassantRow = (gs.player == Player::White ? BoardSize - 4 : 3);
    bool canCaptureEnpassant = (gs.enpassantCaptureX != 0 || gs.enpassantCaptureY != 0);
    for(size_t x = 0; x < BoardSize; x++)
    {
        for(size_t y = 0; y < BoardSize; y++)
        {
            const PieceType piece = gs.board[x][y];
            if(piece != pawn)
                continue;
            if((int)y + yMoveDir < 0 || (size_t)(y + yMoveDir) >= BoardSize)
                continue;
            if(x > 0) // can capture to left
            {
                size_t captureX = x - 1;
                size_t captureY = y + yMoveDir;
                if(canCaptureEnpassant && gs.enpassantCaptureX == captureX && gs.enpassantCaptureY == captureY)
                {
                    addPawnMove(moves, GameStateMove(x, y, captureX, captureY, captureX, y));
                }
                else if(getPieceColor(gs.board[captureX][captureY]))
            }
        }
    }
}
}

const vector<GameStateMove> & GameStateCache::getValidMoves(GameState gs)
{
    Data & data = getGameStateEntry(gs);
    if(data.calculated)
        return data.validMoves;

    addPawnMoves(data.validMoves, gs);
    for(auto i = data.validMoves.begin(); i != data.validMoves.end();)
    {
        GameState finalState = i->apply(gs);
        if(finalState.isKingAttacked(gs.player))
            i = data.validMoves.erase(i);
        else
            i++;
    }
    data.calculated = true;
    return data.validMoves;
}
