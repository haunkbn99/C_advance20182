#include <stdio.h>
#include <string.h>
int main()
{
    char* name = (char*)malloc(100*sizeof(char));
    printf("Nhap cau ban thich: ");
    fflush(stdin);
    scanf("%[^\n]s", name);
    printf("size of name: %d\n",strlen(name));
    for (int i = 0; i < strlen(name); i++)
    {
        printf("%c",name[i]);
    }
    printf("\n Cau cau ban: %s\n",name);
}