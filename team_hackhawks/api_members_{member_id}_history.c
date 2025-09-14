// feature08_member_borrow_history.c
#include <stdio.h>
#include <string.h>

#define MAX_MEMBERS 100
#define MAX_BOOKS 100
#define MAX_BORROWS 500
#define NAME_LEN 100
#define TITLE_LEN 150

typedef struct { int id; char name[NAME_LEN]; int age; } Member;
typedef struct { int id; char title[TITLE_LEN]; char author[NAME_LEN]; int available; } Book;
typedef struct { int borrow_id; int member_id; int book_id; int active;
               int borrow_yyyy, borrow_mm, borrow_dd; int due_yyyy, due_mm, due_dd; int return_yyyy, return_mm, return_dd; } BorrowRecord;

Member members[MAX_MEMBERS]; int member_count=0;
Book books[MAX_BOOKS]; int book_count=0;
BorrowRecord borrows[MAX_BORROWS]; int borrow_count=0;

void seed_data() {
    members[0].id=1; strcpy(members[0].name,"Alice"); members[0].age=20; member_count=1;
    books[0].id=10; strcpy(books[0].title,"C Programming"); books[0].available=1; book_count=1;
    borrows[0].borrow_id=1; borrows[0].member_id=1; borrows[0].book_id=10; borrows[0].active=0;
    borrows[0].borrow_yyyy=2025; borrows[0].borrow_mm=1; borrows[0].borrow_dd=10;
    borrows[0].due_yyyy=2025; borrows[0].due_mm=1; borrows[0].due_dd=24;
    borrows[0].return_yyyy=2025; borrows[0].return_mm=1; borrows[0].return_dd=20;
    borrow_count=1;
}

void member_history() {
    int member_id; printf("Enter member ID: "); if (scanf("%d",&member_id)!=1) return;
    int any=0;
    for (int i=0;i<borrow_count;i++) if (borrows[i].member_id==member_id) {
        any=1;
        printf("BorrowID:%d BookID:%d Active:%d Borrowed:%04d-%02d-%02d Due:%04d-%02d-%02d",
            borrows[i].borrow_id, borrows[i].book_id, borrows[i].active,
            borrows[i].borrow_yyyy, borrows[i].borrow_mm, borrows[i].borrow_dd,
            borrows[i].due_yyyy, borrows[i].due_mm, borrows[i].due_dd);
        if (!borrows[i].active) printf(" Returned:%04d-%02d-%02d\n", borrows[i].return_yyyy, borrows[i].return_mm, borrows[i].return_dd);
        else printf(" (still active)\n");
    }
    if (!any) printf("No history found for this member.\n");
}

int main() {
    seed_data();
    member_history();
    return 0;
}
