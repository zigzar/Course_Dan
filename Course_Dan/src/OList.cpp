﻿#pragma once
#include "../include/OList.h"
#include "../include/RBST.h"
#include <iomanip> // форматирование таблицы

List::Node::Node(Newspaper& value, Node* n)
{
	data = value;
	next = n;
}

List::Node::Node(Node* n)
{
	data = Newspaper();
	data.input();
	next = n;
}


List::List()
{
	size = 0;
	head = nullptr;
}


List::~List()
{
	clear();
}

Newspaper& List::getElem(const int index)
{
	int counter = 0;

	Node* current = this->head;

	while (current != nullptr)
	{
		if (counter == index)
		{
			return current->data;
		}
		current = current->next;
		counter++;
	}
}

void List::pushBack(Newspaper& value)
{
	if (head == nullptr)
		head = new Node(value);
	else
	{
		for (Node* current = head; ; current = current->next)
			if (current->next == nullptr)
			{
				current->next = new Node(value);
				break;
			}
	}
	size++;
}

void List::pushFront(Newspaper& value)
{
	head = new Node(value, head);
	size++;
}


int List::getSize() const
{
	return size;
}


void List::popFront()
{
	if (size <= 0) return;
	Node* temp = head;
	if (head != nullptr) head = head->next;
	delete temp;
	size--;
}


void List::popBack()
{
	removeAt(size - 1);
}


void List::clear()
{
	while (size) popFront();
}


void List::insert(int index, Newspaper& value)
{
	if (index > size || index < 0)
	{
		string message = "Недопустимый индекс ";
		message.append(to_string(index));
		throw out_of_range(message);
	}
	if (index == 0) pushFront(value);
	else
	{
		Node* previous = head;
		for (int i = 0; i < index - 1; i++)
			previous = previous->next;
		previous->next = new Node(value, previous->next);
		size++;
	}
}


void List::removeAt(int index)
{
	if (index > size - 1 || index < 0)
	{
		string message = "Недопустимый индекс ";
		message.append(to_string(index));
		throw out_of_range(message);
	}
	if (index == 0) popFront();
	else
	{
		Node* previous = head;
		for (int i = 0; i < index - 1; i++)
			previous = previous->next;
		Node* temp = previous->next;
		previous->next = temp->next;
		delete temp;
		size--;
	}
}

void List::show() const
{
	system("CLS");
	showHeaders();
	Node* current = head;
	int counter = 1;
	while (current != nullptr)
	{
		cout << setw(3) << counter;
		current->data.show();
		current = current->next;
		counter++;
	}
}

void List::save()
{
	ofstream fout;
	fout.open("data.txt");
	fout.close();
	try
	{
		fout.open("data.txt", ofstream::app);
		Node* tmp;
		tmp = this->head;
		while (tmp != nullptr)
		{
			fout.write((char*)&tmp->data, sizeof(Newspaper));  // Записать объект класса газеты в текстовый файл
			tmp = tmp->next;
		}
	}
	catch (const std::exception&)
	{
		cerr << "Ошибка записи файла.\n";
		system("pause");
	}
	fout.close();
	system("CLS");
	cout << "База данных успешно сохранена!\n";
}

void List::load()
{
	ifstream fin;
	Newspaper current;
	try
	{
		fin.open("data.txt");
		this->clear();
		while (fin.read((char*)&current, sizeof(Newspaper)))  // Считывать объекты классов, пока файл не закончится
		{
			this->pushBack(current);
		}
		fin.close();
	}
	catch (const std::exception&)
	{
		cerr << "Ошибка записи файла.\n";
	}
	system("CLS");
	cout << "База данных успешно загружена!\n";
}

void List::edit()
{
	show();
	int number;
	cout << "Введите номер строки для редактирования: ";
	cin >> number;
	number--;
	int counter = 0;

	Node* current = this->head;

	while (current != nullptr)
	{
		if (counter == number)
		{
			current->data.input();
		}
		current = current->next;
		counter++;
	}
}

void List::showHeaders() const
{
	cout << setw(3) << left << "№"
		<< setw(26) << "Название газеты"
		<< setw(17) << "Индекс издания"
		<< setw(20) << "Фамилия"
		<< setw(40) << "ФИО редактора"
		<< setw(7) << "Цена"
		<< "\n";
}

void List::add()
{
	Node* tmp = new Node();
	Node* current = this->head;
	while (current != nullptr)
	{
		current = current->next;
	}
	pushBack(tmp->data);
}

void List::remove()
{
	system("CLS");
	show();
	cout << "Введите номер строки для удаления (0 - вернуться): ";
	int number;
	cin >> number;
	if (!number) return;
	removeAt(number - 1);
	system("CLS");
	cout << "Строка №" << number << " удалена.\n";
}

void List::sortInsert()
{
	for (int i = 1; i < this->getSize(); i++) // Начинаем сортировать со 2-го элемента, т.к. 1-й элемент по умолчанию отсортирован
	{
		for (int j = i; j > 0 && this->getElem(j - 1).price > this->getElem(j).price; j--)
		{
			swap(this->getElem(j - 1), this->getElem(j));
		}
	}
}


void List::sortSelect()
{
	for (int i = 0; i < this->getSize() - 1; ++i)
	{
		int minIndex = i;

		for (int j = i + 1; j < this->getSize(); ++j)
		{
			if (this->getElem(j).price < this->getElem(minIndex).price)
				minIndex = j;
		}

		swap(this->getElem(i), this->getElem(minIndex));
	}
}

void List::sortBubble()
{
	for (int i = 0; i < this->getSize(); i++)
	{
		for (int j = 0; j < this->getSize() - i - 1; j++)
		{
			if (this->getElem(j).price > this->getElem(j + 1).price)
			{
				swap(this->getElem(j + 1), this->getElem(j));
			}
		}
	}
}

void List::search()
{
	int key = 0;
	system("CLS");
	cout << "Введите индекс газеты для поиска (натуральное число)." << endl;
	cout << "Укажите \"-1\", чтобы вернуться назад." << endl;
	cout << "Ваш индекс: ";
	cin >> key;
	cout << endl << "Результат поиска:" << endl;

	if (key == -1) return;

	TreeNode* tree = nullptr;
	Node* current = head;
	while (current != nullptr)
	{
		Newspaper* data = &current->data;
		tree = tree->insert(tree, data);
		current = current->next;
	}

	TreeNode* node = tree->find(tree, key);
	if (node) 
	{
		showHeaders();
		cout << setw(3) << "1";
		node->key->show();
	}
	else
	{
		cout << "Указанного индекса не существует" << endl;
	}

}
