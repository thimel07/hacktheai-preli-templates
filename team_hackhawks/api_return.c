// feature06_return_book.c
#include <stdio.h>
#include <string.h>
#include <time.h>

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
    members[0].id=1; strcpy(members[0].name,"Alice"); members[0].age=20; member_count=1;
    books[0].id=10; strcpy(books[0].title,"C Programming"); books[0].available=0; book_count=1;
    borrows[0].borrow_id=1; borrows[0].member_id=1; borrows[0].book_id=10; borrows[0].active=1;
    borrows[0].borrow_yyyy=2025; borrows[0].borrow_mm=9; borrows[0].borrow_dd=1;
    borrows[0].due_yyyy=2025; borrows[0].due_mm=9; borrows[0].due_dd=15;
    borrow_count=1;
}

int find_borrow_by_member(int member_id) {
    for (int i=0;i<borrow_count;i++) if (borrows[i].member_id==member_id && borrows[i].active) return i;
    return -1;
}
int find_book_index(int id) { for (int i=0;i<book_count;i++) if (books[i].id==id) return i; return -1; }

void return_book() {
    int member_id; printf("Enter member ID to return book: "); if (scanf("%d",&member_id)!=1) return;
    int idx = find_borrow_by_member(member_id);
    if (idx==-1) { printf("No active borrow for this member.\n"); return; }
    int book_idx = find_book_index(borrows[idx].book_id);
    if (book_idx!=-1) books[book_idx].available = 1;
    borrows[idx].active = 0;
    printf("Book returned successfully (BorrowID %d).\n", borrows[idx].borrow_id);
}

int main() {
    seed_data();
    return_book();
    printf("\nBorrow records:\n");
    for (int i=0;i<borrow_count;i++) printf("BorrowID:%d Member:%d Book:%d Active:%d\n", borrows[i].borrow_id, borrows[i].member_id, borrows[i].book_id, borrows[i].active);
    return 0;
}
