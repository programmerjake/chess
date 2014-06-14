#include "game_state.h"
#include <cstdlib>
#include <termios.h>
#include <signal.h>
#include <cwchar>
#include <unistd.h>
#include <poll.h>
#include <thread>
#include <atomic>
#include <sstream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <tuple>

struct termios originalTermIOS;

void handleSignal(int sig)
{
    signal(sig, SIG_DFL);
    tcsetattr(0, TCSADRAIN, &originalTermIOS);
    raise(sig);
}

void setTerminalToRaw()
{
    struct termios tios;
    tcgetattr(0, &tios);
    originalTermIOS = tios;
    cfmakeraw(&tios);
    tios.c_lflag |= ISIG;
    tcsetattr(0, TCSADRAIN, &tios);
    if(signal(SIGINT, handleSignal) == SIG_IGN)
        signal(SIGINT, SIG_IGN);
    if(signal(SIGTERM, handleSignal) == SIG_IGN)
        signal(SIGTERM, SIG_IGN);
}

enum class KeyPressEvent
{
    Up,
    Down,
    Left,
    Right,
    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    Space,
    Enter,
    Backspace,
    Unknown,
};

const string KeyPressEventTranslations[] =
{
    "Up",
    "Down",
    "Left",
    "Right",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "Space",
    "Enter",
    "Backspace",
    "Unknown",
};

vector<string> eventLog;

void drawEventLog()
{
    const ssize_t displayLineCount = 12;
    ostringstream os;
    os << "\x1B[s";
    for(ssize_t i = (ssize_t)eventLog.size() - displayLineCount; i < (ssize_t)eventLog.size(); i++)
    {
        string str = "";
        if(i >= 0)
            str = eventLog[i];
        os << "\x1B[H\x1B[" << (15 + (eventLog.size() - i - 1)) << "B" << str << "\x1b[K";
    }
    os << "\x1b[u";
    cout << os.str() << flush;
}

mutex keyPressEventMutex;
condition_variable keyPressEventCond;
queue<KeyPressEvent> keyPressEvents;

KeyPressEvent getKeyPressEvent()
{
    unique_lock<mutex> lock(keyPressEventMutex);
    while(keyPressEvents.size() == 0)
        keyPressEventCond.wait(lock);
    KeyPressEvent retval = keyPressEvents.front();
    keyPressEvents.pop();
    return retval;
}

void pushKeyPressEvent(KeyPressEvent event)
{
    unique_lock<mutex> lock(keyPressEventMutex);
    keyPressEvents.push(event);
    keyPressEventCond.notify_all();
}

