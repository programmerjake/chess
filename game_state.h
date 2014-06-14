#ifndef GAME_STATE_H_INCLUDED
#define GAME_STATE_H_INCLUDED

#include <memory>
#include <array>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <cstdint>
#include <tuple>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

enum class PieceType : uint8_t
{
    Empty,
    WhitePawn,
    WhiteRook,
    WhiteKnight,
    WhiteBishop,
    WhiteQueen,
    WhiteKing,
    BlackPawn,
    BlackRook,
    BlackKnight,
    BlackBishop,
    BlackQueen,
    BlackKing
};

constexpr size_t PieceTypeCount = 1 + 6 * 2;

enum class Player
{
    White,
    Black
};

typedef Player BoardColor;

enum class PieceColor
{
    White,
    Black,
    None
};

enum class EndCondition
{
    Nothing,
    Tie,
    Win,
    Lose
};

struct InvalidMove final : public runtime_error
{
    InvalidMove()
        : runtime_error("invalid move")
    {
    }
};

constexpr size_t BoardSize = 8;

inline BoardColor getBoardColor(size_t x, size_t y)
{
    if((x + y) % 2 == 0)
        return BoardColor::Black;
    return BoardColor::White;
}

inline PieceColor getPieceColor(PieceType piece)
{
    switch(piece)
    {
    case PieceType::Empty:
        return PieceColor::None;
    case PieceType::WhitePawn:
        return PieceColor::White;
    case PieceType::WhiteRook:
        return PieceColor::White;
    case PieceType::WhiteKnight:
        return PieceColor::White;
    case PieceType::WhiteBishop:
        return PieceColor::White;
    case PieceType::WhiteQueen:
        return PieceColor::White;
    case PieceType::WhiteKing:
        return PieceColor::White;
    case PieceType::BlackPawn:
        return PieceColor::Black;
    case PieceType::BlackRook:
        return PieceColor::Black;
    case PieceType::BlackKnight:
        return PieceColor::Black;
    case PieceType::BlackBishop:
        return PieceColor::Black;
    case PieceType::BlackQueen:
        return PieceColor::Black;
    case PieceType::BlackKing:
        return PieceColor::Black;
    }
    assert(false);
    return PieceColor::None;
}

inline PieceColor getPieceColor(Player player)
{
    switch(player)
    {
    case Player::Black:
        return PieceColor::Black;
    case Player::White:
        return PieceColor::White;
    }
    assert(false);
    return PieceColor::None;
}

inline Player getOpponent(Player player)
{
    if(player == Player::White)
        return Player::Black;
    return Player::White;
}

inline PieceColor getOpponent(PieceColor color)
{
    switch(color)
    {
    case PieceColor::Black:
        return PieceColor::White;
    case PieceColor::White:
        return PieceColor::Black;
    default:
        return PieceColor::None;
    }
}

inline PieceType setPieceColor(PieceType piece, PieceColor color)
{
    assert(color != PieceColor::None);
    switch(piece)
    {
    case PieceType::Empty:
        return PieceType::Empty;
    case PieceType::WhitePawn:
    case PieceType::BlackPawn:
        if(color == PieceColor::White)
            return PieceType::WhitePawn;
        return PieceType::BlackPawn;
    case PieceType::WhiteRook:
    case PieceType::BlackRook:
        if(color == PieceColor::White)
            return PieceType::WhiteRook;
        return PieceType::BlackRook;
    case PieceType::WhiteKnight:
    case PieceType::BlackKnight:
        if(color == PieceColor::White)
            return PieceType::WhiteKnight;
        return PieceType::BlackKnight;
    case PieceType::WhiteBishop:
    case PieceType::BlackBishop:
        if(color == PieceColor::White)
            return PieceType::WhiteBishop;
        return PieceType::BlackBishop;
    case PieceType::WhiteQueen:
    case PieceType::BlackQueen:
        if(color == PieceColor::White)
            return PieceType::WhiteQueen;
        return PieceType::BlackQueen;
    case PieceType::WhiteKing:
    case PieceType::BlackKing:
        if(color == PieceColor::White)
            return PieceType::WhiteKing;
        return PieceType::BlackKing;
    }
    assert(false);
    return PieceType::Empty;
}

