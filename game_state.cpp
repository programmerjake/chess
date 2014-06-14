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

namespace
{
bool isTieCondition(GameState gs)
{
    size_t blackKingCount = 0;
    size_t whiteKingCount = 0;
    size_t blackKnightCount = 0;
    size_t whiteKnightCount = 0;
    size_t blackBishopCount = 0;
    size_t whiteBishopCount = 0;
    for(const auto & row : gs.board)
    {
        for(auto piece : row)
        {
            if(piece == PieceType::BlackKing)
                blackKingCount++;
            if(piece == PieceType::WhiteKing)
                whiteKingCount++;
        }
    }
    if(blackKingCount < 1 || whiteKingCount < 1)
        return false;
    for(const auto & row : gs.board)
    {
        for(auto piece : row)
        {
            switch(piece)
            {
            case PieceType::Empty:
                break;
            case PieceType::WhitePawn:
            case PieceType::BlackPawn:
            case PieceType::WhiteRook:
            case PieceType::BlackRook:
            case PieceType::WhiteQueen:
            case PieceType::BlackQueen:
                return false;
            case PieceType::WhiteKnight:
                whiteKnightCount++;
                break;
            case PieceType::BlackKnight:
                blackKnightCount++;
                break;
            case PieceType::WhiteBishop:
                whiteBishopCount++;
                break;
            case PieceType::BlackBishop:
                blackBishopCount++;
                break;
            case PieceType::WhiteKing:
            case PieceType::BlackKing:
                break;
            }
        }
    }
    if(whiteBishopCount >= 2 || blackBishopCount >= 2)
        return false;
    if(whiteBishopCount == 1 && whiteKnightCount >= 1)
        return false;
    if(blackBishopCount == 1 && blackKnightCount >= 1)
        return false;
    return true;
}
}

void GameState::calcEndCondition(GameStateCache &cache)
{
    endCondition = EndCondition::Nothing;
    if(isKingAttacked(getOpponent(player)))
        endCondition = EndCondition::Win;
    else if(isKingAttacked(player) && cache.getValidMoves(*this).empty())
        endCondition = EndCondition::Lose;
    else if(isTieCondition(*this) || cache.getValidMoves(*this).empty())
        endCondition = EndCondition::Tie;
    endConditionSet = true;
}

void GameState::calcStaticEvaluation(GameStateCache &cache)
{
    switch(getEndCondition(cache))
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
    staticEvaluation = 0;
    for(size_t x = 0; x < BoardSize; x++)
    {
        for(size_t y = 0; y < BoardSize; y++)
        {
            float pieceValue = 0;
            switch(board[x][y])
            {
            case PieceType::Empty:
                break;
            case PieceType::WhitePawn:
            case PieceType::BlackPawn:
                pieceValue = 1;
                break;
            case PieceType::WhiteRook:
            case PieceType::BlackRook:
                pieceValue = 5;
                break;
            case PieceType::WhiteKnight:
            case PieceType::BlackKnight:
                pieceValue = 3;
                break;
            case PieceType::WhiteBishop:
            case PieceType::BlackBishop:
                pieceValue = 3;
                break;
            case PieceType::WhiteQueen:
            case PieceType::BlackQueen:
                pieceValue = 9;
                break;
            case PieceType::WhiteKing:
            case PieceType::BlackKing:
                pieceValue = 1000;
                break;
            }
            if(getPieceColor(board[x][y]) == getOpponent(getPieceColor(player)))
                pieceValue = -pieceValue;
            staticEvaluation += pieceValue;
        }
    }
    staticEvaluationSet = true;
}

bool GameState::isPositionAttackedByPawn(size_t x, size_t y, Player side) const
{
    if(side == Player::White)
    {
        if(y < BoardSize - 1)
        {
            if(x > 0)
            {
                if(board[x - 1][y + 1] == PieceType::BlackPawn)
                    return true;
            }
            if(x < BoardSize - 1)
            {
                if(board[x + 1][y + 1] == PieceType::BlackPawn)
                    return true;
            }
        }
    }
    else // then it's black
    {
        if(y > 0)
        {
            if(x > 0)
            {
                if(board[x - 1][y - 1] == PieceType::WhitePawn)
                    return true;
            }
            if(x < BoardSize - 1)
            {
                if(board[x + 1][y - 1] == PieceType::WhitePawn)
                    return true;
            }
        }
    }
    return false;
}