struct ParserStateMachine
{
    enum class State
    {
        Start,
        GotEscape,
        GotEscapeLBracket,
    };
    State state = State::Start;
    void parseChar(char ch)
    {
        switch(state)
        {
        case State::Start:
            switch(ch)
            {
            case '\x1B':
                state = State::GotEscape;
                return;
            case '0':
                pushKeyPressEvent(KeyPressEvent::Num0);
                return;
            case '1':
                pushKeyPressEvent(KeyPressEvent::Num1);
                return;
            case '2':
                pushKeyPressEvent(KeyPressEvent::Num2);
                return;
            case '3':
                pushKeyPressEvent(KeyPressEvent::Num3);
                return;
            case '4':
                pushKeyPressEvent(KeyPressEvent::Num4);
                return;
            case '5':
                pushKeyPressEvent(KeyPressEvent::Num5);
                return;
            case '6':
                pushKeyPressEvent(KeyPressEvent::Num6);
                return;
            case '7':
                pushKeyPressEvent(KeyPressEvent::Num7);
                return;
            case '8':
                pushKeyPressEvent(KeyPressEvent::Num8);
                return;
            case '9':
                pushKeyPressEvent(KeyPressEvent::Num9);
                return;
            case 'a':
            case 'A':
                pushKeyPressEvent(KeyPressEvent::A);
                return;
            case 'b':
            case 'B':
                pushKeyPressEvent(KeyPressEvent::B);
                return;
            case 'c':
            case 'C':
                pushKeyPressEvent(KeyPressEvent::C);
                return;
            case 'd':
            case 'D':
                pushKeyPressEvent(KeyPressEvent::D);
                return;
            case 'e':
            case 'E':
                pushKeyPressEvent(KeyPressEvent::E);
                return;
            case 'f':
            case 'F':
                pushKeyPressEvent(KeyPressEvent::F);
                return;
            case 'g':
            case 'G':
                pushKeyPressEvent(KeyPressEvent::G);
                return;
            case 'h':
            case 'H':
                pushKeyPressEvent(KeyPressEvent::H);
                return;
            case 'i':
            case 'I':
                pushKeyPressEvent(KeyPressEvent::I);
                return;
            case 'j':
            case 'J':
                pushKeyPressEvent(KeyPressEvent::J);
                return;
            case 'k':
            case 'K':
                pushKeyPressEvent(KeyPressEvent::K);
                return;
            case 'l':
            case 'L':
                pushKeyPressEvent(KeyPressEvent::L);
                return;
            case 'm':
            case 'M':
                pushKeyPressEvent(KeyPressEvent::M);
                return;
            case 'n':
            case 'N':
                pushKeyPressEvent(KeyPressEvent::N);
                return;
            case 'o':
            case 'O':
                pushKeyPressEvent(KeyPressEvent::O);
                return;
            case 'p':
            case 'P':
                pushKeyPressEvent(KeyPressEvent::P);
                return;
            case 'q':
            case 'Q':
                pushKeyPressEvent(KeyPressEvent::Q);
                return;
            case 'r':
            case 'R':
                pushKeyPressEvent(KeyPressEvent::R);
                return;
            case 's':
            case 'S':
                pushKeyPressEvent(KeyPressEvent::S);
                return;
            case 't':
            case 'T':
                pushKeyPressEvent(KeyPressEvent::T);
                return;
            case 'u':
            case 'U':
                pushKeyPressEvent(KeyPressEvent::U);
                return;
            case 'v':
            case 'V':
                pushKeyPressEvent(KeyPressEvent::V);
                return;
            case 'w':
            case 'W':
                pushKeyPressEvent(KeyPressEvent::W);
                return;
            case 'x':
            case 'X':
                pushKeyPressEvent(KeyPressEvent::X);
                return;
            case 'y':
            case 'Y':
                pushKeyPressEvent(KeyPressEvent::Y);
                return;
            case 'z':
            case 'Z':
                pushKeyPressEvent(KeyPressEvent::Z);
                return;
            case '\r':
            case '\n':
                pushKeyPressEvent(KeyPressEvent::Enter);
                return;
            case ' ':
                pushKeyPressEvent(KeyPressEvent::Space);
                return;
            case '\b':
            case '\x7F':
                pushKeyPressEvent(KeyPressEvent::Backspace);
                return;
            default:
                pushKeyPressEvent(KeyPressEvent::Unknown);
                return;
            }
            break;
        case State::GotEscape:
            if(ch == '[')
            {
                state = State::GotEscapeLBracket;
                return;
            }
            else
            {
                pushKeyPressEvent(KeyPressEvent::Unknown);
                state = State::Start;
                parseChar(ch);
            }
            break;
        case State::GotEscapeLBracket:
            switch(ch)
            {
            case 'A':
                pushKeyPressEvent(KeyPressEvent::Up);
                state = State::Start;
                return;
            case 'B':
                pushKeyPressEvent(KeyPressEvent::Down);
                state = State::Start;
                return;
            case 'C':
                pushKeyPressEvent(KeyPressEvent::Right);
                state = State::Start;
                return;
            case 'D':
                pushKeyPressEvent(KeyPressEvent::Left);
                state = State::Start;
                return;
            default:
                pushKeyPressEvent(KeyPressEvent::Unknown);
                state = State::Start;
                return;
            }
            break;
        }
    }
};

void keyboardThreadFn()
{
    ParserStateMachine psm;
    while(true)
    {
        char buffer[256];
        ssize_t readRetval = read(0, (void *)&buffer[0], sizeof(buffer));
        if(readRetval == 0)
            continue;
        if(readRetval == -1)
            break;
        for(size_t i = 0; i < (size_t)readRetval; i++)
            psm.parseChar(buffer[i]);
    }
}

GameState gs = GameState::makeInitialGameState();
vector<pair<GameState, GameStateMove>> gss;
GameStateCache cache;

void drawBoard(int startX = -1, int startY = -1, int endX = -1, int endY = -1)
{
    gs.drawChessBoard(cache, true, true, startX, startY, endX, endY);
}

void makeMove(GameStateMove m)
{
    gss.push_back(make_pair(gs, m));
    gs = m.apply(gs);
}

void undoMove()
{
    if(!gss.empty())
    {
        auto v = gss.back();
        gss.pop_back();
        gs = get<0>(v);
    }
}

bool anyValidMove(int startX, int startY)
{
    vector<GameStateMove> moves = cache.getValidMoves(gs);
    for(auto m : moves)
    {
        if(m.startX == (size_t)startX && m.startY == (size_t)startY)
            return true;
    }
    return false;
}

void setEventLog()
{
    eventLog.clear();
    for(auto m : gss)
    {
        eventLog.push_back(get<1>(m).toString(get<0>(m)));
    }
}

vector<GameStateMove> getValidMoves(int startX, int startY, int endX, int endY)
{
    vector<GameStateMove> moves = cache.getValidMoves(gs);
    vector<GameStateMove> retval;
    for(auto m : moves)
    {
        if(m.startX == (size_t)startX && m.startY == (size_t)startY && m.endX == (size_t)endX && m.endY == (size_t)endY)
            retval.push_back(m);
    }
    return retval;
}

void runComputerMove()
{
    drawBoard();
    atomic_bool done(false);
    thread waitThread([&done]()
    {
        int i = 0;
        while(!done)
        {
            usleep(200000);
            ostringstream os;
            os << "\x1b[s\x1b[H\x1b[14BWorking";
            for(int j = 0; j < i + 2; j++)
                os << ".";
            os << "\x1b[K\x1b[u";
            cout << os.str() << flush;
            i++;
            i %= 3;
        }
        cout << "\x1b[s\x1b[H\x1b[14B\x1b[K\x1b[u" << flush;
    });
    GameStateMove m = getBestMove(gs, cache, 5);
    drawBoard(m.startX, m.startY, m.endX, m.endY);
    setEventLog();
    eventLog.push_back(m.toString(gs));
    drawEventLog();
    done = true;
    sleep(1);
    waitThread.join();
    makeMove(m);
    drawBoard();
}

