class CircularDeque:
    def __init__(self, n: int, push_force=False):
        if n < 0:
            raise ValueError("Wrong deque capacity")
        self.deque = [0] * n
        self.push_force = push_force
        self.n = n
        self.cur_size = 0
        self.head = 0
        self.tail = 0

    def push_front(self, x: int):
        if self.full():
            if self.push_force and self.n != 0:
                self.tail = (self.n + (self.tail - 1)) % self.n
            else:
                raise OverflowError("Deque is overflowed")

        self.head = (self.n + (self.head - 1)) % self.n if self.cur_size else self.head
        self.deque[self.head] = x

        self.cur_size = min(self.n, self.cur_size+1)

    def push_back(self, x: int):
        if self.full():
            if self.push_force and self.n != 0:
                self.head = (self.head + 1) % self.n
            else:
                raise OverflowError("Deque is overflowed")

        self.tail = (self.tail + 1) % self.n if self.cur_size else self.tail
        self.deque[self.tail] = x

        self.cur_size = min(self.n, self.cur_size + 1)

    def pop_front(self) -> int:
        if self.cur_size != 0:
            popped_head = self.deque[self.head]
            self.deque[self.head] = 0
            self.cur_size -= 1
            if self.cur_size != 0:
                self.head = (self.head + 1) % self.n
            else:
                self.tail = 0
                self.head = 0
            return popped_head
        else:
            raise IndexError("Deque is empty")

    def pop_back(self) -> int:
        if self.cur_size != 0:
            popped_tail = self.deque[self.tail]
            self.deque[self.tail] = 0
            self.cur_size -= 1
            if self.cur_size != 0:
                self.tail = (self.n + (self.tail - 1)) % self.n
            else:
                self.tail = 0
                self.head = 0
            return popped_tail
        else:
            raise IndexError("Deque is empty")

    def front(self) -> int:
        if self.empty():
            raise IndexError("Deque is empty")
        return self.deque[self.head]

    def back(self) -> int:
        if self.empty():
            raise IndexError("Deque is empty")
        return self.deque[self.tail]

    def size(self) -> int:
        return self.cur_size

    def empty(self) -> bool:
        return True if self.cur_size == 0 else False

    def full(self):
        return True if self.cur_size == self.n else False

    def resize(self, new_cap: int):
        if new_cap > 0:
            new_deque = [0] * new_cap
            head = self.head
            tail = self.tail

            if new_cap < self.cur_size:
                tail = new_cap - 1
                self.cur_size = new_cap
            elif new_cap >= self.cur_size:
                tail = self.cur_size - 1

            if self.cur_size == 0:
                self.head = 0
                self.tail = 0
                self.n = new_cap
                self.deque = [0] * new_cap
                return

            for i in range(tail):
                new_deque[i] = self.deque[head]
                head = (head + 1) % self.n

            new_deque[tail] = self.deque[head]

            self.head = 0
            self.tail = tail
            self.n = new_cap
            self.deque = new_deque

        elif new_cap == 0:
            self.head = 0
            self.tail = 0
            self.cur_size = 0
            self.n = 0
            self.deque = []

        else:
            raise ValueError("Wrong new capacity")

