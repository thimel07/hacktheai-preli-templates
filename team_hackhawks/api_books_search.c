// feature13_advanced_search.c
// Note: Advanced but self-contained search demo. Uses:
// - multiple filters (title, author, availability, year range simulated)
// - sorting (by title, author, popularity score)
// - pagination
// - analytics (counts, top authors)
// - fuzzy matching (Levenshtein distance)
// - category hierarchy (simple parent-child categories)
// - popularity scoring based on borrowing records
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BOOKS 300
#define TITLE_LEN 150
#define NAME_LEN 100
#define MAX_CATS 50
#define PAGE_SIZE_DEFAULT 5

typedef struct {
    int id;
    char title[TITLE_LEN];
    char author[NAME_LEN];
    int year;
    int available; // 1/0
    int category_id;
    int borrow_count; // used for popularity
} Book;

typedef struct {
    int id; char name[NAME_LEN]; int parent_id; // parent_id = -1 if root
} Category;

Book books[MAX_BOOKS]; int book_count=0;
Category categories[MAX_CATS]; int cat_count=0;

void seed_data() {
    // categories
    categories[0].id=1; strcpy(categories[0].name,"Programming"); categories[0].parent_id=-1;
    categories[1].id=2; strcpy(categories[1].name,"Algorithms"); categories[1].parent_id=1;
    categories[2].id=3; strcpy(categories[2].name,"Fiction"); categories[2].parent_id=-1;
    cat_count=3;
    // books
    books[0].id=10; strcpy(books[0].title,"C Programming Language"); strcpy(books[0].author,"Kernighan & Ritchie"); books[0].year=1978; books[0].available=1; books[0].category_id=1; books[0].borrow_count=50;
    books[1].id=11; strcpy(books[1].title,"Introduction to Algorithms"); strcpy(books[1].author,"Cormen"); books[1].year=2009; books[1].available=0; books[1].category_id=2; books[1].borrow_count=80;
    books[2].id=12; strcpy(books[2].title,"The Hobbit"); strcpy(books[2].author,"J.R.R. Tolkien"); books[2].year=1937; books[2].available=1; books[2].category_id=3; books[2].borrow_count=200;
    books[3].id=13; strcpy(books[3].title,"Data Structures in C"); strcpy(books[3].author,"Goodrich"); books[3].year=1995; books[3].available=1; books[3].category_id=1; books[3].borrow_count=35;
    books[4].id=14; strcpy(books[4].title,"Algorithm Design Manual"); strcpy(books[4].author,"Skiena"); books[4].year=2008; books[4].available=1; books[4].category_id=2; books[4].borrow_count=60;
    books[5].id=15; strcpy(books[5].title,"C Primer Plus"); strcpy(books[5].author,"Stephen Prata"); books[5].year=2004; books[5].available=1; books[5].category_id=1; books[5].borrow_count=25;
    book_count=6;
}

// Levenshtein distance for fuzzy match
int min3(int a,int b,int c){int m=a<b?a:b; return m<c?m:c;}
int levenshtein(const char *s,const char *t) {
    int n=strlen(s), m=strlen(t);
    int *prev = malloc((m+1)*sizeof(int));
    int *cur = malloc((m+1)*sizeof(int));
    for(int j=0;j<=m;j++) prev[j]=j;
    for(int i=1;i<=n;i++){
        cur[0]=i;
        for(int j=1;j<=m;j++){
            int cost = (tolower(s[i-1])==tolower(t[j-1]))?0:1;
            cur[j]=min3(prev[j]+1, cur[j-1]+1, prev[j-1]+cost);
        }
        memcpy(prev,cur,(m+1)*sizeof(int));
    }
    int res=prev[m];
    free(prev); free(cur);
    return res;
}

int category_is_or_child(int cat_id, int target_cat_id) {
    if (cat_id == target_cat_id) return 1;
    // find cat and walk ancestors
    int idx = -1;
    for (int i=0;i<cat_count;i++) if (categories[i].id==cat_id) { idx=i; break;}
    while (idx!=-1) {
        if (categories[idx].parent_id == target_cat_id) return 1;
        int pid = categories[idx].parent_id;
        idx = -1;
        for (int i=0;i<cat_count;i++) if (categories[i].id==pid) { idx=i; break; }
    }
    return 0;
}

