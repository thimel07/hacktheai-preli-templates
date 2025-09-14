// feature02_get_member_by_id.c
#include <stdio.h>
#include <string.h>

#define MAX_MEMBERS 100
#define NAME_LEN 100

typedef struct {
    int id; char name[NAME_LEN]; int age;
} Member;

Member members[MAX_MEMBERS];
int member_count = 0;

void seed_members() {
    members[0].id = 1; strcpy(members[0].name, "Alice"); members[0].age = 20;
    members[1].id = 2; strcpy(members[1].name, "Bob"); members[1].age = 30;
    member_count = 2;
}

int find_member_index_by_id(int id) {
    for (int i=0;i<member_count;i++) if (members[i].id==id) return i;
    return -1;
}

void get_member_by_id() {
    int id; printf("Enter member ID: "); if (scanf("%d",&id)!=1) return;
    int idx = find_member_index_by_id(id);
    if (idx==-1) { printf("Member not found.\n"); return; }
    printf("Member details:\nID: %d\nName: %s\nAge: %d\n", members[idx].id, members[idx].name, members[idx].age);
}

int main() {
    seed_members();
    get_member_by_id();
    return 0;
}
