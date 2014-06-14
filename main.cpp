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

void handleExit()
{
    tcsetattr(0, TCSADRAIN, &originalTermIOS);
}

void handleSignal(int sig)
{
    signal(sig, SIG_DFL);
    handleExit();
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
        os << "\x1B[H\x1B[" << (3 + (eventLog.size() - i - 1)) << "B\x1b[30C" << str << "\x1b[K";
    }
    os << "\x1b[u";
    cout << os.str() << flush;
}

mutex keyPressEventMutex;
condition_variable keyPressEventCond;
queue<KeyPressEvent> keyPressEvents;
atomic_bool backspacePressed(false);

bool hasKeyPressEvent()
{
    unique_lock<mutex> lock(keyPressEventMutex);
    return keyPressEvents.size() != 0;
}

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
    if(event == KeyPressEvent::Backspace)
        backspacePressed = true;
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
    auto moves = cache.getValidMoves(gs);
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
    auto moves = cache.getValidMoves(gs);
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
    drawEventLog();
    atomic_bool done(false);
    atomic<float> progress(0);
    thread waitThread([&done, &progress]()
    {
        int i = 0;
        while(!done)
        {
            ostringstream os;
            drawBoard();
            drawEventLog();
            os << "\x1b[s\x1b[H\x1b[12BWorking (" << (int)(100 * progress) << "%)";
            for(int j = 0; j < i + 2; j++)
                os << ".";
            os << "\x1b[u";
            cout << os.str() << flush;
            i++;
            i %= 4;
            for(int j = 0; j < 20; j++)
            {
                usleep(20000);
                if(done)
                    break;
            }
        }
        drawBoard();
        drawEventLog();
    });
    backspacePressed = false;
    try
    {
        GameStateMove m = getBestMove(gs, cache, backspacePressed, 5, &progress);
        done = true;
        waitThread.join();
        drawBoard(m.startX, m.startY, m.endX, m.endY);
        setEventLog();
        eventLog.push_back(m.toString(gs));
        drawEventLog();
        for(int i = 0; i < 10; i++)
        {
            usleep(100000);
            if(backspacePressed)
                throw CanceledMove();
        }
        makeMove(m);
        drawBoard();
        drawEventLog();
    }
    catch(CanceledMove &)
    {
        undoMove();
        drawBoard();
        drawEventLog();
    }
    done = true;
    if(waitThread.joinable())
        waitThread.join();
    while(hasKeyPressEvent())
        getKeyPressEvent();
}

vector<GameStateMove> pickMove(vector<GameStateMove> moves)
{
    size_t selected = 0;
    assert(moves.size() >= 1);
    for(;;)
    {
        drawBoard();
        drawEventLog();
        cout << "\x1b[H\x1b[m\x1b[5B";
        for(size_t i = 0; i < moves.size(); i++)
        {
            string str = moves[i].toString(gs);
            const size_t size = 18;
            while(str.size() < size)
            {
                str += " ";
            }
            cout << "\x1b[6C  ";
            if(i == selected)
                cout << "\x1b[1m";
            cout << str << "\x1b[m\r\n";
        }
        switch(getKeyPressEvent())
        {
        case KeyPressEvent::Space:
        case KeyPressEvent::Enter:
            return vector<GameStateMove>{moves[selected]};
        case KeyPressEvent::Q:
        case KeyPressEvent::Backspace:
            return vector<GameStateMove>();
        case KeyPressEvent::Up:
            if(selected > 0)
                selected--;
            break;
        case KeyPressEvent::Down:
            if(selected < moves.size() - 1)
                selected++;
            break;
        default:
            break;
        }
    }
}

void playGame(PieceColor computerColor)
{
    cout << "\x1b[m\x1b[H\x1b[2J";
    drawHeader();
    gss.clear();
    gs = GameState::makeInitialGameState();
    setEventLog();
    drawEventLog();
    int startX = BoardSize / 2, startY = BoardSize / 2;
    int endX = -1, endY = -1;
    while(gs.getEndCondition(cache) == EndCondition::Nothing)
    {
        if(getPieceColor(gs.player) == computerColor)
        {
            runComputerMove();
            continue;
        }
        drawBoard(startX, startY, endX, endY);
        drawEventLog();
        KeyPressEvent event = getKeyPressEvent();
        switch(event)
        {
        case KeyPressEvent::Q:
            return;
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
                vector<GameStateMove> validMoves = getValidMoves(startX, startY, endX, endY);
                if(validMoves.size() > 1)
                    validMoves = pickMove(validMoves);
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
            if(endX != -1)
            {
                endX = -1;
                endY = -1;
            }
            else
            {
                if(computerColor != PieceColor::None)
                    undoMove();
                undoMove();
                setEventLog();
                drawEventLog();
            }
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
    for(;;)
    {
        KeyPressEvent event = getKeyPressEvent();
        if(event == KeyPressEvent::Enter || event == KeyPressEvent::Space)
            break;
    }
}

int main()
{
    setTerminalToRaw();
    atexit(handleExit);
    thread(keyboardThreadFn).detach();
    int selected = 0;
    for(bool done = false;!done;)
    {
        string selectedStrings[4];
        selectedStrings[selected] = "\x1b[;1m";
        cout << "\x1b[m";
        drawHeader();
        cout << selectedStrings[0];
        cout << "<\x1b[4mP\x1b[m" << selectedStrings[0] << "layer versus Player>\x1b[m\r\n" << selectedStrings[1];
        cout << "<Player (\x1b[4mW\x1b[m" << selectedStrings[1] << "hite) versus Computer>\x1b[m\r\n" << selectedStrings[2];
        cout << "<Computer versus Player (\x1b[4mB\x1b[m" << selectedStrings[2] << "lack)>\x1b[m\r\n";
        cout << selectedStrings[3] << "<\x1b[4mQ\x1b[m" << selectedStrings[3] << "uit>\x1b[m\r\n\r\n" << flush;
        switch(getKeyPressEvent())
        {
        case KeyPressEvent::P:
            playGame(PieceColor::None);
            break;
        case KeyPressEvent::W:
            playGame(PieceColor::Black);
            break;
        case KeyPressEvent::B:
            playGame(PieceColor::White);
            break;
        case KeyPressEvent::Q:
            done = true;
            break;
        case KeyPressEvent::Space:
        case KeyPressEvent::Enter:
            switch(selected)
            {
            case 0:
                playGame(PieceColor::None);
                break;
            case 1:
                playGame(PieceColor::Black);
                break;
            case 2:
                playGame(PieceColor::White);
                break;
            default:
                done = true;
                break;
            }
            break;
        case KeyPressEvent::Up:
            selected = max(0, selected - 1);
            break;
        case KeyPressEvent::Down:
            selected = min(3, selected + 1);
            break;
        default:
            break;
        }
    }
    return 0;
}
