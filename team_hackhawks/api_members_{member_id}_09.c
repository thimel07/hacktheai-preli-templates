// feature09_remove_member.c
#include <stdio.h>
#include <string.h>

#define MAX_MEMBERS 100
#define MAX_BORROWS 200
#define NAME_LEN 100

typedef struct { int id; char name[NAME_LEN]; int age; } Member;
typedef struct { int borrow_id; int member_id; int book_id; int active; } BorrowRecord;

Member members[MAX_MEMBERS]; int member_count=0;
BorrowRecord borrows[MAX_BORROWS]; int borrow_count=0;

void seed_data() {
    members[0].id=1; strcpy(members[0].name,"Alice"); members[0].age=20;
    members[1].id=2; strcpy(members[1].name,"Bob"); members[1].age=30;
    member_count=2;
    borrows[0].borrow_id=1; borrows[0].member_id=1; borrows[0].book_id=10; borrows[0].active=1; borrow_count=1;
}

int find_member_index(int id) { for (int i=0;i<member_count;i++) if (members[i].id==id) return i; return -1; }
int member_has_active_borrow(int id) { for (int i=0;i<borrow_count;i++) if (borrows[i].member_id==id && borrows[i].active) return 1; return 0; }

void remove_member() {
    int id; printf("Enter member ID to remove: "); if (scanf("%d",&id)!=1) return;
    int idx = find_member_index(id); if (idx==-1) { printf("Member not found.\n"); return; }
    if (member_has_active_borrow(id)) { printf("Cannot remove: member has active borrows.\n"); return; }
    // remove by shifting
    for (int i=idx;i<member_count-1;i++) members[i]=members[i+1];
    member_count--;
    printf("Member removed successfully.\n");
}

int main() {
    seed_data();
    remove_member();
    printf("\nRemaining members:\n");
    for (int i=0;i<member_count;i++) printf("ID:%d Name:%s Age:%d\n", members[i].id, members[i].name, members[i].age);
    return 0;
}
