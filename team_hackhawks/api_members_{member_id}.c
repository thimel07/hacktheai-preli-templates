// feature04_update_member.c
#include <stdio.h>
#include <string.h>
#define MAX_MEMBERS 100
#define NAME_LEN 100

typedef struct { int id; char name[NAME_LEN]; int age; } Member;
Member members[MAX_MEMBERS];
int member_count=0;

void seed_members() {
    members[0].id=1; strcpy(members[0].name,"Alice"); members[0].age=20;
    members[1].id=2; strcpy(members[1].name,"Bob"); members[1].age=30;
    member_count=2;
}

int find_index(int id) {
    for (int i=0;i<member_count;i++) if (members[i].id==id) return i;
    return -1;
}

void update_member() {
    int id; printf("Enter member ID to update: "); if (scanf("%d",&id)!=1) return;
    int idx = find_index(id); if (idx==-1) { printf("Not found.\n"); return; }
    printf("Current name: %s\n", members[idx].name);
    printf("Enter new name (or press enter to keep): "); getchar();
    char buf[NAME_LEN]; fgets(buf, NAME_LEN, stdin); buf[strcspn(buf,"\n")]=0;
    if (strlen(buf)>0) strncpy(members[idx].name, buf, NAME_LEN-1);
    printf("Current age: %d\nEnter new age: ", members[idx].age);
    int newage; if (scanf("%d",&newage)!=1) return;
    if (newage < 12) { printf("Age must be at least 12.\n"); return; }
    members[idx].age = newage;
    printf("Member updated.\n");
}

int main() {
    seed_members();
    update_member();
    printf("Members now:\n");
    for (int i=0;i<member_count;i++) printf("ID:%d Name:%s Age:%d\n", members[i].id, members[i].name, members[i].age);
    return 0;
}
