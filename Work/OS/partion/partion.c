//
// Created by fivv on 2023/11/10.
//


#include "stdio.h"
#include "stdlib.h"

int memory = 0; // size of memory

struct Storage {
    int id; // task id
    int size; // size of memory
    int start; // start address
    int end; // end address
    int status; // 0: not allocated, 1: allocated
    struct Storage *next;
};

// allocate memory for visitBit task with first fit
int firstFit(struct Storage *head, int id, int size) {
    struct Storage *p = head->next;

    while (p != NULL) {
        if (p->status == 0 && p->size >= size) {
            break;
        }
        p = p->next;
    }

    if (p == NULL) {
        return 0;
    }


    int sizeOld = p->size;
    p->id = id;
    p->size = size;
    p->end = p->start + size - 1;
    p->status = 1;

    struct Storage *new = (struct Storage *) malloc(sizeof(struct Storage));
    new->id = -1;
    new->size = sizeOld - size;
    new->start = p->end + 1;
    if (new->start >= memory - 1) {
        p->next = NULL;
        return 1;
    }
    new->end = new->start + new->size - 1;
    new->status = 0;
    new->next = p->next;
    p->next = new;
    return 1;
}

// allocate memory for visitBit task with best fit
int bestFit(struct Storage *head, int id, int size) {
    struct Storage *p = head->next;
    struct Storage *best = NULL;
    int min = memory;

    while (p != NULL) {
        if (p->status == 0 && p->size >= size && p->size < min) {
            best = p;
            min = p->size;
        }
        p = p->next;
    }

    if (best == NULL) {
        return 0;
    }

    int sizeOld = best->size;
    best->id = id;
    best->size = size;
    best->end = best->start + size - 1;
    best->status = 1;

    struct Storage *new = (struct Storage *) malloc(sizeof(struct Storage));
    new->id = -1;
    new->size = sizeOld - size;
    new->start = best->end + 1;
    if (new->start >= memory - 1) {
        best->next = NULL;
        return 1;
    }
    new->end = new->start + new->size - 1;
    new->status = 0;
    new->next = best->next;
    best->next = new;
    return 1;
}

// allocate memory for visitBit task with worst fit
int worstFit(struct Storage *head, int id, int size) {
    struct Storage *p = head->next;
    struct Storage *worst = NULL;
    int max = 0;

    while (p != NULL) {
        if (p->status == 0 && p->size >= size && p->size > max) {
            worst = p;
            max = p->size;
        }
        p = p->next;
    }

    if (worst == NULL) {
        return 0;
    }

    int sizeOld = worst->size;
    worst->id = id;
    worst->size = size;
    worst->end = worst->start + size - 1;
    worst->status = 1;

    struct Storage *new = (struct Storage *) malloc(sizeof(struct Storage));
    new->id = -1;
    new->size = sizeOld - size;
    new->start = worst->end + 1;
    if (new->start >= memory - 1) {
        worst->next = NULL;
        return 1;
    }
    new->end = new->start + new->size - 1;
    new->status = 0;
    new->next = worst->next;
    worst->next = new;
    return 1;
}

// free memory of visitBit task
int delete(struct Storage *head, int id) {
    struct Storage *p = head->next;
    struct Storage *pre = head;

    while (p != NULL) {
        if (p->id == id) {
            break;
        }
        pre = p;
        p = p->next;
    }

    if (p == NULL) {
        return 0;
    }

    p->id = -1;
    p->status = 0;

    // merge with the next block
    if (p->next != NULL && p->next->status == 0) {
        p->size += p->next->size;
        p->end = p->next->end;
        p->next = p->next->next;
    }

    // merge with the previous block
    if (pre->status == 0) {
        pre->size += p->size;
        pre->end = p->end;
        pre->next = p->next;
    }

    return 1;
}

// print the memory
void print(struct Storage *head) {
    struct Storage *p = head->next;
    int cnt = 0;
    while (p != NULL) {
        printf("id: %d, size: %d, start: %d, end: %d, status: %d\n", p->id, p->size, p->start, p->end, p->status);
        p = p->next;
        cnt++;
    }
    if (cnt == 0) {
        printf("No task!\n");
    } else {
        printf("Total: %d\n", cnt);
    }
}

void menu() {
    printf("----- menu -----\n");
    printf("1. Add visitBit task with first fit\n");
    printf("2. Add visitBit task with best fit\n");
    printf("3. Add visitBit task with worst fit\n");
    printf("4. Delete visitBit task\n");
    printf("5. Print the memory\n");
    printf("6. Exit\n");
}

// allocate memory for init
void allocate(struct Storage *head, int start, int size) {
    struct Storage *p = head->next;
    struct Storage *pre = head;

    while (p != NULL) {
        if (p->status == 0 && p->start <= start && p->end >= start + size - 1) {
            break;
        }
        pre = p;
        p = p->next;
    }

    if (p == NULL) {
        return;
    }

    struct Storage *new = (struct Storage *) malloc(sizeof(struct Storage));
    new->id = -1;
    new->size = size;
    new->start = start;
    new->end = start + size - 1;
    new->status = 1;

    if (p->end > new->end) {
        struct Storage *next_part = (struct Storage *) malloc(sizeof(struct Storage));
        next_part->id = -1;
        next_part->size = p->end - new->end;
        next_part->start = new->end + 1;
        next_part->end = p->end;
        next_part->status = 0;
        next_part->next = p->next;
        new->next = next_part;
        p->next = new;
        p->end = new->start - 1;
        p->size = p->end - p->start + 1;
    } else {
        new->next = p->next;
        p->next = new;
        p->end = new->start - 1;
    }

    if (p->start == new->start) {
        pre->next = new;
        free(p);
    }
}

struct Storage *init() {

    memory = 3000; // total size of memory

    struct Storage *head = (struct Storage *) malloc(sizeof(struct Storage));
    struct Storage *tail = (struct Storage *) malloc(sizeof(struct Storage));

    tail->id = -1;
    tail->size = memory;
    tail->start = 0;
    tail->end = memory - 1;
    tail->status = 0;
    tail->next = NULL;
    head->next = tail;

    int start[] = {500, 1000, 2000};
    int size[] = {100, 200, 400};
    for (int i = 0; i < 3; i++) {
        allocate(head, start[i], size[i]);
    }
    return head;
}


int main() {
    struct Storage *head = init();
    printf("Successfully initialized!\n");
    print(head);

    int choice, id, size;
    while (1) {
        menu();
        printf("Please input your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Please input the id and size: ");
                scanf("%d %d", &id, &size);
                if (firstFit(head, id, size)) {
                    printf("Successfully allocated!\n");
                } else {
                    printf("Failed to allocate!\n");
                }
                print(head);
                break;
            case 2:
                printf("Please input the id and size: ");
                scanf("%d %d", &id, &size);
                if (bestFit(head, id, size)) {
                    printf("Successfully allocated!\n");
                } else {
                    printf("Failed to allocate!\n");
                }
                print(head);
                break;
            case 3:
                printf("Please input the id and size: ");
                scanf("%d %d", &id, &size);
                if (worstFit(head, id, size)) {
                    printf("Successfully allocated!\n");
                } else {
                    printf("Failed to allocate!\n");
                }
                print(head);
                break;
            case 4:
                printf("Please input the id: ");
                scanf("%d", &id);
                if (delete(head, id)) {
                    printf("Successfully deleted!\n");
                } else {
                    printf("Failed to delete!\n");
                }
                print(head);
                break;
            case 5:
                print(head);
                break;
            case 6:
                return 0;
            default:
                printf("Invalid input!\n");
        }
    }
}
