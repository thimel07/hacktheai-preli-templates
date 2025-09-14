// feature11_add_book.c
#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 200
#define TITLE_LEN 150
#define NAME_LEN 100

typedef struct { int id; char title[TITLE_LEN]; char author[NAME_LEN]; int available; } Book;
Book books[MAX_BOOKS];
int book_count = 0;

int find_book_index(int id) { for (int i=0;i<book_count;i++) if (books[i].id==id) return i; return -1; }

void seed_books() {
    books[0].id=10; strcpy(books[0].title,"C Programming"); strcpy(books[0].author,"K&R"); books[0].available=1; book_count=1;
}

void add_book() {
    if (book_count >= MAX_BOOKS) { printf("Catalog full.\n"); return; }
    int id; char title[TITLE_LEN]; char author[NAME_LEN];
    printf("Enter book ID: "); if (scanf("%d",&id)!=1) return;
    if (find_book_index(id) != -1) { printf("Book ID exists.\n"); return; }
    printf("Enter title: "); getchar(); fgets(title, TITLE_LEN, stdin); title[strcspn(title,"\n")]=0;
    printf("Enter author: "); fgets(author, NAME_LEN, stdin); author[strcspn(author,"\n")]=0;
    books[book_count].id = id;
    strncpy(books[book_count].title, title, TITLE_LEN-1);
    strncpy(books[book_count].author, author, NAME_LEN-1);
    books[book_count].available = 1;
    book_count++;
    printf("Book added.\n");
}

int main() {
    seed_books();
    add_book();
    printf("\nCatalog now:\n");
    for (int i=0;i<book_count;i++) printf("ID:%d Title:%s Author:%s Avail:%d\n", books[i].id, books[i].title, books[i].author, books[i].available);
    return 0;
}
