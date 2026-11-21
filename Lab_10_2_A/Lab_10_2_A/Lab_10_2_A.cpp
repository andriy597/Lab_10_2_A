#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <Windows.h>               
using namespace std;

enum Specialnist { KOMP_NAUKY, INFORMATYKA, MATEM_EKON, FIZ_INF, TRUD_NAVCH };

struct Student
{
    string      prizv;
    int         kurs;
    Specialnist spec;
    int         fizyka;
    int         matematyka;
    int         informatyka;
};

int InputInt(const string prompt, const int lo, const int hi);
string InputPrizv();
void Create(Student* p, const int N);
void Print(Student* p, const int N, const string* specStr);
void Sort(Student* p, const int N);
int BinSearch(Student* p, const int N, const string prizv, const int kurs, const int informatyka);
int* IndexSort(Student* p, const int N);
void PrintIndexSorted(Student* p, int* I, const int N, const string* specStr);

int main()
{                             
    SetConsoleCP(1251);       
    SetConsoleOutputCP(1251); 

    // назви спеціальностей (локальний масив, оскільки глобальні змінні не допускаються)
    string specStr[] = { "Комп'ютерні науки", "Інформатика", "Математика та економіка", "Фізика та інформатика", "Трудове навчання" };

    int N = InputInt("Введіть кількість студентів N: ", 1, 1000);

    Student* p = new Student[N];

    int kurs;
    int informatyka;
    string prizv;
    int found;

    int menuItem;
    do {
        cout << endl << endl << endl;
        cout << "Виберіть дію:" << endl << endl;
        cout << " [1] - введення даних з клавіатури" << endl;
        cout << " [2] - вивід даних на екран" << endl;
        cout << " [3] - фізичне впорядкування даних" << endl;
        cout << " [4] - бінарний пошук студента за прізвищем, курсом" << endl;
        cout << "        та оцінкою з інформатики" << endl;
        cout << " [5] - індексне впорядкування та вивід даних"
            << endl << endl;
        cout << " [0] - вихід та завершення роботи програми" << endl << endl;
        cout << "Введіть значення: ";
        if (!(cin >> menuItem))   // контроль помилок: введено не число
        {
            cin.clear();
            cin.ignore(10000, '\n');
            menuItem = -1; // ставить неіснуючий пункт, щоб спрацювала гілка default
        }
        cout << endl << endl << endl;

        switch (menuItem)
        {
        case 1: // введення даних
            Create(p, N);
            break;
        case 2: // виведення таблиці
            Print(p, N, specStr);
            break;
        case 3: // впорядкування масиву
            Sort(p, N);
            break;
        case 4: // бінарний пошук
            cout << "Введіть ключі пошуку:" << endl;
            kurs = InputInt("     курс (1-6): ", 1, 6);
            informatyka = InputInt("     оцінка з інформатики (1-5): ", 1, 5);
            cin.get(); // очищення буфер клавіатури – бо залишаються символи
            cin.sync(); // "кінець рядка", які не дають ввести наступний літерний рядок

            cout << "     прізвище: "; prizv = InputPrizv();
            cout << endl;
            if ((found = BinSearch(p, N, prizv, kurs, informatyka)) != -1) // шукає стуудента і повертає число(якщо не знайшла -1)
                cout << "Знайдено студента в позиції " << found + 1 << endl; // (якщо знаййшла 0, 1, 2)
            else
                cout << "Шуканого студента не знайдено" << endl;
            break;
        case 5: // будує індексний масив і повертає ввказівник на нього
            PrintIndexSorted(p, IndexSort(p, N), N, specStr);
            break;
        case 0:
            break;
        default:
            cout << "Ви ввели помилкове значення! "
                "Слід ввести число - номер вибраного пункту меню" << endl;
        }
    } while (menuItem != 0); // повторює меню, поки не введено 0

    return 0;
}

int InputInt(const string prompt, const int lo, const int hi) // ввід числаа з перевіркою
{
    int x;
    cout << prompt;
    while (!(cin >> x) || x < lo || x > hi)
    {
        if (cin.eof())
            exit(1);
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Помилка! Слід ввести ціле число від " << lo << " до " << hi << "." << endl;
        cout << prompt;
    }
    return x;
}

string InputPrizv() // вводить прізвище
{
    string s;
    while (true)
    {
        getline(cin, s);
        if (!s.empty() && s.size() <= 15) // (непорожнє, не довше за 15 символів);
            return s;
        if (cin.eof())
            exit(1);
        cout << "Помилка! Прізвище має бути непорожнім та не довшим за 15 символів." << endl;
        cout << "     прізвище: ";
    }
}

void Create(Student* p, const int N) // введення масиву
{
    for (int i = 0; i < N; i++)
    {
        cout << "Студент № " << i + 1 << ":" << endl;

        cin.get(); // очищення буферу клавіатури – бо залишаються символи
        cin.sync(); // "кінець рядка", які не дають ввести наступний літерний рядок

        cout << "     прізвище: "; p[i].prizv = InputPrizv();
        p[i].kurs = InputInt("     курс (1-6): ", 1, 6);
        p[i].spec = (Specialnist)InputInt(
            "     спеціальність (0 - Комп'ютерні науки, 1 - Інформатика,\n"
            "                    2 - Математика та економіка, 3 - Фізика та інформатика,\n"
            "                    4 - Трудове навчання): ", 0, 4);
        p[i].fizyka = InputInt("     оцінка з фізики (1-5): ", 1, 5);
        p[i].matematyka = InputInt("     оцінка з математики (1-5): ", 1, 5);
        p[i].informatyka = InputInt("     оцінка з інформатики (1-5): ", 1, 5);
        cout << endl;
    }
}

