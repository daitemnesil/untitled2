#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

// Значения клеток
#define EMPTY '.'
#define CROSS 'X'
#define ZERO 'O'

// Статус игры
#define IN_PROGRESS 0
#define FIRST_PLAYER_WON 1
#define SECOND_PLAYER_WON 2
#define DRAW 3

// Максимальный размер поля
const int MAX_HEIGHT = 10;
const int MAX_WIDTH = 10;
const int MIN_WIDTH = 2;
const int MIN_HEIGHT = 2;

char field[MAX_HEIGHT][MAX_WIDTH];

// Текущий статус игры
int game_status = IN_PROGRESS;

void print_position(int n, int m);
void checkstatus(int n, int m, int k, int x, int y, bool turn, int count_moves);
void random_bot(int n, int m, bool turn, int *x, int *y);

// Режим игры: Player versus Player
string game_process_player_vs_player(int n, int m, int k)
{
    cout << "\n\nGAME STARTING...\n\n";
    string players[2] = { "Player1", "Player2" };
    bool turn = 0;
    int count_moves = 0;

    while (game_status == IN_PROGRESS)
    {
        // Печать текущей позиции игры
        print_position(n, m);
        count_moves++;
        cout << players[turn] << " make your move: ";
        int x, y;
        cin >> x >> y;
        while (x < 1 || y < 1 || x > n || y > m)
        {
            cout << "Incorrect position...\nRetry...: ";
            cin >> x >> y;
        }

        // Проверка, что позиция свободна
        while (field[x - 1][y - 1] != EMPTY)
        {
            cout << "This cell is busy already\nRetry...: ";
            cin >> x >> y;
        }

        // Сделать ход
        field[--x][--y] = (turn ? ZERO : CROSS);

        // Проверка статуса игры после последнего хода
        checkstatus(n, m, k, x, y, turn, count_moves);

        // Изменить ход
        turn = (turn == 0);
    }

    print_position(n, m);

    if (game_status == DRAW)
    {
        return "DRAW!";
    }
    return (players[game_status - 1] + " WON!");
}

// Режим игры: Player versus Bot
string game_process_player_vs_bot(int n, int m, int k)
{
    cout << "\n\nGAME STARTING...\n\n";
    string players[2] = { "Player1", "Bot" };
    bool turn = 0;
    int count_moves = 0;
    while (game_status == IN_PROGRESS)
    {
        // Печать текущей позиции игры
        print_position(n, m);
        count_moves++;
        cout << players[turn] << " make your move: ";
        if (!turn)
        {
            // Игрок делает ход
            int x, y;
            cin >> x >> y;
            while (x < 1 || y < 1 || x > n || y > m)
            {
                cout << "Incorrect position...\nRetry...: ";
                cin >> x >> y;
            }
            // Проверка, что позиция свободна
            while (field[x - 1][y - 1] != EMPTY)
            {
                cout << "This cell is busy already\nRetry...: ";
                cin >> x >> y;
            }

            // Сделать ход
            field[--x][--y] = (turn ? ZERO : CROSS);
            checkstatus(n, m, k, x, y, turn, count_moves);
        }
        else
        {
            // Бот делает ход
            int x, y;
            random_bot(n, m, turn, &x, &y);
            cout << x + 1 << ' ' << y + 1 << '\n';
            checkstatus(n, m, k, x, y, turn, count_moves);
        }

        turn = (turn == 0);
    }

    print_position(n, m);
    if (game_status == DRAW)
    {
        return "DRAW!";
    }
    return (players[game_status - 1] + " WON!");
}

// Инициализация полей
void initialize_nmk()
{
    for (int i = 0; i < MAX_HEIGHT; i++)
    {
        fill(field[i], field[i] + MAX_WIDTH, EMPTY);
    }
}

