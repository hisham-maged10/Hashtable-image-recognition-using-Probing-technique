/**
 * @author ${hisham_maged10}
 *https://github.com/hisham-maged10
 * ${DesktopApps}
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#define ImagesNumber 1000
#define ArraySize 784
#define PxSize 785
#define PxLineSize 28
#define TYPE int
/*                                                       Main.c                                                             */
typedef struct
{
    int img_array[ArraySize];
}img;
typedef struct
{
    img ImgData;
    TYPE key;
}data;
typedef struct
{
    TYPE HashTable[ImagesNumber];
}Linear;
int LinearCollisions=0;
int firstrun=1;
int LoadedImagesNo=0;
int AVAILABLE = -1;
data Img_Data[ImagesNumber];
data ForSeachImg[ImagesNumber];
Linear HashMap;
int firstProbe=1;
int returnKey;
void OperationMenu();
void ProbingMenu();
void MainMenu();
void Load();
void Put(TYPE MapKey,img CurrImage);
int removeFromProbing(img CurrImage);
int HashCode(img Img);
int Get(img CurrImage);

void Load()
{
    FILE *file;
    int PxCounter=1,ImgCounter=0;
    char File_Name[25];
    if(!firstrun)
    {
    puts("Enter the name of the file you want to search in:-\n");
    scanf("%s",&File_Name);
    }
    else
    {
        strcpy(File_Name,"Data.txt");
    }
    if((strstr(File_Name,".txt")!=NULL))
    {
    file=fopen(File_Name,"r");
    }
    else
    {
        strcat(File_Name,".txt");
        file=fopen(File_Name,"r");
    }
    if(file)
    {
        while(!feof(file))
        {
            if(PxCounter==PxSize)
            {
                if(firstrun)
                {
                fscanf(file,"%d",&Img_Data[ImgCounter++].key);
                PxCounter=1;
                }
                else if(firstrun==0)
                {
                    ImgCounter++;
                    LoadedImagesNo++;
                    PxCounter=1;
                }
                if(ImgCounter==ImagesNumber)
                {
                    break;
                }
            }
            if(firstrun)
            {
            fscanf(file,"%d",&Img_Data[ImgCounter].ImgData.img_array[(PxCounter-1)]);
            }
            else
            {
            fscanf(file,"%d",&ForSeachImg[ImgCounter].ImgData.img_array[(PxCounter-1)]);
            }
            PxCounter++;


        }
        firstrun=0;
    fclose(file);
    }
    else if(file==NULL)
    {
        puts("Error, File Doesn't Exist.");
        if(getch())
            Load();
    }


}
int Get(img CurrImage)
{
    int Key,i,PxCounter,Hash,FirstHash,ImageCounter,ErrorFlag;
    PxCounter=ImageCounter=ErrorFlag=0;
    Hash = HashCode(CurrImage);
    FirstHash = HashCode(CurrImage);
    while(ImageCounter<ImagesNumber)
    {
        Key = HashMap.HashTable[FirstHash];
        i = HashCode(Img_Data[Key].ImgData);
        if(Key == -1)//means it was removed
        {
            return -1;//return error
        }
        if(Hash == i)
        {
            PxCounter=0;
            ErrorFlag=0;
            while(1)
            {
                if(PxCounter==ArraySize) break;

                if(Img_Data[Key].ImgData.img_array[PxCounter] != CurrImage.img_array[PxCounter])
                {
                    ErrorFlag=-1;
                    break;
                }
                else
                {
                    ErrorFlag=0;
                }
            PxCounter++;
            }
            if(ErrorFlag!=-1)
            {
                return Key;
            }
        }
        FirstHash++;
        ImageCounter++;

    }
    if(ImageCounter==1000)
        return -1;
}/*
1.get the hashcode of the passed image
2.make an Infinity Loop that does 2 ifs and break if it finds a place in the hashtable to put the key into
3.if the place in the hashtable not equal to NULL and not equal to (-1) which is the value Given to the place if the element is removed, then put the key into that place
4.else increase the number of collisions by one and the Hash by one to find the next free spot
*/
void Put(int Key, img CurrImage)
{
    int Hash = HashCode(CurrImage);//return HashCode of the image
    while(1)
    {
        if(HashMap.HashTable[Hash]==NULL && HashMap.HashTable[Hash]!=-1)
        {
           HashMap.HashTable[Hash] = Key;
           break;
        }
        else
        {
        Hash++; //increase the hash to find the next empty spot
        LinearCollisions++;//increase the number of collisions happened
        }
    }
    //enter the id or the data in the index
}
/* This methods is a copy of the get method but the last conditions are modified for Removal if found*/
int removeFromProbing(img CurrImage)
{
    int Key,i,PxCounter,Hash,FirstHash,ImageCounter,ErrorFlag;
    PxCounter=ImageCounter=ErrorFlag=0;
    Hash = HashCode(CurrImage);
    FirstHash = HashCode(CurrImage);
    while(ImageCounter<ImagesNumber)
    {
        Key = HashMap.HashTable[FirstHash];
        i = HashCode(Img_Data[Key].ImgData);
        if(Hash == i)
        {
            PxCounter=0;
            ErrorFlag=0;
            while(1)
            {
                if(PxCounter==ArraySize) break;

                if(Img_Data[Key].ImgData.img_array[PxCounter] != CurrImage.img_array[PxCounter])
                {
                    ErrorFlag=-1;
                    break;
                }
                else
                {
                    ErrorFlag=0;
                }
            PxCounter++;
            }
            if(ErrorFlag!=-1)//if we find it we will return the id and make it equals to available to know that we can enter in it as it empty place
            {
                HashMap.HashTable[FirstHash] = -1;
                returnKey=Key;
                return Key;
            }
        }
        FirstHash++;
        ImageCounter++;
        if(FirstHash == 1000)
        {
            Hash = 0;
        }
    }
    if(ImageCounter==1000)
        return -1;
}
int HashCode(img Img)
{
    int TotalLine,RowNo,PxCount,Hash;
    TotalLine=PxCount=Hash=0;
    RowNo=1;
    while(1)
    {
        /* The loop ends when all the data is Covered */
        if(PxCount==ArraySize)
        {
            break;
        }
    /* Multiply the Sum of values covered in the loop till now with the number of rows, and get the hashcode where the hashcode is the sum of all
    the columns multiplied by the Rows' numbers and inc. the Rows' Number by one and make the Sum of values covered equals zero to sum the next row separately*/
    if(PxCount%PxLineSize==0)
    {
        TotalLine*=(RowNo++);
        Hash+=TotalLine;
        TotalLine=0;
    }
    //To sum all the Values of the Columns
    else
    {
     TotalLine+=Img.img_array[PxCount];
    }
    PxCount++; //for breaking condition
    }
    Hash%=1000; //to get the correct hash
    return Hash;
}