bool GameState::isPositionAttackedByRookOrQueenOnOrthogonals(size_t x, size_t y, Player side) const
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
        if(PieceType::Empty != piece)
            break;
    }
    for(size_t xSearch = x + 1, ySearch = y; xSearch < BoardSize; xSearch++)
    {
        const PieceType piece = board[xSearch][ySearch];
        if(searchForRook == piece)
            return true;
        if(searchForQueen == piece)
            return true;
        if(PieceType::Empty != piece)
            break;
    }
    for(size_t xSearch = x, ySearch = y - 1, i = 0; i < y; i++, ySearch--)
    {
        const PieceType piece = board[xSearch][ySearch];
        if(searchForRook == piece)
            return true;
        if(searchForQueen == piece)
            return true;
        if(PieceType::Empty != piece)
            break;
    }
    for(size_t xSearch = x, ySearch = y + 1; ySearch < BoardSize; ySearch++)
    {
        const PieceType piece = board[xSearch][ySearch];
        if(searchForRook == piece)
            return true;
        if(searchForQueen == piece)
            return true;
        if(PieceType::Empty != piece)
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
                if(PieceType::Empty != piece)
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
    if(isPositionAttackedByRookOrQueenOnOrthogonals(x, y, side))
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
    const PieceType pawn = (gs.player == Player::White ? PieceType::WhitePawn : PieceType::BlackPawn);
    const int yMoveDir = (gs.player == Player::White ? 1 : -1);
    const size_t startRow = (gs.player == Player::White ? 1 : BoardSize - 2);
    //const size_t captureEnpassantRow = (gs.player == Player::White ? BoardSize - 4 : 3);
    const bool canCaptureEnpassant = (gs.enpassantCaptureX != 0 || gs.enpassantCaptureY != 0);
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
                const size_t captureX = x - 1;
                const size_t captureY = y + yMoveDir;
                if(canCaptureEnpassant && gs.enpassantCaptureX == captureX && gs.enpassantCaptureY == captureY)
                {
                    addPawnMove(moves, GameStateMove(x, y, captureX, captureY, captureX, y), gs.player);
                }
                else if(getPieceColor(gs.board[captureX][captureY]) == getOpponent(getPieceColor(piece)))
                {
                    addPawnMove(moves, GameStateMove(x, y, captureX, captureY), gs.player);
                }
            }
            if(x < BoardSize - 1) // can capture to right
            {
                const size_t captureX = x + 1;
                const size_t captureY = y + yMoveDir;
                if(canCaptureEnpassant && gs.enpassantCaptureX == captureX && gs.enpassantCaptureY == captureY)
                {
                    addPawnMove(moves, GameStateMove(x, y, captureX, captureY, captureX, y), gs.player);
                }
                else if(getPieceColor(gs.board[captureX][captureY]) == getOpponent(getPieceColor(piece)))
                {
                    addPawnMove(moves, GameStateMove(x, y, captureX, captureY), gs.player);
                }
            }
            if(gs.board[x][y + yMoveDir] == PieceType::Empty)
            {
                addPawnMove(moves, GameStateMove(x, y, x, y + yMoveDir), gs.player);
                if(y == startRow)
                {
                    if(gs.board[x][y + yMoveDir * 2] == PieceType::Empty)
                        addPawnMove(moves, GameStateMove(x, y, x, y + yMoveDir * 2), gs.player);
                }
            }
        }
    }
}

void addRookBishopQueenAndKingMoves(vector<GameStateMove> & moves, GameState gs)
{
    const PieceType rook = setPieceColor(PieceType::WhiteRook, gs.player);
    const PieceType bishop = setPieceColor(PieceType::WhiteBishop, gs.player);
    const PieceType queen = setPieceColor(PieceType::WhiteQueen, gs.player);
    const PieceType king = setPieceColor(PieceType::WhiteKing, gs.player);
    for(size_t pieceX = 0; pieceX < BoardSize; pieceX++)
    {
        for(size_t pieceY = 0; pieceY < BoardSize; pieceY++)
        {
            const PieceType piece = gs.board[pieceX][pieceY];
            if(piece != rook && piece != queen && piece != bishop && piece != king)
                continue;
            for(int dx : {-1, 0, 1})
            {
                for(int dy : {-1, 0, 1})
                {
                    if(dx == 0 && dy == 0)
                        continue;
                    if(piece == rook && dx != 0 && dy != 0)
                        continue;
                    if(piece == bishop && (dx == 0 || dy == 0))
                        continue;
                    for(int x = pieceX + dx, y = pieceY + dy; (size_t)x < BoardSize && (size_t)y < BoardSize && x >= 0 && y >= 0; x += dx, y += dy)
                    {
                        PieceColor pieceColor = getPieceColor(gs.board[x][y]);
                        if(pieceColor == getPieceColor(gs.player))
                            break;
                        moves.push_back(GameStateMove(pieceX, pieceY, x, y));
                        if(pieceColor != PieceColor::None)
                            break;
                        if(piece == king)
                            break;
                    }
                }
            }
        }
    }
}

