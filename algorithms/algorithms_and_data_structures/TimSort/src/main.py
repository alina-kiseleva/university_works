class Stack:
    def __init__(self):
        self.length = 0
        self.stack = []

    def push(self, item):
        self.stack.append(item)
        self.length += 1

    def pop(self):
        if not self.length:
            return 0
        removed = self.stack.pop()
        self.length -= 1
        return removed

    def top(self):
        return self.stack[-1]


def get_minrun(n: int) -> int:
    flag = 0
    while n >= 16:
        flag |= n & 1
        n >>= 1
    return n + flag


def divide_into_runs(arr: list, minrun: int) -> list:
    runs = []
    cur_run = []
    n = len(arr)
    sort_flag = 0
    ind_run = 0
    i = 0

    while i < n:
        while i < n - 1 and abs(arr[i]) < abs(arr[i + 1]) and sort_flag != -1:
            cur_run.append(arr[i])
            sort_flag = 1
            i += 1
        while i < n - 1 and abs(arr[i]) >= abs(arr[i + 1]) and sort_flag != 1:
            cur_run.append(arr[i])
            sort_flag = -1
            i += 1
        cur_run.append(arr[i])

        if sort_flag == 1:
            cur_run.reverse()

        while i < n - 1 and len(cur_run) < minrun:
            i += 1
            cur_run.append(arr[i])

        cur_run = insert_sort(cur_run)
        runs.append(cur_run)
        print(f"Part {ind_run}:", *cur_run)
        i += 1
        ind_run += 1
        cur_run = []
        sort_flag = 0

    return runs


def insert_sort(arr: list) -> list:
    arr_length = len(arr)
    i = 1
    while i < arr_length:
        j = i
        while j > 0 and abs(arr[j]) > abs(arr[j - 1]):
            cur = arr[j - 1]
            arr[j - 1] = arr[j]
            arr[j] = cur
            j -= 1
        i += 1
    return arr


def bsearch(arr: list, target: int) -> int:
    left = 0
    right = len(arr)
    while left < right:
        mid = (left + right) // 2
        if abs(arr[mid]) < abs(target):
            right = mid
        else:
            left = mid + 1
    return right


def galloping_merge(arr1: list, arr2: list):
    rez = []
    first_cnt = 0
    second_cnt = 0
    first_ind = 0
    second_ind = 0
    cnt_gallops = 0

    while first_ind < len(arr1) and second_ind < len(arr2):
        if abs(arr1[first_ind]) >= abs(arr2[second_ind]):
            rez.append(arr1[first_ind])
            first_ind += 1
            first_cnt += 1
            second_cnt = 0
        else:
            rez.append(arr2[second_ind])
            second_ind += 1
            second_cnt += 1
            first_cnt = 0

        if first_cnt >= 3 and second_ind < len(arr2):
            found_ind = bsearch(arr1[first_ind:], arr2[second_ind]) + first_ind
            rez.extend(arr1[first_ind:found_ind])
            first_ind = found_ind
            first_cnt = 0
            cnt_gallops += 1

        if second_cnt >= 3 and first_ind < len(arr1):
            found_ind = bsearch(arr2[second_ind:], arr1[first_ind]) + second_ind
            rez.extend(arr2[second_ind:found_ind])
            second_ind = found_ind
            second_cnt = 0
            cnt_gallops += 1

    rez.extend(arr1[first_ind:])
    rez.extend(arr2[second_ind:])

    return rez, cnt_gallops


def merge_order(blocks: list) -> list:
    stack = Stack()
    cnt_merges = 0

    for block in blocks:
        stack.push(block)
        while stack.length >= 2:
            if stack.length == 2:
                X = stack.pop()
                Y = stack.pop()
                if len(Y) <= len(X):
                    merged_blocks, cnt_gallops = galloping_merge(Y, X)
                    stack.push(merged_blocks)
                    print(f"Gallops {cnt_merges}:", cnt_gallops)
                    print(f"Merge {cnt_merges}:", *stack.top())
                    cnt_merges += 1
                else:
                    stack.push(Y)
                    stack.push(X)
                    break

            elif stack.length >= 3:
                X = stack.pop()
                Y = stack.pop()
                Z = stack.pop()
                if len(Z) <= len(Y) + len(X) or len(Y) <= len(X):
                    if len(Z) > len(X):
                        stack.push(Z)
                        merged_blocks, cnt_gallops = galloping_merge(Y, X)
                        stack.push(merged_blocks)
                        print(f"Gallops {cnt_merges}:", cnt_gallops)
                        print(f"Merge {cnt_merges}:", *stack.top())

                    else:
                        merged_blocks, cnt_gallops = galloping_merge(Y, Z)
                        stack.push(merged_blocks)
                        print(f"Gallops {cnt_merges}:", cnt_gallops)
                        print(f"Merge {cnt_merges}:", *stack.top())
                        stack.push(X)

                    cnt_merges += 1

                else:
                    stack.push(Z)
                    stack.push(Y)
                    stack.push(X)
                    break

    while stack.length != 1:
        if stack.length == 2:
            X = stack.pop()
            Y = stack.pop()
            merged_blocks, cnt_gallops = galloping_merge(Y, X)
            stack.push(merged_blocks)
            print(f"Gallops {cnt_merges}:", cnt_gallops)
            print(f"Merge {cnt_merges}:", *stack.top())
            cnt_merges += 1

        elif stack.length >= 3:
            X = stack.pop()
            Y = stack.pop()
            Z = stack.pop()

            if len(Z) > len(X):
                stack.push(Z)
                merged_blocks, cnt_gallops = galloping_merge(Y, X)
                stack.push(merged_blocks)
                print(f"Gallops {cnt_merges}:", cnt_gallops)
                print(f"Merge {cnt_merges}:", *stack.top())

            else:
                merged_blocks, cnt_gallops = galloping_merge(Y, Z)
                stack.push(merged_blocks)
                print(f"Gallops {cnt_merges}:", cnt_gallops)
                print(f"Merge {cnt_merges}:", *stack.top())
                stack.push(X)

            cnt_merges += 1

    return stack.pop()

def TimSort(arr: list) -> list:
    minrun = get_minrun(len(arr))
    runs = divide_into_runs(arr, minrun)
    res = merge_order(runs)
    return res

n = int(input())
arr = list(map(int, input().split()))
ans = TimSort(arr)
print("Answer:", *ans)