void playGame()
{
    int startX = BoardSize / 2, startY = BoardSize / 2;
    int endX = -1, endY = -1;
    while(gs.getEndCondition(cache) == EndCondition::Nothing)
    {
        if(gs.player == Player::Black)
        {
            runComputerMove();
            continue;
        }
        drawBoard(startX, startY, endX, endY);
        KeyPressEvent event = getKeyPressEvent();
        switch(event)
        {
        case KeyPressEvent::Q:
            exit(0);
            break;
        case KeyPressEvent::Space:
        case KeyPressEvent::Enter:
            if(endX == -1)
            {
                if(anyValidMove(startX, startY))
                {
                    endX = startX;
                    endY = startY;
                }
            }
            else if(startX == endX && startY == endY)
            {
                endX = -1;
                endY = -1;
            }
            else
            {
                auto validMoves = getValidMoves(startX, startY, endX, endY);
                if(!validMoves.empty())
                {
                    assert(validMoves.size() == 1);
                    makeMove(validMoves.front());
                    setEventLog();
                    drawEventLog();
                    endX = -1;
                    endY = -1;
                }
            }
            break;
        case KeyPressEvent::Left:
            if(endX == -1)
                startX = max(0, startX - 1);
            else
                endX = max(0, endX - 1);
            break;
        case KeyPressEvent::Down:
            if(endX == -1)
                startY = max(0, startY - 1);
            else
                endY = max(0, endY - 1);
            break;
        case KeyPressEvent::Up:
            if(endX == -1)
                startY = min<int>(BoardSize - 1, startY + 1);
            else
                endY = min<int>(BoardSize - 1, endY + 1);
            break;
        case KeyPressEvent::Right:
            if(endX == -1)
                startX = min<int>(BoardSize - 1, startX + 1);
            else
                endX = min<int>(BoardSize - 1, endX + 1);
            break;
        case KeyPressEvent::A:
            if(endX == -1)
                startX = 0;
            else
                endX = 0;
            break;
        case KeyPressEvent::B:
            if(endX == -1)
                startX = 1;
            else
                endX = 1;
            break;
        case KeyPressEvent::C:
            if(endX == -1)
                startX = 2;
            else
                endX = 2;
            break;
        case KeyPressEvent::D:
            if(endX == -1)
                startX = 3;
            else
                endX = 3;
            break;
        case KeyPressEvent::E:
            if(endX == -1)
                startX = 4;
            else
                endX = 4;
            break;
        case KeyPressEvent::F:
            if(endX == -1)
                startX = 5;
            else
                endX = 5;
            break;
        case KeyPressEvent::G:
            if(endX == -1)
                startX = 6;
            else
                endX = 6;
            break;
        case KeyPressEvent::H:
            if(endX == -1)
                startX = 7;
            else
                endX = 7;
            break;
        case KeyPressEvent::Num1:
            if(endX == -1)
                startY = 0;
            else
                endY = 0;
            break;
        case KeyPressEvent::Num2:
            if(endX == -1)
                startY = 1;
            else
                endY = 1;
            break;
        case KeyPressEvent::Num3:
            if(endX == -1)
                startY = 2;
            else
                endY = 2;
            break;
        case KeyPressEvent::Num4:
            if(endX == -1)
                startY = 3;
            else
                endY = 3;
            break;
        case KeyPressEvent::Num5:
            if(endX == -1)
                startY = 4;
            else
                endY = 4;
            break;
        case KeyPressEvent::Num6:
            if(endX == -1)
                startY = 5;
            else
                endY = 5;
            break;
        case KeyPressEvent::Num7:
            if(endX == -1)
                startY = 6;
            else
                endY = 6;
            break;
        case KeyPressEvent::Num8:
            if(endX == -1)
                startY = 7;
            else
                endY = 7;
            break;
        case KeyPressEvent::Backspace:
            undoMove();
            undoMove();
            setEventLog();
            drawEventLog();
            break;
        default:
            break;
        }
    }
    drawBoard();
    string sideStr = "White";
    if(gs.player == Player::Black)
    {
        sideStr = "Black";
    }
    switch(gs.getEndCondition(cache))
    {
    case EndCondition::Win:
        eventLog.push_back(sideStr + " Wins");
        break;
    case EndCondition::Lose:
        eventLog.push_back(sideStr + " Loses");
        break;
    case EndCondition::Tie:
        eventLog.push_back("Tied");
        break;
    default:
        assert(false);
        break;
    }
    drawEventLog();
}

int main()
{
    setTerminalToRaw();
    atexit([](){raise(SIGTERM);});
    thread(keyboardThreadFn).detach();
    playGame();
    raise(SIGTERM);
    return 0;
}