void addKnightMoves(vector<GameStateMove> & moves, GameState gs)
{
    const PieceType knight = setPieceColor(PieceType::WhiteKnight, gs.player);
    for(size_t pieceX = 0; pieceX < BoardSize; pieceX++)
    {
        for(size_t pieceY = 0; pieceY < BoardSize; pieceY++)
        {
            const PieceType piece = gs.board[pieceX][pieceY];
            if(piece != knight)
                continue;
            for(int dx : {-2, -1, 1, 2})
            {
                const int yStep = 3 - abs(dx);
                for(const int dy : {-yStep, yStep})
                {
                    const int x = pieceX + dx, y = pieceY + dy;
                    if(x < 0 || (size_t)x >= BoardSize || y < 0 || (size_t)y >= BoardSize)
                        continue;
                    PieceColor pieceColor = getPieceColor(gs.board[x][y]);
                    if(pieceColor != getPieceColor(gs.player))
                        moves.push_back(GameStateMove(pieceX, pieceY, x, y));
                }
            }
        }
    }
}

bool isRangeAttacked(GameState gs, size_t minX, size_t maxX, size_t y)
{
    for(size_t x = minX; x <= maxX; x++)
    {
        if(gs.isPositionAttacked(x, y))
            return true;
    }
    return false;
}

bool isRangeEmpty(GameState gs, size_t minX, size_t maxX, size_t y)
{
    for(size_t x = minX; x <= maxX; x++)
    {
        if(gs.board[x][y] != PieceType::Empty)
            return false;
    }
    return true;
}

void addCastlingMoves(vector<GameStateMove> & moves, GameState gs)
{
    if(gs.player == Player::White)
    {
        if(gs.whiteCanCastleLeft && isRangeEmpty(gs, 1, 3, 0) && !isRangeAttacked(gs, 2, 4, 0))
        {
            moves.push_back(GameStateMove(4, 0, 2, 0));
        }
        if(gs.whiteCanCastleRight && isRangeEmpty(gs, 5, 6, 0) && !isRangeAttacked(gs, 4, 6, 0))
        {
            moves.push_back(GameStateMove(4, 0, 6, 0));
        }
    }
    else
    {
        if(gs.blackCanCastleLeft && isRangeEmpty(gs, 1, 3, 7) && !isRangeAttacked(gs, 2, 4, 7))
        {
            moves.push_back(GameStateMove(4, 7, 2, 7));
        }
        if(gs.blackCanCastleRight && isRangeEmpty(gs, 5, 6, 7) && !isRangeAttacked(gs, 4, 6, 7))
        {
            moves.push_back(GameStateMove(4, 7, 6, 7));
        }
    }
}
}

