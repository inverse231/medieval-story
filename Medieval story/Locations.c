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

/*Главный свич по локациям*/
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

/*Локации*/
void StartLocation(){               /*Стартовая локация             (0)*/
    short pointer=1;
    while(HERO.location==0)
    {
        DrawUI(show);
        printc("\t\t\tПоле\n",CYAN);
        if(LV.CanGetBook==true){
            printf("Направиться к страннику,отдыхающему рядом со своей лошадью");
        }
        else{
            printf("Пройти к ");
            printc("Равнинам",CYAN);
        }
        arrow(pointer,1);

        if(LV.CanGetApple==true){
            printf("\nПродолжить движение прямо, к яблочному саду");
        }
        else{
            printf("\nПройти к воротам ");
            printc("Королевского Замка",CYAN);
        }
        arrow(pointer,2);

        printf("\nИдти направо к черным горам");
        arrow(pointer,3);

        if(menuEnter(&pointer,3,WithESC)){
            switch(pointer){
                case 1:{
                    if(LV.CanGetBook==true){
                        printf("Странник приветствует вас, дарит маленькую книгу и показывает дорогу к ");
                        printc("Равнинам",CYAN);
                        printf(".\n");
                        printc("Книга добавлена в инвентарь. (+5 к умениям)\n", YELLOW);
                        HERO.skills+=5;
                        LV.CanGetBook=false;
                        sleep_ms(3000);
                        printf("Вы видите перед собой дорогу и решаете продолжить движение");
                        sleep_ms(2000);
                    }
                    /*Переход на Равнины*/
                    HERO.location=1;
                    break;
                }
                case 2:{
                    if(LV.CanGetApple==true){
                        printf("Вы проходите мимо яблонь и решаете сорвать немного спелых яблок,висящих на ветках\n");
                        printc("Ваше максимальное и текущее здоровье увеличено на 10.\n", YELLOW);
                        HERO.maxhp+=10;
                        HERO.hp+=10;
                        LV.CanGetApple=false;
                        sleep_ms(3000);
                        printf("\nСквозь ветви яблонь вы видите башни ");
                        printc("Королевского Замка",CYAN);
                        printf(" и идете к нему через узкую тропу");
                        sleep_ms(2000);
                    }
                    /*Переход к Королевскому Замку*/
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
void Plains(){                      /*Равнины                       (1)*/
    short pointer=1;
    while(HERO.location==1){
        DrawUI(show);
        printc("\t\t\tРавнины",CYAN);

        printf("\nНаправиться к торговому каравану, остановившемуся неподалёку.");
        arrow(pointer,1);
        printf("\nПриблизиться к калитке загадочного особняка.");
        arrow(pointer,2);
        printf("\nПройти по дороге в ");
        printc("Деревню",CYAN);
        arrow(pointer,3);
        printf("\nПройти в ");
        printc("Поле",CYAN);
        arrow(pointer,4);

        if(menuEnter(&pointer,4,WithESC)){
            switch(pointer){
                case 1:{
                    printf("\nХозяин каравана заинтересованно смотрит на вас");
                    HERO.location=2;
                    break;
                }
                case 2:{
                    if(LV.civquest){
                        printf("\nВ окне дома появляется едва видимый силуэт и манит вас\n");
                        sleep_ms(1000);
                        printf("Вы входите в особняк, одержимый призраками.\n");
                        sleep_ms(1000);
                        printc("Дверь за вами захлопывается сама",RED);
                        HERO.location=10;
                    }
                    else if(LV.qcompleted){
                        printc("\nВы уже были здесь.",YELLOW);
                    }
                    else{
                        printf("\nВы оглядываете дом, но зайти не решаетесь.");
                    }
                    break;
                }
                case 3:{
                    printf("\nВы видите дорогу и идете по ней.");
                    sleep_ms(1000);
                    printf("\nОна приводит вас в ");
                    printc("Деревню",CYAN);
                    HERO.location=8;
                    break;
                }
                case 4:{
                    printf("\nВы разворачиваетесь и уходите в ");
                    printc("Поле",CYAN);
                    HERO.location=0;
                    break;
                }
            }
            sleep_ms(2000);
        }
    }
}
void Caravan(){                     /*Торговля с караваном          (2)*/
    short pointer=1;
    while(HERO.location==2){
        DrawUI(show);
        printc("\t\t\tКараван\n",CYAN);

        printf("Список товаров,который вы можете приобрести:");
        printf("\nЗелье здоровья - 40 золотых");
        arrow(pointer,1);
        printf("\nЗелье маны - 30 золотых");
        arrow(pointer,2);
        printf("\nЯд(Отравляет вашего врага в течение следующего боя) - 20 золотых");
        arrow(pointer,3);
        printf("\nПокинуть ");
        printc("Караван",CYAN);
        arrow(pointer,4);

        if(menuEnter(&pointer,4,WithESC)){
            switch(pointer){
                case 1:{
                    buyItem("Зелье Здоровья",40,1);
                    break;
                }
                case 2:{
                    buyItem("Зелье Маны",30,1);
                    break;
                }
                case 3:{
                    if(LV.Poison==true)
                        printf("\nЯд уже нанесен на ваше оружие.");
                    else if(HERO.money-20>=0){
                        printc("\nЯд нанесен на ваше оружие.",YELLOW);
                        HERO.money-=20;
                        LV.Poison=true;
                    }
                    else{
                        printf("\nУ вас недостаточно денег!");
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
void CastleEnter(){                 /*Вход в Королевский замок      (3)*/
    short pointer=1;
    if(LV.CanEnterCastle) HERO.location=4;
    else{
        while(HERO.location==3){
            DrawUI(show);
            printc("\t\tВход в королевский замок\n",CYAN);

            printf("Один из стражников говорит вам: \"Для прохода вам нужно заплатить налог в 10 золотых.\"\n");
            printf("\nПодойти к стражникам,загораживающим путь в замок");
            arrow(pointer,1);
            printf("\nВернуться назад");
            arrow(pointer,2);

            if(menuEnter(&pointer,2,WithESC)){
                switch(pointer){
                    case 1:{
                        DrawUI(show);
                        if(HERO.money-10>=0)
                        {
                            LV.CanEnterCastle=true;
                            printf("Вы уплачиваете налог и стражник уступает вам дорогу.");
                            HERO.location=4;
                            HERO.money-=10;
                        }
                        else{
                            printf("У вас недостаточно денег, возвращайтесь позже.");
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
void Castle(){                      /*Королевский замок             (4)*/
    short pointer=1;
    while(HERO.location==4){
        DrawUI(show);
        printc("\t\tКоролевский замок",CYAN);

        printf("\nПройти в покои короля");
        arrow(pointer,1);
        printf("\nНаправиться в лавку торговца");
        arrow(pointer,2);
        printf("\nТренироваться на арене");
        arrow(pointer,3);
        printf("\nПройти к западным воротам");
        arrow(pointer,4);
        printf("\nВернуться назад в ");
        printc("Поле",CYAN);
        arrow(pointer,5);

        if(menuEnter(&pointer,5,WithESC)){
            switch(pointer){
                case 1:{
                    printf("Вы подходите к трону,на котором сидит король");
                    HERO.location=12;
                    sleep_ms(2000);
                    break;
                }
                case 2:{
                    printf("Торговец подходит к прилавку и предлагает вам свой товар");
                    HERO.location=5;
                    sleep_ms(2000);
                    break;
                }
                case 3:{
                    HERO.location=13;
                    break;
                }
                case 4:{
                    printf("Вы проходите через ворота и идете по дороге.");
                    sleep_ms(1000);
                    printf("\nВы видите маленькую деревню и направляетесь в неё.");
                    HERO.location=8;
                    sleep_ms(2000);
                    break;
                }
                case 5:{
                    printf("Вы разворачиваетесь и выходите из ворот замка");
                    HERO.location=0;
                    sleep_ms(2000);
                    break;
                }
            }
        }
    }
}
void Trader(){                      /*Торговец в замке              (5)*/
    short pointer=1;
    while(HERO.location==5){
        DrawUI(show);
        printc("\t\tЛавка торговца",CYAN);

        printf("\nДоспехи");
        arrow(pointer,1);
        printf("\nОружие");
        arrow(pointer,2);
        printf("\nВернуться назад");
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
                    printf("Вы отходите от прилавка.");
                    HERO.location=4;
                    sleep_ms(2000);
                    break;
                }
            }
        }
    }
}
void TraderArmor(){                 /*Покупка брони                 (6)*/
    short pointer=1;
    while(HERO.location==6){
        DrawUI(show);
        printf("\t\tСписок доспехов:");

        printf("\nКольчужные доспехи - 300 золотых");
        arrow(pointer,1);
        printf("\nЗачарованная мантия - 400 золотых");
        arrow(pointer,2);
        printf("\nЛатные доспехи - 500 золотых");
        arrow(pointer,3);
        printf("\nВернуться назад");
        arrow(pointer,4);

        if(menuEnter(&pointer,4,WithESC)){
            switch(pointer){
                case 1:{
                    buyItem("Кольчужные доспехи",300,15);
                    break;
                }
                case 2:{
                    buyItem("Зачарованную мантию",400,30);
                    break;
                }
                case 3:{
                    buyItem("Латные доспехи",500,50);
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
void TraderWeapons(){               /*Покупка оружия                (7)*/
    short pointer=1;
    while(HERO.location==7){
        DrawUI(show);
        printf("\t\tСписок оружия:");
        if(HERO.CLASS==1){
            printf("\nОдноручный длинный меч - 500 золотых");
            arrow(pointer,1);
            printf("\nДвуручный длинный меч - 800 золотых");
            arrow(pointer,2);
            printf("\nПарные кинжалы - 950 золотых");
            arrow(pointer,3);
            printf("\nВернуться назад");
            arrow(pointer,4);
        }
        else if(HERO.CLASS==2){
            printf("\nДлинный лук - 450 золотых");
            arrow(pointer,1);
            printf("\nКороткий лук - 600 золотых");
            arrow(pointer,2);
            printf("\nАрбалет - 900 золотых");
            arrow(pointer,3);
            printf("\nВернуться назад");
            arrow(pointer,4);
        }
        else if(HERO.CLASS==3){
            printf("\nДеревянный посох - 350 золотых");
            arrow(pointer,1);
            printf("\nОдноручный посох и щит - 600 золотых");
            arrow(pointer,2);
            printf("\nГримуар - 5000 золотых");
            arrow(pointer,3);
            printf("\nВернуться назад");
            arrow(pointer,4);
        }
        if(menuEnter(&pointer,5,WithESC)){
            switch(pointer){
                case 1:{
                    if(HERO.CLASS==1)       buyItem("Одноручный длинный меч",500,25);
                    else if(HERO.CLASS==2)  buyItem("Длинный лук",450,25);
                    else if(HERO.CLASS==3)  buyItem("Деревянный посох",350,30);
                    break;
                }
                case 2:{
                    if(HERO.CLASS==1)       buyItem("Двуручный длинный меч",800,40);
                    else if(HERO.CLASS==2)  buyItem("Короткий лук",600,45);
                    else if(HERO.CLASS==3)  buyItem("Одноручный посох и щит",600,50);
                    break;
                }
                case 3:{
                    if(HERO.CLASS==1)       buyItem("Парные кинжалы",950,120);
                    else if(HERO.CLASS==2)  buyItem("Арбалет",900,110);
                    else if(HERO.CLASS==3)  buyItem("Гримуар",2000,200);
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
void Village(){                     /*Деревня                       (8)*/
    short pointer=1;
    while(HERO.location==8){
        DrawUI(show);
        printc("\t\tДеревня",CYAN);
        printf("\nНаправиться в центр деревни");
        arrow(pointer,1);
        printf("\nПройти по дороге из деревни");
        arrow(pointer,2);
        printf("\nУйти в ");
        printc("Королевский Замок",CYAN);
        arrow(pointer,3);

        if(menuEnter(&pointer,3,WithESC)){
            switch(pointer){
                case 1:{
                    printf("\nВы проходите вглубь деревни.");
                    sleep_ms(2000);
                    if(LV.civquest==false){
                        printf("\nСтароста подходит к вам и начинает разговор.");
                    }
                    sleep_ms(2000);
                    HERO.location=9;
                    return;
                    break;
                }
                case 2:{
                    printf("\nВы идете по дороге из деревни.");
                    HERO.location=1;
                    break;
                }
                case 3:{
                    printf("\nВы разворачиваетесь и уходите в королевский замок.");
                    HERO.prevlocation=HERO.location;
                    HERO.location=3;
                    break;
                }
            }
            sleep_ms(2000);
        }
    }
}
void civdial(){                     /*Диалог со старостой           (9)*/
    short pointer=1;
    while(HERO.location==9){
        if(LV.civquest==false&&!LV.qcompleted){
            dialUI("Староста");
            printf("-Здравствуй, путник. Я прошу тебя о помощи. На нашу деревню уже в течение недели совершаются нападения, в основном крадут скот. У нас уже почти ничего не осталось, пару коров и овец. Поможешь нам найти наш пропавший скот?");
            printf("\n(Хорошо,я готов выполнить ваше задание.)");
            arrow(pointer,1);
            printf("\n(Сейчас я не готов помочь вам.)");
            arrow(pointer,2);
            if(menuEnter(&pointer,2,WithoutESC)){
                DrawUI(show);
                switch(pointer){
                    case 1:{

                        printf("\n-Спасибо, один из жителей сказал,что видел,как кто-то убегал в особняк на равнинах ночью. Удачи тебе.");
                        LV.civquest=true;
                        sleep_ms(3500);
                        break;
                    }
                    case 2:{
                        printf("\n-Ну, что ж, ничего не поделать.");
                        sleep_ms(2000);
                        break;
                    }
                }
                HERO.location=8;
            }
        }
        else if(LV.Map&&LV.qcompleted&&LV.civquest){
            system("CLS");
            printf("Вы подходите к старосте деревни и отдаёте карту.");
            sleep_ms(1000);
            printf("\n-Спасибо,герой! Ты спас нашу деревню. Вот твоя награда, а ещё письмо от короля.");
            HERO.money+=1000;
            sleep_ms(3000);
            printf("\nВ письме король просит вас отправиться к нему.");
            printf("\nВы сразу же отправляетесь в замок.");
            sleep_ms(3000);
            HERO.location=4;
            LV.CanEnterCastle=true;
            LV.civquest=false;
        }
        else if(!LV.qcompleted&&LV.civquest){
            system("CLS");
            printc("\t\t\tВы уже взяли задание старосты!",LIGHTBLUE);
            sleep_ms(2000);
            HERO.location=8;
        }
        else if(LV.qcompleted&&!LV.civquest){
            system("CLS");
            printc("\t\t\tВы уже выполнили задание старосты!",LIGHTBLUE);
            sleep_ms(2000);
            HERO.location=8;
        }
    }
}
void HauntedH(){                    /*Особняк с призраками          (10)*/
    short pointer=1;
    while(HERO.location==10){
        DrawUI(show);
        printc("\t\tОдержимый особняк",CYAN);

        printf("\nПройти в жилую комнату");
        arrow(pointer,1);
        printf("\nПодняться на второй этаж");
        arrow(pointer,2);
        printf("\nПройти налево и открыть дверь");
        arrow(pointer,3);

        if(menuEnter(&pointer,3,WithESC)){
            switch(pointer){
                case 1:{
                    if(LV.room1==false){
                        printf("\nВы входите в жилую комнату. Из противоположного дверного проема слышен шум.");
                        sleep_ms(2000);
                        StartBattle(ghost);
                        LV.room1=true;
                    }
                    else{
                        printf("\nВы уже были здесь");
                        sleep_ms(2000);
                    }
                    break;
                }
                case 2:{
                    if(LV.room2==false){
                        printf("\nВы поднимаетесь на второй этаж и проходите в комнату с балконом.\n");
                        sleep_ms(2000);
                        printf("\nНа полу вы обнаруживаете маленькую фигурку кошки и кладете ее себе в карман.");
                        sleep_ms(1500);
                        printc("(+5 к ловкости.)",YELLOW);
                        HERO.ag+=5;
                        sleep_ms(2000);
                        LV.room2=true;
                    }
                    else{
                       printf("\nВы уже были здесь");
                       sleep_ms(2000);
                    }
                    break;
                }
                case 3:{
                    if(LV.room1&&LV.room2){
                        printf("\nВы открываете дверь.");
                        HERO.location=11;
                    }
                    else{
                        printf("\nВы дергаете за ручку,но дверь не поддается.");
                    }
                    sleep_ms(2000);
                    break;
                }
            }
            pointer=1;
        }
    }
}
void Basement(){                    /*Подвал особняка с привидениями(11)*/
    short pointer=1;
    LV.room1=false;
    LV.room2=false;
    while(HERO.location==11){
        DrawUI(show);
        printc("\t\tПодвал",CYAN);

        printf("\nПройти прямо");
        arrow(pointer,1);
        printf("\nОбыскать деревянные ящики");
        arrow(pointer,2);
        printf("\nОсмотреть стену слева от вас");
        arrow(pointer,3);

        if(menuEnter(&pointer,3,WithESC)){
            switch(pointer){
                case 1:{
                    if(LV.room1==false){
                        printf("\nВы проходите через арку и оказываетесь в темной комнате. Внезапно, в комнате вспыхивает свет факелов.");
                        StartBattle(ghost);
                        LV.room1=true;
                    }
                    else{
                        printf("Вы уже были здесь!");
                        sleep_ms(2000);
                    }
                    break;
                }
                case 2:{
                 if(LV.room2==false){
                    printf("\nВы роетесь в ящиках и находите там маленький талисман.");
                    sleep_ms(1500);
                    printc("(+10 к интеллекту.)",YELLOW);
                    HERO.intel+=10;
                    sleep_ms(2000);
                    LV.room2=true;
                 }
                 else{
                    printf("Вы уже были здесь!");
                    sleep_ms(2000);}
                break;
                }
                case 3:{
                    if(LV.room1&&LV.room2){
                        printf("\nВы понимаете, что стена фальшивая. Вы бьете по стене и она ломается, за ней - вход в другую комнату. Вы входите и видите там человека,который при виде вас обнажает свой клинок.\n");
                        sleep_ms(2000);
                        StartBattle(boss);
                        LV.qcompleted=true;
                        sleep_ms(2000);
                        DrawUI(show);
                        printf("Вы находите карту, на которой отображено положение скота.");
                        LV.Map=true;
                        sleep_ms(2000);
                        HERO.location=8;
                        break;
                    }
                    else{
                        printf("\nВы не обнаружили ничего интересного");
                        sleep_ms(2000);
                        break;
                    }
                }
            }
        }
    }
}
void kingdial(){				    /*Диалог с Королем              (12)*/
    short pointer=1;
    while(HERO.location==12){
        if(LV.qcompleted){
            dialUI("Король");
            printf("-Здравствуй, герой. Благодаря тебе, наше королевство теперь свободно от демона, терроризировавшего нас уже очень долгое время.");
            printf("\nУ меня есть к тебе одно предложение. Вступай на службу ко мне, и у тебя будет всё, что захочешь. Ну, что думаешь?");
            printf("\n\n(Поступить на службу к королю)");
            arrow(pointer,1);
            printf("\n(Отказаться от предложения)");
            arrow(pointer,2);
            if(menuEnter(&pointer,2,WithoutESC)){
                switch(pointer){
                    case 1:{
                        consoleclear();
                        printf("Вы поступаете на службу к королю.");
                        sleep_ms(1500);
                        printf("\nВы вместе с армией королевства прогоняете нежить с вашей территории и живете беззаботно.\n");
                        sleep_ms(2000);
                        break;
                    }
                    case 2:{
                        consoleclear();
                        printf("Вы отказываетесь от службы у короля, предпочитая свою свободу выше материальных благ.");
                        sleep_ms(1500);
                        printf("\nВы скитаетесь по свету, попадаете в разные приключения и живете одним днем.\n");
                        sleep_ms(2000);
                        break;
                    }
                }
                printx("(Конец)\n",24);
                system("pause");
                HERO.location=-1;
                InGame=false;
            }
        }
        else{
            dialUI("Король");
            printf("\n-У меня нет ничего для тебя");
            sleep_ms(1500);
            HERO.location=4;
        }
    }
}
void Arena(){                       /*Арена                         (13)*/
    short pointer=2;
    while(HERO.location==13){
        DrawUI(show);
        printc("\t\tАрена",CYAN);
        printf("\nВыйти из Арены");
        arrow(pointer,1);
        printf("\n\nВыберите характеристику для тренировки(50 золотых,+5 к характеристике):");
        printf("\nСила");
        arrow(pointer,2);
        printf("\nЛовкость");
        arrow(pointer,3);
        printf("\nИнтелект");
        arrow(pointer,4);

        if(menuEnter(&pointer,4,WithoutESC)){
            switch(pointer){
                case 1:{
                    HERO.location=4;
                    break;
                }
                case 2:{
                    buyItem("Силу",50,5);
                    break;
                }
                case 3:{
                    buyItem("Ловкость",50,5);
                    break;
                }
                case 4:{
                    buyItem("Интелект",50,5);
                    break;
                }
            }
        }
    }
}
void Cave(){                        /*Пещера                        (14) */
    short pointer=1;
    while(HERO.location==14){
        DrawUI(show);
        printc("\tПещера",CYAN);
        printf("\nВзять факел");
        arrow(pointer,1);
        printf("\nОсмотреть скелет, лежащий у входа");
        arrow(pointer,2);
        printf("\nПройти в ");
        printc("Подземелье",CYAN);
        arrow(pointer,3);
        printf("\nВернуться в ");
        printc("Поле", CYAN);
        arrow(pointer,4);

        if(menuEnter(&pointer,4,WithESC)){
            switch(pointer){
                case 1:{
                    if (!LV.lighter){
                        printf("\nВы берете факел и видите путь вглубь пещеры.");
                        LV.lighter=true;
                    }
                    else {
                        printf("\nВы уже взяли факел.");
                    }
                    sleep_ms(1500);
                    break;
                }
                case 2:{
                    if (!LV.skelet){
                        printf("\nВы осматриваете скелет и находите кошель с золотом");
                        HERO.money+=10;
                        LV.skelet=true;
                    }
                    else {
                        printf("\nВы уже осмотрели скелет.");
                    }
                    sleep_ms(1500);
                    break;
                }
                case 3:{
                    if (LV.lighter){
                        printf("\nДержа факел перед собой, вы идете вглубь пещеры.");
                        HERO.location=15;
                    }
                    else {
                        printf("\nВы не решаетесь идти вглубь.");
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
void Dungeon(){                     /*Подземелье                    (15) */
    short pointer=1;
    bool again;
    while(HERO.location==15){
        DrawUI(show);
        printc("\tПодземелье",CYAN);
        printf("\nОсмотреть древнюю вазу");
        arrow(pointer,1);
        printf("\nОткрыть ящики");
        arrow(pointer,2);
        printf("\nПройти в дверь");
        arrow(pointer,3);
        printf("\nИдти дальше");
        arrow(pointer,4);
        printf("\nВернуться в начало ");
        printc("Пещеры",CYAN);
        arrow(pointer,5);

        if(menuEnter(&pointer,5,WithESC)){
            switch(pointer){
                case 1:{
                    if(LV.key==false){
                        printf("\nВы осматриваете вазу и находите в ней ключ");
                        LV.key=true;
                    }
                    else printf("\nВы уже осмотрели вазу");
                    sleep_ms(2000);
                    break;
                }
                case 2:{
                    if(LV.box==false){
                        printf("\nВы роетесь в ящиках и находите несколько монет");
                        HERO.money+=rand()%10+2;
                        LV.box=true;
                    }
                    else printf("\nВы уже осмотрели ящики");
                    sleep_ms(2000);
                    break;
                }
                case 3:{
                    if(LV.key&&LV.box){
                        printf("\nВы открываете дверь ключом и входите в просторный зал");
                        StartBattle(diablo);
                        if(HERO.location==-1) break;
                        system("CLS");
                        printf("Сразив демона, вы подходите к сундуку и открываете его. На вашем лице появляется довольная улыбка.");
                        sleep_ms(2000);
                        HERO.money+=1000;
                        HERO.location=0;
                    }
                    else printf("\nВы пытаетесь открыть дверь но она не поддается");
                    sleep_ms(2000);
                    break;
                }
                case 4:{
                    again=true;
                    printf("\nВы углубляетесь в коридоры пещеры.");
                    StartBattle(mob);
                    pointer=1;
                    while(again){
                        DrawUI(show);
                        printf("Вы желаете продолжить изучение подземелья?");
                        printf("\nДа");
                        arrow(pointer,1);
                        printf("\nНет");
                        arrow(pointer,2);
                        if(menuEnter(&pointer,2,WithESC)){
                            switch (pointer){
                                case 1:{
                                    printf("\nВы углубляетесь в коридоры пещеры.");
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
