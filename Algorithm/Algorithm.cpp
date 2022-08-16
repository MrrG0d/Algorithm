#include <locale.h>
#include <ctime>
#include "windows.h"
#include <iostream>
using namespace std;


class Queue {
	// Счетчик операций
	
	// класс элемента очереди
	class node {
		// значение
		int key;
		// указатель на следующий элемент
		node* tail = NULL;
		// предоставление доступа к данным класса
		friend class Queue;
	};
private:
	// голова очереди
	node* head = NULL;
public:
	unsigned long long int N_op = 0;
	
	bool empty() { // 2
		return head == NULL;
	}
	
	// Добавление элемента в очередь
	void push(int key) { // (3 + 2) + (2 + 1) + (3n + 2) = 10 + 3n
		node* n = new node(); // 3
		n->key = key; // 2
		if (head == NULL) { // 2
			head = n; // 1
		}
		else {
			node* temp = head; // 1
			while (temp->tail != NULL) { // n*(2+1) = 3n
				temp = temp->tail; // 2
			}
			temp->tail = n; // 2
		}
	}

	// Удаление элемента очереди
	int pop() { // 1 + 2 + 2 + 1 = 6	
		node* tmp = head; // 1
		head = head->tail; // 2
		int key = tmp->key; // 2
		delete tmp;
		return key; // 1
	}

	// Вывод очереди
	void print() {
		int key = head->key;
		node* temp = head;
		cout << "stack state: {";
		while (temp != NULL) {
			cout << temp->key << ' ';
			temp = temp->tail;
		}
		cout << '}' << endl;
	}

	// Длина очереди
	int size() {
		int n = 0;
		node* temp = head;
		while (temp != NULL) {
			n += 1;
			temp = temp->tail;
		}
		return n;
	}
};

void scrolling(Queue& queue, Queue* C) { // 3n^2 + 16n + 1
	// переливаем отсавшиеся элементы в С
	while (!queue.empty()) { // 1
		C->push(queue.pop());
	}

	// возвращаем все элементы в исходную очередь
	while (!C->empty()) { // 15n + 3n^2
		queue.push(C->pop()); // 10 + 3n + 6 = 16 + 3n
	}

	delete C;
}

//Доступ к произвольному элементу очереди
int get(Queue &queue, int pos) { // 2 + 3n^2 + 20n + 4 + 1 + 9 + 3n + 3n^2 + 16n + 6 + 1 = 6n^2 + 39n + 23
	// определяем и инициализируем вспомогательную структуру того же типа
	Queue* C = new Queue(); // 2
	// переливае элементы до искомого из queue в С
	for (int i = 0; ((!queue.empty()) && (i < pos)); i++) { // 4 + n*(3n + 16 + 4) = 3n^2 + 20n + 4
		// берем элемент и добавляем в С
		C->push(queue.pop()); // 3n + 16
	}
	//запоминаем искомое значение
	int res = queue.pop(); // 6
	//добавляем в очередь искомое значение
	C->push(res); // 10 + 3n
	
	// формирование первоначального порядка в очереди
	scrolling(queue, C); // 3n^2 + 16n + 1

	return res; // 1
}

//
void set(Queue &queue, int pos, int n) { //2 + 3n^2 + 20n + 4 + 1 + 9 + 3n + 3n^2 + 16n + 1= 6n^2 + 39n + 22
	// определяем и инициализируем вспомогательную структуру того же типа
	Queue* C = new Queue(); // 2
	// переливае элементы до искомого из queue в С
	for (int i = 0; ((!queue.empty()) && (i < pos)); i++) { // 3n^2 + 20n + 4
		// берем элемент и добавляем в С
		C->push(queue.pop()); // 3n + 16
	}
	// значение, на место которого вставим новый элемент
	int res = queue.pop(); // 6
	//добавляем новое значение в очередь
	C->push(n); // 10 + 3n
	
	// формирование первоначального порядка в очереди
	scrolling(queue, C); // 3n^2 + 16n + 1
}

