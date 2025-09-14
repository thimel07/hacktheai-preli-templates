// feature14_reservation_system.c
// Self-contained reservation system demo with priority-based queueing and scoring.
// Notifications are simulated by console messages. Dynamic pricing and group reservations are illustrated.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MEMBERS 200
#define MAX_BOOKS 200
#define MAX_RES 500
#define NAME_LEN 100
#define TITLE_LEN 150

typedef struct { int id; char name[NAME_LEN]; int age; int membership_tier; // 0=basic,1=premium
    int total_borrows; int punctual_returns; } Member;
typedef struct { int id; char title[TITLE_LEN]; char author[NAME_LEN]; int available; } Book;
typedef struct {
    int res_id;
    int book_id;
    int member_id;
    time_t request_time;
    int group_id; // 0 for single
    double price; // dynamic price
    int active; // 1 active, 0 expired/fulfilled
} Reservation;

Member members[MAX_MEMBERS]; int member_count=0;
Book books[MAX_BOOKS]; int book_count=0;
Reservation reservations[MAX_RES]; int res_count=0;

void seed() {
    members[0].id=1; strcpy(members[0].name,"Alice"); members[0].age=20; members[0].membership_tier=1; members[0].total_borrows=20; members[0].punctual_returns=18;
    members[1].id=2; strcpy(members[1].name,"Bob"); members[1].age=25; members[1].membership_tier=0; members[1].total_borrows=5; members[1].punctual_returns=4;
    member_count=2;
    books[0].id=10; strcpy(books[0].title,"Popular Book"); strcpy(books[0].author,"Famous"); books[0].available=0;
    books[1].id=11; strcpy(books[1].title,"Less Popular"); strcpy(books[1].author,"Author"); books[1].available=1;
    book_count=2;
}

// compute priority score for a member for reservation
double compute_priority_score(Member *m) {
    double score = 0;
    score += m->total_borrows * 0.5;
    score += m->punctual_returns * 1.0;
    score += (m->membership_tier==1) ? 50 : 0;
    // special circumstances could add bonus (simulate)
    return score;
}

// dynamic price: base + demand factor (number of active reservations for the book)
double compute_dynamic_price(int book_id) {
    double base = 0.0;
    for (int i=0;i<book_count;i++) if (books[i].id==book_id) base = 0.0; // base price 0 (free) then premium if demand
    int demand = 0; for (int i=0;i<res_count;i++) if (reservations[i].book_id==book_id && reservations[i].active) demand++;
    return base + (demand>3 ? 10.0 + demand*2.0 : demand>0 ? 2.0*demand : 0.0);
}

void add_reservation() {
    if (res_count >= MAX_RES) { printf("Reservation storage full.\n"); return; }
    int member_id, book_id, group_id=0;
    printf("Enter member ID: "); if (scanf("%d",&member_id)!=1) return;
    printf("Enter book ID to reserve: "); if (scanf("%d",&book_id)!=1) return;
    printf("Enter group id (0 for single reservation): "); if (scanf("%d",&group_id)!=1) group_id=0;
    Member *m = NULL; for (int i=0;i<member_count;i++) if (members[i].id==member_id) { m=&members[i]; break; }
    if (!m) { printf("Member not found.\n"); return; }
    Book *b = NULL; for (int i=0;i<book_count;i++) if (books[i].id==book_id) { b=&books[i]; break; }
    if (!b) { printf("Book not found.\n"); return; }
    // compute price and priority score
    double price = compute_dynamic_price(book_id);
    double priority = compute_priority_score(m);
    // create
    reservations[res_count].res_id = res_count+1;
    reservations[res_count].book_id = book_id;
    reservations[res_count].member_id = member_id;
    reservations[res_count].request_time = time(NULL);
    reservations[res_count].group_id = group_id;
    reservations[res_count].price = price;
    reservations[res_count].active = 1;
    res_count++;
    printf("Reservation added. Price: %.2f PriorityScore(%.1f)\n", price, priority);
}

// process queues for a given book: sort by computed priority and fulfill top slot if available
void process_reservations_for_book(int book_id) {
    // collect active reservations for the book
    int ids[MAX_RES]; int n=0;
    for (int i=0;i<res_count;i++) if (reservations[i].book_id==book_id && reservations[i].active) ids[n++]=i;
    if (n==0) { printf("No reservations for book %d\n", book_id); return; }
    // compute scores and sort indices by priority score and request time (tie-breaker earliest)
    typedef struct { int idx; double score; time_t t; } RObj;
    RObj arr[MAX_RES]; for (int i=0;i<n;i++) {
        int idx=ids[i]; int mid = reservations[idx].member_id;
        Member *m=NULL; for (int j=0;j<member_count;j++) if (members[j].id==mid) { m=&members[j]; break; }
        double sc = m ? compute_priority_score(m) : 0;
        arr[i].idx = idx; arr[i].score = sc; arr[i].t = reservations[idx].request_time;
    }
    for (int i=0;i<n-1;i++) for (int j=i+1;j<n;j++) {
        if (arr[j].score > arr[i].score || (arr[j].score==arr[i].score && arr[j].t < arr[i].t)) { RObj tmp=arr[i]; arr[i]=arr[j]; arr[j]=tmp; }
    }
    // simulate fulfillment: if book available -> assign to first and notify, else just re-order queue
    Book *b=NULL; for (int i=0;i<book_count;i++) if (books[i].id==book_id) b=&books[i];
    if (b && b->available) {
        int chosenIdx = arr[0].idx;
        reservations[chosenIdx].active = 0; // fulfilled
        b->available = 0;
        printf("Reservation #%d for member %d fulfilled. Notifying member...\n", reservations[chosenIdx].res_id, reservations[chosenIdx].member_id);
        // simulate notification
    } else {
        printf("Book not available. Queue order:\n");
        for (int i=0;i<n;i++) {
            int ridx = arr[i].idx;
            printf("ResID:%d Member:%d Priority:%.1f Requested:%s", reservations[ridx].res_id, reservations[ridx].member_id, arr[i].score, ctime(&reservations[ridx].request_time));
        }
    }
}

// expire old reservations (e.g., older than X seconds for demo)
void expire_old_reservations(int seconds_threshold) {
    time_t now = time(NULL);
    for (int i=0;i<res_count;i++) if (reservations[i].active) {
        if (difftime(now, reservations[i].request_time) > seconds_threshold) {
            reservations[i].active = 0;
            printf("Reservation %d expired; notifying next in line (if any)...\n", reservations[i].res_id);
        }
    }
}

void show_reservations() {
    printf("Active reservations:\n");
    for (int i=0;i<res_count;i++) if (reservations[i].active) {
        printf("ResID:%d BookID:%d MemberID:%d Price:%.2f Requested:%s", reservations[i].res_id, reservations[i].book_id, reservations[i].member_id, reservations[i].price, ctime(&reservations[i].request_time));
    }
}

int main() {
    seed();
    printf("Reservation system demo\n");
    while (1) {
        printf("\nMenu: 1=add reservation 2=process book queue 3=expire old 4=show 5=quit\nChoose: ");
        int cmd; if (scanf("%d",&cmd)!=1) break;
        if (cmd==1) add_reservation();
        else if (cmd==2) { int bid; printf("Enter book ID to process: "); if (scanf("%d",&bid)!=1) continue; process_reservations_for_book(bid); }
        else if (cmd==3) { int sec; printf("Expire after seconds: "); if (scanf("%d",&sec)!=1) continue; expire_old_reservations(sec); }
        else if (cmd==4) show_reservations();
        else break;
    }
    return 0;
}
