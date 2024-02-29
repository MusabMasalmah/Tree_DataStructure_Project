#include <stdio.h>
#include <stdlib.h>
typedef struct course * Course;

struct course{
    char courseName[150];
    int creditHours;
    char courseCode[150];
    char courseDepartment[150];
    char courseTopics[150];
    Course Left;
    Course Right;
    int Height; //Balance information
};

int NOC(char fileName[20]){
    int count = 0;      //function to get number of lines from file
    FILE *f;
    f = fopen(fileName,"r");
    char *token;
    char line[200];
    while(fgets(line,sizeof(line),f)){
       if(strcmp(line,"") == 10){ //cut the courses about the spaces
        count++;
       }
    }
    fclose(f);
    return count+1;
}

Course MakeEmpty(Course T ){ //make the node empty

    if( T != NULL )
    {
        MakeEmpty( T->Left );
        MakeEmpty( T->Right );
        free(T);
    }
    return NULL;
}

int Height( Course P ){
    if( P == NULL )
        return -1;
    else
        return P->Height;  //get the hight of tree
}

int Max( int Lhs, int Rhs ){
    return Lhs > Rhs ? Lhs : Rhs;
}

Course SingleRotateWithLeft( Course K2 ){
    Course  K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    K2->Height = Max( Height( K2->Left ), Height( K2->Right ) ) + 1;
    K1->Height = Max( Height( K1->Left ), K2->Height ) + 1;

    return K1;  /* New root */
}

Course SingleRotateWithRight( Course K1 ){
    Course K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    K1->Height = Max( Height( K1->Left ), Height( K1->Right ) ) + 1;
    K2->Height = Max( Height( K2->Right ), K1->Height ) + 1;

    return K2;  /* New root */
}

Course DoubleRotateWithLeft( Course K3 ){
    /* Rotate between K1 and K2 */
    K3->Left = SingleRotateWithRight( K3->Left );

    /* Rotate between K3 and K2 */
    return SingleRotateWithLeft( K3 );
}

Course DoubleRotateWithRight(Course K1 ){
    /* Rotate between K3 and K2 */
    K1->Right = SingleRotateWithLeft( K1->Right );

    /* Rotate between K1 and K2 */
    return SingleRotateWithRight( K1 );
}

Course Insert(Course T,Course node){
    if( T == NULL )
    {
        if( node == NULL )
            printf( "Out of space!!!" );
        else
        {
            T = node;
            T->Left = NULL;
            T->Right = NULL;
            T->Height = 0;
        }
    }

    else if( strcmp(node->courseCode,T->courseCode) < 0 ) //if the code less than insert to left else to right
    {
        T->Left = Insert(T->Left,node);

        if( Height( T->Left ) - Height( T->Right ) == 2 )
            if( strcmp(node->courseCode,T->Left->courseCode) < 0 )
                T = SingleRotateWithLeft( T );
            else
                T = DoubleRotateWithLeft( T );
    }
    else if( strcmp(node->courseCode,T->courseCode) > 0 )
    {
        T->Right = Insert(T->Right,node);

        if( Height( T->Right ) - Height( T->Left ) == 2 )
            if( strcmp(node->courseCode,T->Left->courseCode) > 0 )
                T = SingleRotateWithRight( T );
            else
                T = DoubleRotateWithRight( T );
    }
    /* Else X is in the tree already; we'll do nothing*/

    T->Height = Max( Height( T->Left ), Height( T->Right ) ) + 1; //make new hight
    return T;
}

Course readFile(Course T){

    int NumOfCourse = NOC("courses.txt");      //function to read bus file
    FILE *f;
    f = fopen("courses.txt","r");
    char *token;
    char line[200];
    char courseInfo[NumOfCourse][750];
    for(int i=0 ; i<NumOfCourse ; i++)
        strcpy(courseInfo[i],"");
    int i = 0;
    while(fgets(line,sizeof(line),f)){

       if(strcmp(line,"") != 10){  //read the lines to the first space
        strcat(courseInfo[i],line);
       }
       else{
        i++;
       }
    }

    int j=0;
    while(j < NOC("courses.txt")){     //get the lines line by line
        int i=0;
        token = strtok(courseInfo[j],":#/");  //cut the line to words in all #
        char take[5][150];
        while(token != NULL){
            strcpy(take[i],token);
            token = strtok(NULL,":#/");
            i++;
            }
           Course node = malloc(sizeof(struct course));
           node->Left = NULL;
           node->Right = NULL;
           node->Height = 0;
           strcpy(node->courseName,take[0]);
           printf("%s\n\n",take[0]);
           int courseHoure = atoi(take[1]);
           node->creditHours = courseHoure;
           strcpy(node->courseCode,take[2]);
           strcpy(node->courseDepartment,take[3]);
           strcpy(node->courseTopics,take[4]);
           T = Insert(T,node);
            j++;
    }
    fclose(f);
    printf("-The file was readen.\n");
    return T;
}

