#include <iostream>
#include <stack>
#include <stdlib.h>
#include <fstream>
using namespace std;

fstream f("date.in", ios :: in);
const int NMAX = 100;

struct Point
{
    int x, y;
}p0; ///punct folosit la sortarea celorlalte puncte dupa unghiul polar

Point nextToTop(stack<Point> &S) ///functie care gaseste urmatorul element dupa varful stivei
{
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}

int swap(Point &p1, Point &p2)
{
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}

int distSq(Point p1, Point p2) ///functie care returneaza patratul distantei dintre doua puncte
{
    return (p1.x - p2.x)*(p1.x - p2.x) +
          (p1.y - p2.y)*(p1.y - p2.y);
}

///Testul de orientare pentru tripletul ordonat (p, q, r)
int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  ///cele trei puncte sunt coliniare
    return (val > 0)? 1: 2; /// 1 -> sens invers triconometric ; 2 -> sens trigonometric
}

int compare(const void *vp1, const void *vp2) ///functie care compara punctele pentru qsort
{
   Point *p1 = (Point *)vp1;
   Point *p2 = (Point *)vp2;

   int o = orientation(p0, *p1, *p2);
   if (o == 0) ///daca punctele sunt coliniare
     return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1; ///cautam punctul cel mai apropiat de p0

   return (o == 2)? -1: 1; ///altfel returnam -1 daca punctele sunt in sens trigonometric si 1 daca sunt in sens invers trigonometric
}

void convexHull(Point points[], int n)
{
   int ymin = points[0].y; ///cautam punctul cel mai din stanga jos
   int min = 0;
   for (int i = 1; i < n; i++)
   {
     int y = points[i].y;
     ///alegem punctul cel mai de jos sau cel mai din stanga in caz ca au acelasi y
     if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
        ymin = points[i].y, min = i;
   }

   ///plaseaza punctul cel mai din stanga jos la inceputul vectorului de puncte
   swap(points[0], points[min]);

   p0 = points[0];
   qsort(&points[1], n-1, sizeof(Point), compare); ///sortam punctele ramase dupa unghiul polar in ordine trigonometrica in functie de p0

   ///daca doua / mai multe puncte au acelasi unghi polar atunci se elimina toate cu acelasi unghi polar mai putin acela dintre ele care este cel mai
   ///indepartat de p0

   int m = 1; ///va retine noua lungime a vectorului
   for (int i=1; i<n; i++)
   {
       while (i < n-1 && orientation(p0, points[i],points[i+1]) == 0)
          i++;

       points[m] = points[i];
       m++;
   }

   if (m < 3) return; ///daca dupa stergerea punctelor au ramas 3 sau mai putine puncte atunci acoperirea convexa nu este posibila

   ///altfel
   ///creem o stiva in care adaugam initial primele 3 puncte din vector
   stack<Point> S;
   S.push(points[0]);
   S.push(points[1]);
   S.push(points[2]);

   for (int i = 3; i < m; i++)
   {
      ///eliminam varful stivei cat timp ungiul format din punctele (urmatorul dupa varf, varful, punctul curent)face un viraj in sens invers trigonometric
      while (orientation(nextToTop(S), S.top(), points[i]) != 2)
         S.pop();
      S.push(points[i]);
   }

   while (!S.empty()) ///acum stiva contine acoperirea convexa si afisam continutul ei
   {
       Point p = S.top();
       cout << "(" << p.x << ", " << p.y <<")" << endl;
       S.pop();
   }
}

int main()
{
    Point points[NMAX];
    int n;

    f>>n;

    for(int i = 0;i < n; i++)
        f>>points[i].x>>points[i].y;

    convexHull(points, n);
    return 0;
}
