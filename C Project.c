/* Assignment: Final Project
Students: 
Ofek Elgozi, ID: 318432085
Shlomi Danino, ID: 316534064
*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
typedef enum { False, True } boolean;
typedef struct//
{
	char name[30];
	int sign;
} Player;

int** BuildBoard(int** board, int* rows, int* colums);//פונקציה שבונה לוח לפי גדלים שהמשתמש הכניס בעזרת הקצאות דינמיות
void PrintBoard(int** board, int rows, int colums);//פונקציה שמדפיסה לוח לפי הנתונים שהזנו לו
Player PlayerInfo(Player* pPlayer, int i);//פרטי השחקנים
boolean ValidCol(int* num, int colums);//חריגה מגבולות
boolean FullCol(int j, int** board);//בודקת אם העמודה שהתקבלה מלאה
void Game(Player* pPlayer, int** board, int rows, int colums);//קליטת ערכים לתוך הלוח לפי התנאים המבוקשים ובחירת מנצח בהתאם להכנסת הערכים
boolean IsTaken(int** board, int i, int j);//אם המיקום ששולחים לפונקציה כבר משומש
int validName(char str[]);//אם השם שנכנס של השחקן הוא תקין ומתחיל עם אות ראשונה
void FreeAll(Player* pPlayer, int** board, int rows, int colums);//פונקציה שמשחררת את כל ההקצאות שעשינו במהלך התוכנית
boolean IsWinner(int** board, int i, int j, int rows, int column);//פונקציה שמקבלת מיקום בלוח וממנו בודקת את כל האפשרויות האפשריות לניצחון כלומר שורה עמודה או אלכסון

int main()
{
	Player* pPlayer = NULL;
	pPlayer = (Player*)malloc(2 * sizeof(Player));//מקצים מערך של מבנים בגודל 2 כי ידוע מראש שהמשחק הוא רק ל2 שחקנים
	int** board = 0;
	int rows = 0, colums = 0;
	for (int i = 0; i < 2; i++)//קולטים פרטים של שני שחקנים
	{
		printf("Please enter player number %d first name:", i + 1);
		PlayerInfo(&(pPlayer[i]), i);
	}
	board = BuildBoard(board, &rows, &colums);
	PrintBoard(board, rows, colums);
	Game(pPlayer, board, rows, colums);
	FreeAll(pPlayer, board, rows, colums);
	return 0;
}

int** BuildBoard(int** board, int* rows, int* colums)//פונקציה שבונה את המטריצה
{
	printf("Please enter the size of the board:\n");
	printf("Rows:");
	scanf("%d", rows);
	while (*rows < 4)//אם הכמות קטנה מ4 שורות מבקשים שוב
	{
		printf("wrong rows board's size please try again:");
		scanf("%d", rows);
	}
	printf("Colums:");
	scanf("%d", colums);
	while (*colums < 4)//אם הכמות קטנה מ4 עמודות מבקשים שוב
	{
		printf("wrong colums board's size please try again:");
		scanf("%d", colums);
	}
	board = (int**)malloc((*rows) * sizeof(int*));//מקצים גודל דינמי בזיכרון למערך של מצביעים במטריצה
	if (board == NULL)
	{
		printf("Error\n");
		return 0;
	}
	for (int i = 0; i < *rows; i++)//לולאה שעוברת על כל שורה עד גודל השורות שנתון לנו מהמשתמש 
	{
		board[i] = (int*)malloc((*colums) * sizeof(int));//מקצים גודל דינמי בזיכרון בכל מיקום\מצביע וכך יוצרים את העמודות במטריצה
		if (board[i] == NULL)
		{
			printf("Error\n");
			return 0;
		}
	}
	for (int i = 0; i < (*rows); i++)//רצים על השורות במטריצה ומאתחלים אותה 
	{
		for (int j = 0; j < (*colums); j++)//רצים על כל עמודה
		{
			board[i][j] = 0;//מכניסה אפסים
		}
	}
	printf("\n");
	return board;
}

void PrintBoard(int** board, int rows, int colums)
{
	for (int i = 0; i < rows; i++)
	{
		if (board[i][0] == 1 || board[i][0] == 2 || board[i][0] == 0)//אם אנחנו נמצאים בעמודה הראשונה משמאל
		{
			printf("(%d)-->", i + 1);//כותבים את מספר השורה
			printf("|");//שמים קיר ביטחון
		}
		for (int j = 0; j < colums; j++)
		{
			printf("%d|", board[i][j]);//הדפסנו את הערך ושמנו סוגר מצד ימין
		}
		printf("\n");
		printf("      ");//השלמת רווחים שנגרמה מכתיבת מספר השורות
		for (int k = 0; k < (2 * colums) + 1; k++)//רווח בין כל שורה לשורה בלוח באורך של כל העמודות של המטריצה פלוס רווחים בין התאים
		{
			printf("-");
		}
		printf("\n");
	}
}

boolean ValidCol(int* num, int colums)
{
	if (*num >= 1 && *num <= colums)//אם מספר העמודה לא בטווח הרצוי
	{
		return True;
	}
	printf("\n");
	printf("Wrong colum. please try again: ");//מבקשים שוב קלט חוזר
	scanf("%d", num);
	return ValidCol(num, colums);//קוראים לפונקציה בצורה רקורסיבית עם הערך החדש ומחזירה ערך בהתאם
}

Player PlayerInfo(Player* pPlayer, int i)//הכנסת פרטים על כל שחקן, שם פרטי וסימן במשחק
{
	char name[30];
	scanf("%s", name);
	while (validName(name) == 0)//אם השם לא תקין או שהוא כבר קיים לכתוב הודעת שגיאה ולקלוט שוב
	{
		printf("Bad name. Try again:");
		scanf("%s", name);
	}
	strcpy(pPlayer->name, name);
	pPlayer->sign = i + 1;//סימן השחקן
	printf("your sign in the board is %d\n", pPlayer->sign);
}

boolean FullCol(int j, int** board)
{
	if (board[0][j] == 0)//אם האיבר בשורה הראשונה ריק מחזירים שקר
		return False;
	return True;
}

void Game(Player* pPlayer, int** board, int rows, int colums)
{
	boolean won = False;//שיגיד לי אם הפעולה התקיימה בהצלחה או לא flag משתנה מסוג 
	int selection, turn = 0, sign = 0;
	while (turn != rows*colums && won == False)//הלולאה מסתיימת כאשר מספר התורים המקסימילי התרחש או שאף אחד לא ניצח
	{
		printf("Please enter the colum you want enter your disk to:");//מבקשים מהמשתמש לבחור באיזה עמודה הוא רוצה להכניס את הדיסקית
		scanf("%d", &selection);
		if (turn % 2 == 0)//אם מספר התור הוא אי זוגי אז זאת אומרת שהשחקן הראשון משחק עכשיו
			sign = pPlayer[0].sign;
		else//אם מספר התור הוא זוגי אז זאת אומרת שהשחקן השני משחק עכשיו
			sign = pPlayer[1].sign;
		while (ValidCol(&selection, colums) == False)//כל עוד העמודה שנקלטה לא בטווח הנכון דורשים מהשחקן להכניס עמודה חדשה שוב ושוב
		{
			printf("The colum is out of range please try again.\n");
			scanf("%d", &selection);
		}

		while (FullCol(selection - 1, board) == True)//אם אחת העמודות בלוח התמלאה כאשר השחקן רצה למלא בעמודה זו אז מודיעים לשחקן שיבחר עמודה חדשה
		{
			printf("The colum is full, please choose another one.\n");
			scanf("%d", &selection);
		}

		for (int i = rows - 1; i >= 0; i--)//לולאה שרצה מהשורה התחתונה עד לעליונה
		{
			if ((IsTaken(board, i, selection - 1) == True) && (FullCol(selection - 1, board) == False))//אם המשפצת תפוסה עם דיסקית אחרת וגם העמודה עדיין לא מלאה עולים שורה אחת למעלה
				continue;
			else//אחרת מכניסים את הדיסקית של אותו שחקן בתא המבוקש
			{
				board[i][selection - 1] = sign;
				break;//ויוצאים מהלולאה
			}
		}
		for (int i = 0; i < rows; i++)//רצים על השורות בלוח
		{
			for (int j = 0; j < colums; j++)//רצים על העמודות בלוח
			{
				won = IsWinner(board, i, j, rows, colums);//בודקים בכל מיקום בלוח עם מתקיים אלכסון שורה או עמודה שנותנת לשחקן ניצחון
				if (won == True)//אם כן נצא מהלולאה ונסיים את המשחק
					break;
			}//אחרת נמשיך לבדוק את המיקום הבא בלוח ואת האפשרויות שלו לניצחון
			if (won==True)
				break;
		}
		PrintBoard(board, rows, colums);//מדפיסים את הלוח לאחר השינויים שביצענו עם הדיסקיות
		turn++;//תור השחקן הבא
	}
	if (won == True)//אם התרחש ניצחון במהלך המשחק
	{
		if (sign == 1)//אם הדיסקית האחרונה שהוכנסה היא של השחקן הראשון מדפיסים הודעה בהתאם
			printf("The winner is the first player:%s\n", pPlayer[0].name);
		else//אם הדיסקית האחרונה שהוכנסה היא של השחקן השני מדפיסים הודעה בהתאם
			printf("The winner is the second player:%s\n", pPlayer[1].name);
	}
	else//אחרת אף אחד לא ניצח ויש תיקו
		printf("\nTie");
	return;
}

boolean IsTaken(int** board, int i, int j)
{
	if (board[i][j] == 0)//אם האיבר במיקום הנל לא משומש מחזירים שקר
		return False;
	return True;
}

boolean IsWinner(int** board, int i, int j, int rows, int column) 
{
	if (board[i][j] == 0)
		return False;

	// Check for LEFT (מהנקודה בודקים את השורה לכיוון שמאלה)
	if (j - 3 >= 0) 
	{
		if (board[i][j] == board[i][j - 1] && board[i][j] == board[i][j - 2] && board[i][j] == board[i][j - 3])
			return True;
	}

	// Check for RIGHT (מהנקודה בודקים את השורה לכיוון ימינה)
	if (j + 3 < column)
	{
		if (board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3])
			return True;
	}

	// Check for TOP (מהנקודה בודקים את העמודה כלפי מעלה)
	if (i - 3 >= 0) 
	{
		if (board[i][j] == board[i - 1][j] && board[i][j] == board[i - 2][j] && board[i][j] == board[i - 3][j])
			return True;
	}


	// Check for BOTTOM (מהנקודה בודקים את העמודה כלפי מטה)
	if (i + 3 < rows) 
	{
		if (board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] && board[i][j] == board[i + 3][j])
			return True;
	}


	// Check for TOP - LEFT (מהנקודה בודקים את האלכסון למעלה שמאלה)
	if (j - 3 >= 0 && i - 3 >= 0)
	{
		if (board[i][j] == board[i - 1][j - 1] && board[i][j] == board[i - 2][j - 2] && board[i][j] == board[i - 3][j - 3])
			return True;
	}

	// Check for TOP - RIGHT (מהנקודה בודקים את האלכסון למעלה ימינה)
	if (j + 3 < column  && i - 3 >= 0)
	{
		if (board[i][j] == board[i - 1][j + 1] && board[i][j] == board[i - 2][j + 2] && board[i][j] == board[i - 3][j + 3])
			return True;
	}


	// Check for  BOTTOM - LEFT (מהנקודה בודקים את האלכסון למטה שמאלה)
	if (j - 3 >= 0 && i + 3 < rows)
	{
		if (board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 2][j - 2] && board[i][j] == board[i + 3][j - 3])
			return True;
	}


	// Check for BOTTOM - RIGHT (מהנקודה בודקים את האלכסון למטה ימינה)
	if (j + 3 < column && i + 3 < rows)
	{
		if (board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i][j] == board[i + 3][j + 3])
			return True;
	}

	return False;//אחרת מחזירים אפס כלומר לא נמצא ניצחון
}

int validName(char str[])//פונקציה שבודקת אם השם חוקי או לא
{
	if (str[0] >= 'a' && str[0] <= 'z')//אם האות הראשונה בשם היא אות קטנה אז מחזירים שהוא לא תקין
	{
		return 0;
	}
	for (int i = 1; str[i] != '\0'; i++)
	{
		if (!(str[i] >= 'a' && str[i] <= 'z'))//אם אחרי האות הראשונה בשם כל האותיות לא קטנות אלא כל דבר אחר אז זה מחזיר 0 ואומר שהשם לא תקין 
		{
			return 0;
		}
	}
	return 1;//אם כל התנאים מתקיימים והכל בסדר אז מחזירים 1
}

void FreeAll(Player* pPlayer, int** board, int rows, int colums)
{
	for (int i = 0; i < rows; i++)//לולאה שעוברת על כל שורה עד גודל השורות שנתון לנו מהמשתמש 
	{
		free(board[i]);//משחררים כל מצביע לתא שהקצאנו בבנית הלוח
	}
	free(board);//משחררים את כל ההקצאה של הלוח עצמו
	free(pPlayer);//משחררים את ההקצאה שעשינו למבנה
}
