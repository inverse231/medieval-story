#include "headers.h"
#include "defines.h"
#include "ExternalFunctions.h"
#include "Locations.h"
struct Player{
    char NAME[20],CLASSNAME[6];
    int CLASS;
    int money;
    int lvl,maxhp,maxmp,maxxp,hp,mp,xp;
    int str,ag,intel;
    int weapon,armor,skills;
    char weaponString[32],armorString[32];
    short hppotion,mppotion;
    int location;
    int prevlocation;
}Player;
struct CM{
    char NAME[20];
    int number,attack,agility,defence,maxhp,hp;
}CM;
struct LocationVars{
    bool Poison;
    bool civquest,kingquest,qcompleted,Map;
    bool room1,room2;
    bool CanGetBook,CanGetApple,CanEnterCastle;
    bool skelet, lighter;
    bool key,box;
}LocationVars;
struct Player HERO;
struct CM CurrentMob;
struct LocationVars LV;
bool InGame;
bool InBattle;
short difficulty=1;
extern HANDLE h;

/*���������� � ��������*/
void saver(){
    FILE *current=fopen("save.bin","wb");
    fwrite(&HERO,sizeof(Player),1,current);
    fseek(current,sizeof(Player),SEEK_SET);
    fwrite(&LV,sizeof(LocationVars),1,current);
    fclose(current);
}
bool loader(){
    FILE *current=fopen("save.bin","a+b");
    fseek(current,0,SEEK_END);
    if(ftell(current)==0){
        printx("���� ����\n",24);
        return false;
    }
    fseek(current,0,SEEK_SET);
    fread(&HERO,sizeof(Player),1,current);
    fseek(current,sizeof(Player),SEEK_SET);
    fread(&LV,sizeof(LV),1,current);
    fclose(current);
    return true;
}

