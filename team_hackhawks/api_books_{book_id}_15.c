// feature15_remove_book.c
#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 200
#define MAX_BORROWS 500
#define TITLE_LEN 150
#define NAME_LEN 100

typedef struct { int id; char title[TITLE_LEN]; char author[NAME_LEN]; int available; } Book;
typedef struct { int borrow_id; int book_id; int member_id; int active; } BorrowRecord;

Book books[MAX_BOOKS]; int book_count=0;
BorrowRecord borrows[MAX_BORROWS]; int borrow_count=0;

void seed() {
    books[0].id=10; strcpy(books[0].title,"C Programming"); strcpy(books[0].author,"K&R"); books[0].available=1;
    books[1].id=11; strcpy(books[1].title,"Data Structures"); strcpy(books[1].author,"Goodrich"); books[1].available=0;
    book_count=2;
    borrows[0].borrow_id=1; borrows[0].book_id=11; borrows[0].member_id=1; borrows[0].active=1; borrow_count=1;
}

int find_book_index(int id) { for (int i=0;i<book_count;i++) if (books[i].id==id) return i; return -1; }
int book_has_active_borrows(int book_id) { for (int i=0;i<borrow_count;i++) if (borrows[i].book_id==book_id && borrows[i].active) return 1; return 0; }

void remove_book() {
    int id; printf("Enter book ID to remove: "); if (scanf("%d",&id)!=1) return;
    int idx = find_book_index(id); if (idx==-1) { printf("Book not found.\n"); return; }
    if (book_has_active_borrows(id)) { printf("Cannot remove: book has active borrows.\n"); return; }
    // shift
    for (int i=idx;i<book_count-1;i++) books[i]=books[i+1];
    book_count--;
    printf("Book removed.\n");
}

int main() {
    seed();
    remove_book();
    printf("\nBooks now:\n");
    for (int i=0;i<book_count;i++) printf("ID:%d Title:%s Author:%s Avail:%d\n", books[i].id, books[i].title, books[i].author, books[i].available);
    return 0;
}
