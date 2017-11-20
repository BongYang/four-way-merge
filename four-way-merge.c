#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define SWAP(x,y,t) ((t)=(x),(x)=(y),(y)=(t))
#define RUNMAX 8
#define MAX_TREE_SIZE 8
#define Total 2000000

typedef struct WinnerTreeNode {
    int num, runNum;
}WtreeNode;

typedef struct WinnerTree {
    WtreeNode WtreeArray[MAX_TREE_SIZE];
}Wtree;

void selection(int a[]){
    int i,j,min,temp,n;
    
    n = 1024;
    
    for(i=0; i<n; i++)
    {
        if( a[i] == -1)
            break;
    }
    n = i;
    for(i=0; i<n; i++)
    {
        min = i;
        for(j=i+1; j<n; j++){
            if( a[min] > a[j] )
                min = j;
        }
        SWAP(a[min],a[i],temp);
    }
}
void Merge2(int *arr, int left, int mid, int right)
{
    int front = left;
    int rear = mid + 1;
    int *a = (int*)malloc(sizeof(int)*(right + 1));
    int index = left;
    
    while (front <= mid && rear <= right) {
        if (arr[front] <= arr[rear] )
            a[index] = arr[front++];
        else
            a[index] = arr[rear++];
        index++;
    }
    if (front > mid) {
        for (int i = rear; i <= right; i++, index++){
            a[index] = arr[i];
        }
    }
    else {
        for (int i = front; i <= mid; i++, index++)
            a[index] = arr[i];
    }
    for (int i = left; i <= right; i++)
        arr[i] = a[i];

    free(a);
}

void MergeSort(int arr[], int left, int right) {
    int mid;
    if (left < right) {
        mid = (left + right) / 2;
        MergeSort(arr, left, mid);
        MergeSort(arr, mid + 1, right);
        Merge2(arr, left, mid, right);
    }
}

int pow2(int NumbeOfWay)
{
    int log2OfWay = (int)log2(NumbeOfWay);
    int res = (int)pow(2, log2OfWay + 1);
    return res;
}

void WinnerTreeInit(Wtree *pWtree, int k)
{
    int ClosedNumber = 4;
//    pow2(k);
    int i;
    for (i = 0; i<ClosedNumber; i++)
        pWtree->WtreeArray[i + ClosedNumber].runNum = i;
}

int GetLeftChildIndex(int parent) {
    return 2 * parent;
}

int GetRightChildIndex(int parent) {
    return 2 * parent + 1;
}

