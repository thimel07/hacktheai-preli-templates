1. // feature01_register_member.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MEMBERS 100
#define NAME_LEN 100

typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
} Member;

Member members[MAX_MEMBERS];
int member_count = 0;

int find_member_index_by_id(int id) {
    for (int i = 0; i < member_count; ++i)
        if (members[i].id == id) return i;
    return -1;
}

void seed_members() {
    members[0].id = 1; strcpy(members[0].name, "Alice"); members[0].age = 20;
    members[1].id = 2; strcpy(members[1].name, "Bob"); members[1].age = 30;
    member_count = 2;
}

void register_member() {
    if (member_count >= MAX_MEMBERS) {
        printf("Member storage full.\n"); return;
    }
    int id; char name[NAME_LEN]; int age;
    printf("Enter new member ID (integer): "); if (scanf("%d", &id)!=1) return;
    if (find_member_index_by_id(id) != -1) {
        printf("ID already exists.\n"); return;
    }
    printf("Enter name: "); getchar(); fgets(name, NAME_LEN, stdin);
    name[strcspn(name, "\n")] = 0;
    printf("Enter age: "); if (scanf("%d", &age)!=1) return;
    if (age < 12) {
        printf("Age must be at least 12.\n"); return;
    }
    members[member_count].id = id;
    strncpy(members[member_count].name, name, NAME_LEN-1);
    members[member_count].age = age;
    member_count++;
    printf("Member registered successfully.\n");
}

int main() {
    seed_members();
    printf("---- Register Member ----\n");
    register_member();
    printf("\nCurrent members:\n");
    for (int i=0;i<member_count;i++) printf("ID:%d Name:%s Age:%d\n", members[i].id, members[i].name, members[i].age);
    return 0;
}