/*������������� ����*/
void setConsoleSize(){                                  /*��� �� ��������� ��� ��������� ���� http://www.cyberforum.ru/cpp-beginners/thread520249.html*/
    HWND hwnd;
    char Title[1024];
    GetConsoleTitle(Title, 1024); // ������ ��� ����
    hwnd=FindWindow(NULL, Title); // ������ hwnd ����
    MoveWindow(hwnd,640,360,630,500,TRUE);//xnew,ynew,wnew,hnew -����� ��������� x,y, ������ � ������
}
void setDefaultVariables(){
    strcpy(HERO.weaponString,"�����");
    strcpy(HERO.armorString,"�����");
    HERO.hppotion=2;
    HERO.mppotion=2;
    HERO.lvl=1;
    HERO.weapon=0;
    HERO.armor=0;
    HERO.skills=0;
    HERO.xp=0;
    HERO.maxxp=50;
    HERO.money=0;
    HERO.skills=0;
    LV.Poison=false;                                                                    /*�� �� ������*/
    LV.civquest=false, LV.kingquest=false,LV.qcompleted=false, LV.Map=false;            /*�����*/
    LV.room1=false, LV.room2=false;                                                     /*�������*/
    LV.CanGetBook=true, LV.CanGetApple=true, LV.CanEnterCastle=false;                   /*��������, ��� � ���� � �����*/
    LV.lighter=false, LV.skelet=false;                                                  /*������*/
    LV.key=false, LV.box=false;                                                         /*����������*/
}
void ChooseName(){
    consoleclear();
    printf("������� ��� ������ ���������:\n");
    scanf("%s",HERO.NAME);
}
void ChooseClass(){
    short pointer=1;
    bool again=true;
    consoleclear();
    while(again){
        consoleclear();
        printf("-------------------------------����� ������-------------------------------\n");
        arrowLeft(pointer,1);
        printf("���� - ������� ���  \n-������� �������� - ����     \n-������� ���������� ��������\n-������� ���������� �����                                      \n-������ �������� �����                                            \n-��� ����� ���������� ������������ ���� �������� ����������\n-���� ��������� �� ���� ���������� �����\n");

        arrowLeft(pointer,2);
        printf("������ - ������� ���\n-������� �������� - �������� \n-������� ���������� ��������\n-������� ��������� ����,�� ����� ���� ��������� ������ �� ���  \n-������� ���� ���������� �� ���� ����������                       \n");

        arrowLeft(pointer,3);
        printf("��� - ������� ���   \n-������� �������� - ���������\n-������ ���������� �������� \n-������� ������� ���� � ������� ����������                     \n-���� ���� �������� ����� ���������� � ������� ����������� ����   \n");
        if(menuEnter(&pointer,3,WithoutESC)){
            again=false;
            HERO.CLASS=pointer;
            switch(pointer)
            {
                case 1:
                    printf("�� ������� ����� ����\n");
                    strcpy(HERO.CLASSNAME,"����");
                    HERO.maxhp=1000;
                    HERO.str=50;
                    HERO.ag=5;
                    HERO.intel=20;
                    break;
                case 2:
                    printf("�� ������� ����� ������\n");
                    strcpy(HERO.CLASSNAME,"������");
                    HERO.maxhp=750;
                    HERO.str=30;
                    HERO.ag=20;
                    HERO.intel=30;
                    break;
                case 3:
                    printf("�� ������� ����� ���\n");
                    strcpy(HERO.CLASSNAME,"���");
                    HERO.maxhp=600;
                    HERO.str=20;
                    HERO.ag=10;
                    HERO.intel=50;
                    break;
            }
            HERO.maxmp=HERO.intel*5;
            HERO.hp=HERO.maxhp;
            HERO.mp=HERO.maxmp;
            break;
        }
    }
}
void ChooseDifficulty(){
    bool again=true;
    short pointer=1;
    while(again)
    {
        consoleclear();
        printf("\t\t\t�����");
        arrow(pointer,1);
        printf("\n\t\t\t������");
        arrow(pointer,2);
        printf("\n\t\t\t������");
        arrow(pointer,3);
        if(menuEnter(&pointer,3,WithoutESC)){
            switch(pointer){
                case 1:{
                    difficulty=1;
                    break;
                }
                case 2:{
                    difficulty=2;
                    break;
                }
                case 3:{
                    difficulty=3;
                    break;
                }
                default:{
                    break;
                }
            }
            again=false;
        }
    }
}
void Initialize(){
    setConsoleSize();
    srand(time(NULL));
    SetConsoleTitle("---------------------------------MEDIEVAL STORY---------------------------------");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL,"Russian");
    h=GetStdHandle(STD_OUTPUT_HANDLE);
}