int sameCode(Course T,char code[150]){  //check if there a node have the same code
    if(T == NULL)
        return 0;
    else if(T != NULL && strcmp(T->courseCode,code) == 0){
      return 1;
    }
    else if(strcmp(T->courseCode,code) > 0 && T->Left != NULL){
        sameCode(T->Left,code);
    }
    else if(strcmp(T->courseCode,code) < 0 && T->Right != NULL){
        sameCode(T->Right,code);
    }
    return 0;

}

Course insertCourse(Course T){ //insert new course
    char courseName[150],courseCode[150],courseDepartment[150],courseTopics[150];
    int creditHours;
    printf("Enter the course name: ");
    getchar();
    gets(courseName);
    printf("Enter the course code: ");
    scanf("%s",courseCode);
    int flag = sameCode(T,courseCode);
    printf("%d",flag);
    while(flag == 1){
                printf("This course code is inserted, insert anther one: ");
                scanf("%s",courseCode);
                flag = sameCode(T,courseCode);
           }
    printf("Enter the course Department: ");
    getchar();
    gets(courseDepartment);
    printf("Enter the course topics with ',': ");
    getchar();
    gets(courseTopics);
    printf("Enter the course credit Hours: ");
    scanf("%d",&creditHours);
    Course node = malloc(sizeof(struct course));
           node->Left = NULL;
           node->Right = NULL;
           node->Height = 0;
           strcpy(node->courseName,courseName);
           node->creditHours = creditHours;
           strcpy(node->courseCode,courseCode);
           strcpy(node->courseDepartment,courseDepartment);
           strcpy(node->courseTopics,courseTopics);
           T = Insert(T,node);

    return T;
}

Course Find(Course T ,char code[150]){ //find course from code

    if( T == NULL )
        return NULL;
    if( strcmp(T->courseCode,code) > 0 )
        return Find( T->Left, code );
    else if( strcmp(T->courseCode,code) < 0 )
        return Find( T->Right, code );
    else
        return T;
}

void FindAndUpdate(Course T){
    char code[150];
    printf("Enter code to find the course:");
    scanf("%s",code);
    Course temp = Find(T,code);//find
    if(temp == NULL){
        printf("no course with this code.");
    }
    else{
            printf("Name:%s Code:%s.\n",temp->courseName,temp->courseCode);
        printf("\n1. Update name.\n2. Update cridet houres.\n3. Update topics.\n4. Update department name.\n5. Exit.\n\nEnter your choice"); //update
        int choice,houres;
        char name[150],topics[150],depName[150];
        scanf("%d",&choice);
        while(choice > 0 && choice < 5){
            switch (choice){
                case 1:
                    printf("Enter the name:");
                    scanf("%s",name);
                    strcpy(temp->courseName,name);
                    break;
                case 2:
                    printf("Enter the houres:");
                    scanf("%d",&houres);
                    temp->creditHours = houres;
                    break;
                case 3:
                    printf("Enter the topics with ',':");
                    scanf("%s",topics);
                    strcpy(temp->courseTopics,topics);
                    break;
                case 4:
                    printf("Enter the department name:");
                    scanf("%s",depName);
                    strcpy(temp->courseTopics,depName);
                    break;
            }
    printf("1. Update name.\n2. Update cridet houres.\n3. Update topics.\n4. Update department name.\n5. Exit.\nEnter your choice");
    scanf("%d",&choice);
    }

    }
}

void printSameDep(Course T,char depName[150]){ //print in order the same departments

    if (T != NULL) {
        printSameDep(T->Left,depName);

        if (strcmp(T->courseDepartment,depName) == 0)
            printf("%s ", T->courseCode);

        printSameDep(T->Right,depName);
   }
}

Course minValueNode(Course node){ //find minimum

    Course current = node;

    while (current->Left != NULL)
        current = current->Left;

    return current;
}

int getBalance(Course N){ //get the balance

    if (N == NULL)
        return 0;
    return Height(N->Left) - Height(N->Right);
}

Course deleteNode(Course root, char code[100]){ //delete node and make the balance

    if (root == NULL)
        return root;

    if ( strcmp(code,root->courseCode) < 0 )
        root->Left = deleteNode(root->Left, code);

    else if( strcmp(code,root->courseCode) > 0 )
        root->Right = deleteNode(root->Right, code);

    else
    {
        if( (root->Left == NULL) || (root->Right == NULL) )
        {
            Course temp = root->Left ? root->Left : root->Right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
             *root = *temp;

            free(temp);
        }
        else
        {
            Course temp = minValueNode(root->Right);

            strcpy(root->courseCode,temp->courseCode);

            root->Right = deleteNode(root->Right, temp->courseCode);
        }
    }

    if (root == NULL)
        return root;

    root->Height = 1 + Max(Height(root->Left),Height
                           (root->Right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->Left) >= 0) //balanced the tree
        return SingleRotateWithRight(root);

    if (balance > 1 && getBalance(root->Left) < 0)
    {
        root->Left =  SingleRotateWithLeft(root->Left);
        return SingleRotateWithRight(root);
    }
    if (balance < -1 && getBalance(root->Right) <= 0)
        return SingleRotateWithLeft(root);

    if (balance < -1 && getBalance(root->Right) > 0)
    {
        root->Right = SingleRotateWithRight(root->Right);
        return SingleRotateWithLeft(root);
    }

    return root;
}

