// feature12_get_book_by_id.c
#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 200
#define TITLE_LEN 150
#define NAME_LEN 100

typedef struct { int id; char title[TITLE_LEN]; char author[NAME_LEN]; int available; } Book;
Book books[MAX_BOOKS]; int book_count=0;

void seed_books() {
    books[0].id=10; strcpy(books[0].title,"C Programming"); strcpy(books[0].author,"K&R"); books[0].available=1;
    books[1].id=11; strcpy(books[1].title,"Data Structures"); strcpy(books[1].author,"Goodrich"); books[1].available=0;
    book_count=2;
}

int find_index(int id) { for (int i=0;i<book_count;i++) if (books[i].id==id) return i; return -1; }

void get_book() {
    int id; printf("Enter book ID: "); if (scanf("%d",&id)!=1) return;
    int idx = find_index(id); if (idx==-1) { printf("Not found.\n"); return; }
    printf("ID:%d\nTitle:%s\nAuthor:%s\nAvailable:%s\n", books[idx].id, books[idx].title, books[idx].author, books[idx].available?"Yes":"No");
}

int main() {
    seed_books();
    get_book();
    return 0;
}