const vector<GameStateMove> & GameStateCache::getValidMoves(GameState gs)
{
    Data & data = getGameStateEntry(gs);
    if(data.calculated)
        return data.validMoves;

    if(isTieCondition(gs))
    {
        data.calculated = true;
        return data.validMoves;
    }
    addPawnMoves(data.validMoves, gs);
    addRookBishopQueenAndKingMoves(data.validMoves, gs);
    addKnightMoves(data.validMoves, gs);
    addCastlingMoves(data.validMoves, gs);
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

void GameState::drawChessBoard(GameStateCache &cache, bool useUnicode, bool moveToHome, int startX, int startY, int endX, int endY) const
{
    if(moveToHome)
        cout << "\x1b[H";
    else
        cout << "\r\n";
    cout << (player == Player::White ? "White" : "Black") << "'s Turn.\x1b[K\r\n";
    const vector<GameStateMove> &moves = cache.getValidMoves(*this);
    vector<GameStateMove> filteredMoves;
    for(GameStateMove m : moves)
    {
        if((int)m.startX == startX && (int)m.startY == startY)
            filteredMoves.push_back(m);
    }
    for(size_t y = BoardSize - 1, i = 0; i < BoardSize; i++, y--)
    {
        for(size_t x = 0; x < BoardSize; x++)
        {
            int selectedColor = -1;
            if(getPieceColor(board[x][y]) != getPieceColor(player) && isPositionAttacked(x, y, getOpponent(player)))
                selectedColor = 0;
            if((int)x == startX && (int)y == startY && (int)x == endX && (int)y == endY)
            {
                selectedColor = 1;
            }
            else if((int)x == startX && (int)y == startY)
            {
                selectedColor = 4;
            }
            else if((int)x == endX && (int)y == endY)
            {
                selectedColor = 1;
                for(GameStateMove m : filteredMoves)
                {
                    if(m.endX == x && m.endY == y)
                    {
                        selectedColor = 7;
                        break;
                    }
                }
            }
            else
            {
                for(GameStateMove m : filteredMoves)
                {
                    if(m.endX == x && m.endY == y)
                    {
                        selectedColor = 2;
                        break;
                    }
                }
            }
            assert(selectedColor >= -1 && selectedColor <= 7);
            string openingChar = " ", closingChar = " ";
            if(selectedColor >= 0)
            {
                openingChar = "<";
                closingChar = ">";
            }
            if(getBoardColor(x, y) == BoardColor::Black)
            {
                cout << "\x1b[;40;1;3" << max(0, selectedColor) << "m" << openingChar;
            }
            else
            {
                cout << "\x1b[;47;1;3" << max(0, selectedColor) << "m" << openingChar;
            }
            cout << "\x1b[;" << (getPieceColor(board[x][y]) == PieceColor::Black ? "30" : "37") << ";1;" << (getBoardColor(x, y) == BoardColor::Black ? 40 : 47) << "m" << getPieceString(setPieceColor(board[x][y], PieceColor::Black), useUnicode);
            if(getBoardColor(x, y) == BoardColor::Black)
            {
                cout << "\x1b[;40;1;3" << max(0, selectedColor) << "m" << closingChar;
            }
            else
            {
                cout << "\x1b[;47;1;3" << max(0, selectedColor) << "m" << closingChar;
            }
        }
        cout << "\x1b[m\x1b[K\r\n";
    }
    cout << "\x1b[K\r\n" << flush;
}

namespace
{
float getBestMoveHelper(GameState gs, GameStateCache &cache, int depth, float bestValue = 1000, float worstValue = -1000)
{
    if(depth <= 0)
        return gs.getStaticEvaluation(cache);
    EndCondition endCondition = gs.getEndCondition(cache);
    if(endCondition != EndCondition::Nothing)
    {
        if(endCondition == EndCondition::Win)
            return 1000;
        if(endCondition == EndCondition::Lose)
            return -1000;
        return 0;
    }
    const vector<GameStateMove> moves = cache.getValidMoves(gs);
    assert(moves.size() != 0);
    float retval = worstValue;
    for(auto m : moves)
    {
        float v = -getBestMoveHelper(m.apply(gs), cache, depth - 1, -retval, -bestValue);
        retval = max(retval, v);
        if(retval >= bestValue)
            return retval;
    }
    return retval;
}
}

GameStateMove getBestMove(GameState gs, GameStateCache &cache, int depth)
{
    EndCondition endCondition = gs.getEndCondition(cache);
    if(endCondition != EndCondition::Nothing)
    {
        throw InvalidMove();
    }
    const vector<GameStateMove> moves = cache.getValidMoves(gs);
    assert(moves.size() != 0);
    float score;
    size_t bestMoveIndex;
    bool anyScore = false;
    for(size_t i = 0; i < moves.size(); i++)
    {
        auto m = moves[i];
        float v = -getBestMoveHelper(m.apply(gs), cache, depth - 1);
        if(!anyScore || v > score || (v == score && rand() % 3 == 0))
        {
            anyScore = true;
            score = v;
            bestMoveIndex = i;
        }
    }
    assert(anyScore);
    return moves[bestMoveIndex];
}