/*������ � ����*/
void Menu(){
    short pointer=1;
    bool InMenu=true;
    while(InMenu){
        consoleclear();
        printf("-------------------����� ���������� � Medieval Story--------------------\n");
        printf("\t\t\t����� ����");
        arrow(pointer,1);
        printf("\n\t\t\t��������� ����");
        arrow(pointer,2);
        printf("\n\t\t\t������� ���������");
        arrow(pointer,3);
        printf("\n\t\t\t�����");
        arrow(pointer,4);
        if(menuEnter(&pointer,4,WithoutESC)){
            switch(pointer){
                case 1:{
                    HERO.location=0;
                    InGame=true;
                    ChooseName();
                    ChooseClass();
                    consoleclear();
                    printc("�� ������������ ����� ������� ���.\n",YELLOW);
                    sleep_ms(1500);
                    printf("�� ������������,������� � ����� ������ ������� � ����� �� ������ � ");
                    printc("����",CYAN);
                    sleep_ms(2500);
                    setDefaultVariables();
                    Start();
                    break;
                }
                case 2:{
                    consoleclear();
                    if(loader()){
                        Start();
                        break;
                    }
                    setXposition(h,24);
                    printc("������ ��������",RED);
                    setXposition(h,0);
                    sleep_ms(1500);
                    break;
                }
                case 3:{
                    ChooseDifficulty();
                    break;
                }
                case 4:{
                    InMenu=false;
                    break;
                }
            }
            pointer=1;
        }
    }
}
bool InGameMenu(){
    char input;
    bool again;
    short pointer=1;
    do{
        consoleclear();
        printf("\t\t\t��������� � ����");
        arrow(pointer,1);
        printf("\n\t\t\t���������");
        arrow(pointer,2);
        printf("\n\t\t\t��������� ����");
        arrow(pointer,3);
        printf("\n\t\t\t����� � ������� ����");
        arrow(pointer,4);
        input=getch();
        switch(input){
            case UP:{               //UP_ARROW
                if(pointer>1)
                    pointer--;
                break;
            }
            case DOWN:{             //DOWN_ARROW
                if(pointer<4)
                    pointer++;
                break;
            }
            case ENTER:{            //ENTER
                switch(pointer){
                    case 1:{
                        input=27;
                        break;
                    }
                    case 2:{      //���������
                        InventoryUI();
                        break;
                    }
                    case 3:{      //����������
                        consoleclear();
                        saver();
                        printc("���� ������� ���������\n\n", GREEN);
                        sleep_ms(1000);
                        return true;
                        break;
                    }
                    case 4:{      //����� � ������� ����
                        pointer=1;
                        consoleclear();
                        again=true;
                        pointer=1;
                        while(again){
                            consoleclear();
                            printf("�� �������, ��� ������ ����� � ������� ����?");
                            printf("\n��");
                            arrow(pointer,1);
                            printf("\n���");
                            arrow(pointer,2);
                            switch(getch()){
                                case UP:{               //UP_ARROW
                                    if(pointer>1)
                                        pointer--;
                                    break;
                                }
                                case DOWN:{             //DOWN_ARROW
                                    if(pointer<2)
                                        pointer++;
                                    break;
                                }
                                case ENTER:{            //ENTER
                                    again=false;
                                    switch(pointer){
                                        case 1:{
                                            InGame=false;
                                            return false;
                                        }
                                        case 2:{

                                        }
                                    }
                                }
                            }
                        }
                        pointer=1;
                    }
                }
            }
        }
    }
    while(input!=27);
    return true;
}
bool menuEnter(short *pointer,short MAX,bool HaveESC){
    if(HaveESC){
        switch(getch()){
            case UP:{                //UP_ARROW
                if(*pointer>1)
                    *pointer=*pointer-1;
                break;
            }
            case DOWN:{                //DOWN_ARROW
                if(*pointer<MAX)
                    *pointer=*pointer+1;
                break;
            }
            case ESC:{                //ESCAPE
                if(InGameMenu()==false){
                    InGame=0;
                    HERO.location=-1;
                }
                break;
            }
            case ENTER:{                //ENTER
                DrawUI(show);
                return true;
            }
        }
        return false;
    }
    else{
        switch(getch()){
            case UP:{                //UP_ARROW
                if(*pointer>1)
                    *pointer=*pointer-1;
                break;
            }
            case DOWN:{                //DOWN_ARROW
                if(*pointer<MAX)
                    *pointer=*pointer+1;
                break;
            }
            case ENTER:{                //ENTER
                return true;
            }
        }
        return false;
    }
}
void arrow(short pointer,short menu){
    if(pointer==menu){
        printc("<-",LIGHTGREEN);
    }
}
void arrowLeft(short pointer,short menu){
    if(pointer==menu){
        printc("->",LIGHTGREEN);
    }
    else
        printf("  ");
}

