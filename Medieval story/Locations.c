#include "headers.h"
#include "defines.h"
#include "ExternalFunctions.h"
#include "Locations.h"
extern struct Player{
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
extern struct CM{
    char NAME[20];
    int number,attack,agility,defence,maxhp,hp;
}CM;
extern struct LocationVars{
    bool Poison;
    bool civquest,kingquest,qcompleted,Map;
    bool room1,room2;
    bool CanGetBook,CanGetApple,CanEnterCastle;
    bool skelet, lighter;
    bool key,box;
}LocationVars;
extern struct Player HERO;
extern struct CM CurrentMob;
extern struct LocationVars LV;
extern bool InGame;
extern bool InBattle;
extern short difficulty;
extern HANDLE h;

/*������� ���� �� ��������*/
void Start(){
    InGame=true;
    while(InGame){
        switch(HERO.location){
            case 0:{
                StartLocation();
                break;
            }
            case 1:{
                Plains();
                break;
            }
            case 2:{
                Caravan();
                break;
            }
            case 3:{
                CastleEnter();
                break;
            }
            case 4:{
                Castle();
                break;
            }
            case 5:{
                Trader();
                break;
            }
            case 6:{
                TraderArmor();
                break;
            }
            case 7:{
                TraderWeapons();
                break;
            }
            case 8:{
                Village();
                break;
            }
            case 9:{
                civdial();
                break;
            }
            case 10:{
                HauntedH();
                break;
            }
            case 11:{
                Basement();
                break;
            }
			case 12:{
				kingdial();
				break;
			}
			case 13:{
                Arena();
                break;
			}
			case 14:{
                Cave();
                break;
			}
			case 15:{
                Dungeon();
                break;
			}
        }
    }
}

/*�������*/
void StartLocation(){               /*��������� �������             (0)*/
    short pointer=1;
    while(HERO.location==0)
    {
        DrawUI(show);
        printc("\t\t\t����\n",CYAN);
        if(LV.CanGetBook==true){
            printf("����������� � ���������,����������� ����� �� ����� �������");
        }
        else{
            printf("������ � ");
            printc("��������",CYAN);
        }
        arrow(pointer,1);

        if(LV.CanGetApple==true){
            printf("\n���������� �������� �����, � ��������� ����");
        }
        else{
            printf("\n������ � ������� ");
            printc("������������ �����",CYAN);
        }
        arrow(pointer,2);

        printf("\n���� ������� � ������ �����");
        arrow(pointer,3);

        if(menuEnter(&pointer,3,WithESC)){
            switch(pointer){
                case 1:{
                    if(LV.CanGetBook==true){
                        printf("�������� ������������ ���, ����� ��������� ����� � ���������� ������ � ");
                        printc("��������",CYAN);
                        printf(".\n");
                        printc("����� ��������� � ���������. (+5 � �������)\n", YELLOW);
                        HERO.skills+=5;
                        LV.CanGetBook=false;
                        sleep_ms(3000);
                        printf("�� ������ ����� ����� ������ � ������� ���������� ��������");
                        sleep_ms(2000);
                    }
                    /*������� �� �������*/
                    HERO.location=1;
                    break;
                }
                case 2:{
                    if(LV.CanGetApple==true){
                        printf("�� ��������� ���� ������ � ������� ������� ������� ������ �����,������� �� ������\n");
                        printc("���� ������������ � ������� �������� ��������� �� 10.\n", YELLOW);
                        HERO.maxhp+=10;
                        HERO.hp+=10;
                        LV.CanGetApple=false;
                        sleep_ms(3000);
                        printf("\n������ ����� ������ �� ������ ����� ");
                        printc("������������ �����",CYAN);
                        printf(" � ����� � ���� ����� ����� �����");
                        sleep_ms(2000);
                    }
                    /*������� � ������������ �����*/
                    HERO.prevlocation=HERO.location;
                    HERO.location=3;
                    break;
                }
                case 3:{
                    StartBattle(mob);
                    HERO.location=14;
                    break;
                }
            }
            pointer=1;
            break;
        }
    }
}
void Plains(){                      /*�������                       (1)*/
    short pointer=1;
    while(HERO.location==1){
        DrawUI(show);
        printc("\t\t\t�������",CYAN);

        printf("\n����������� � ��������� ��������, ��������������� ���������.");
        arrow(pointer,1);
        printf("\n������������ � ������� ����������� ��������.");
        arrow(pointer,2);
        printf("\n������ �� ������ � ");
        printc("�������",CYAN);
        arrow(pointer,3);
        printf("\n������ � ");
        printc("����",CYAN);
        arrow(pointer,4);

        if(menuEnter(&pointer,4,WithESC)){
            switch(pointer){
                case 1:{
                    printf("\n������ �������� ��������������� ������� �� ���");
                    HERO.location=2;
                    break;
                }
                case 2:{
                    if(LV.civquest){
                        printf("\n� ���� ���� ���������� ���� ������� ������ � ����� ���\n");
                        sleep_ms(1000);
                        printf("�� ������� � �������, ��������� ����������.\n");
                        sleep_ms(1000);
                        printc("����� �� ���� ������������� ����",RED);
                        HERO.location=10;
                    }
                    else if(LV.qcompleted){
                        printc("\n�� ��� ���� �����.",YELLOW);
                    }
                    else{
                        printf("\n�� ����������� ���, �� ����� �� ���������.");
                    }
                    break;
                }
                case 3:{
                    printf("\n�� ������ ������ � ����� �� ���.");
                    sleep_ms(1000);
                    printf("\n��� �������� ��� � ");
                    printc("�������",CYAN);
                    HERO.location=8;
                    break;
                }
                case 4:{
                    printf("\n�� ���������������� � ������� � ");
                    printc("����",CYAN);
                    HERO.location=0;
                    break;
                }
            }
            sleep_ms(2000);
        }
    }
}
void Caravan(){                     /*�������� � ���������          (2)*/
    short pointer=1;
    while(HERO.location==2){
        DrawUI(show);
        printc("\t\t\t�������\n",CYAN);

        printf("������ �������,������� �� ������ ����������:");
        printf("\n����� �������� - 40 �������");
        arrow(pointer,1);
        printf("\n����� ���� - 30 �������");
        arrow(pointer,2);
        printf("\n��(��������� ������ ����� � ������� ���������� ���) - 20 �������");
        arrow(pointer,3);
        printf("\n�������� ");
        printc("�������",CYAN);
        arrow(pointer,4);

        if(menuEnter(&pointer,4,WithESC)){
            switch(pointer){
                case 1:{
                    buyItem("����� ��������",40,1);
                    break;
                }
                case 2:{
                    buyItem("����� ����",30,1);
                    break;
                }
                case 3:{
                    if(LV.Poison==true)
                        printf("\n�� ��� ������� �� ���� ������.");
                    else if(HERO.money-20>=0){
                        printc("\n�� ������� �� ���� ������.",YELLOW);
                        HERO.money-=20;
                        LV.Poison=true;
                    }
                    else{
                        printf("\n� ��� ������������ �����!");
                    }
                    sleep_ms(2000);
                    break;
                }
                case 4:{
                    HERO.location=1;
                    break;
                }
            }
            pointer=1;
        }
    }
}
void CastleEnter(){                 /*���� � ����������� �����      (3)*/
    short pointer=1;
    if(LV.CanEnterCastle) HERO.location=4;
    else{
        while(HERO.location==3){
            DrawUI(show);
            printc("\t\t���� � ����������� �����\n",CYAN);

            printf("���� �� ���������� ������� ���: \"��� ������� ��� ����� ��������� ����� � 10 �������.\"\n");
            printf("\n������� � ����������,�������������� ���� � �����");
            arrow(pointer,1);
            printf("\n��������� �����");
            arrow(pointer,2);

            if(menuEnter(&pointer,2,WithESC)){
                switch(pointer){
                    case 1:{
                        DrawUI(show);
                        if(HERO.money-10>=0)
                        {
                            LV.CanEnterCastle=true;
                            printf("�� ����������� ����� � �������� �������� ��� ������.");
                            HERO.location=4;
                            HERO.money-=10;
                        }
                        else{
                            printf("� ��� ������������ �����, ������������� �����.");
                        }
                        sleep_ms(2000);
                        break;
                    }
                    case 2:{
                        HERO.location=HERO.prevlocation;
                        break;
                    }
                }
                pointer=1;
            }
        }
    }
}
void Castle(){                      /*����������� �����             (4)*/
    short pointer=1;
    while(HERO.location==4){
        DrawUI(show);
        printc("\t\t����������� �����",CYAN);

        printf("\n������ � ����� ������");
        arrow(pointer,1);
        printf("\n����������� � ����� ��������");
        arrow(pointer,2);
        printf("\n������������� �� �����");
        arrow(pointer,3);
        printf("\n������ � �������� �������");
        arrow(pointer,4);
        printf("\n��������� ����� � ");
        printc("����",CYAN);
        arrow(pointer,5);

        if(menuEnter(&pointer,5,WithESC)){
            switch(pointer){
                case 1:{
                    printf("�� ��������� � �����,�� ������� ����� ������");
                    HERO.location=12;
                    sleep_ms(2000);
                    break;
                }
                case 2:{
                    printf("�������� �������� � �������� � ���������� ��� ���� �����");
                    HERO.location=5;
                    sleep_ms(2000);
                    break;
                }
                case 3:{
                    HERO.location=13;
                    break;
                }
                case 4:{
                    printf("�� ��������� ����� ������ � ����� �� ������.");
                    sleep_ms(1000);
                    printf("\n�� ������ ��������� ������� � ������������� � ��.");
                    HERO.location=8;
                    sleep_ms(2000);
                    break;
                }
                case 5:{
                    printf("�� ���������������� � �������� �� ����� �����");
                    HERO.location=0;
                    sleep_ms(2000);
                    break;
                }
            }
        }
    }
}
void Trader(){                      /*�������� � �����              (5)*/
    short pointer=1;
    while(HERO.location==5){
        DrawUI(show);
        printc("\t\t����� ��������",CYAN);

        printf("\n�������");
        arrow(pointer,1);
        printf("\n������");
        arrow(pointer,2);
        printf("\n��������� �����");
        arrow(pointer,3);

        if(menuEnter(&pointer,3,WithESC)){
            switch(pointer){
                case 1:{
                    HERO.location=6;
                    break;
                }
                case 2:{
                    HERO.location=7;
                    break;
                }
                case 3:{
                    DrawUI(show);
                    printf("�� �������� �� ��������.");
                    HERO.location=4;
                    sleep_ms(2000);
                    break;
                }
            }
        }
    }
}
void TraderArmor(){                 /*������� �����                 (6)*/
    short pointer=1;
    while(HERO.location==6){
        DrawUI(show);
        printf("\t\t������ ��������:");

        printf("\n���������� ������� - 300 �������");
        arrow(pointer,1);
        printf("\n������������ ������ - 400 �������");
        arrow(pointer,2);
        printf("\n������ ������� - 500 �������");
        arrow(pointer,3);
        printf("\n��������� �����");
        arrow(pointer,4);

        if(menuEnter(&pointer,4,WithESC)){
            switch(pointer){
                case 1:{
                    buyItem("���������� �������",300,15);
                    break;
                }
                case 2:{
                    buyItem("������������ ������",400,30);
                    break;
                }
                case 3:{
                    buyItem("������ �������",500,50);
                    break;
                }
                case 4:{
                    HERO.location=5;
                    return;
                    break;
                }
            }
            sleep_ms(1000);
            pointer=1;
        }
    }
}
void TraderWeapons(){               /*������� ������                (7)*/
    short pointer=1;
    while(HERO.location==7){
        DrawUI(show);
        printf("\t\t������ ������:");
        if(HERO.CLASS==1){
            printf("\n���������� ������� ��� - 500 �������");
            arrow(pointer,1);
            printf("\n��������� ������� ��� - 800 �������");
            arrow(pointer,2);
            printf("\n������ ������� - 950 �������");
            arrow(pointer,3);
            printf("\n��������� �����");
            arrow(pointer,4);
        }
        else if(HERO.CLASS==2){
            printf("\n������� ��� - 450 �������");
            arrow(pointer,1);
            printf("\n�������� ��� - 600 �������");
            arrow(pointer,2);
            printf("\n������� - 900 �������");
            arrow(pointer,3);
            printf("\n��������� �����");
            arrow(pointer,4);
        }
        else if(HERO.CLASS==3){
            printf("\n���������� ����� - 350 �������");
            arrow(pointer,1);
            printf("\n���������� ����� � ��� - 600 �������");
            arrow(pointer,2);
            printf("\n������� - 5000 �������");
            arrow(pointer,3);
            printf("\n��������� �����");
            arrow(pointer,4);
        }
        if(menuEnter(&pointer,5,WithESC)){
            switch(pointer){
                case 1:{
                    if(HERO.CLASS==1)       buyItem("���������� ������� ���",500,25);
                    else if(HERO.CLASS==2)  buyItem("������� ���",450,25);
                    else if(HERO.CLASS==3)  buyItem("���������� �����",350,30);
                    break;
                }
                case 2:{
                    if(HERO.CLASS==1)       buyItem("��������� ������� ���",800,40);
                    else if(HERO.CLASS==2)  buyItem("�������� ���",600,45);
                    else if(HERO.CLASS==3)  buyItem("���������� ����� � ���",600,50);
                    break;
                }
                case 3:{
                    if(HERO.CLASS==1)       buyItem("������ �������",950,120);
                    else if(HERO.CLASS==2)  buyItem("�������",900,110);
                    else if(HERO.CLASS==3)  buyItem("�������",2000,200);
                    break;
                }
                case 4:{
                    HERO.location=5;
                    return;
                    break;
                }
            }
            sleep_ms(2000);
            pointer=1;
        }
    }
}
void Village(){                     /*�������                       (8)*/
    short pointer=1;
    while(HERO.location==8){
        DrawUI(show);
        printc("\t\t�������",CYAN);
        printf("\n����������� � ����� �������");
        arrow(pointer,1);
        printf("\n������ �� ������ �� �������");
        arrow(pointer,2);
        printf("\n���� � ");
        printc("����������� �����",CYAN);
        arrow(pointer,3);

        if(menuEnter(&pointer,3,WithESC)){
            switch(pointer){
                case 1:{
                    printf("\n�� ��������� ������ �������.");
                    sleep_ms(2000);
                    if(LV.civquest==false){
                        printf("\n�������� �������� � ��� � �������� ��������.");
                    }
                    sleep_ms(2000);
                    HERO.location=9;
                    return;
                    break;
                }
                case 2:{
                    printf("\n�� ����� �� ������ �� �������.");
                    HERO.location=1;
                    break;
                }
                case 3:{
                    printf("\n�� ���������������� � ������� � ����������� �����.");
                    HERO.prevlocation=HERO.location;
                    HERO.location=3;
                    break;
                }
            }
            sleep_ms(2000);
        }
    }
}
void civdial(){                     /*������ �� ���������           (9)*/
    short pointer=1;
    while(HERO.location==9){
        if(LV.civquest==false&&!LV.qcompleted){
            dialUI("��������");
            printf("-����������, ������. � ����� ���� � ������. �� ���� ������� ��� � ������� ������ ����������� ���������, � �������� ������ ����. � ��� ��� ����� ������ �� ��������, ���� ����� � ����. �������� ��� ����� ��� ��������� ����?");
            printf("\n(������,� ����� ��������� ���� �������.)");
            arrow(pointer,1);
            printf("\n(������ � �� ����� ������ ���.)");
            arrow(pointer,2);
            if(menuEnter(&pointer,2,WithoutESC)){
                DrawUI(show);
                switch(pointer){
                    case 1:{

                        printf("\n-�������, ���� �� ������� ������,��� �����,��� ���-�� ������ � ������� �� �������� �����. ����� ����.");
                        LV.civquest=true;
                        sleep_ms(3500);
                        break;
                    }
                    case 2:{
                        printf("\n-��, ��� �, ������ �� ��������.");
                        sleep_ms(2000);
                        break;
                    }
                }
                HERO.location=8;
            }
        }
        else if(LV.Map&&LV.qcompleted&&LV.civquest){
            system("CLS");
            printf("�� ��������� � �������� ������� � ������ �����.");
            sleep_ms(1000);
            printf("\n-�������,�����! �� ���� ���� �������. ��� ���� �������, � ��� ������ �� ������.");
            HERO.money+=1000;
            sleep_ms(3000);
            printf("\n� ������ ������ ������ ��� ����������� � ����.");
            printf("\n�� ����� �� ������������� � �����.");
            sleep_ms(3000);
            HERO.location=4;
            LV.CanEnterCastle=true;
            LV.civquest=false;
        }
        else if(!LV.qcompleted&&LV.civquest){
            system("CLS");
            printc("\t\t\t�� ��� ����� ������� ��������!",LIGHTBLUE);
            sleep_ms(2000);
            HERO.location=8;
        }
        else if(LV.qcompleted&&!LV.civquest){
            system("CLS");
            printc("\t\t\t�� ��� ��������� ������� ��������!",LIGHTBLUE);
            sleep_ms(2000);
            HERO.location=8;
        }
    }
}
void HauntedH(){                    /*������� � ����������          (10)*/
    short pointer=1;
    while(HERO.location==10){
        DrawUI(show);
        printc("\t\t��������� �������",CYAN);

        printf("\n������ � ����� �������");
        arrow(pointer,1);
        printf("\n��������� �� ������ ����");
        arrow(pointer,2);
        printf("\n������ ������ � ������� �����");
        arrow(pointer,3);

        if(menuEnter(&pointer,3,WithESC)){
            switch(pointer){
                case 1:{
                    if(LV.room1==false){
                        printf("\n�� ������� � ����� �������. �� ���������������� �������� ������ ������ ���.");
                        sleep_ms(2000);
                        StartBattle(ghost);
                        LV.room1=true;
                    }
                    else{
                        printf("\n�� ��� ���� �����");
                        sleep_ms(2000);
                    }
                    break;
                }
                case 2:{
                    if(LV.room2==false){
                        printf("\n�� ������������ �� ������ ���� � ��������� � ������� � ��������.\n");
                        sleep_ms(2000);
                        printf("\n�� ���� �� ������������� ��������� ������� ����� � ������� �� ���� � ������.");
                        sleep_ms(1500);
                        printc("(+5 � ��������.)",YELLOW);
                        HERO.ag+=5;
                        sleep_ms(2000);
                        LV.room2=true;
                    }
                    else{
                       printf("\n�� ��� ���� �����");
                       sleep_ms(2000);
                    }
                    break;
                }
                case 3:{
                    if(LV.room1&&LV.room2){
                        printf("\n�� ���������� �����.");
                        HERO.location=11;
                    }
                    else{
                        printf("\n�� �������� �� �����,�� ����� �� ���������.");
                    }
                    sleep_ms(2000);
                    break;
                }
            }
            pointer=1;
        }
    }
}
void Basement(){                    /*������ �������� � ������������(11)*/
    short pointer=1;
    LV.room1=false;
    LV.room2=false;
    while(HERO.location==11){
        DrawUI(show);
        printc("\t\t������",CYAN);

        printf("\n������ �����");
        arrow(pointer,1);
        printf("\n�������� ���������� �����");
        arrow(pointer,2);
        printf("\n��������� ����� ����� �� ���");
        arrow(pointer,3);

        if(menuEnter(&pointer,3,WithESC)){
            switch(pointer){
                case 1:{
                    if(LV.room1==false){
                        printf("\n�� ��������� ����� ���� � ������������ � ������ �������. ��������, � ������� ���������� ���� �������.");
                        StartBattle(ghost);
                        LV.room1=true;
                    }
                    else{
                        printf("�� ��� ���� �����!");
                        sleep_ms(2000);
                    }
                    break;
                }
                case 2:{
                 if(LV.room2==false){
                    printf("\n�� ������� � ������ � �������� ��� ��������� ��������.");
                    sleep_ms(1500);
                    printc("(+10 � ����������.)",YELLOW);
                    HERO.intel+=10;
                    sleep_ms(2000);
                    LV.room2=true;
                 }
                 else{
                    printf("�� ��� ���� �����!");
                    sleep_ms(2000);}
                break;
                }
                case 3:{
                    if(LV.room1&&LV.room2){
                        printf("\n�� ���������, ��� ����� ���������. �� ����� �� ����� � ��� ��������, �� ��� - ���� � ������ �������. �� ������� � ������ ��� ��������,������� ��� ���� ��� �������� ���� ������.\n");
                        sleep_ms(2000);
                        StartBattle(boss);
                        LV.qcompleted=true;
                        sleep_ms(2000);
                        DrawUI(show);
                        printf("�� �������� �����, �� ������� ���������� ��������� �����.");
                        LV.Map=true;
                        sleep_ms(2000);
                        HERO.location=8;
                        break;
                    }
                    else{
                        printf("\n�� �� ���������� ������ �����������");
                        sleep_ms(2000);
                        break;
                    }
                }
            }
        }
    }
}
void kingdial(){				    /*������ � �������              (12)*/
    short pointer=1;
    while(HERO.location==12){
        if(LV.qcompleted){
            dialUI("������");
            printf("-����������, �����. ��������� ����, ���� ����������� ������ �������� �� ������, ������������������ ��� ��� ����� ������ �����.");
            printf("\n� ���� ���� � ���� ���� �����������. ������� �� ������ �� ���, � � ���� ����� ��, ��� ��������. ��, ��� �������?");
            printf("\n\n(��������� �� ������ � ������)");
            arrow(pointer,1);
            printf("\n(���������� �� �����������)");
            arrow(pointer,2);
            if(menuEnter(&pointer,2,WithoutESC)){
                switch(pointer){
                    case 1:{
                        consoleclear();
                        printf("�� ���������� �� ������ � ������.");
                        sleep_ms(1500);
                        printf("\n�� ������ � ������ ����������� ���������� ������ � ����� ���������� � ������ ����������.\n");
                        sleep_ms(2000);
                        break;
                    }
                    case 2:{
                        consoleclear();
                        printf("�� ������������� �� ������ � ������, ����������� ���� ������� ���� ������������ ����.");
                        sleep_ms(1500);
                        printf("\n�� ���������� �� �����, ��������� � ������ ����������� � ������ ����� ����.\n");
                        sleep_ms(2000);
                        break;
                    }
                }
                printx("(�����)\n",24);
                system("pause");
                HERO.location=-1;
                InGame=false;
            }
        }
        else{
            dialUI("������");
            printf("\n-� ���� ��� ������ ��� ����");
            sleep_ms(1500);
            HERO.location=4;
        }
    }
}
void Arena(){                       /*�����                         (13)*/
    short pointer=2;
    while(HERO.location==13){
        DrawUI(show);
        printc("\t\t�����",CYAN);
        printf("\n����� �� �����");
        arrow(pointer,1);
        printf("\n\n�������� �������������� ��� ����������(50 �������,+5 � ��������������):");
        printf("\n����");
        arrow(pointer,2);
        printf("\n��������");
        arrow(pointer,3);
        printf("\n��������");
        arrow(pointer,4);

        if(menuEnter(&pointer,4,WithoutESC)){
            switch(pointer){
                case 1:{
                    HERO.location=4;
                    break;
                }
                case 2:{
                    buyItem("����",50,5);
                    break;
                }
                case 3:{
                    buyItem("��������",50,5);
                    break;
                }
                case 4:{
                    buyItem("��������",50,5);
                    break;
                }
            }
        }
    }
}
void Cave(){                        /*������                        (14) */
    short pointer=1;
    while(HERO.location==14){
        DrawUI(show);
        printc("\t������",CYAN);
        printf("\n����� �����");
        arrow(pointer,1);
        printf("\n��������� ������, ������� � �����");
        arrow(pointer,2);
        printf("\n������ � ");
        printc("����������",CYAN);
        arrow(pointer,3);
        printf("\n��������� � ");
        printc("����", CYAN);
        arrow(pointer,4);

        if(menuEnter(&pointer,4,WithESC)){
            switch(pointer){
                case 1:{
                    if (!LV.lighter){
                        printf("\n�� ������ ����� � ������ ���� ������ ������.");
                        LV.lighter=true;
                    }
                    else {
                        printf("\n�� ��� ����� �����.");
                    }
                    sleep_ms(1500);
                    break;
                }
                case 2:{
                    if (!LV.skelet){
                        printf("\n�� ������������ ������ � �������� ������ � �������");
                        HERO.money+=10;
                        LV.skelet=true;
                    }
                    else {
                        printf("\n�� ��� ��������� ������.");
                    }
                    sleep_ms(1500);
                    break;
                }
                case 3:{
                    if (LV.lighter){
                        printf("\n����� ����� ����� �����, �� ����� ������ ������.");
                        HERO.location=15;
                    }
                    else {
                        printf("\n�� �� ��������� ���� ������.");
                    }
                    sleep_ms(1500);
                    break;
                }
                case 4:{
                    HERO.location=0;
                    break;
                }
            }
        }
    }
}
void Dungeon(){                     /*����������                    (15) */
    short pointer=1;
    bool again;
    while(HERO.location==15){
        DrawUI(show);
        printc("\t����������",CYAN);
        printf("\n��������� ������� ����");
        arrow(pointer,1);
        printf("\n������� �����");
        arrow(pointer,2);
        printf("\n������ � �����");
        arrow(pointer,3);
        printf("\n���� ������");
        arrow(pointer,4);
        printf("\n��������� � ������ ");
        printc("������",CYAN);
        arrow(pointer,5);

        if(menuEnter(&pointer,5,WithESC)){
            switch(pointer){
                case 1:{
                    if(LV.key==false){
                        printf("\n�� ������������ ���� � �������� � ��� ����");
                        LV.key=true;
                    }
                    else printf("\n�� ��� ��������� ����");
                    sleep_ms(2000);
                    break;
                }
                case 2:{
                    if(LV.box==false){
                        printf("\n�� ������� � ������ � �������� ��������� �����");
                        HERO.money+=rand()%10+2;
                        LV.box=true;
                    }
                    else printf("\n�� ��� ��������� �����");
                    sleep_ms(2000);
                    break;
                }
                case 3:{
                    if(LV.key&&LV.box){
                        printf("\n�� ���������� ����� ������ � ������� � ���������� ���");
                        StartBattle(diablo);
                        if(HERO.location==-1) break;
                        system("CLS");
                        printf("������ ������, �� ��������� � ������� � ���������� ���. �� ����� ���� ���������� ��������� ������.");
                        sleep_ms(2000);
                        HERO.money+=1000;
                        HERO.location=0;
                    }
                    else printf("\n�� ��������� ������� ����� �� ��� �� ���������");
                    sleep_ms(2000);
                    break;
                }
                case 4:{
                    again=true;
                    printf("\n�� ������������ � �������� ������.");
                    StartBattle(mob);
                    pointer=1;
                    while(again){
                        DrawUI(show);
                        printf("�� ������� ���������� �������� ����������?");
                        printf("\n��");
                        arrow(pointer,1);
                        printf("\n���");
                        arrow(pointer,2);
                        if(menuEnter(&pointer,2,WithESC)){
                            switch (pointer){
                                case 1:{
                                    printf("\n�� ������������ � �������� ������.");
                                    StartBattle(mob);
                                    break;
                                }
                                case 2:{
                                    again=false;
                                    break;
                                }
                            }
                        }
                    }
                    pointer=1;
                    break;
                }
                case 5:{
                    HERO.location=14;
                    break;
                }
            }
        }
    }
}
