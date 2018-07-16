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

/*Сохранение и загрузка*/
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
        printx("Слот пуст\n",24);
        return false;
    }
    fseek(current,0,SEEK_SET);
    fread(&HERO,sizeof(Player),1,current);
    fseek(current,sizeof(Player),SEEK_SET);
    fread(&LV,sizeof(LV),1,current);
    fclose(current);
    return true;
}

/*Инициализация игры*/
void setConsoleSize(){                                  /*Код из интернета для изменения окна http://www.cyberforum.ru/cpp-beginners/thread520249.html*/
    HWND hwnd;
    char Title[1024];
    GetConsoleTitle(Title, 1024); // Узнаем имя окна
    hwnd=FindWindow(NULL, Title); // Узнаем hwnd окна
    MoveWindow(hwnd,640,360,630,500,TRUE);//xnew,ynew,wnew,hnew -новые положение x,y, ширина и высота
}
void setDefaultVariables(){
    strcpy(HERO.weaponString,"Пусто");
    strcpy(HERO.armorString,"Пусто");
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
    LV.Poison=false;                                                                    /*Яд на оружии*/
    LV.civquest=false, LV.kingquest=false,LV.qcompleted=false, LV.Map=false;            /*Квест*/
    LV.room1=false, LV.room2=false;                                                     /*Особняк*/
    LV.CanGetBook=true, LV.CanGetApple=true, LV.CanEnterCastle=false;                   /*Странник, сад и вход в замок*/
    LV.lighter=false, LV.skelet=false;                                                  /*Пещера*/
    LV.key=false, LV.box=false;                                                         /*Подземелье*/
}
void ChooseName(){
    consoleclear();
    printf("Введите имя вашего персонажа:\n");
    scanf("%s",HERO.NAME);
}
void ChooseClass(){
    short pointer=1;
    bool again=true;
    consoleclear();
    while(again){
        consoleclear();
        printf("-------------------------------ВЫБОР КЛАССА-------------------------------\n");
        arrowLeft(pointer,1);
        printf("Воин - ближний бой  \n-Главный аттрибут - Сила     \n-Высокий показатель здоровья\n-Средний показатель урона                                      \n-Низкая скорость атаки                                            \n-При ударе существует определенный шанс оглушить противника\n-Шанс уклонения от атак противника низок\n");

        arrowLeft(pointer,2);
        printf("Лучник - дальний бой\n-Главный аттрибут - Ловкость \n-Средний показатель здоровья\n-Наносит небольшой урон,но имеет шанс атаковать дважды за раз  \n-Высокий шанс уклониться от атак противника                       \n");

        arrowLeft(pointer,3);
        printf("Маг - дальний бой   \n-Главный аттрибут - Интеллект\n-Низкий показатель здоровья \n-Наносит высокий урон с помощью заклинаний                     \n-Есть шанс отразить атаку противника с помощью магического щита   \n");
        if(menuEnter(&pointer,3,WithoutESC)){
            again=false;
            HERO.CLASS=pointer;
            switch(pointer)
            {
                case 1:
                    printf("Вы выбрали класс воин\n");
                    strcpy(HERO.CLASSNAME,"Воин");
                    HERO.maxhp=1000;
                    HERO.str=50;
                    HERO.ag=5;
                    HERO.intel=20;
                    break;
                case 2:
                    printf("Вы выбрали класс лучник\n");
                    strcpy(HERO.CLASSNAME,"Лучник");
                    HERO.maxhp=750;
                    HERO.str=30;
                    HERO.ag=20;
                    HERO.intel=30;
                    break;
                case 3:
                    printf("Вы выбрали класс маг\n");
                    strcpy(HERO.CLASSNAME,"Маг");
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
        printf("\t\t\tЛегко");
        arrow(pointer,1);
        printf("\n\t\t\tСредне");
        arrow(pointer,2);
        printf("\n\t\t\tТяжело");
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

/*Работа с меню*/
void Menu(){
    short pointer=1;
    bool InMenu=true;
    while(InMenu){
        consoleclear();
        printf("-------------------Добро пожаловать в Medieval Story--------------------\n");
        printf("\t\t\tНовая игра");
        arrow(pointer,1);
        printf("\n\t\t\tЗагрузить игру");
        arrow(pointer,2);
        printf("\n\t\t\tУровень сложности");
        arrow(pointer,3);
        printf("\n\t\t\tВыход");
        arrow(pointer,4);
        if(menuEnter(&pointer,4,WithoutESC)){
            switch(pointer){
                case 1:{
                    HERO.location=0;
                    InGame=true;
                    ChooseName();
                    ChooseClass();
                    consoleclear();
                    printc("Вы просыпаетесь после долгого сна.\n",YELLOW);
                    sleep_ms(1500);
                    printf("Вы поднимаетесь,уходите с места своего привала и идете по дороге в ");
                    printc("Поле",CYAN);
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
                    printc("ОШИБКА ЗАГРУЗКИ",RED);
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
        printf("\t\t\tВернуться в игру");
        arrow(pointer,1);
        printf("\n\t\t\tИнвентарь");
        arrow(pointer,2);
        printf("\n\t\t\tСохранить игру");
        arrow(pointer,3);
        printf("\n\t\t\tВыйти в главное меню");
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
                    case 2:{      //Инвентарь
                        InventoryUI();
                        break;
                    }
                    case 3:{      //Сохранение
                        consoleclear();
                        saver();
                        printc("Игра успешно сохранена\n\n", GREEN);
                        sleep_ms(1000);
                        return true;
                        break;
                    }
                    case 4:{      //Выход в главное меню
                        pointer=1;
                        consoleclear();
                        again=true;
                        pointer=1;
                        while(again){
                            consoleclear();
                            printf("Вы уверены, что хотите выйти в главное меню?");
                            printf("\nДа");
                            arrow(pointer,1);
                            printf("\nНет");
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

/*Инвентарь*/
void InventoryUI(){
    short pointer=1;
    bool inInventory=true;
    while(inInventory){
        consoleclear();
        printf("--------------------------------------------------------------------------\n");
        printf("Доспехи:%s",HERO.armorString);
        printf("\nОружие:%s",HERO.weaponString);
        printf("\nЯд на оружии:");
        if(LV.Poison==true)
            printf("Есть");
        else
            printf("Нет");
        printf("\n\nЗелья здоровья:%d",HERO.hppotion);
        arrow(pointer,1);
        printf("\nЗелья маны:%d",HERO.mppotion);
        arrow(pointer,2);
        printf("\n--------------------------------------------------------------------------\n");
        printf("Выйти из инвентаря");
        arrow(pointer,3);
        if(menuEnter(&pointer,3,WithoutESC)){
            consoleclear();
            switch (pointer){
                case 1:{
                    if(HERO.hp==HERO.maxhp){
                        printf("\nЗдоровье уже максимально!");
                        sleep_ms(2000);
                        break;
                    }
                    else{
                        printf("\nВаше здоровье полностью восстановлено!");
                        HERO.hp=HERO.maxhp;
                        HERO.hppotion-=1;
                        sleep_ms(2000);
                        break;
                    }
                }
                case 2:{
                    if(HERO.mp==HERO.maxmp){
                        printf("\nМана уже максимальна!");
                        sleep_ms(2000);
                        break;
                    }
                    else{
                        printf("\nВаша мана полностью восстановлена!");
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

/*Отрисовка частей интерфейса*/
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
    printf("Оружие   %d\n",HERO.weapon);

    setXposition(h,0);
    printf("Здоровье %d/%d",HERO.hp,HERO.maxhp);
    setXposition(h,30);
    printf("Сила      %d",HERO.str);
    setXposition(h,60);
    printf("Доспехи  %d\n",HERO.armor);

    setXposition(h,0);
    printf("Мана     %d/%d",HERO.mp,HERO.maxmp);
    setXposition(h,30);
    printf("Ловкость  %d",HERO.ag);
    setXposition(h,60);
    printf("Навыки   %d\n",HERO.skills);

    if(!InBattle){
        setXposition(h,0);
        printf("Опыт     %d/%d",HERO.xp,HERO.maxxp);
        setXposition(h,30);
        printf("Интеллект %d",HERO.intel);
        setXposition(h,60);
        printf("Золото   %d\n",HERO.money);
        printf("--------------------------------------------------------------------------\n");
    }
    if(ShowBar) printf("Для входа во внутриигровое меню нажмите <ESC>\n");
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
        printf("Рубящий удар");
        arrow(pointer,1);
        printf("\nАтака щитом (%d mp)",HERO.lvl*3);
        arrow(pointer,2);
        printf("\nЯрость (%d mp)",HERO.lvl*15);
        arrow(pointer,3);
        break;
    case 2:
        printf("Выстрел из лука");
        arrow(pointer,1);
        printf("\nЛедяная стрела (%d mp)",HERO.lvl*5);
        arrow(pointer,2);
        printf("\nПроклятая стрела (%d mp)",HERO.lvl*10);
        arrow(pointer,3);
        break;
    case 3:
        printf("Атака посохом");
        arrow(pointer,1);
        printf("\nМагический щит (%d mp)",HERO.lvl*4);
        arrow(pointer,2);
        printf("\nОгненный шар (%d mp)",HERO.lvl*20);
        arrow(pointer,3);
        break;
    }
    printf("\n");
}

/*Функции для битвы*/
void StartBattle(short mobType){
    int counter2=0,counter3=0,buff=1;
    short GetXP,GetMoney;
    GenerateMob(mobType);
    InBattle=true;
    printc("\nВы видите странный силуэт, надвигающийся на вас",RED);
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
            printf("Вы победили!");

            GetXP=CurrentMob.attack*HERO.lvl/difficulty+rand()%30;
            HERO.xp+=GetXP;
            printf("\nПолучено %d опыта.",GetXP);

            GetMoney=10*CurrentMob.number*HERO.lvl/difficulty+rand()%30;
            HERO.money+=GetMoney;
            printf("\nПолучено %d золота.",GetMoney);

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
        printf("Вы обладаете отравленым клинком, враг получает %d дополнительного урона\n",GivenRandomly);
    }
    switch(HERO.CLASS){
        case 2:{
            if(*counter2!=0){
                printf("\nВраг заморожен, пропускает свой ход");
            }
            if(*counter3!=0){
                GivenRandomly=(1+rand()%HERO.lvl+HERO.skills)*HERO.str;
                CurrentMob.hp-=GivenRandomly;
                printf("\nВраг получает дополнительные %d урона из-за проклятья",GivenRandomly);
            }
            break;
        }
        case 3:{
            if(*counter2!=0){
                printf("\nВаш магический щит отражает удар противника");
            }
            if(*counter3!=0){
                GivenRandomly=(1+rand()%HERO.lvl+HERO.skills)*HERO.str;
                CurrentMob.hp-=GivenRandomly;
                printf("\nВраг получает дополнительные %d урона из-за поджога",GivenRandomly);
            }
            break;
        }
    }
    if(rand()%100<HERO.ag&&*counter2==0&&CurrentMob.hp>0){
        printc("\nВы смогли уклониться от удара врага благодаря ловкости!",YELLOW);
    }
    else if(((HERO.CLASS==2||HERO.CLASS==3)&&CurrentMob.hp>0&&*counter2==0)||(HERO.CLASS==1&&CurrentMob.hp>0)){
        GotDamage=(1+rand()%3)*HERO.lvl*CurrentMob.attack/(HERO.armor+1);
        HERO.hp-=GotDamage;
        printf("\n%s нанес(ла) вам %d урона",CurrentMob.NAME,GotDamage);
    }
}
bool SkillOne(int *buff){                               /*Первый скилл со свичем по классу*/
    int GivenDamage;
    switch(HERO.CLASS){         /*Cвич по классу*/
        case 1:{
            GivenDamage=(rand()%100+HERO.str+HERO.weapon)*HERO.lvl/CurrentMob.defence*(*buff)/difficulty;
            CurrentMob.hp-=GivenDamage;
            printf("\nВы нанесли %d урона врагу.",GivenDamage);
            break;
        }
        case 2:{
            GivenDamage=(rand()%50+HERO.str+HERO.weapon)*2*HERO.lvl/CurrentMob.defence/difficulty;
            CurrentMob.hp-=GivenDamage;
            printf("\nВы нанесли %d урона врагу.",GivenDamage);
            if(rand()%100<HERO.ag){
                printc("\nВам повезло и Вы успели нанести второй удар по врагу!",YELLOW);
                GivenDamage=(rand()%100+HERO.str+HERO.weapon)*HERO.lvl*(*buff)/CurrentMob.defence/difficulty;
                CurrentMob.hp-=GivenDamage;
                printf("\nВы нанесли %d урона врагу.",GivenDamage);
            }
            break;
        }
        case 3:{
            GivenDamage=(rand()%50+HERO.str+HERO.weapon)*2*HERO.lvl/CurrentMob.defence/difficulty;
            CurrentMob.hp-=GivenDamage;
            printf("\nВы нанесли %d урона врагу.",GivenDamage);
            break;
        }
    }
    return true;
}
bool SkillTwo(int *counter2,int *buff){                 /*Второй скилл со свичем по классу*/
    int GivenDamage;
    switch(HERO.CLASS){         /*Cвич по классу*/
        case 1:{
            GivenDamage=(HERO.intel+HERO.skills+rand()%100+HERO.str+HERO.weapon)*HERO.lvl*(*buff)/difficulty;
            CurrentMob.hp-=GivenDamage;
            HERO.mp-=(HERO.lvl*3);
            printf("\nВы нанесли врагу %d урона!",GivenDamage);
            break;
        }
        case 2:{
            if(*counter2>0){
                printf("\nЗаклинание уже применено");
                return false;
            }
            else if(HERO.mp-(HERO.lvl*5)<0){
                printf("\nНе хватает маны.");
                return false;
            }
            *counter2=2+rand()%HERO.lvl;
            GivenDamage=(HERO.intel+HERO.skills+rand()%100+3*HERO.str+HERO.weapon)*HERO.lvl/CurrentMob.defence/difficulty;
            CurrentMob.hp-=GivenDamage;
            HERO.mp-=(HERO.lvl*5);
            printf("\nВраг заморожен на %d хода",*counter2);
            printf("\nВы нанесли врагу %d урона!",GivenDamage);
            break;
        }
        case 3:{
            if(*counter2>0){
                printf("\nЗаклинание уже применено");
                return false;
            }
            else if(HERO.mp-(HERO.lvl*5)<0){
                printf("\nНе хватает маны.");
                return false;
            }
            *counter2=2+rand()%HERO.lvl;
            HERO.mp-=(HERO.lvl*5);
            printf("Вы наложили на себя заклинание магического щита!");
            printf("\nОно будет действовать в течение %d ходов врага.",*counter2);
            break;
        }
    }
    return true;
}
bool SkillThree(int *counter3,int *buff){               /*Третий скилл со свичем по классу*/
    short GivenDamage;
    switch(HERO.CLASS){                                 /*Cвич по классу*/
        case 1:{
            if(*counter3>0){
                printf("\nЗаклинание уже применено.");
                return false;
                break;
            }
            else if(HERO.mp-(HERO.lvl*15)<0)
            {
                printf("\nНе хватает маны.");
                return false;
                break;
            }
            *buff=2;
            *counter3=3;
            printf("\nВаш урон увеличен на 2 следующих удара!");
            HERO.mp-=(HERO.lvl*15);
            break;
        }
        case 2:{
            if(*counter3>0){
                printf("\nЗаклинание уже применено.");
                return false;
                break;
            }
            else if(HERO.mp-(HERO.lvl*10)<0)
            {
                printf("\nНе хватает маны.");
                return false;
                break;
            }
            *counter3=2+rand()%HERO.lvl;
            GivenDamage=(HERO.intel+HERO.skills+rand()%50+3*HERO.str+HERO.weapon)*HERO.lvl/CurrentMob.defence/difficulty;
            CurrentMob.hp-=GivenDamage;
            HERO.mp-=(HERO.lvl*10);
            printf("\nВы прокляли врага и нанесли ему %d урона!",GivenDamage);
            printf("\nСледующие %d ходов враг будет получать дополнительный урон!",*counter3);
            break;
        }
        case 3:{
            if(*counter3>0){
                printf("\nЗаклинание уже применено.");
                return false;
                break;
            }
            else if(HERO.mp-(HERO.lvl*20)<0)
            {
                printf("\nНе хватает маны.");
                return false;
                break;
            }
            *counter3=2+rand()%HERO.lvl;
            GivenDamage=(HERO.intel+HERO.skills+rand()%50+3*HERO.str+HERO.weapon)*HERO.lvl/CurrentMob.defence/difficulty;
            CurrentMob.hp-=GivenDamage;
            HERO.mp-=(HERO.lvl*20);
            printf("\nВы подожгли врага и нанесли ему %d урона!",GivenDamage);
            printf("\nСледующие %d ходов враг будет получать дополнительный урон!",*counter3);
            break;
        }
    }
    return true;
}
void GenerateMob(short mobType){                        /*Генератор мобов*/
    if(mobType==4) CurrentMob.number=4+(rand()%AmountOfMobs);
    else CurrentMob.number=mobType;
    switch(CurrentMob.number)
    {
        case 1:{
            strcpy(CurrentMob.NAME,"Босс");        //Босс
            CurrentMob.attack=30;
            CurrentMob.agility=20;
            CurrentMob.defence=20;
            CurrentMob.maxhp=800+(rand()%HERO.str)/3;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
        case 2:{
            strcpy(CurrentMob.NAME,"Призрак");        //Призрак
            CurrentMob.attack=10;
            CurrentMob.agility=20;
            CurrentMob.defence=3;
            CurrentMob.maxhp=250+(rand()%HERO.str)/3;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
        case 3:{
            strcpy(CurrentMob.NAME,"Демон");        //Демон
            CurrentMob.attack=60;
            CurrentMob.agility=30;
            CurrentMob.defence=50;
            CurrentMob.maxhp=1500;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
        case 4:{
            strcpy(CurrentMob.NAME,"Крыса");        //Крыса
            CurrentMob.attack=8;
            CurrentMob.agility=8;
            CurrentMob.defence=8;
            CurrentMob.maxhp=300+(rand()%HERO.str)/3;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
        case 5:{
            strcpy(CurrentMob.NAME,"Гоблин");     //Гоблин
            CurrentMob.attack=12;
            CurrentMob.agility=20;
            CurrentMob.defence=4;
            CurrentMob.maxhp=200+(rand()%HERO.str)/3;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
        case 6:{
            strcpy(CurrentMob.NAME,"Тролль");       //Тролль
            CurrentMob.attack=20;
            CurrentMob.agility=2;
            CurrentMob.defence=10;
            CurrentMob.maxhp=500+(rand()%HERO.str)/3;
            CurrentMob.hp=CurrentMob.maxhp;
            break;
        }
    }
}

/*Игровые функции*/
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
            printc("Ваш уровень увеличен!",YELLOW);
            printf("\nВыберите, какую характеристику улучшить(+1):");
            printf("\nСила");
            arrow(pointer,1);
            printf("\nЛовкость");
            arrow(pointer,2);
            printf("\nИнтеллект");
            arrow(pointer,3);
            if(menuEnter(&pointer,3,WithoutESC)){
                again=false;
                switch(pointer){
                    case 1:{
                        printf("\nВаша сила увеличена.");
                        HERO.str+=1;
                        break;
                    }
                    case 2:{
                        printf("\nВаша ловкость увеличена.");
                        HERO.str+=1;
                        break;
                    }
                    case 3:{
                        printf("\nВаш интеллект увеличен.");
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
        if(HERO.location==13) printf("\nВы успешно улучшили ");
        else printf("\nВы успешно приобрели ");
        printc(item,YELLOW);
        HERO.money-=cost;
        switch (HERO.location){
            case 2:{
                if(strcmp("Зелье Здоровья",item)==0){
                    HERO.hppotion+=1;
                    printf("\nТеперь их %d",HERO.hppotion);
                }
                else if(strcmp("Зелье Маны",item)==0){
                    HERO.mppotion+=1;
                    printf("\nТеперь их %d",HERO.mppotion);
                }
                break;
            }
            case 6:{
                HERO.armor=upgrade;
                printf("\nДоспехи <- %d",upgrade);
                strcpy(HERO.armorString,item);
                break;
            }
            case 7:{
                HERO.weapon=upgrade;
                printf("\nОружие <- %d",upgrade);
                strcpy(HERO.weaponString,item);
                break;
            }
            case 13:{
                if(strcmp(item,"Силу")==0)             HERO.str+=upgrade;
                else if(strcmp(item,"Ловкость")==0)    HERO.ag+=upgrade;
                else if(strcmp(item,"Интелект")==0)    HERO.intel+=upgrade;
                break;
            }
        }
    }
    else{
        printf("\nУ вас недостаточно денег!");
    }
    sleep_ms(2000);
}
