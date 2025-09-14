// feature05_borrow_book.c
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
typedef struct { int borrow_id; int member_id; int book_id; int active; // 1 active, 0 returned
               int borrow_yyyy, borrow_mm, borrow_dd; int due_yyyy, due_mm, due_dd; } BorrowRecord;

Member members[MAX_MEMBERS]; int member_count=0;
Book books[MAX_BOOKS]; int book_count=0;
BorrowRecord borrows[MAX_BORROWS]; int borrow_count=0;

void seed_data() {
    members[0].id=1; strcpy(members[0].name,"Alice"); members[0].age=20;
    member_count=1;
    books[0].id=10; strcpy(books[0].title,"C Programming"); strcpy(books[0].author,"K&R"); books[0].available=1;
    books[1].id=11; strcpy(books[1].title,"Data Structures"); strcpy(books[1].author,"Goodrich"); books[1].available=1;
    book_count=2;
}

int member_has_active_borrow(int member_id) {
    for (int i=0;i<borrow_count;i++) if (borrows[i].member_id==member_id && borrows[i].active) return 1;
    return 0;
}

int find_book_index(int id) { for (int i=0;i<book_count;i++) if (books[i].id==id) return i; return -1; }
int find_member_index(int id) { for (int i=0;i<member_count;i++) if (members[i].id==id) return i; return -1; }

void borrow_book() {
    int member_id, book_id;
    printf("Enter member ID: "); if (scanf("%d",&member_id)!=1) return;
    if (find_member_index(member_id)==-1) { printf("Member not found.\n"); return; }
    if (member_has_active_borrow(member_id)) { printf("Member already has an active borrow. Cannot borrow another.\n"); return; }
    printf("Enter book ID: "); if (scanf("%d",&book_id)!=1) return;
    int bi = find_book_index(book_id); if (bi==-1) { printf("Book not found.\n"); return; }
    if (!books[bi].available) { printf("Book not available.\n"); return; }
    // create borrow record
    if (borrow_count >= MAX_BORROWS) { printf("Borrow storage full.\n"); return; }
    time_t t = time(NULL); struct tm *tm = localtime(&t);
    borrows[borrow_count].borrow_id = borrow_count+1;
    borrows[borrow_count].member_id = member_id;
    borrows[borrow_count].book_id = book_id;
    borrows[borrow_count].active = 1;
    borrows[borrow_count].borrow_yyyy=tm->tm_year+1900; borrows[borrow_count].borrow_mm=tm->tm_mon+1; borrows[borrow_count].borrow_dd=tm->tm_mday;
    // due date 14 days later (naive)
    borrows[borrow_count].due_yyyy = borrows[borrow_count].borrow_yyyy;
    borrows[borrow_count].due_mm = borrows[borrow_count].borrow_mm;
    borrows[borrow_count].due_dd = borrows[borrow_count].borrow_dd + 14;
    books[bi].available = 0;
    borrow_count++;
    printf("Book borrowed successfully. Due in 14 days.\n");
}

int main() {
    seed_data();
    printf("---- Borrow Book ----\n");
    borrow_book();
    printf("\nActive borrows:\n");
    for (int i=0;i<borrow_count;i++) if (borrows[i].active) printf("BorrowID:%d Member:%d Book:%d Due:%04d-%02d-%02d\n",
        borrows[i].borrow_id, borrows[i].member_id, borrows[i].book_id, borrows[i].due_yyyy, borrows[i].due_mm, borrows[i].due_dd);
    return 0;
}
