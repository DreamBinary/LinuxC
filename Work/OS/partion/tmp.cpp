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