void PrintInOrder( Course t){ //print the node in order

    if( t != NULL)
    {
        PrintInOrder( t->Left );
        printf("Code: %s, credit hours: %d, topics: %s,\n", t->courseCode,t->creditHours,t->courseTopics);
        PrintInOrder( t->Right );
    }
}

void printWithCode(Course T){  //print the node topics from code
    char code[150];
    printf("Enter the code: ");
    scanf("%s",code);
    Course Temp = Find(T,code);
    if(Temp == NULL)
        printf("NULL TREE .\n");
    else
    printf("The topics of the course that have code %s : %s.\n",Temp->courseCode,Temp->courseTopics);
}

Course deleteWithLetter(Course T,char c) { //dleete the node if the character is similar to the first character
    int noc = NOC("offered_courses.txt");
    int i=0;
    char code[noc][150];
    if (T != NULL) {
        if ( T->courseCode[0] == c ){               //insert the node code to array
                strcpy(code[i],T->courseCode);
                i++;
        }
        T->Left = deleteWithLetter(T->Left,c);
        T->Right = deleteWithLetter(T->Right,c);
   }
   for(int j=0 ; j<i ; j++){
    T = deleteNode(T,code[j]);
   }

   return T;
   }

Course deleteWithDepName(Course T,char depName[150]) {  //
    int noc = NOC("offered_courses.txt");
    int i=0;
    char code[noc][150];
    if (T != NULL) {
        if ( strcmp(T->courseDepartment,depName) == 0 ){
                strcpy(code[i],T->courseCode);   //insert to the array
                i++;
        }
        T->Left = deleteWithDepName(T->Left,depName);
        T->Right = deleteWithDepName(T->Right,depName);
   }
   for(int j=0 ; j<i ; j++){
    T = deleteNode(T,code[j]);        //delete
   }

   return T;
}

void PrintInFile(Course T,FILE *f){ //print to the file

    if( T != NULL)
    {
        PrintInFile( T->Left,f );
        fprintf(f,"Code: %s, credit hours: %d, topics: %s,\n", T->courseCode,T->creditHours,T->courseTopics);
        PrintInFile( T->Right,f );
    }
}

int main()
{
    Course T = NULL;
    int count = NOC("courses.txt");
    printf("The Menu:\n1. Read the file.\n2. Insert a new course. \n3. Find a course. \n4. List courses in lexicographic order.\n5. List all topics associated with a given course.\n6. List all courses in lexicographic order that belong to the same department.\n7. Delete a course.\n8. Delete all courses that start with a specific letter.\n9. Delete all courses belong to a given department\n10. Save all words in file offered_courses.txt  \nEnter your choice : ");
    int choice ;
    char code[150],depName[150];
    char c;
    FILE *f;
    scanf("%d",&choice);
    while(choice > 0 && choice < 12){
            switch (choice){       //switch statment
                case 1:
                    T = readFile(T);
                    break;
                case 2:
                    T = insertCourse(T);
                    break;
                case 3:
                    FindAndUpdate(T);
                    break;
                case 4:
                    PrintInOrder(T);
                    break;
                case 5:
                    printWithCode(T);
                    break;
                case 6:
                    printf("Enter the department name: ");
                    getchar();
                    gets(depName);
                    printSameDep(T,depName);
                    printf("\n");
                    break;
                case 7:
                    printf("Enter the code of the course you want to delete it: ");
                    scanf("%s",code);
                    T = deleteNode(T,code);
                    break;
                case 8:
                    printf("Enter the letter: ");
                    getchar();
                    scanf("%c",&c);
                    printf("%c",c);
                  T = deleteWithLetter(T,c);
                   printf("\n");
                    break;
                case 9:
                    printf("Enter the department name: ");
                    getchar();
                    gets(depName);
                    printf("%s",depName);
                   T = deleteWithDepName(T,depName);
                   printf("\n");
                    break;
                case 10:
                    f = fopen("offered_courses.txt","w");
                    PrintInFile(T,f);
                    fclose(f);
                    break;
            }
            printf("-----------------------------------------------------------------------------------\n");
        printf("The Menu:\n1. Read the file.\n2. Insert a new course. \n3. Find a course. \n4. List courses in lexicographic order.\n5. List all topics associated with a given course.\n6. List all courses in lexicographic order that belong to the same department.\n7. Delete a course.\n8. Delete all courses that start with a specific letter.\n9. Delete all courses belong to a given department\n10. Save all words in file offered_courses.txt  \nEnter your choice : ");
    scanf("%d",&choice);
    }
    return 0;
}


















