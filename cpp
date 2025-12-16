#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

enum Speciality { CS, IT, MathEco, PhysInfo, Labor };

const char *specialityToStr(Speciality s) {
  switch (s) {
  case CS:
    return "CS";
  case IT:
    return "IT";
  case MathEco:
    return "MathEco";
  case PhysInfo:
    return "PhysInfo";
  case Labor:
    return "Labor";
  default:
    return "Unknown";
  }
}

union ThirdMark {
  int programming;
  int numerical;
  int pedagogy;
};

struct Student {
  int id;
  char surname[32];
  int course;
  Speciality spec;
  int physics;
  int math;
  ThirdMark third;
};

struct Index {
  int pos;
  int course;
  int profile;
  char surname[32];
};

int getProfileMark(const Student &s) {
  switch (s.spec) {
  case CS:
    return s.third.programming;
  case IT:
    return s.third.numerical;
  default:
    return s.third.pedagogy;
  }
}

bool compareStudents(const Student &a, const Student &b) {
  int pa = getProfileMark(a);
  int pb = getProfileMark(b);
  if (pa != pb)
    return pa < pb;
  if (a.course != b.course)
    return a.course < b.course;
  return strcmp(a.surname, b.surname) > 0;
}

bool compareIndex(const Index &a, const Index &b) {
  if (a.profile != b.profile)
    return a.profile < b.profile;
  if (a.course != b.course)
    return a.course < b.course;
  return strcmp(a.surname, b.surname) < 0;
}

bool binarySearch(Index arr[], int n, const char *surname, int course,
                  int profile) {
  int left = 0, right = n - 1;
  while (left <= right) {
    int mid = (left + right) / 2;
    int cmp = strcmp(arr[mid].surname, surname);
    if (arr[mid].profile == profile && arr[mid].course == course && cmp == 0)
      return true;
    if (arr[mid].profile < profile ||
        (arr[mid].profile == profile && arr[mid].course < course) ||
        (arr[mid].profile == profile && arr[mid].course == course && cmp < 0))
      left = mid + 1;
    else
      right = mid - 1;
  }
  return false;
}

void printTable(Student arr[], int n) {
  cout << left << setw(5) << "ID" << setw(15) << "Прізвище" << setw(10)
       << "Курс" << setw(10) << "Спец." << setw(10) << "Фізика" << setw(10)
       << "Математика" << setw(10) << "Програм." << setw(15) << "Числ.мет."
       << setw(10) << "Педагогіка" << endl;

  cout << string(90, '-') << endl;

  for (int i = 0; i < n; i++) {
    cout << left << setw(5) << arr[i].id << setw(18) << arr[i].surname
         << setw(10) << arr[i].course << setw(10)
         << specialityToStr(arr[i].spec) << setw(10) << arr[i].physics
         << setw(10) << arr[i].math;

    if (arr[i].spec == CS)
      cout << setw(10) << arr[i].third.programming << setw(10) << "-"
           << setw(12) << "-";
    else if (arr[i].spec == IT)
      cout << setw(10) << "-" << setw(10) << arr[i].third.numerical << setw(12)
           << "-";
    else
      cout << setw(10) << "-" << setw(10) << "-" << setw(12)
           << arr[i].third.pedagogy;

    cout << endl;
  }
}

int main() {
  const int N = 5;
  Student students[N] = {
      {1, "Іваненко", 2, CS, 85, 90, {.programming = 95}},
      {2, "Петренко", 1, IT, 70, 80, {.numerical = 75}},
      {3, "Сидоренко", 3, MathEco, 88, 92, {.pedagogy = 95}},
      {4, "Коваленко", 2, PhysInfo, 60, 65, {.pedagogy = 70}},
      {5, "Мельник", 1, Labor, 77, 81, {.pedagogy = 85}}};

  ofstream fout("students.pt", ios::binary);
  fout.write((char *)students, sizeof(students));
  fout.close();

  Student loaded[N];
  ifstream fin("students.pt", ios::binary);
  fin.read((char *)loaded, sizeof(loaded));
  fin.close();

  for (int i = 0; i < N - 1; i++)
    for (int j = 0; j < N - i - 1; j++)
      if (compareStudents(loaded[j + 1], loaded[j])) {
        Student tmp = loaded[j];
        loaded[j] = loaded[j + 1];
        loaded[j + 1] = tmp;
      }

  Index indexFile[N];
  for (int i = 0; i < N; i++) {
    indexFile[i].pos = i;
    indexFile[i].course = loaded[i].course;
    indexFile[i].profile = getProfileMark(loaded[i]);
    strcpy(indexFile[i].surname, loaded[i].surname);
  }

  for (int i = 0; i < N - 1; i++)
    for (int j = 0; j < N - i - 1; j++)
      if (compareIndex(indexFile[j + 1], indexFile[j])) {
        Index tmp = indexFile[j];
        indexFile[j] = indexFile[j + 1];
        indexFile[j + 1] = tmp;
      }

  cout << "Таблиця студентів (після сортування):\n";
  printTable(loaded, N);

  const char *targetSurname = "Сидоренко";
  int targetCourse = 3;
  int targetProfile = 95;

  if (binarySearch(indexFile, N, targetSurname, targetCourse, targetProfile))
    cout << "Студент знайдений!" << endl;
  else
    cout << "Студента не знайдено." << endl;

  return 0;
}
