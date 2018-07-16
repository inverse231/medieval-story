#ifndef EXTERNALFUNCTIONS_H_INCLUDED
#define EXTERNALFUNCTIONS_H_INCLUDED
/*���������� � ��������*/
void saver();
bool loader();

/*������������� ����*/
void setConsoleSize();
void setDefaultVariables();
void ChooseName();
void ChooseClass();
void ChooseDifficulty();
void Initialize();

/*������ � ����*/
void Menu();
bool InGameMenu();
bool menuEnter(short *pointer,short MAX,bool HaveESC);
void arrow(short pointer,short menu);
void arrowLeft(short pointer,short menu);

/*���������*/
void InventoryUI();

/*User-friendly*/
void printc(char text[64],int color);
void consoleclear();
void sleep_ms(int milliseconds);

/*��������� ������ ����������*/
void setXposition(HANDLE h,SHORT x);
void printx(char text[64],short x);
void DrawUI(bool ShowBar);
void DrawBattleUI();
void dialUI(char name[15]);
void DrawHeroSkills(int pointer);

/*������� ��� �����*/
void StartBattle(short mobType);
void Attack(int *counter2,int *counter3,int *buff);
void Defence(int *counter2,int *counter3);
bool SkillOne(int *buff);
bool SkillTwo(int *counter2,int *buff);
bool SkillThree(int *counter3,int *buff);
void GenerateMob(short mobType);

/*������� �������*/
void levelUp();
void buyItem(char item[64],short cost,short upgrade);
#endif // EXTERNALFUNCTIONS_H_INCLUDED
