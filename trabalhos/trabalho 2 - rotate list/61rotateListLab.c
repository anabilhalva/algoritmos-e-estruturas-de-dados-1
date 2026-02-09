/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* rotateRight(struct ListNode* head, int k) {
    if (!head || !head->next || k==0)
        return head;

    struct ListNode* tail = head;
    int size = 1;
    while (tail->next){
        tail = tail->next;
        size++;
    }

    tail->next = head;

    k = k % size;
    int stepsToNewHead = size - k;

    struct ListNode* newTail = tail;
    while (stepsToNewHead--){
        newTail = newTail->next;
    }
    
    struct ListNode* newHead = newTail->next;
    newTail->next = NULL;

    return newHead;
}