void makeFP(FILE* fp[],int KeyCount,int NumberOfRun,int tempIndex){
    int i;
    if( tempIndex == 1 ){
        for (i = 0; i < NumberOfRun+4; i++) {
            fp[i] = fopen("temp0.dat", "rb");
            fseek(fp[i], sizeof(int)*(KeyCount + 1)*i, 0);
        }
    }
    else if( tempIndex == 0 ){
        for (int i = 0; i < NumberOfRun+4; i++) {
            fp[i] = fopen("temp1.dat", "rb");
            fseek(fp[i], sizeof(int)*(KeyCount + 1)*i, 0);
        }
    }
}
void readFile(int KeyCount,int NumberOfRun,char filename[]){
    FILE *fp = fopen(filename, "rb");
    int i,j,count = 0,total_count = 0,count2 = 0;
    int *a;
    a = malloc(sizeof(int)*KeyCount);
    
    for(i=0; i<NumberOfRun; i++){
        fread(a, sizeof(int), KeyCount, fp);
        for(j=0; j<KeyCount; j++)
        {
            printf("%d ",a[j]);
            count++;
            total_count++;
//            if( a[j] != -1){
            
            if( total_count == Total +NumberOfRun)
                {
                    fclose(fp);
                    printf("\nAtotal_cnt = %d\n",total_count);
//                    return;
                }
//            }
        }
//        printf("");
        count2++;
        printf("\ncount = %d\n",count);
        count = 0;
    }
    printf("count2 = %d total_cnt = %d\n",count2,total_count);
    fclose(fp);
}
void closeFile(FILE* fp[],int KeyCount,int NumberOfRun,int tempIndex){
    for (int j = 0; j < NumberOfRun; j++) {
        fclose(fp[j]);
    }
}
void MakeWinnerTree(Wtree *pWtree, FILE *fpArray[], double NumberOfWay,  char finalFileName[])
{
    int i, LeftChild, RightChild, min = -1;
    int ClosedNumber = 4;
    int KeyCount = 1024; // 1024
//    pow2(NumberOfWay);// NumberOfWay = 1954 //
    int count = 0;
    FILE *fp_final;
    FILE *ftemp[2];
    int tempIndex = 1;
    
    ftemp[0] = fopen("temp0.dat","rb");
    ftemp[1] = fopen("temp1.dat","wb");
    fp_final = fopen(finalFileName, "wb");
    
    while(1){
        if( NumberOfWay == 1 ) // 마지막;
            break;
        
        makeFP(fpArray, KeyCount, NumberOfWay,tempIndex);
        WinnerTreeInit(pWtree, NumberOfWay);
        
        for (i = ClosedNumber; i <= 2 * ClosedNumber - 1; i++)//heap에 밑부분 만들기
            if (i - ClosedNumber < NumberOfWay)
                fread(&pWtree->WtreeArray[i].num, sizeof(int), 1, fpArray[i - ClosedNumber]);
            else pWtree->WtreeArray[i].num = -1;
        
        while (1)
        {
            for (i = ClosedNumber - 1; i >= 1; i--) {
                LeftChild = GetLeftChildIndex(i);
                RightChild = GetRightChildIndex(i);
                int LeftChildNode = pWtree->WtreeArray[LeftChild].num;
                int RightChildNode = pWtree->WtreeArray[RightChild].num;
                if (LeftChildNode < RightChildNode)
                    if (LeftChildNode == -1)
                        pWtree->WtreeArray[i] = pWtree->WtreeArray[RightChild];
                    else
                        pWtree->WtreeArray[i] = pWtree->WtreeArray[LeftChild];
                
                    else
                        if (RightChildNode == -1)
                            pWtree->WtreeArray[i] = pWtree->WtreeArray[LeftChild];
                        else
                            pWtree->WtreeArray[i] = pWtree->WtreeArray[RightChild];
            }
            
            if (pWtree->WtreeArray[1].num == -1){
                pWtree->WtreeArray[4].runNum += ClosedNumber;
                pWtree->WtreeArray[5].runNum += ClosedNumber;
                pWtree->WtreeArray[6].runNum += ClosedNumber;
                pWtree->WtreeArray[7].runNum += ClosedNumber;
                fwrite(&min, sizeof(int), 1, ftemp[tempIndex]);
                if( pWtree->WtreeArray[4].runNum >= NumberOfWay && pWtree->WtreeArray[5].runNum >= NumberOfWay && pWtree->WtreeArray[6].runNum >= NumberOfWay && pWtree->WtreeArray[7].runNum >= NumberOfWay)
                {
                    if( tempIndex == 1 ){
                        tempIndex = 0;
                        fclose(ftemp[0]);
                        fclose(ftemp[1]);
                        ftemp[0] = fopen("temp0.dat", "wb");
                        ftemp[1] = fopen("temp1.dat", "rb");
                        readFile(KeyCount, NumberOfWay,"temp1.dat");
                    }
                    else if(tempIndex == 0){
                        tempIndex = 1;
                        fclose(ftemp[0]);
                        fclose(ftemp[1]);
                        ftemp[0] = fopen("temp0.dat", "rb");
                        ftemp[1] = fopen("temp1.dat", "wb");
                        readFile(KeyCount, NumberOfWay,"temp0.dat");
                    }
                    closeFile(fpArray, KeyCount, NumberOfWay, tempIndex);
                    KeyCount = KeyCount * 4; // 만들어지는 런의 크기
                    NumberOfWay = NumberOfWay / 4; // 만들어지는 런의 갯수
                    NumberOfWay = floor(NumberOfWay + 0.5);
                    break;
                }
                continue;
            }
            if( count == 4096 )
                printf("");
            fwrite(&pWtree->WtreeArray[1].num, sizeof(int), 1, ftemp[tempIndex]);
            int runIndex = pWtree->WtreeArray[1].runNum;
            int t;
            if (pWtree->WtreeArray[runIndex % ClosedNumber + ClosedNumber].num == -1)
                pWtree->WtreeArray[runIndex + ClosedNumber].num = -1;
            else {
                fread(&t, sizeof(int), 1, fpArray[runIndex]);
                pWtree->WtreeArray[runIndex % ClosedNumber + ClosedNumber].num = t;
            }
            count++;
        }
        count = 0;
        closeFile(fpArray, KeyCount, NumberOfWay, tempIndex);
//        fwrite(&min, sizeof(int), 1, ftemp[tempIndex]);
    }

    fclose(fp_final);
}
void MakeWinnerTree2(Wtree *pWtree, double NumberOfWay, char finalFileName[])
{
    FILE *fpArray[2][1958] = {{0},{0}};
    int i, j, LeftChild, RightChild;
    int min = -1;
    int ClosedNumber = 4, KeyCount = 1024;
    int count = 0,count2 = 0;
    FILE *fp_final;
    FILE *ftemp[2];
    int tempIndex = 1;
    ftemp[0] = fopen("temp0.dat", "rb");
    ftemp[1] = fopen("temp1.dat", "wb");
//    fp_final = fopen(finalFileName, "wb");
    


    while(1) {//아래의 과정 반복
        
        if( NumberOfWay <= 1)
            break;
        
        WinnerTreeInit(pWtree, 4);
//        makeFP(fpArray, KeyCount, NumberOfWay, tempIndex);
        makeFP(fpArray[tempIndex], KeyCount, NumberOfWay, tempIndex);

        
        while(1) {//4개 런 정렬을 여러번
            for (i = ClosedNumber; i <= 2 * ClosedNumber - 1; i++)
                if (i - ClosedNumber < NumberOfWay){
                    int k = pWtree->WtreeArray[i].runNum;
                    fread(&pWtree->WtreeArray[i].num, sizeof(int), 1, fpArray[tempIndex][k]);
                }
                else pWtree->WtreeArray[i].num = -1;
            
            while (1)//4개의 런 정렬
            {
                for (i = ClosedNumber - 1; i >= 1; i--) {
                    LeftChild = GetLeftChildIndex(i);
                    RightChild = GetRightChildIndex(i);
                    int LeftChildNode = pWtree->WtreeArray[LeftChild].num;
                    int RightChildNode = pWtree->WtreeArray[RightChild].num;
                    if (LeftChildNode < RightChildNode)
                        if (LeftChildNode == -1)
                            pWtree->WtreeArray[i] = pWtree->WtreeArray[RightChild];
                        else
                            pWtree->WtreeArray[i] = pWtree->WtreeArray[LeftChild];
                    
                        else
                            if (RightChildNode == -1)
                                pWtree->WtreeArray[i] = pWtree->WtreeArray[LeftChild];
                            else
                                pWtree->WtreeArray[i] = pWtree->WtreeArray[RightChild];
                    
                }
                
                if (pWtree->WtreeArray[1].num == -1)
                    break;
                if( count == 4096 )
                    printf("");
                fwrite(&pWtree->WtreeArray[1].num, sizeof(int), 1, ftemp[tempIndex]);
                int runIndex = pWtree->WtreeArray[1].runNum;
                int t;
                if (pWtree->WtreeArray[runIndex % ClosedNumber + ClosedNumber].num == -1)
                    pWtree->WtreeArray[runIndex % ClosedNumber + ClosedNumber].num = -1;
                else {
                    fread(&t, sizeof(int), 1, fpArray[tempIndex][runIndex]);
                    pWtree->WtreeArray[runIndex % ClosedNumber + ClosedNumber].num = t;
                }
                count++;
            }
            count = 0;
            count2++;
            if( count2 == 610 )
                printf("");
            pWtree->WtreeArray[4].runNum += ClosedNumber;
            pWtree->WtreeArray[5].runNum += ClosedNumber;
            pWtree->WtreeArray[6].runNum += ClosedNumber;
            pWtree->WtreeArray[7].runNum += ClosedNumber;
            fwrite(&min, sizeof(int), 1, ftemp[tempIndex]);
            if( pWtree->WtreeArray[4].runNum >= NumberOfWay && pWtree->WtreeArray[5].runNum > NumberOfWay && pWtree->WtreeArray[6].runNum >= NumberOfWay && pWtree->WtreeArray[7].runNum >= NumberOfWay)
            {
                KeyCount *= 4;
                NumberOfWay /= 4;
                NumberOfWay = ceil(NumberOfWay);
                for(i=0; i<KeyCount*NumberOfWay-Total; i++)
                    fwrite(&min, sizeof(int), 1, ftemp[tempIndex]);
                break;
            }
        }
        if( tempIndex == 1 ){
            fclose(ftemp[0]);
            fclose(ftemp[1]);
            ftemp[0] = fopen("temp0.dat", "wb");
            ftemp[1] = fopen("temp1.dat", "rb");
//            readFile(KeyCount+1, NumberOfWay, "temp1.dat");
            tempIndex = 0;
        }
        else if( tempIndex == 0 ){
            fclose(ftemp[0]);
            fclose(ftemp[1]);
            ftemp[0] = fopen("temp0.dat", "rb");
            ftemp[1] = fopen("temp1.dat", "wb");
//            readFile(KeyCount+1, NumberOfWay, "temp0.dat");
            tempIndex = 1;
        }
    }
    fclose(fp_final);
}

