// feature03_list_all_members.c
#include <stdio.h>
#include <string.h>
#define MAX_MEMBERS 100
#define NAME_LEN 100

typedef struct { int id; char name[NAME_LEN]; int age; } Member;
Member members[MAX_MEMBERS];
int member_count = 0;

void seed_members() {
    members[0].id = 1; strcpy(members[0].name,"Alice"); members[0].age=20;
    members[1].id = 2; strcpy(members[1].name,"Bob"); members[1].age=30;
    members[2].id = 3; strcpy(members[2].name,"Charlie"); members[2].age=15;
    member_count = 3;
}

void list_all_members() {
    if (member_count==0) { printf("No members.\n"); return; }
    printf("All members (%d):\n", member_count);
    for (int i=0;i<member_count;i++)
        printf("ID:%d | Name:%s | Age:%d\n", members[i].id, members[i].name, members[i].age);
}

int main() {
    seed_members();
    list_all_members();
    return 0;
}
