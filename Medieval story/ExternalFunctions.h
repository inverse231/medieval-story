#ifndef EXTERNALFUNCTIONS_H_INCLUDED
#define EXTERNALFUNCTIONS_H_INCLUDED
/*Сохранение и загрузка*/
void saver();
bool loader();

/*Инициализация игры*/
void setConsoleSize();
void setDefaultVariables();
void ChooseName();
void ChooseClass();
void ChooseDifficulty();
void Initialize();

/*Работа с меню*/
void Menu();
bool InGameMenu();
bool menuEnter(short *pointer,short MAX,bool HaveESC);
void arrow(short pointer,short menu);
void arrowLeft(short pointer,short menu);

/*Инвентарь*/
void InventoryUI();

/*User-friendly*/
void printc(char text[64],int color);
void consoleclear();
void sleep_ms(int milliseconds);

/*Отрисовка частей интерфейса*/
void setXposition(HANDLE h,SHORT x);
void printx(char text[64],short x);
void DrawUI(bool ShowBar);
void DrawBattleUI();
void dialUI(char name[15]);
void DrawHeroSkills(int pointer);

/*Функции для битвы*/
void StartBattle(short mobType);
void Attack(int *counter2,int *counter3,int *buff);
void Defence(int *counter2,int *counter3);
bool SkillOne(int *buff);
bool SkillTwo(int *counter2,int *buff);
bool SkillThree(int *counter3,int *buff);
void GenerateMob(short mobType);

/*Игровые функции*/
void levelUp();
void buyItem(char item[64],short cost,short upgrade);
#endif // EXTERNALFUNCTIONS_H_INCLUDED