/*���������*/
void InventoryUI(){
    short pointer=1;
    bool inInventory=true;
    while(inInventory){
        consoleclear();
        printf("--------------------------------------------------------------------------\n");
        printf("�������:%s",HERO.armorString);
        printf("\n������:%s",HERO.weaponString);
        printf("\n�� �� ������:");
        if(LV.Poison==true)
            printf("����");
        else
            printf("���");
        printf("\n\n����� ��������:%d",HERO.hppotion);
        arrow(pointer,1);
        printf("\n����� ����:%d",HERO.mppotion);
        arrow(pointer,2);
        printf("\n--------------------------------------------------------------------------\n");
        printf("����� �� ���������");
        arrow(pointer,3);
        if(menuEnter(&pointer,3,WithoutESC)){
            consoleclear();
            switch (pointer){
                case 1:{
                    if(HERO.hp==HERO.maxhp){
                        printf("\n�������� ��� �����������!");
                        sleep_ms(2000);
                        break;
                    }
                    else{
                        printf("\n���� �������� ��������� �������������!");
                        HERO.hp=HERO.maxhp;
                        HERO.hppotion-=1;
                        sleep_ms(2000);
                        break;
                    }
                }
                case 2:{
                    if(HERO.mp==HERO.maxmp){
                        printf("\n���� ��� �����������!");
                        sleep_ms(2000);
                        break;
                    }
                    else{
                        printf("\n���� ���� ��������� �������������!");
                        HERO.mp=HERO.maxmp;
                        HERO.mppotion-=1;
                        sleep_ms(2000);
                        break;
                    }
                }
                case 3:{
                    inInventory=false;
                    break;
                }
            }
        }
    }
}

/*User-friendly*/
void printc(char text[64],int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    printf("%s",text);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
}
void consoleclear(){
    #ifdef _WIN32
    system("CLS");
    #elif __linux__
    system("clear");
    #endif // _WIN32
}
void sleep_ms(int milliseconds){                        /*cross-platform sleep function*/
    #ifdef _WIN32
    Sleep(milliseconds*2);
    #elif __linux__
    usleep(milliseconds*1000);
    #endif
}

/*��������� ������ ����������*/
void setXposition(HANDLE h,SHORT x){
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(h,&info);
    info.dwCursorPosition.X=x;
    SetConsoleCursorPosition(h,info.dwCursorPosition);
}
void printx(char text[64],short x){
    setXposition(h,x);
    printf("%s",text);
    setXposition(h,0);
}
void DrawUI(bool ShowBar){
    consoleclear();
    printf("--------------------------------------------------------------------------\n");
    setXposition(h,0);
    printf("%s",HERO.NAME);
    setXposition(h,30);
    printf("%s",HERO.CLASSNAME);
    setXposition(h,40);
    printf("%d Lvl", HERO.lvl);
    setXposition(h,60);
    printf("������   %d\n",HERO.weapon);

    setXposition(h,0);
    printf("�������� %d/%d",HERO.hp,HERO.maxhp);
    setXposition(h,30);
    printf("����      %d",HERO.str);
    setXposition(h,60);
    printf("�������  %d\n",HERO.armor);

    setXposition(h,0);
    printf("����     %d/%d",HERO.mp,HERO.maxmp);
    setXposition(h,30);
    printf("��������  %d",HERO.ag);
    setXposition(h,60);
    printf("������   %d\n",HERO.skills);

    if(!InBattle){
        setXposition(h,0);
        printf("����     %d/%d",HERO.xp,HERO.maxxp);
        setXposition(h,30);
        printf("��������� %d",HERO.intel);
        setXposition(h,60);
        printf("������   %d\n",HERO.money);
        printf("--------------------------------------------------------------------------\n");
    }
    if(ShowBar) printf("��� ����� �� ������������� ���� ������� <ESC>\n");
    setXposition(h,0);
    printf("--------------------------------------------------------------------------\n");
}
void DrawBattleUI(){
    DrawUI(notshow);
    printf(">>");
    setXposition(h,15);
    printf("%s",CurrentMob.NAME);
    setXposition(h,45);
    printf("hp %d/%d",CurrentMob.hp,CurrentMob.maxhp);
    setXposition(h,72);
    printf("<<\n");
    printf("--------------------------------------------------------------------------\n");
}
void dialUI(char name[15]){
    consoleclear();
    printx("------------------------------------------\n",15);
    printx("|\t\t\t\t\t|\n",15);
    setXposition(h,15);
    printf("|\t\t%s\t\t",name);
    setXposition(h,56);
    printf("|\n");
    setXposition(h,0);
    printx("|\t\t\t\t\t|\n",15);
    printx("------------------------------------------\n",15);
}
void DrawHeroSkills(int pointer){
    switch(HERO.CLASS){
    case 1:
        printf("������� ����");
        arrow(pointer,1);
        printf("\n����� ����� (%d mp)",HERO.lvl*3);
        arrow(pointer,2);
        printf("\n������ (%d mp)",HERO.lvl*15);
        arrow(pointer,3);
        break;
    case 2:
        printf("������� �� ����");
        arrow(pointer,1);
        printf("\n������� ������ (%d mp)",HERO.lvl*5);
        arrow(pointer,2);
        printf("\n��������� ������ (%d mp)",HERO.lvl*10);
        arrow(pointer,3);
        break;
    case 3:
        printf("����� �������");
        arrow(pointer,1);
        printf("\n���������� ��� (%d mp)",HERO.lvl*4);
        arrow(pointer,2);
        printf("\n�������� ��� (%d mp)",HERO.lvl*20);
        arrow(pointer,3);
        break;
    }
    printf("\n");
}

