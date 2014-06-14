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
#include <atomic>
#include "static_vector.h"

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

struct CanceledMove final : public runtime_error
{
    CanceledMove()
        : runtime_error("move canceled")
    {
    }
};

struct GameStateMove final
{
    unsigned startX : 4, startY : 4;
    unsigned endX : 4, endY : 4;
    unsigned captureX : 4, captureY : 4;
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
        os << getPieceString(setPieceColor(movingPiece, Player::Black), true);
        os << (char)('a' + startX) << (char)('1' + startY);
        if(capturedPiece != PieceType::Empty)
            os << "x";
        os << (char)('a' + endX) << (char)('1' + endY);
        if(promoteToType != PieceType::Empty)
        {
            os << "=" << getPieceString(setPieceColor(promoteToType, Player::Black), true);
        }
        if(captureX != endX || captureY != endY)
            os << "e.p.";
        return os.str();
    }
};

class GameStateCache final
{
    static constexpr size_t maxMovesPerRook = 14;
    static constexpr size_t maxMovesPerKnight = 8;
    static constexpr size_t maxMovesPerBishop = 15;
    static constexpr size_t maxMovesPerQueen = maxMovesPerRook + maxMovesPerBishop;
    static constexpr size_t maxMovesPerPawn = maxMovesPerQueen; // after they have been promoted
    static constexpr size_t maxMovesPerKing = 8; // only 5 other moves when we can castle
public:
    static constexpr size_t maxMoves = maxMovesPerPawn * 8 + maxMovesPerRook * 2 + maxMovesPerKnight * 2 + maxMovesPerBishop * 2 + maxMovesPerQueen + maxMovesPerKing;
    typedef vector<GameStateMove> MovesList;
    const MovesList & getValidMoves(GameState gs);
private:
    struct EvaluationEntry final
    {
        float minValue;
        float maxValue;
        bool haveMin = false;
        bool haveMax = false;
        EvaluationEntry()
        {
        }
        EvaluationEntry(float v)
            : minValue(v), maxValue(v), haveMin(true), haveMax(true)
        {
        }
        inline float getAverage() const
        {
            assert(haveMin || haveMax);
            if(haveMin && haveMax)
                return 0.5f * (minValue + maxValue);
            if(haveMin)
                return minValue;
            return maxValue;
        }
        inline bool hasAnyData() const
        {
            return haveMin || haveMax;
        }
    };
    struct Data final
    {
        GameState gs;
        Data * hashNext = nullptr;
        MovesList validMoves;
        bool used = true;
        bool calculated = false;
        vector<EvaluationEntry> evaluationValue;
        Data(GameState gs)
            : gs(gs)
        {
        }
    };
    inline float getSortingEvaluation(Data & data)
    {
        while(!data.evaluationValue.empty() && !data.evaluationValue.back().hasAnyData())
            data.evaluationValue.pop_back();
        if(data.evaluationValue.empty())
            data.evaluationValue.push_back(data.gs.getStaticEvaluation(*this));
        return data.evaluationValue.back().getAverage();
    }
    void sortValidMoves(Data & data);
    void sortValidMoves(GameState gs)
    {
        sortValidMoves(getGameStateEntry(gs));
    }
    static constexpr size_t hashPrime = 100003;
    array<Data *, hashPrime> hashTable;
    union FreeListElement
    {
        char data[sizeof(Data)];
        FreeListElement * next;
    };
    static_assert(sizeof(FreeListElement) == sizeof(Data), "FreeListElement is not the right size");
    struct DataArena final
    {
        static const size_t BlockSize = 1024;
        typedef FreeListElement MemoryBlock;
        array<MemoryBlock, BlockSize> blocks;
        size_t allocated = 0;
    };
    vector<DataArena *> arenas;
    FreeListElement * freeListHead = nullptr;
    size_t hashTableSize = 0;
    inline Data * allocateData(GameState gs)
    {
        FreeListElement * mem = freeListHead;
        if(freeListHead != nullptr)
            freeListHead = freeListHead->next;
        else
        {
            if(arenas.empty() || arenas.back()->allocated >= DataArena::BlockSize)
            {
                arenas.push_back(new DataArena);
            }
            mem = &arenas.back()->blocks[arenas.back()->allocated++];
        }
        return new((void *)mem) Data(gs);
    }
    inline void freeData(Data * data)
    {
        data->~Data();
        FreeListElement * fle = (FreeListElement *)data;
        fle->next = freeListHead;
        freeListHead = fle;
    }
    std::hash<GameState> hasher;
    inline Data & find(GameState gs)
    {
        size_t hash = hasher(gs) % hashPrime;
        Data ** ppnode = &hashTable[hash];
        Data * pnode = *ppnode;
        while(pnode != nullptr)
        {
            if(pnode->gs == gs)
            {
                *ppnode = pnode->hashNext;
                pnode->hashNext = hashTable[hash];
                hashTable[hash] = pnode;
                return *pnode;
            }
            ppnode = &pnode->hashNext;
            pnode = *ppnode;
        }
        pnode = allocateData(gs);
        hashTableSize++;
        pnode->hashNext = hashTable[hash];
        hashTable[hash] = pnode;
        return *pnode;
    }
    static constexpr size_t maxEntryCount = 1000000;
    static constexpr size_t entryCountSlop = 100000;
public:
    GameStateCache()
    {
        for(Data *& v : hashTable)
        {
            v = nullptr;
        }
    }
    ~GameStateCache()
    {
        for(Data * node : hashTable)
        {
            while(node != nullptr)
            {
                Data * deleteMe = node;
                node = node->hashNext;
                freeData(deleteMe);
            }
        }
        for(DataArena * arena : arenas)
            delete arena;
    }
private:
    GameStateCache(const GameStateCache &) = delete;
    const GameStateCache & operator =(const GameStateCache &) = delete;
    static constexpr size_t maxCollectTime = maxEntryCount * 20;
    size_t collectTimeLeft = maxCollectTime;
    static constexpr size_t collectTimeSlop = maxCollectTime / 10;
    inline Data & getGameStateEntry(GameState gs)
    {
        if((hashTableSize > maxEntryCount + entryCountSlop && collectTimeLeft < maxCollectTime - collectTimeSlop) || (hashTableSize > maxEntryCount && --collectTimeLeft == 0) || hashTableSize > maxEntryCount + entryCountSlop * 2)
        {
            collectTimeLeft = maxCollectTime;
            for(Data *& tableEntry : hashTable)
            {
                Data ** ppnode = &tableEntry;
                Data * pnode = *ppnode;
                for(;pnode != nullptr;)
                {
                    Data & data = *pnode;
                    if(!data.used)
                    {
                        *ppnode = pnode->hashNext;
                        freeData(pnode);
                        pnode = *ppnode;
                        hashTableSize--;
                    }
                    else
                    {
                        data.used = false;
                        ppnode = &pnode->hashNext;
                        pnode = *ppnode;
                    }
                }
            }
        }
        Data & retval = find(gs);
        retval.used = true;
        return retval;
    }
    float evaluateMoveHelper(GameState gs, atomic_bool &canceled, int depth, float bestValue, float worstValue);
    float evaluateMove(GameState gs, atomic_bool &canceled, int depth);
public:
    void dumpStats()
    {
        cout << "Game State Count : " << hashTableSize;
    }
    GameStateMove getBestMove(GameState gs, atomic_bool &canceled, int depth = 3, atomic<float> *progress = nullptr);
};

inline GameStateMove getBestMove(GameState gs, GameStateCache &cache, atomic_bool &canceled, int depth = 3, atomic<float> *progress = nullptr)
{
    return cache.getBestMove(gs, canceled, depth, progress);
}

inline GameStateMove getBestMove(GameState gs, GameStateCache &cache, int depth = 3, atomic<float> *progress = nullptr)
{
    atomic_bool canceled(false);
    return cache.getBestMove(gs, canceled, depth, progress);
}

inline void drawHeader()
{
    cout << "\x1b[H\x1b[m\x1b[2J    Chess 1.0   By Jacob Lifshay (c) 2014\r\n\r\n" << flush;
}

#endif // GAME_STATE_H_INCLUDED
