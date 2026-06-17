'''Kiseleva_Alina_lb2'''

class Node:
    '''Узел однонаправленного связного списка.
    Поля:
        data: данные элемента;
        next: ссылка на следующий элемент.
    '''

    def __init__(self, data, next=None):
        '''Инициализация узла однонаправленного списка.
           Сложность О(1)'''
        self.__data = data
        self.next = next

    def get_data(self):
        '''Возвращает значение поля data.
           Сложность О(1)'''
        return self.__data

    def __str__(self):
        '''Преобразование к строке.
           Сложность О(1)'''
        next_data = self.next.get_data() if self.next else None
        return f'data: {self.__data}, next: {next_data}'


class LinkedList:
    '''Класс, который описывает связный однонаправленный список.
    Поля:
        head: данные первого элемента списка.
        length: количество элементов в списке.
    '''

    def __init__(self, head=None):
        self.head = head
        self.length = 0
        if self.head:
            self.head = Node(head)
            self.length = 1

    def __len__(self):
        '''Вычисление длины списка.
           Сложность О(1)'''
        return self.length

    def append(self, element):
        '''Добавление элемента в конец списка.
           Сложность О(n) - проходка по всем элементам списка.'''
        if self.head:
            last_node = self.head
            while last_node.next:
                last_node = last_node.next
            last_node.next = Node(element)
        else:
            self.head = Node(element)
        self.length += 1

    def __str__(self):
        '''Преобразование к строке.
           Сложность О(n) - проходка по всем элементам списка.'''
        res = 'LinkedList[]'
        if self.head:
            res = f'LinkedList[length = {len(self)}, ['
            last_node = self.head
            while last_node:
                data = last_node.get_data()
                nxt = last_node.next.get_data() if last_node.next else None
                res += f'data: {data}, next: {nxt}; '
                last_node = last_node.next
            res = res[:-2] + ']]'
        return res

    def pop(self):
        '''Удаление последнего элемента.
           Сложность О(n) - проходка по всем элементам списка.'''
        if self.head:
            if len(self) == 1:
                self.head = None
            else:
                prev_last_node = self.head
                while prev_last_node.next.next:
                    prev_last_node = prev_last_node.next
                prev_last_node.next = None
            self.length -= 1
        else:
            raise IndexError('LinkedList is empty!')

    def clear(self):
        '''Очищение списка.
           Сложность О(n) - проходка по всем элементам списка.'''
        if self.head:
            while self.head:
                self.head = self.head.next
        self.length = 0

    def delete_on_end(self, n):
        '''Удаление n-того элемента с конца списка.
           Сложность О(n) - проходка по всем элементам списка.'''
        if len(self) < n or n <= 0:
            raise KeyError("Element doesn't exist!")
        if n == len(self):
            self.head = self.head.next
        else:
            n = len(self) - n
            cnt = 1
            prev_node = self.head
            while cnt < n:
                prev_node = prev_node.next
                cnt += 1
            prev_node.next = prev_node.next.next
        self.length -= 1