// Печать текущего состояния доски
void print_position(int n, int m)
{
    for (int i = -1; i < n; i++)
    {
        for (int j = -1; j < m; j++)
        {
            if (i == -1 && j >= 0)
            {
                cout << "\t" << j + 1;
            }
            else if (i >= 0 && j == -1)
            {
                cout << i + 1 << "\t";
            }

            if (i >= 0 && i < n && j >= 0 && j < m)
            {
                cout << field[i][j] << "\t";
            }

            if (j == m - 1)
            {
                cout << '\n';
            }
        }
    }
}

// Проверка текущего статуса игры
void checkstatus(int n, int m, int k, int x, int y, bool turn, int count_moves)
{
    // Проверка вертикали
    int cnt = 0;
    for (int i = 0; i < n; i++)
    {
        if (field[i][y] == (turn ? ZERO : CROSS))
            cnt++;
        else
            cnt = 0;
        if (cnt == k)
        {
            game_status = (turn ? SECOND_PLAYER_WON : FIRST_PLAYER_WON);
            break;
        }
    }

    // Проверка горизонтали
    for (int i = 0; i < m; i++)
    {
        if (field[x][i] == (turn ? ZERO : CROSS))
            cnt++;
        else
            cnt = 0;
        if (cnt == k)
        {
            game_status = (turn ? SECOND_PLAYER_WON : FIRST_PLAYER_WON);
            break;
        }
    }

    cnt = 0;
    // Проверка первичной диагонали
    for (int i = x, j = y; i >= 0 && j >= 0 && i < n && j < m; i++, j++)
    {
        if (field[i][j] != (turn ? ZERO : CROSS))
            break;

        if (field[i][j] == (turn ? ZERO : CROSS))
            cnt++;
    }

    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0 && i < n && j < m; i--, j--)
    {
        if (field[i][j] != (turn ? ZERO : CROSS))
            break;

        if (field[i][j] == (turn ? ZERO : CROSS))
            cnt++;
    }

    if (cnt >= k)
    {
        game_status = (turn ? SECOND_PLAYER_WON : FIRST_PLAYER_WON);
    }

    cnt = 0;

    // Проверка вторичной диагонали
    for (int i = x, j = y; i >= 0 && j >= 0 && i < n && j < m; i--, j++)
    {
        if (field[i][j] != (turn ? ZERO : CROSS))
            break;

        if (field[i][j] == (turn ? ZERO : CROSS))
            cnt++;
    }

    for (int i = x + 1, j = y - 1; i >= 0 && j >= 0 && i < n && j < m; i++, j--)
    {
        if (field[i][j] != (turn ? ZERO : CROSS))
            break;

        if (field[i][j] == (turn ? ZERO : CROSS))
            cnt++;
    }

    if (cnt >= k)
    {
        game_status = (turn ? SECOND_PLAYER_WON : FIRST_PLAYER_WON);
    }

    // Проверка игры
    if (count_moves == n * m && game_status == IN_PROGRESS)
    {
        game_status = DRAW;
    }
}

// Бот движущийся рандомно
void random_bot(int n, int m, bool turn, int *x, int *y)
{
    srand(time(NULL));
    *x = rand() % n;
    *y = rand() % m;
    while (field[*x][*y] != EMPTY)
    {
        *x = rand() % n;
        *y = rand() % m;
    }
    field[*x][*y] = turn ? ZERO : CROSS;
}

int main(int argc, const char *argv[])
{
    // Игра начинается
    initialize_nmk();
    int n, m, k;
    cout << "Print n, m and k: ";
    cin >> n >> m >> k;
    if (n <= 2 || m <= 2 || n > MAX_HEIGHT || m > MAX_WIDTH || (k > n && k > m))
    {
        cerr << "Incorrect input...";
        return 1;
    }
    cout << "Choose opponent:\n\
    1)Player\n\
    2)Random Bot\n";
    int opponent = 0;
    cin >> opponent;
    switch (opponent)
    {
        case 1:
            cout << game_process_player_vs_player(n, m, k) << '\n';
            break;

        case 2:
            cout << game_process_player_vs_bot(n, m, k) << '\n';
            break;
        default:
            cerr << "Undefined opponent type\n";
            return 1;
    }

    // Игра заканчивается
    return 0;
}