// search with many filters, sorting, pagination
void advanced_search() {
    char title_q[TITLE_LEN]; char author_q[NAME_LEN];
    int avail_filter = -1; // -1 any, 1 available, 0 not available
    int year_from=0, year_to=9999;
    int cat_filter = -1;
    int page_size = PAGE_SIZE_DEFAULT;

    printf("Title query (empty for none): "); getchar(); fgets(title_q, TITLE_LEN, stdin); title_q[strcspn(title_q,"\n")]=0;
    printf("Author query (empty for none): "); fgets(author_q, NAME_LEN, stdin); author_q[strcspn(author_q,"\n")]=0;
    printf("Filter avail? (1=only available,0=only not available,-1=any) : "); if (scanf("%d",&avail_filter)!=1) avail_filter=-1;
    printf("Year from (0 for any): "); if (scanf("%d",&year_from)!=1) year_from=0;
    printf("Year to (0 for any): "); if (scanf("%d",&year_to)!=1) year_to=9999;
    if (year_from==0) year_from=0; if (year_to==0) year_to=9999;
    printf("Filter by category ID (0 for any): "); if (scanf("%d",&cat_filter)!=1) cat_filter=-1; if (cat_filter==0) cat_filter=-1;
    printf("Page size (enter 0 for default %d): ", PAGE_SIZE_DEFAULT); if (scanf("%d",&page_size)!=1) page_size=PAGE_SIZE_DEFAULT; if (page_size<=0) page_size=PAGE_SIZE_DEFAULT;
    // Build candidate list with scores
    typedef struct { int idx; double score; int lev_title; int lev_author; } Candidate;
    Candidate cand[MAX_BOOKS]; int cand_count=0;
    for (int i=0;i<book_count;i++) {
        Book *b =&books[i];
        if (avail_filter!=-1 && b->available!=avail_filter) continue;
        if (b->year < year_from || b->year > year_to) continue;
        if (cat_filter!=-1 && !category_is_or_child(b->category_id, cat_filter)) continue;
        // scoring: popularity + fuzzy match bonuses
        double score = (double)b->borrow_count; // base popularity
        int lev_t = 1000, lev_a = 1000;
        if (strlen(title_q)>0) { lev_t = levenshtein(title_q, b->title); score += (lev_t==0?100: (lev_t<5?20:5)); }
        if (strlen(author_q)>0) { lev_a = levenshtein(author_q, b->author); score += (lev_a==0?80:(lev_a<4?15:3)); }
        // availability boost
        if (b->available) score += 10;
        cand[cand_count].idx = i; cand[cand_count].score = score; cand[cand_count].lev_title = lev_t; cand[cand_count].lev_author = lev_a;
        cand_count++;
    }
    if (cand_count==0) { printf("No results.\n"); return; }
    // sorting choices
    int sort_choice; printf("Sort by: 1=score(desc) 2=title 3=author 4=year desc : "); if (scanf("%d",&sort_choice)!=1) sort_choice=1;
    if (sort_choice==1) {
        for (int i=0;i<cand_count-1;i++) for (int j=i+1;j<cand_count;j++) if (cand[j].score > cand[i].score) { Candidate t=cand[i]; cand[i]=cand[j]; cand[j]=t;}
    } else if (sort_choice==2) {
        for (int i=0;i<cand_count-1;i++) for (int j=i+1;j<cand_count;j++) if (strcmp(books[cand[i].idx].title, books[cand[j].idx].title) > 0) { Candidate t=cand[i]; cand[i]=cand[j]; cand[j]=t; }
    } else if (sort_choice==3) {
        for (int i=0;i<cand_count-1;i++) for (int j=i+1;j<cand_count;j++) if (strcmp(books[cand[i].idx].author, books[cand[j].idx].author) > 0) { Candidate t=cand[i]; cand[i]=cand[j]; cand[j]=t; }
    } else {
        for (int i=0;i<cand_count-1;i++) for (int j=i+1;j<cand_count;j++) if (books[cand[j].idx].year > books[cand[i].idx].year) { Candidate t=cand[i]; cand[i]=cand[j]; cand[j]=t; }
    }
    // pagination
    int pages = (cand_count + page_size -1)/page_size;
    int page = 1;
    while (1) {
        int start = (page-1)*page_size; int end = start + page_size; if (end>cand_count) end = cand_count;
        printf("\n--- Page %d/%d ---\n", page, pages);
        for (int i=start;i<end;i++) {
            Book *b = &books[cand[i].idx];
            printf("ID:%d Title:\"%s\" Author:%s Year:%d Avail:%s Borrows:%d Score:%.1f LevT:%d LevA:%d\n",
                b->id, b->title, b->author, b->year, b->available?"Y":"N", b->borrow_count, cand[i].score, cand[i].lev_title, cand[i].lev_author);
        }
        printf("Commands: n=next p=prev q=quit analytics=a\nEnter cmd: ");
        char cmd[10]; scanf("%s", cmd);
        if (cmd[0]=='n' && page<pages) page++;
        else if (cmd[0]=='p' && page>1) page--;
        else if (cmd[0]=='a') {
            // analytics: top authors & category counts
            printf("\nAnalytics:\n");
            // top authors
            typedef struct { char name[NAME_LEN]; int count; } AuthStat;
            AuthStat stats[100]; int sc=0;
            for (int i=0;i<cand_count;i++) {
                char *auth = books[cand[i].idx].author; int found=-1;
                for (int k=0;k<sc;k++) if (strcmp(stats[k].name, auth)==0) { found=k; break; }
                if (found==-1) { strcpy(stats[sc].name, auth); stats[sc].count=1; sc++; } else stats[found].count++;
            }
            printf("Top authors among results:\n");
            for (int i=0;i<sc;i++) printf("%s (%d)\n", stats[i].name, stats[i].count);
            // categories count
            for (int c=0;c<cat_count;c++) {
                int cnt=0;
                for (int i=0;i<cand_count;i++) if (books[cand[i].idx].category_id == categories[c].id) cnt++;
                printf("Category %s : %d results\n", categories[c].name, cnt);
            }
        }
        else if (cmd[0]=='q') break;
    }
}

int main() {
    seed_data();
    printf("Advanced search demo\n");
    advanced_search();
    return 0;
}