void Print(Student* p, const int N, const string* specStr) // вивід таблиці
{
    cout << "Список студентів" << endl;
    cout << "=============================================================================================="
        << endl;
    cout << "| №   | Прізвище        | Курс | Спеціальність           | Фізика | Математика | Інформатика |"
        << endl;
    cout << "----------------------------------------------------------------------------------------------"
        << endl;

    for (int i = 0; i < N; i++)
    {
        cout << "| " << setw(3) << right << i + 1 << " ";
        cout << "| " << setw(15) << left << p[i].prizv
            << " | " << setw(4) << right << p[i].kurs
            << " | " << setw(23) << left << specStr[p[i].spec]
            << " | " << setw(6) << right << p[i].fizyka
            << " | " << setw(10) << right << p[i].matematyka
            << " | " << setw(11) << right << p[i].informatyka << " |" << endl;
    }
    cout << "=============================================================================================="
        << endl;
    cout << endl;
}

void Sort(Student* p, const int N) // фізиччне впорядкування
{
    Student tmp;
    for (int i0 = 0; i0 < N - 1; i0++)        // метод бульбашки
        for (int i1 = 0; i1 < N - i0 - 1; i1++)
            if ((p[i1].informatyka > p[i1 + 1].informatyka) // оцінка з інформатики першого більша
                ||
                (p[i1].informatyka == p[i1 + 1].informatyka && 
                    p[i1].kurs > p[i1 + 1].kurs) // оцінки рівні, а курс першого більший
                ||
                (p[i1].informatyka == p[i1 + 1].informatyka &&
                    p[i1].kurs == p[i1 + 1].kurs &&
                    p[i1].prizv < p[i1 + 1].prizv)) // оцінки й курси рівні, а прізвище першого менше
            {
                tmp = p[i1];
                p[i1] = p[i1 + 1];
                p[i1 + 1] = tmp; // Обмін двох студентів місцями
            }
}

int BinSearch(Student* p, const int N, const string prizv, const int kurs, const int informatyka) // бінарний пошук
{ 
    int L = 0, R = N - 1, m;
    do {
        m = (L + R) / 2; // середній елемент області
        if (p[m].prizv == prizv && p[m].kurs == kurs && p[m].informatyka == informatyka) // якщо збіглися три ключі повертаємо індекс студента
            return m;
        if ((p[m].informatyka < informatyka)
            ||
            (p[m].informatyka == informatyka && p[m].kurs < kurs)
            ||
            (p[m].informatyka == informatyka && p[m].kurs == kurs &&
                p[m].prizv > prizv))        // прізвища впорядковані за спаданням
        {
            L = m + 1; // відкидаємо ліву половину, бо елемент правіше
        }
        else
        {
            R = m - 1; // відкидаємо праву половину, бо елемент лівіше
        }
    } 
    while (L <= R); // цикл ппродовжується поки облаасть не порожжня
    return -1;
}

int* IndexSort(Student* p, const int N) // індексний масив
{ 

    int* I = new int[N];    // створили індексний масив

    for (int i = 0; i < N; i++)
        I[i] = i;             // заповнили його початковими даними

    int i, j, value;         // починаємо сортувати масив індексів
    for (i = 1; i < N; i++)
    {
        value = I[i];
        for (j = i - 1;
            j >= 0 && ((p[I[j]].informatyka > p[value].informatyka) // оцінка з інформатики першого більша
                || 
                (p[I[j]].informatyka == p[value].informatyka &&
                    p[I[j]].kurs > p[value].kurs) // оцінки рівні, а курс першого більший
                ||
                (p[I[j]].informatyka == p[value].informatyka &&
                    p[I[j]].kurs == p[value].kurs &&
                    p[I[j]].prizv > p[value].prizv)); // оцінки й курси рівні, а прізвище першого менше
            j--)
        {
            I[j + 1] = I[j];
        }
        I[j + 1] = value;
    }

    return I;
}

void PrintIndexSorted(Student* p, int* I, const int N, const string* specStr) // заголовок і шаапка таблиці перед списком студентів
{ 
    cout << "Список студентів (індексне впорядкування)" << endl;
    cout << "=============================================================================================="
        << endl;
    cout << "| №   | Прізвище        | Курс | Спеціальність           | Фізика | Математика | Інформатика |"
        << endl;
    cout << "----------------------------------------------------------------------------------------------"
        << endl;

    for (int i = 0; i < N; i++)
    {
        cout << "| " << setw(3) << right << i + 1 << " ";
        cout << "| " << setw(15) << left << p[I[i]].prizv
            << " | " << setw(4) << right << p[I[i]].kurs
            << " | " << setw(23) << left << specStr[p[I[i]].spec]
            << " | " << setw(6) << right << p[I[i]].fizyka
            << " | " << setw(10) << right << p[I[i]].matematyka
            << " | " << setw(11) << right << p[I[i]].informatyka << " |" << endl;
    }
    cout << "=============================================================================================="
        << endl;
    cout << endl;
}