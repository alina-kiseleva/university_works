import pytest
from main import *

def test_decreasing_arr():
    arr = [20, -19, -18, -17, 16, 15, 13, -12, 10, -8, 7, 5, -4, 3, 2, 1]
    assert TimSort(arr) == [20, -19, -18, -17, 16, 15, 13, -12, 10, -8, 7, 5, -4, 3, 2, 1]

def test_increasing_arr():
    arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18]
    assert TimSort(arr) == [18, 17, 16, 15, 14, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1]

def test_stability():
    arr = [-1, 1, -1, 1, 2, 0, -10, -7, -7, 8, 6, 6, -6, 6, -6, -6, -6]
    assert TimSort(arr) == [-10, 8, -7, -7, 6, 6, -6, 6, -6, -6, -6, 2, -1, 1, -1, 1, 0]

def test_huge_arr():
    arr = [5, 5, 0, -4, 7, 2, -3, -4, 0, 5, 7, 8, 2, 7, -4, 4, 7, 0, 4, -2, 3, -2, -6, 6, 0, 8, -3, 4, -4, -6, 8, -4, -3, -4, 2]
    assert TimSort(arr) == [8, 8, 8, 7, 7, 7, 7, -6, 6, -6, 5, 5, 5, -4, -4, -4, 4, 4, 4, -4, -4, -4, -3, 3, -3, -3, 2, 2, -2, -2, 2, 0, 0, 0, 0]

def test_single_arr():
    arr = [11]
    assert TimSort(arr) == [11]

def test_parts():
    arr = [1, -2, 3, -4, 5, 6, -7, -8, 9, -10, 11, -10, -9, 8, 7, -7, -6, 6, 5, 4]
    parts = divide_into_runs(arr, get_minrun(len(arr)))
    assert len(parts) == 2
    assert len(parts[0]) == 11
    assert len(parts[1]) == 9

def test_gallop_merge():
    arr1 = [20, 19, 18, 1, 0]
    arr2 = [21, 17, 16, 2, -2, -1]
    expect_arr, cnt_gallops = galloping_merge(arr1, arr2)
    assert expect_arr == [21, 20, 19, 18, 17, 16, 2, -2, -1, 1, 0]
    assert cnt_gallops == 2