inline PieceType setPieceColor(PieceType piece, Player player)
{
    switch(piece)
    {
    case PieceType::Empty:
        return PieceType::Empty;
    case PieceType::WhitePawn:
    case PieceType::BlackPawn:
        if(player == Player::White)
            return PieceType::WhitePawn;
        return PieceType::BlackPawn;
    case PieceType::WhiteRook:
    case PieceType::BlackRook:
        if(player == Player::White)
            return PieceType::WhiteRook;
        return PieceType::BlackRook;
    case PieceType::WhiteKnight:
    case PieceType::BlackKnight:
        if(player == Player::White)
            return PieceType::WhiteKnight;
        return PieceType::BlackKnight;
    case PieceType::WhiteBishop:
    case PieceType::BlackBishop:
        if(player == Player::White)
            return PieceType::WhiteBishop;
        return PieceType::BlackBishop;
    case PieceType::WhiteQueen:
    case PieceType::BlackQueen:
        if(player == Player::White)
            return PieceType::WhiteQueen;
        return PieceType::BlackQueen;
    case PieceType::WhiteKing:
    case PieceType::BlackKing:
        if(player == Player::White)
            return PieceType::WhiteKing;
        return PieceType::BlackKing;
    }
    assert(false);
    return PieceType::Empty;
}

inline string getPieceString(PieceType piece, bool useUnicode = true)
{
    switch(piece)
    {
    case PieceType::Empty:
        return " ";
    case PieceType::WhitePawn:
        return useUnicode ? "♙" : "P";
    case PieceType::WhiteRook:
        return useUnicode ? "♖" : "R";
    case PieceType::WhiteKnight:
        return useUnicode ? "♘" : "N";
    case PieceType::WhiteBishop:
        return useUnicode ? "♗" : "B";
    case PieceType::WhiteQueen:
        return useUnicode ? "♕" : "Q";
    case PieceType::WhiteKing:
        return useUnicode ? "♔" : "K";
    case PieceType::BlackPawn:
        return useUnicode ? "♟" : "p";
    case PieceType::BlackRook:
        return useUnicode ? "♜" : "r";
    case PieceType::BlackKnight:
        return useUnicode ? "♞" : "n";
    case PieceType::BlackBishop:
        return useUnicode ? "♝" : "b";
    case PieceType::BlackQueen:
        return useUnicode ? "♛" : "q";
    case PieceType::BlackKing:
        return useUnicode ? "♚" : "k";
    }
    assert(false);
    return " ";
}

struct GameStateCache;

