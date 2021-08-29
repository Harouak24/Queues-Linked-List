#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct tag
{
    char FullName[60];
    int ID;
    struct tag* next;
} node;
void menu(void);
node* Create_Fill_node(char* name, int id);
void enqueue(node** front, node** rear, node* ToAdd);
node dequeue(node** front, node** rear);
void office_hours_over(node** front1, node** front2, node** rear1, node** rear2);
node* change_queue(node** front, node** rear, int id);
int main(void)
{
    int choice, type, id, turn = 0, flag = 0;
    char name[60], temp;
    node* advising, * adv_rear, * course_discussion, * crs_rear, * student, student_to_serve;
    do
    {
        menu();
        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            scanf("%c", &temp);
            printf("\n\tEnter your name: ");
            gets(name);
            printf("\tEnter your ID: ");
            scanf("%d", &id);
            student = Create_Fill_node(name, id);
            printf("\n\n\tAre you here for:\n\n");
            printf("\t1. Course Discussion.\n");
            printf("\t2. Advising.\n");
            printf("\n\tEnter your choice: ");
            scanf("%d", &type);
            if (type == 1)
            {
                enqueue(&course_discussion, &crs_rear, student);
                printf("\n\tThe student with the name %s and ID %d has been successfully added to the course discussion queue.\n", student->FullName, student->ID);
            }
            else if (type == 2)
            {
                enqueue(&advising, &adv_rear, student);
                printf("\n\tThe student with the name %s and ID %d has been successfully added to the advising queue.\n", student->FullName, student->ID);
            }
            else
                printf("\n\n\tError! There is no such queue.\n\n");
            break;
        case 2:
            if (turn == 1 || (flag == 0 && course_discussion == NULL))
            {
                if (advising != NULL)
                {
                    student_to_serve = dequeue(&advising, &adv_rear);
                    printf("\n\tThe student with the name %s and ID %d was just served from the advising queue.\n", student_to_serve.FullName, student_to_serve.ID);
                }
                else
                    printf("\n\tAdvising queue is empty.\n");
                turn = 0;
                flag = 1;
                break;
            }
            else if (turn == 0)
            {
                if (course_discussion != NULL)
                {
                    student_to_serve = dequeue(&course_discussion, &crs_rear);
                    printf("\n\tThe student with the name %s and ID %d was just served from the course discussion queue.\n", student_to_serve.FullName, student_to_serve.ID);
                }
                else
                    printf("\n\tCourse discussion queue is empty.\n");
                turn = 1;
            }
            break;
        case 3:
            printf("\n\tOffice hours are over gentlemen.\n");
            if (advising != NULL || course_discussion != NULL)
            {
                office_hours_over(&course_discussion, &advising, &crs_rear, &adv_rear);
                printf("\n\tA new file called 'to_serve_later.txt' was generated with the names of all students who were not served today.\n");
            }
            else
                printf("\n\tSince there are no students left unserved, no file will be generated.\n");
            break;
        case 4:
            printf("\n\n\tWhich queue do you currently belong to and wish to change?\n\n");
            printf("\t\t1. Course Discussion.\n");
            printf("\t\t2. Advising.\n");
            printf("\n\t\tEnter your choice: ");
            scanf("%d", &type);
            printf("\n\tPlease enter your id: ");
            scanf("%d", &id);
            if (type == 1)
            {
                student = change_queue(&course_discussion, &crs_rear, id);
                if (student == NULL)
                    printf("\n\nID number %d could not be found in the course discussion queue!\n\n", id);
                else
                {
                    enqueue(&advising, &crs_rear, student);
                    printf("\n\tThe student with the name %s and ID %d has been successfully added to the advising queue.\n", student->FullName, student->ID);
                }
            }
            else if (type == 2)
            {
                student = change_queue(&advising, &adv_rear, id);
                if (student == NULL)
                    printf("\n\nID number %d could not be found in the course discussion queue!\n\n", id);
                else
                {
                    enqueue(&course_discussion, &crs_rear, student);
                    printf("\n\tThe student with the name %s and ID %d has been successfully added to the course discussion queue.\n", student->FullName, student->ID);
                }
            }
            else
                printf("\n\n\tError! There is no such queue.\n\n");
            break;
        case 5:
            printf("\n\nLater Alligator!\n\n");
            break;
        default:
            printf("\n\nError! There is no such choice.\n\n");
        }
    } while (choice != 5);
    return 0;
}
void menu(void)
{
    printf("\n\n");
    printf("---------------------------------------------\n");
    printf("|                   MENU                    |\n");
    printf("---------------------------------------------\n");
    printf("\t1. Assist to Mrs. Talei office hours.\n");
    printf("---------------------------------------------\n");
    printf("\t2. Mrs Talei saying 'Coming In'.\n");
    printf("---------------------------------------------\n");
    printf("\t3. Office hours are over.\n");
    printf("---------------------------------------------\n");
    printf("\t4. Change queue.\n");
    printf("---------------------------------------------\n");
    printf("\t5. Quit.\n");
    printf("---------------------------------------------\n");
}
node* Create_Fill_node(char* name, int id)
{
    node* student;
    student = (node*)malloc(sizeof(node));
    strcpy(student->FullName, name);
    student->ID = id;
    student->next = NULL;
    return student;
}
void enqueue(node** front, node** rear, node* ToAdd)
{
    if (*front == NULL)
        *front = ToAdd;
    else
        (*rear)->next = ToAdd;
    *rear = ToAdd;
}
node dequeue(node** front, node** rear)
{
    node student, * temp;
    student = **front;
    temp = *front;
    free(temp);
    if (*front == *rear)
        *front = *rear = NULL;
    else
        *front = (*front)->next;
    return student;
}
void office_hours_over(node** course_discussion, node** advising, node** crs_rear, node** adv_rear)
{
    int count = 1;
    FILE* outfp;
    node temp;
    outfp = fopen("to_serve_later.txt", "w");
    if (course_discussion != NULL)
    {
        fprintf(outfp, "Course discussion students:\n\n");
        while (*course_discussion != NULL)
        {
            temp = dequeue(course_discussion, crs_rear);
            fprintf(outfp, "%d. Name: %s\t ID: %d\n", count, temp.FullName, temp.ID);
            count++;
        }
    }
    count = 1;
    if (*advising != NULL)
    {
        fprintf(outfp, "\nAdvising students:\n\n");
        while (*advising != NULL)
        {
            temp = dequeue(advising, adv_rear);
            fprintf(outfp, "%d. Name: %s\t ID: %d\n", count, temp.FullName, temp.ID);
            count++;
        }
    }
    fclose(outfp);
}
node* change_queue(node** front, node** rear, int id)
{
    node* student = NULL, * walker;
    if (*front == NULL)
        return student;
    else if ((*front)->ID == id)
    {
        student = *front;
        *front = (*front)->next;
        if (*front == NULL)
            *rear = NULL;
        return student;
    }
    else
    {
        walker = *front;
        while (walker->next != NULL)
        {
            if (walker->next->ID == id)
            {
                student = walker->next;
                walker->next = student->next;
                if (walker->next == NULL)
                    *rear = walker;
                return student;
            }
            else
                walker = walker->next;
        }
        return student;
    }
}