void InitRandom(int randomNum[]){
    int i;
    for(i=0; i<1024; i++)
    {
        randomNum[i] = -1;
    }
}

void origin() {// Total개를 입력받는다. //
    srand(time(NULL));
    int i,j, randomNum[1024], Total_cnt = 0;
    
    FILE *fp;
    
    fp = fopen("origin.dat", "wb");
    
    for (i = 0; i <= (Total/1024)+1 ; i++)
    {
        InitRandom(randomNum);
        for(j=0; j<1024; j++){
            randomNum[j] = rand()%Total+1;
            Total_cnt++;
            if( Total_cnt == Total ){
                fwrite(randomNum, sizeof(int), sizeof(randomNum)/sizeof(int), fp);
                return;
            }
        }
        fwrite(randomNum, sizeof(int), sizeof(randomNum)/sizeof(int), fp);
    }
    fclose(fp);
}
void func(int InitRunNumber) {
    int EachRunSize = 1024; // E = 1024 Init = 1954 //
    int *a = (int*)malloc(sizeof(int)*EachRunSize);
    int min1 = -1;
    FILE *fOrigin;
    fOrigin = fopen("origin.dat", "rb");
    FILE *fTemp;
    fTemp = fopen("temp0.dat", "wb");
    
    for (int i = 0; i < InitRunNumber; i++) {// 1024개씩 정렬한 다음 fTemp에 저장 //
        fread(a, sizeof(int)*EachRunSize, 1, fOrigin);

        if( i == InitRunNumber-1 ){
            selection(a);
        }
        else{
            MergeSort(a, 0, EachRunSize - 1);
        }
        fwrite(a, sizeof(int)*EachRunSize, 1, fTemp);
        fwrite(&min1, sizeof(int), 1, fTemp);
    }
    free(a);
    fclose(fOrigin);
    fclose(fTemp);
}

