import pytest
from lab_1 import CircularDeque


def test_init():
    with pytest.raises(ValueError):
        CircularDeque(-1, False)


def test_push_front_in_empty_deque():
    deq = CircularDeque(0, True)
    with pytest.raises(OverflowError):
        deq.push_front(1)


def test_push_front_with_push_force_false():
    deq = CircularDeque(1, False)
    deq.push_front(1)
    with pytest.raises(OverflowError):
        deq.push_front(2)


def test_push_front_basic():
    deq = CircularDeque(2, True)
    deq.push_front(1)
    deq.push_front(2)
    deq.push_front(3)
    assert deq.cur_size == deq.n


def test_push_back_in_empty_deque():
    deq = CircularDeque(0, True)
    with pytest.raises(OverflowError):
        deq.push_back(1)


def test_push_back__with_push_force_false():
    deq = CircularDeque(1, False)
    deq.push_back(1)
    with pytest.raises(OverflowError):
        deq.push_back(2)


def test_push_back_basic():
    deq = CircularDeque(1, True)
    deq.push_back(1)
    deq.push_back(2)
    assert deq.cur_size == deq.n


def test_pop_front():
    deq = CircularDeque(1, False)
    with pytest.raises(IndexError):
        deq.pop_front()

    deq.push_back(1)
    deq.pop_front()
    assert deq.cur_size == 0


def test_pop_back():
    deq = CircularDeque(1, False)
    with pytest.raises(IndexError):
        deq.pop_back()

    deq.push_back(1)
    deq.pop_front()
    assert deq.cur_size == 0


def test_front_and_back():
    deq = CircularDeque(2, True)
    deq.push_back(1)
    assert deq.front() == 1
    assert deq.back() == 1
    deq.push_back(2)
    assert deq.front() == 1
    assert deq.back() == 2


def test_front_and_back_in_empty_deque():
    deq = CircularDeque(1, True)
    with pytest.raises(IndexError):
        deq.front()
        deq.back()


def test_size():
    deq = CircularDeque(1, True)
    assert deq.size() == 0
    deq.push_front(1)
    assert deq.size() == 1
    deq.pop_front()
    assert deq.size() == 0


def test_empty():
    deq = CircularDeque(1, True)
    assert deq.empty()
    deq.push_back(1)
    assert not deq.empty()


def test_full():
    deq = CircularDeque(1, True)
    assert not deq.full()
    deq.push_back(1)
    assert deq.full()


def test_resize_from_null():
    deq = CircularDeque(0, True)
    deq.resize(3)
    assert deq.n == 3


def test_resize_more():
    deq = CircularDeque(2, True)
    deq.push_front(1)
    deq.push_front(2)
    head_before = deq.front()
    tail_before = deq.back()
    deq.resize(4)
    assert deq.front() == head_before and deq.back() == tail_before
    assert deq.deque[deq.tail + 1] == 0


def test_resize_less():
    deq = CircularDeque(2, True)
    deq.push_front(1)
    deq.push_front(2)
    head_before = deq.front()
    tail_before = deq.tail
    size_before = deq.size()
    deq.resize(1)
    assert deq.front() == head_before and deq.back() == deq.deque[tail_before-1]
    assert deq.size() == size_before - 1


def test_resize_to_null():
    deq = CircularDeque(2, True)
    deq.push_front(1)
    deq.push_front(2)
    deq.resize(0)
    assert deq.size() == 0 and deq.n == 0


def test_push_pop_push():
    deq = CircularDeque(3, True)
    for i in range(3):
        deq.push_back(i)
    deq_before = deq.deque
    for i in range(3):
        deq.pop_back()
    for i in range(3):
        deq.push_back(i)
    assert deq.deque == deq_before


def test_push_force_false():
    deq = CircularDeque(2, False)
    deq.push_front(1)
    deq.push_front(2)
    with pytest.raises(OverflowError):
        deq.push_back(3)


def test_push_force_true():
    deq = CircularDeque(2, True)
    deq.push_front(1)
    deq.push_front(2)
    deq.push_back(3)
    assert deq.size() == 2 and deq.deque == [1, 3]
    assert deq.front() == 1