struct GameState final
{
    array<array<PieceType, BoardSize>, BoardSize> board;
    Player player = Player::White;
    bool blackCanCastleLeft = true;
    bool blackCanCastleRight = true;
    bool whiteCanCastleLeft = true;
    bool whiteCanCastleRight = true;
    size_t enpassantCaptureX = 0;
    size_t enpassantCaptureY = 0;
    GameState()
    {
        for(auto & row : board)
        {
            for(auto & square : row)
            {
                square = PieceType::Empty;
            }
        }
    }
    static GameState makeInitialGameState()
    {
        GameState retval;
        for(auto & row : retval.board)
        {
            row[1] = PieceType::WhitePawn;
            row[6] = PieceType::BlackPawn;
        }
        retval.board[0][0] = PieceType::WhiteRook;
        retval.board[1][0] = PieceType::WhiteKnight;
        retval.board[2][0] = PieceType::WhiteBishop;
        retval.board[3][0] = PieceType::WhiteQueen;
        retval.board[4][0] = PieceType::WhiteKing;
        retval.board[5][0] = PieceType::WhiteBishop;
        retval.board[6][0] = PieceType::WhiteKnight;
        retval.board[7][0] = PieceType::WhiteRook;

        retval.board[0][7] = PieceType::BlackRook;
        retval.board[1][7] = PieceType::BlackKnight;
        retval.board[2][7] = PieceType::BlackBishop;
        retval.board[3][7] = PieceType::BlackQueen;
        retval.board[4][7] = PieceType::BlackKing;
        retval.board[5][7] = PieceType::BlackBishop;
        retval.board[6][7] = PieceType::BlackKnight;
        retval.board[7][7] = PieceType::BlackRook;
        retval.player = Player::White;
        return retval;
    }
    friend bool operator ==(const GameState &l, const GameState &r)
    {
        for(size_t x = 0; x < BoardSize; x++)
        {
            for(size_t y = 0; y < BoardSize; y++)
            {
                if(l.board[x][y] != r.board[x][y])
                    return false;
            }
        }
        if(l.player != r.player)
            return false;
        if(l.enpassantCaptureX != r.enpassantCaptureX)
            return false;
        if(l.enpassantCaptureY != r.enpassantCaptureY)
            return false;
        if(l.blackCanCastleLeft != r.blackCanCastleLeft)
            return false;
        if(l.blackCanCastleRight != r.blackCanCastleRight)
            return false;
        if(l.whiteCanCastleLeft != r.whiteCanCastleLeft)
            return false;
        if(l.whiteCanCastleRight != r.whiteCanCastleRight)
            return false;
        return true;
    }
    friend bool operator !=(const GameState &l, const GameState &r)
    {
        return !operator ==(l, r);
    }
private:
    EndCondition endCondition = EndCondition::Nothing;
    bool endConditionSet = false;
    void calcEndCondition(GameStateCache &cache);
public:
    inline EndCondition getEndCondition(GameStateCache &cache)
    {
        //if(!endConditionSet)
            calcEndCondition(cache);
        return endCondition;
    }
private:
    float staticEvaluation;
    bool staticEvaluationSet = false;
    void calcStaticEvaluation(GameStateCache &cache);
public:
    inline float getStaticEvaluation(GameStateCache &cache)
    {
        //if(!staticEvaluationSet)
            calcStaticEvaluation(cache);
        return staticEvaluation;
    }
private:
    bool isPositionAttackedByPawn(size_t x, size_t y, Player side) const;
    bool isPositionAttackedByRookOrQueenOnOrthogonals(size_t x, size_t y, Player side) const;
    bool isPositionAttackedByBishopOrQueenOnDiagonals(size_t x, size_t y, Player side) const;
    bool isPositionAttackedByKnight(size_t x, size_t y, Player side) const;
    bool isPositionAttackedByKing(size_t x, size_t y, Player side) const;
public:
    bool isPositionAttacked(size_t x, size_t y, Player side) const;
    inline bool isPositionAttacked(size_t x, size_t y) const
    {
        return isPositionAttacked(x, y, player);
    }
    bool isKingAttacked(Player side) const;
    inline bool isKingAttacked() const
    {
        return isKingAttacked(player);
    }
    void drawChessBoard(GameStateCache &cache, bool useUnicode = true, bool moveToHome = true, int startX = -1, int startY = -1, int endX = -1, int endY = -1) const;
};

namespace std
{
template <>
struct hash<GameState> final
{
    size_t operator ()(const GameState &gs) const
    {
        size_t retval = 0;
        for(size_t x = 0; x < BoardSize; x++)
        {
            for(size_t y = 0; y < BoardSize; y++)
            {
                retval += (size_t)gs.board[x][y];
                retval *= 3;
            }
        }
        return retval + (size_t)gs.player;
    }
};
}

