import numpy as np


def check_collision(bot1, bot2):

    ratio = np.array([
        [bot1[0], bot1[1]],
        [bot2[0], bot2[1]]
    ])

    free_term = np.array([-bot1[2], -bot2[2]])

    if np.linalg.matrix_rank(ratio) >= 2:
        solution = np.linalg.solve(ratio, free_term)
    else:
        return None

    return tuple(round(x, 2) for x in solution)


def check_surface(point1, point2, point3):

    ratio = np.array([
        [point1[0], point1[1], 1],
        [point2[0], point2[1], 1],
        [point3[0], point3[1], 1]
    ])

    free_term = np.array([point1[2], point2[2], point3[2]])

    if np.linalg.matrix_rank(ratio) >= 3:
        solution = np.linalg.solve(ratio, free_term)
    else:
        return None

    return np.array([round(x, 2) for x in solution])


def check_rotation(vec, rad):
    
    rotation_matrix = np.array([
        [np.cos(rad), -np.sin(rad), 0],
        [np.sin(rad), np.cos(rad), 0],
        [0, 0, 1]
    ])

    result = np.dot(rotation_matrix, vec)

    return np.array([round(x, 2) for x in result])