void func2(int NumberOfRun, char finalFileName[]) {
    int KeyCount = (Total/NumberOfRun)+1; // KeyCount = 1024 NumberOfRun = 1954 //
    Wtree WinnerTreePtr;
//    WinnerTreeInit(&WinnerTreePtr, NumberOfRun);
//    makeFP(fp, KeyCount, NumberOfRun);
    MakeWinnerTree2(&WinnerTreePtr, NumberOfRun, finalFileName);
}
void printFin(char *finalFileName) {
    FILE *fp_final;
    int i = 0;
    fp_final = fopen(finalFileName, "rb");
    
    while (!feof(fp_final))
    {
        fread(&i, sizeof(int), 1, fp_final);
        printf("%d ", i);
    }
    fclose(fp_final);
}

int main() {
    int EachRunSize = 1024;
    int InitialRunNumber = Total/EachRunSize+1; //처음에 만들어지는 런의 갯수 1954
    char final_name[30];
    origin(); // random number Total개 파일 origin에 저장
//    readFile(1024, 1954, "origin.dat");
    func(InitialRunNumber);//블럭단위로 정렬후 파일에 입력
//    readFile(1024+1, 1954, "temp0.dat");
    func2(InitialRunNumber, final_name);//솔팅
    readFile(1024, 1954, "temp0.dat");
//    printFin("temp0.dat");
    return 0;
}
