#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <cstdlib>

using namespace std;

//Прототипы функций
void PrintMatr(int X, int Y, double** A);//вывод матрицы 
bool TriangMatr(double** A, double** B, int n, int m, double& det);//треугольная матрица 
bool Solve(double** A, double** B, double* x, int n, int m, double& det);//Решение СЛАУ
double Delta(double** A, double* x, int n, int m);//определитель 
void SwitchStrings(double** B, int n, int m, int k);//замена строк 
bool DoppelStrings(double** B, int n, int m, int k);//Повторные строки(повторяющиеся)
int ZeroStrings(double** B, int n, int m, int k);//нулевая строка
void GetMass(int count1, int count2, double** arr);//вывод значений из файла 


int main() {
	srand(time(0));
	setlocale(LC_ALL, "rus");

	int X, Y;
	double** A;
	double** B;
	double* results;
	double* resbuf;
	short int elementmode;
	double det = 1;

	//Создание двумерного массива
	cout << "\nВведите размерность массива: ";
	cin >> X;
	Y = X + 1;

	A = new double* [X];
	for (int i = 0; i < X; i++) A[i] = new double[Y];

	B = new double* [X];
	for (int i = 0; i < X; i++) B[i] = new double[Y];

	results = new double[X];

	//Выбор режима ввода
	cout << "\nВыберите режим заполнения \n0 - с клавиатуры\n1 - случайные\n2 - из файла: ";
	cin >> elementmode;

	//Заполнение матрицы элементами
	if (elementmode < 2) {
		for (int i = 0; i < X; i++) for (int j = 0; j < Y; j++) {
			if (elementmode == 0) {
				cout << "\nВведите элемент [" << i + 1 << "][" << j + 1 << "]: ";
				cin >> A[i][j];
			}
			else if (elementmode == 1) A[i][j] = rand() % 50;
		}
	}
	else GetMass(X, Y, A);


	//Вывод элементов матрицы на экран
	PrintMatr(X, Y, A);

	for (int i = 0; i < X; i++) for (int j = 0; j < Y; j++) B[i][j] = A[i][j];
	//Устранение одиннаковых строк
	for (int i = X - 1; i >= 1; i--) {
		DoppelStrings(B, X, Y, i);
	}
	//Устранение нулевых строк
	for (int i = 0; i < X; i++) {
		int pos = ZeroStrings(B, X, Y, i);
		if (pos != -1) {
			//Заменяем нулевую строку
			for (int l = 0; l < Y; l++) {
				B[pos][l] = B[X - 1][l];
			}
			//Зануляем крайнюю строку
			for (int l = 0; l < Y; l++) {
				B[X - 1][l] = 0;
			}
			X--;
		}
	}
	while (ZeroStrings(B, X, Y, X - 1) != -1) {
		X--;
	}
	PrintMatr(X, Y, B);

	//Решение задач
	if (Solve(A, B, results, X, Y, det) == true)//если есть решение - заходим в условие 
	{
		cout << "Ответы: \n";
		for (int i = 0; i < X; i++) {
			cout << results[i];
			if (i != X - 1) cout << ", ";
			else cout << endl;
			//1) Ответ для СЛАУ 
			//2) Матрица правых частей 
		}

		cout << "\nПогрешность = " << Delta(A, results, X, Y) << endl;
		cout << "\nОпределитель = " << det << endl;
	}
	//Освобождение памяти
	for (int i = 0; i < X; i++) delete[] A[i];
	delete[] A;
	A = NULL;

	for (int i = 0; i < X; i++) delete[] B[i];
	delete[] B;
	B = NULL;

	delete[] results;
	results = NULL;

	return 0;
}

//Функции
void PrintMatr(int X, int Y, double** A) {
	cout << endl;
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			cout.width(5);//устанавливает ширину поля вывода
			cout << A[i][j] << ' ';
		}
		cout << endl;
	}
}

