// feature07_get_current_borrows.c
#include <stdio.h>
#include <string.h>

#define MAX_MEMBERS 100
#define MAX_BOOKS 100
#define MAX_BORROWS 200
#define NAME_LEN 100
#define TITLE_LEN 150

typedef struct { int id; char name[NAME_LEN]; int age; } Member;
typedef struct { int id; char title[TITLE_LEN]; char author[NAME_LEN]; int available; } Book;
typedef struct { int borrow_id; int member_id; int book_id; int active;
               int borrow_yyyy, borrow_mm, borrow_dd; int due_yyyy, due_mm, due_dd; } BorrowRecord;

Member members[MAX_MEMBERS]; int member_count=0;
Book books[MAX_BOOKS]; int book_count=0;
BorrowRecord borrows[MAX_BORROWS]; int borrow_count=0;

void seed_data() {
    members[0].id=1; strcpy(members[0].name,"Alice"); members[0].age=20;
    members[1].id=2; strcpy(members[1].name,"Bob"); members[1].age=30;
    member_count=2;
    books[0].id=10; strcpy(books[0].title,"C Programming"); strcpy(books[0].author,"K&R"); books[0].available=0;
    books[1].id=11; strcpy(books[1].title,"Data Structures"); strcpy(books[1].author,"Goodrich"); books[1].available=1;
    book_count=2;
    borrows[0].borrow_id=1; borrows[0].member_id=1; borrows[0].book_id=10; borrows[0].active=1;
    borrows[0].borrow_yyyy=2025; borrows[0].borrow_mm=9; borrows[0].borrow_dd=1;
    borrows[0].due_yyyy=2025; borrows[0].due_mm=9; borrows[0].due_dd=15;
    borrow_count=1;
}

Book* find_book_by_id(int id) { for (int i=0;i<book_count;i++) if (books[i].id==id) return &books[i]; return NULL; }
Member* find_member_by_id(int id) { for (int i=0;i<member_count;i++) if (members[i].id==id) return &members[i]; return NULL; }

void get_current_borrows() {
    printf("Currently borrowed books:\n");
    int any = 0;
    for (int i=0;i<borrow_count;i++) {
        if (!borrows[i].active) continue;
        any = 1;
        Member *m = find_member_by_id(borrows[i].member_id);
        Book *b = find_book_by_id(borrows[i].book_id);
        printf("BorrowID:%d | Member: %s (ID %d) | Book: %s (ID %d) | Due: %04d-%02d-%02d\n",
            borrows[i].borrow_id, m?m->name:"(unknown)", borrows[i].member_id, b?b->title:"(unknown)", borrows[i].book_id,
            borrows[i].due_yyyy, borrows[i].due_mm, borrows[i].due_dd);
    }
    if (!any) printf("No active borrows.\n");
}

int main() {
    seed_data();
    get_current_borrows();
    return 0;
}