struct GameStateMove final
{
    size_t startX, startY;
    size_t endX, endY;
    size_t captureX, captureY;
    PieceType promoteToType;
    GameStateMove(size_t startX, size_t startY, size_t endX, size_t endY, size_t captureX, size_t captureY, PieceType promoteToType = PieceType::Empty)
        : startX(startX), startY(startY), endX(endX), endY(endY), captureX(captureX), captureY(captureY), promoteToType(promoteToType)
    {
        assert(startX < BoardSize);
        assert(startY < BoardSize);
        assert(endX < BoardSize);
        assert(endY < BoardSize);
        assert(captureX < BoardSize);
        assert(captureY < BoardSize);
    }
    GameStateMove(size_t startX, size_t startY, size_t endX, size_t endY, PieceType promoteToType = PieceType::Empty)
        : GameStateMove(startX, startY, endX, endY, endX, endY, promoteToType)
    {
    }
    inline GameState apply(GameState gs) const
    {
        PieceType destType = promoteToType;
        if(destType == PieceType::Empty)
            destType = gs.board[startX][startY];
        gs.enpassantCaptureX = 0;
        gs.enpassantCaptureY = 0;
        gs.board[startX][startY] = PieceType::Empty;
        gs.board[captureX][captureY] = PieceType::Empty;
        gs.board[endX][endY] = destType;
        if(destType == PieceType::BlackKing && startX == 4 && startY == 7 && gs.blackCanCastleLeft && endX == 2 && endY == 7)
        {
            gs.board[3][7] = gs.board[0][7];
            gs.board[0][7] = PieceType::Empty;
        }
        else if(destType == PieceType::BlackKing && startX == 4 && startY == 7 && gs.blackCanCastleRight && endX == 6 && endY == 7)
        {
            gs.board[5][7] = gs.board[7][7];
            gs.board[7][7] = PieceType::Empty;
        }
        else if(destType == PieceType::WhiteKing && startX == 4 && startY == 0 && gs.whiteCanCastleLeft && endX == 2 && endY == 0)
        {
            gs.board[3][0] = gs.board[0][0];
            gs.board[0][0] = PieceType::Empty;
        }
        else if(destType == PieceType::WhiteKing && startX == 4 && startY == 0 && gs.whiteCanCastleRight && endX == 6 && endY == 0)
        {
            gs.board[5][0] = gs.board[7][0];
            gs.board[7][0] = PieceType::Empty;
        }
        if(destType == PieceType::BlackKing)
        {
            gs.blackCanCastleLeft = false;
            gs.blackCanCastleRight = false;
        }
        else if(destType == PieceType::WhiteKing)
        {
            gs.whiteCanCastleLeft = false;
            gs.whiteCanCastleRight = false;
        }
        else if(destType == PieceType::BlackRook && startX == 0)
        {
            gs.blackCanCastleLeft = false;
        }
        else if(destType == PieceType::BlackRook && startX == 7)
        {
            gs.blackCanCastleRight = false;
        }
        else if(destType == PieceType::WhiteRook && startX == 0)
        {
            gs.whiteCanCastleLeft = false;
        }
        else if(destType == PieceType::WhiteRook && startX == 7)
        {
            gs.whiteCanCastleRight = false;
        }
        else if(destType == PieceType::BlackPawn && startY == 6 && endY == 4)
        {
            gs.enpassantCaptureX = startX;
            gs.enpassantCaptureY = 5;
        }
        else if(destType == PieceType::WhitePawn && startY == 1 && endY == 3)
        {
            gs.enpassantCaptureX = startX;
            gs.enpassantCaptureY = 2;
        }
        gs.player = (gs.player == Player::Black ? Player::White : Player::Black);
        return gs;
    }
    string toString(GameState gs) const
    {
        ostringstream os;
        PieceType movingPiece = gs.board[startX][startY];
        if(movingPiece == PieceType::BlackKing || movingPiece == PieceType::WhiteKing)
        {
            if(abs((int)endX - (int)startX) >= 2)
            {
                if(endX < startX)
                    return "0-0-0";
                return "0-0";
            }
        }
        PieceType capturedPiece = gs.board[captureX][captureY];
        os << getPieceString(setPieceColor(movingPiece, Player::White), true);
        os << (char)('a' + startX) << (char)('1' + startY);
        if(capturedPiece != PieceType::Empty)
            os << "x";
        os << (char)('a' + endX) << (char)('1' + endY);
        if(promoteToType != PieceType::Empty)
        {
            os << "=" << getPieceString(setPieceColor(promoteToType, Player::White), true);
        }
        if(captureX != endX || captureY != endY)
            os << "e.p.";
        return os.str();
    }
};

struct GameStateCache final
{
    const vector<GameStateMove> & getValidMoves(GameState gs);
private:
    struct Data final
    {
        vector<GameStateMove> validMoves;
        uint64_t lastAccessTimeStamp = 0;
        bool calculated = false;
    };
    unordered_map<GameState, Data> validMovesMap;
    static constexpr size_t maxEntryCount = 200000;
    static constexpr size_t entryCountSlop = 20000;
    uint64_t currentTimeStamp = 0;
    inline Data & getGameStateEntry(GameState gs)
    {
        if(validMovesMap.size() > maxEntryCount + entryCountSlop)
        {
            const uint64_t minTimeStampToKeep = currentTimeStamp - maxEntryCount;
            for(auto i = validMovesMap.begin(); i != validMovesMap.end();)
            {
                Data & data = get<1>(*i);
                if(data.lastAccessTimeStamp < minTimeStampToKeep)
                    i = validMovesMap.erase(i);
                else
                    i++;
            }
        }
        Data & retval = validMovesMap[gs];
        retval.lastAccessTimeStamp = ++currentTimeStamp;
        return retval;
    }
public:
    void dumpStats()
    {
        cout << "Game State Count : " << validMovesMap.size() << "\x1b[K\r\n" << flush;
    }
};

GameStateMove getBestMove(GameState gs, GameStateCache &cache, int depth = 3);

#endif // GAME_STATE_H_INCLUDED