/*������� ��� �����*/
void StartBattle(short mobType){
    int counter2=0,counter3=0,buff=1;
    short GetXP,GetMoney;
    GenerateMob(mobType);
    InBattle=true;
    printc("\n�� ������ �������� ������, ������������� �� ���",RED);
    sleep_ms(1500);
    while(InBattle){
        if(CurrentMob.hp>0){
            if(counter2>0)
                counter2--;
            if(counter3>0)
                counter3--;
            else
                buff=1;
            DrawBattleUI();
            Attack(&counter2,&counter3,&buff);
        }
        if(CurrentMob.hp<=0){
            CurrentMob.hp=0;
            DrawBattleUI();
            printf("�� ��������!");

            GetXP=CurrentMob.attack*HERO.lvl/difficulty+rand()%30;
            HERO.xp+=GetXP;
            printf("\n�������� %d �����.",GetXP);

            GetMoney=10*CurrentMob.number*HERO.lvl/difficulty+rand()%30;
            HERO.money+=GetMoney;
            printf("\n�������� %d ������.",GetMoney);

            InBattle=false;
            sleep_ms(2000);
        }
        else{
            DrawBattleUI();
            Defence(&counter2,&counter3);
            sleep_ms(2000);
            if(HERO.hp<=0)
            {
                DrawUI(notshow);
                printf("\n--------------------------------GAME OVER--------------------------------\n");
                system("pause");
                InBattle=false;
                InGame=false;
                HERO.location=-1;
            }
        }
    }
    LV.Poison=false;
    levelUp();
}
void Attack(int *counter2,int *counter3,int *buff){
    bool DealedAnyDamage=false;
    short pointer=1;
    while(!DealedAnyDamage){
        DrawBattleUI();
        DrawHeroSkills(pointer);
        if(menuEnter(&pointer,3,WithoutESC)){
            switch(pointer){
                case 1:{
                    if(SkillOne(buff))
                        DealedAnyDamage=true;
                    break;
                }
                case 2:{
                    if(SkillTwo(counter2,buff))
                        DealedAnyDamage=true;
                    break;
                }
                case 3:{
                    if(SkillThree(counter3,buff))
                        DealedAnyDamage=true;
                    break;
                }
            }
            sleep_ms(2000);
        }
    }
}
void Defence(int *counter2,int *counter3){
    int GotDamage;
    int GivenRandomly;
    if(LV.Poison==true){
        GivenRandomly=rand()%30+HERO.skills;
        CurrentMob.hp-=GivenRandomly;
        printf("�� ��������� ���������� �������, ���� �������� %d ��������������� �����\n",GivenRandomly);
    }
    switch(HERO.CLASS){
        case 2:{
            if(*counter2!=0){
                printf("\n���� ���������, ���������� ���� ���");
            }
            if(*counter3!=0){
                GivenRandomly=(1+rand()%HERO.lvl+HERO.skills)*HERO.str;
                CurrentMob.hp-=GivenRandomly;
                printf("\n���� �������� �������������� %d ����� ��-�� ���������",GivenRandomly);
            }
            break;
        }
        case 3:{
            if(*counter2!=0){
                printf("\n��� ���������� ��� �������� ���� ����������");
            }
            if(*counter3!=0){
                GivenRandomly=(1+rand()%HERO.lvl+HERO.skills)*HERO.str;
                CurrentMob.hp-=GivenRandomly;
                printf("\n���� �������� �������������� %d ����� ��-�� �������",GivenRandomly);
            }
            break;
        }
    }
    if(rand()%100<HERO.ag&&*counter2==0&&CurrentMob.hp>0){
        printc("\n�� ������ ���������� �� ����� ����� ��������� ��������!",YELLOW);
    }
    else if(((HERO.CLASS==2||HERO.CLASS==3)&&CurrentMob.hp>0&&*counter2==0)||(HERO.CLASS==1&&CurrentMob.hp>0)){
        GotDamage=(1+rand()%3)*HERO.lvl*CurrentMob.attack/(HERO.armor+1);
        HERO.hp-=GotDamage;
        printf("\n%s �����(��) ��� %d �����",CurrentMob.NAME,GotDamage);
    }
}
bool SkillOne(int *buff){                               /*������ ����� �� ������ �� ������*/
    int GivenDamage;
    switch(HERO.CLASS){         /*C��� �� ������*/
        case 1:{
            GivenDamage=(rand()%100+HERO.str+HERO.weapon)*HERO.lvl/CurrentMob.defence*(*buff)/difficulty;
            CurrentMob.hp-=GivenDamage;
            printf("\n�� ������� %d ����� �����.",GivenDamage);
            break;
        }
        case 2:{
            GivenDamage=(rand()%50+HERO.str+HERO.weapon)*2*HERO.lvl/CurrentMob.defence/difficulty;
            CurrentMob.hp-=GivenDamage;
            printf("\n�� ������� %d ����� �����.",GivenDamage);
            if(rand()%100<HERO.ag){
                printc("\n��� ������� � �� ������ ������� ������ ���� �� �����!",YELLOW);
                GivenDamage=(rand()%100+HERO.str+HERO.weapon)*HERO.lvl*(*buff)/CurrentMob.defence/difficulty;
                CurrentMob.hp-=GivenDamage;
                printf("\n�� ������� %d ����� �����.",GivenDamage);
            }
            break;
        }
        case 3:{
            GivenDamage=(rand()%50+HERO.str+HERO.weapon)*2*HERO.lvl/CurrentMob.defence/difficulty;
            CurrentMob.hp-=GivenDamage;
            printf("\n�� ������� %d ����� �����.",GivenDamage);
            break;
        }
    }
    return true;
}
bool SkillTwo(int *counter2,int *buff){                 /*������ ����� �� ������ �� ������*/
    int GivenDamage;
    switch(HERO.CLASS){         /*C��� �� ������*/
        case 1:{
            GivenDamage=(HERO.intel+HERO.skills+rand()%100+HERO.str+HERO.weapon)*HERO.lvl*(*buff)/difficulty;
            CurrentMob.hp-=GivenDamage;
            HERO.mp-=(HERO.lvl*3);
            printf("\n�� ������� ����� %d �����!",GivenDamage);
            break;
        }
        case 2:{
            if(*counter2>0){
                printf("\n���������� ��� ���������");
                return false;
            }
            else if(HERO.mp-(HERO.lvl*5)<0){
                printf("\n�� ������� ����.");
                return false;
            }
            *counter2=2+rand()%HERO.lvl;
            GivenDamage=(HERO.intel+HERO.skills+rand()%100+3*HERO.str+HERO.weapon)*HERO.lvl/CurrentMob.defence/difficulty;
            CurrentMob.hp-=GivenDamage;
            HERO.mp-=(HERO.lvl*5);
            printf("\n���� ��������� �� %d ����",*counter2);
            printf("\n�� ������� ����� %d �����!",GivenDamage);
            break;
        }
        case 3:{
            if(*counter2>0){
                printf("\n���������� ��� ���������");
                return false;
            }
            else if(HERO.mp-(HERO.lvl*5)<0){
                printf("\n�� ������� ����.");
                return false;
            }
            *counter2=2+rand()%HERO.lvl;
            HERO.mp-=(HERO.lvl*5);
            printf("�� �������� �� ���� ���������� ����������� ����!");
            printf("\n��� ����� ����������� � ������� %d ����� �����.",*counter2);
            break;
        }
    }
    return true;
}
bool SkillThree(int *counter3,int *buff){               /*������ ����� �� ������ �� ������*/
    short GivenDamage;
    switch(HERO.CLASS){                                 /*C��� �� ������*/
        case 1:{
            if(*counter3>0){
                printf("\n���������� ��� ���������.");
                return false;
                break;
            }
            else if(HERO.mp-(HERO.lvl*15)<0)
            {
                printf("\n�� ������� ����.");
                return false;
                break;
            }
            *buff=2;
            *counter3=3;
            printf("\n��� ���� �������� �� 2 ��������� �����!");
            HERO.mp-=(HERO.lvl*15);
            break;
        }
        case 2:{
            if(*counter3>0){
                printf("\n���������� ��� ���������.");
                return false;
                break;
            }
            else if(HERO.mp-(HERO.lvl*10)<0)
            {
                printf("\n�� ������� ����.");
                return false;
                break;
            }
            *counter3=2+rand()%HERO.lvl;
            GivenDamage=(HERO.intel+HERO.skills+rand()%50+3*HERO.str+HERO.weapon)*HERO.lvl/CurrentMob.defence/difficulty;
            CurrentMob.hp-=GivenDamage;
            HERO.mp-=(HERO.lvl*10);
            printf("\n�� �������� ����� � ������� ��� %d �����!",GivenDamage);
            printf("\n��������� %d ����� ���� ����� �������� �������������� ����!",*counter3);
            break;
        }
        case 3:{
            if(*counter3>0){
                printf("\n���������� ��� ���������.");
                return false;
                break;
            }
            else if(HERO.mp-(HERO.lvl*20)<0)
            {
                printf("\n�� ������� ����.");
                return false;
                break;
            }
            *counter3=2+rand()%HERO.lvl;
            GivenDamage=(HERO.intel+HERO.skills+rand()%50+3*HERO.str+HERO.weapon)*HERO.lvl/CurrentMob.defence/difficulty;
            CurrentMob.hp-=GivenDamage;
            HERO.mp-=(HERO.lvl*20);
            printf("\n�� �������� ����� � ������� ��� %d �����!",GivenDamage);
            printf("\n��������� %d ����� ���� ����� �������� �������������� ����!",*counter3);
            break;
        }
    }
    return true;
}
void GenerateMob(short mobType){                        /*��������� �����*/
    if(mobType==4) CurrentMob.number=4+(rand()%AmountOfMobs);
    else CurrentMob.number=mobType;
    switch(CurrentMob.number)
    {
        case 1:{
            strcpy(CurrentMob.NAME,"����");        //����
            CurrentMob.attack=30;
            CurrentMob.agility=20;
            CurrentMob.defence=20;
            CurrentMob.maxhp=800+(rand()%HERO.str)/3;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
        case 2:{
            strcpy(CurrentMob.NAME,"�������");        //�������
            CurrentMob.attack=10;
            CurrentMob.agility=20;
            CurrentMob.defence=3;
            CurrentMob.maxhp=250+(rand()%HERO.str)/3;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
        case 3:{
            strcpy(CurrentMob.NAME,"�����");        //�����
            CurrentMob.attack=60;
            CurrentMob.agility=30;
            CurrentMob.defence=50;
            CurrentMob.maxhp=1500;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
        case 4:{
            strcpy(CurrentMob.NAME,"�����");        //�����
            CurrentMob.attack=8;
            CurrentMob.agility=8;
            CurrentMob.defence=8;
            CurrentMob.maxhp=300+(rand()%HERO.str)/3;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
        case 5:{
            strcpy(CurrentMob.NAME,"������");     //������
            CurrentMob.attack=12;
            CurrentMob.agility=20;
            CurrentMob.defence=4;
            CurrentMob.maxhp=200+(rand()%HERO.str)/3;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
        case 6:{
            strcpy(CurrentMob.NAME,"������");       //������
            CurrentMob.attack=20;
            CurrentMob.agility=2;
            CurrentMob.defence=10;
            CurrentMob.maxhp=500+(rand()%HERO.str)/3;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
    }
}

/*������� �������*/
void levelUp(){
    if(HERO.xp>=HERO.maxxp){
        HERO.lvl++;
        HERO.xp-=HERO.maxxp;
        HERO.maxxp*=HERO.lvl;
        sleep_ms(1500);
        bool again=true;
        short pointer=1;
        while(again){
            consoleclear();
            printc("��� ������� ��������!",YELLOW);
            printf("\n��������, ����� �������������� ��������(+1):");
            printf("\n����");
            arrow(pointer,1);
            printf("\n��������");
            arrow(pointer,2);
            printf("\n���������");
            arrow(pointer,3);
            if(menuEnter(&pointer,3,WithoutESC)){
                again=false;
                switch(pointer){
                    case 1:{
                        printf("\n���� ���� ���������.");
                        HERO.str+=1;
                        break;
                    }
                    case 2:{
                        printf("\n���� �������� ���������.");
                        HERO.str+=1;
                        break;
                    }
                    case 3:{
                        printf("\n��� ��������� ��������.");
                        HERO.intel+=1;
                        break;
                    }
                }
            }
        }
    }
}
void buyItem(char item[64],short cost,short upgrade){
    DrawUI(show);
    if(HERO.money-cost>=0){
        if(HERO.location==13) printf("\n�� ������� �������� ");
        else printf("\n�� ������� ��������� ");
        printc(item,YELLOW);
        HERO.money-=cost;
        switch (HERO.location){
            case 2:{
                if(strcmp("����� ��������",item)==0){
                    HERO.hppotion+=1;
                    printf("\n������ �� %d",HERO.hppotion);
                }
                else if(strcmp("����� ����",item)==0){
                    HERO.mppotion+=1;
                    printf("\n������ �� %d",HERO.mppotion);
                }
                break;
            }
            case 6:{
                HERO.armor=upgrade;
                printf("\n������� <- %d",upgrade);
                strcpy(HERO.armorString,item);
                break;
            }
            case 7:{
                HERO.weapon=upgrade;
                printf("\n������ <- %d",upgrade);
                strcpy(HERO.weaponString,item);
                break;
            }
            case 13:{
                if(strcmp(item,"����")==0)             HERO.str+=upgrade;
                else if(strcmp(item,"��������")==0)    HERO.ag+=upgrade;
                else if(strcmp(item,"��������")==0)    HERO.intel+=upgrade;
                break;
            }
        }
    }
    else{
        printf("\n� ��� ������������ �����!");
    }
    sleep_ms(2000);
}
