// feature10_overdue_books.c
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_MEMBERS 100
#define MAX_BOOKS 100
#define MAX_BORROWS 500
#define NAME_LEN 100
#define TITLE_LEN 150

typedef struct { int id; char name[NAME_LEN]; int age; } Member;
typedef struct { int id; char title[TITLE_LEN]; char author[NAME_LEN]; int available; } Book;
typedef struct {
    int borrow_id; int member_id; int book_id; int active;
    int borrow_yyyy, borrow_mm, borrow_dd; int due_yyyy, due_mm, due_dd;
} BorrowRecord;

Member members[MAX_MEMBERS]; int member_count=0;
Book books[MAX_BOOKS]; int book_count=0;
BorrowRecord borrows[MAX_BORROWS]; int borrow_count=0;

void seed_data() {
    members[0].id=1; strcpy(members[0].name,"Alice"); members[0].age=20; member_count=1;
    books[0].id=10; strcpy(books[0].title,"C Programming"); books[0].available=0; book_count=1;
    borrows[0].borrow_id=1; borrows[0].member_id=1; borrows[0].book_id=10; borrows[0].active=1;
    borrows[0].borrow_yyyy=2025; borrows[0].borrow_mm=8; borrows[0].borrow_dd=1;
    borrows[0].due_yyyy=2025; borrows[0].due_mm=8; borrows[0].due_dd=15;
    borrow_count=1;
}

int date_to_days(int y,int m,int d) {
    // naive conversion using tm and mktime
    struct tm tmdate = {0}; tmdate.tm_year = y-1900; tmdate.tm_mon = m-1; tmdate.tm_mday = d;
    time_t t = mktime(&tmdate);
    return (int)(t / 86400);
}

void overdue_books() {
    time_t t = time(NULL); struct tm *tmn = localtime(&t);
    int today_days = date_to_days(tmn->tm_year+1900, tmn->tm_mon+1, tmn->tm_mday);
    int any=0;
    for (int i=0;i<borrow_count;i++) {
        if (!borrows[i].active) continue;
        int due_days = date_to_days(borrows[i].due_yyyy, borrows[i].due_mm, borrows[i].due_dd);
        if (due_days < today_days) {
            any=1;
            printf("BorrowID:%d MemberID:%d BookID:%d Due:%04d-%02d-%02d (OVERDUE)\n",
                borrows[i].borrow_id, borrows[i].member_id, borrows[i].book_id,
                borrows[i].due_yyyy, borrows[i].due_mm, borrows[i].due_dd);
        }
    }
    if (!any) printf("No overdue books.\n");
}

int main() {
    seed_data();
    printf("Today's date will be taken from system.\n");
    overdue_books();
    return 0;
}