bool TriangMatr(double** A, double** B, int n, int m, double& det) 
{
	double koef;
	for (int k = 0; k < n; k++) 
		for (int i = k + 1; i < n; i++) 
	{
		if (B[k][k] == 0) SwitchStrings(B, n, m, k);
		koef = -1 * B[i][k] / B[k][k];
		for (int j = k; j < n + 1; j++) B[i][j] = B[i][j] + B[k][j] * koef;
	}
	for (int j = 0; j < n; j++) {
		det *= B[j][j];
	}

	//PrintMatr(n, n + 1, B);
	if (det == 0) return false;
	return true;
}

bool Solve(double** A, double** B, double* x, int n, int m, double& det) 
{
	double res = 0;
	if (TriangMatr(A, B, n, m, det) == true)//Проверка на треугольность матрицы 
	{
		for (int i = n - 1; i >= 0; i--) 
		{
			res = 0;
			for (int j = i + 1; j <= n - 1; j++)//Ищет вектор икс
			res = res - x[j] * B[i][j];
			res += B[i][m - 1];
			x[i] = res / B[i][i];
		}

		return true;
	}
	cout << "\nСистема не имеет единственного решения\n";
	return false;
}

double Delta(double** A, double* x, int n, int m) {
	double* y = new double[n];

	for (int i = 0; i < n; i++) {
		double res = 0;
		for (int j = 0; j < n; j++) {
			res += A[i][j] * x[j];
		}

		y[i] = res;
		cout << y[i] << ' ';
	}

	cout << endl;
	double max;
	max = fabs(y[0] - A[0][m - 1]);
	for (int i = 1; i < n; i++) {
		if (fabs(y[i] - A[i][m - 1]) > max) max = fabs(y[i] - A[i][m - 1]);
	}

	delete[] y;
	y = NULL;

	return max;
}

void SwitchStrings(double** B, int n, int m, int k) {
	double* buf = new double[m];
	//Нахождение строки с максимальным по модулю элементом
	int pos = k + 1;
	double max = 0;
	if (n - k > 1) {
		for (int i = k + 1; i < n; i++) {
			if (fabs(B[i][k]) > max) {
				max = fabs(B[i][k]);
				pos = i;
				cout << endl << max << endl;
			}
		}
	}
	//Перемещение строк
	//Буфер
	for (int l = 0; l < m; l++) {
		buf[l] = B[k][l];
	}
	//Заменяем k-тую строку
	for (int l = 0; l < m; l++) {
		B[k][l] = B[pos][l];
	}
	//Заменяем целевую строку
	for (int l = 0; l < m; l++) {
		B[pos][l] = buf[l];
	}

	PrintMatr(n, m, B);

	delete[] buf;
}

bool DoppelStrings(double** B, int n, int m, int k)//Повторные строки(повторяющиеся)
{
	for (int i = 0; i < k; i++) 
	{
		bool doppel = true;
		for (int j = 0; j < m - 1; j++) 
		{
			if (B[i][j] != B[k][j]) doppel = false;
		}
		if (doppel == true) 
		{
			for (int j = 0; j < m; j++) B[k][j] = 0;//Повторные строки приравниваются к нулю
			PrintMatr(n, m, B);
			return true;
		}
	}
	return false;
}

int ZeroStrings(double** B, int n, int m, int k) {
	for (int i = 0; i < m - 1; i++) {
		if (B[k][i] != 0) return -1;
	}
	return k;
}

void GetMass(int count1, int count2, double** arr) {
	ifstream f("Input3.txt");

	if (!f) {
		cout << "\nВходной файл не обнаружен, генерирую случайную матрицу";
		for (int i = 0; i < count1; i++) 
			for (int j = 0; j < count2; j++) 
		{
			arr[i][j] = rand() % 50;
		}
	}
	else {
		for (int i = 0; i < count1; i++) {
			for (int j = 0; j < count2; j++) {
				f >> arr[i][j];
			}
		}
	}

	f.close();
}