void ProbingMenu()
{
    system("cls");
    printf("      ---------------------------------------ProbingMenu---------------------------------------\n\n");
    int j;
    if(firstProbe)
    {
    Load();
    fflush(stdin);
    clock_t Start = clock();
    for(j=0; j<ImagesNumber; j++)
    {
        Put(Img_Data[j].key,Img_Data[j].ImgData);
    }
    clock_t End = clock();
    double time_spent = (double)(End - Start) / CLOCKS_PER_SEC;
    printf("Collisions: %d\n\n",LinearCollisions);
    printf("Time: %lf\n\n",time_spent);
    Load();
    firstProbe=0;
    }
    printf("                                    Search (1)\n\n");
    printf("                                    Remove (2)\n\n");
    printf("                                    please choose an option: \n");
    switch(getch())
    {
    case '1':
        {
             for(j=0;j<=LoadedImagesNo;j++)
            {
             if(Get(ForSeachImg[j].ImgData)==-1)
             {
                 puts("Error, Not Found!!\n");
             }
             else
             {
                 printf("Image Found and of ID[%d]\n\n",Get(ForSeachImg[j].ImgData));
             }
            }
            printf("press any key to continue...");
            getch();
            ProbingMenu();
            break;
    }
    case '2':
        {
        for(j=0;j<=LoadedImagesNo;j++)
            {
                if(removeFromProbing(ForSeachImg[j].ImgData)==-1)
             {
                 puts("Error, Not Found!!\n");
             }
             else
             {
                 printf("Image Found and of ID[%d] and deleted\n\n",returnKey);
             }
            }
            printf("press any key to continue...");
            getch();
            ProbingMenu();
            break;
        }
    default:
    {
        printf("Please choose a correct option ! (press any key to continue)");
        getch();
        system("cls");
        MainMenu();
        break;
    }
    }
}

/*
OperationMenu:
the menu that gives the users the option to navigate the program using getch where a switch case is applied to getch
*/
void OperationMenu()
{
    system("cls");
     printf("      ---------------------------------------OperationsMenu (Linear Probing)---------------------------------------\n\n");
    printf("                                    HashMap by Probing (1)\n\n");
    printf("                                    Return to Main Menu(2)\n\n");
    printf("                                    please choose an option: \n");
    switch(getch())
    {
    case '1':
        {
            ProbingMenu();
            break;
        }
    case '2':
        {
            MainMenu();
            break;
        }
    default:
    {
        printf("Please choose a correct option ! (press any key to continue)");
        getch();
        system("cls");
        MainMenu();
        break;
    }
    }
}
/*
MainMenu:
the menu that gives the users the option to start the program or exit it using getch where a switch case is applied to getch
*/
void MainMenu()
{
    system("cls");
     printf("      ---------------------------------------MainMenu---------------------------------------\n\n");
    printf("                                    Operation (1)\n");
    printf("                                    Exit (2)\n");
    printf("                                    please choose an option: \n");
    switch(getch())
    {
    case '1':
        {
            OperationMenu();
            break;
        }
    case '2':
        {
            exit(1);
            break;
        }
    default:
    {
        printf("Please choose a correct option ! (press any key to continue)");
        getch();
        system("cls");
        MainMenu();
        break;
    }
    }
}
/* Main function: it changes the color for the whole program to background:blue and foreground:white then loads the Images then opens the MainMenu*/
int main()
{
    system("color 17");
    MainMenu();
    return 0;
}