void siftDown(Queue &queue, int root, int bottom) // 1 + 
{
	// индекс максимального потомка
	int maxChild;
	// флаг того, что куча сформирована
	int done = 0; // 1
	// Пока не дошли до последнего ряда
	while ((root * 2 <= bottom) && (!done)) { // 2 * (4 + 7 + 12n^2 + 78n + 48 + 12n^2 + 78n + 48 + 12n^2 + 74n + 36 + 1) =
											  // = 2 * (36n^2 + 234n + 144) = 72n^2 + 234n + 144
		// если мы в последнем ряду
		if (root * 2 == bottom) { // 2 + 2 = 4
			// запоминаем левый потомок
			maxChild = root * 2; // 2
		}
		// иначе запоминаем больший потомок из двух
		else if (get(queue, root * 2) > get(queue, root * 2 + 1)) { // 1 + 6n^2 + 39n + 23 + 6n^2 + 39n + 22 + 2 = 12n^2 78n + 48
			maxChild = root * 2; // 2
		}
		else { // 3
			maxChild = root * 2 + 1; // 3
		}
		int n1 = get(queue, root); // 1 + 6n^2 + 39n + 23
		int n2 = get(queue, maxChild); // 1 + 6n^2 + 39n + 23
		// если элемент вершины меньше максимального потомка
		if (n1 < n2) { // 1 + 2 * (6n^2 + 37n + 17) + 1 = 12n^2 + 74n + 36
			// меняем их местами
			int temp = n1; // 1
			set(queue, root, n2);// 6n^2 + 39n + 22
			set(queue, maxChild, n1); // 6n^2 + 39n + 22
			root = maxChild; // 1
		}
		// иначе
		else { // 1
			// пирамида сформирована
			done = 1; // 1
		}
	}
}

// Функция сортировки на куче
void heapSort(Queue &queue, int array_size) //  36n^3 + 45n^2 - 141n - 142 + 96n^3 + 159n^2 - 488n - 425 = 132n^3 + 204n^2 -629n - 567
{
	// Формируем нижний ряд пирамиды
	for (int i = (array_size / 2) - 1; i >= 0; i--) { // 4 + (n/2-1)*(72n^2 + 234n + 144 + 2) = 36n^3 + 45n^2 - 141n - 142
		siftDown(queue, i, array_size - 1); // 72n^2 + 234n + 144
	}
	// Просеиваем через пирамиду остальные элементы
	for (int i = array_size - 1; i >= 1; i--) { // 3 + (n-2) * (6n^2 + 39n + 24 + 12n^2 + 78n + 44 + 72n^2 + 234n + 144 + 2) =
												// = 96n^3 + 159n^2 - 488n - 425
		int temp = get(queue, 0); // 1 + 6n^2 + 39n + 23
		set(queue, 0, get(queue, i)); // 6n^2 + 39n + 22
		set(queue, i, temp); // 6n^2 + 39n + 22
		siftDown(queue, 0, i - 1); // 72n^2 + 234n + 144
	}
}


int main()
{
	setlocale(LC_ALL, "ru");
	srand(time(NULL));

	// Схема эксперимента
	// Инициализация очереди и заполнение хранилища ключей
	int time_start, time_finish;
	// Хранилище ключей
	int Key[1500];
	int N = 150;
	Queue queue;
	
	for (int i = 0; i < N; i++)
	{
		// Заполнение хранилища случайными числами до 1000
		Key[i] = rand() % 999;
	}
	for (int i = 0; i < 10; i++)
	{
		for (int z = N - 150; z < N; z++)
		{
			queue.push(Key[z]);
		}
		time_start = GetTickCount64();
		heapSort(queue, N);
		time_finish = GetTickCount64();

		cout << "Номер сортировки: " << i + 1 << " Колличество отсортированных элементов: " << N << " Время сортировки (ms): "
			 << time_finish - time_start << " Колличество операций (N_op): " << queue.N_op << endl; // Шаг в 150 элементов

		N = N + 150;
	}
}