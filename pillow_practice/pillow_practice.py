from PIL import Image, ImageDraw

# Задача 1
def triangle(img, x0, y0, x1, y1, x2, y2, thickness, color, fill_color):
    
    drawing = ImageDraw.Draw(img)
    
    if fill_color != None:
        drawing.polygon(((x0, y0), (x1, y1), (x2, y2)), tuple(fill_color), tuple(color), thickness)
    else:
        drawing.polygon(((x0, y0), (x1, y1), (x2, y2)), None, tuple(color), thickness)
    
    return img

# Задача 2
def change_color(img, color):
    
    pixels = img.load()
    
    size = img.size

    dict_cnt_colors = {}

    for x in range(size[0]):
        for y in range(size[1]):
            if pixels[x, y] not in dict_cnt_colors:
                dict_cnt_colors[pixels[x, y]] = 0
            dict_cnt_colors[pixels[x, y]] += 1
    
    frequency_color = max(dict_cnt_colors, key=dict_cnt_colors.get)
    
    for x in range(size[0]):
        for y in range(size[1]):
            if pixels[x, y] == frequency_color:
                pixels[x, y] = tuple(color)
    
    return img
            
# Задача 3
def collage(img, N, M):

    img_size = img.size
    new_img = Image.new('RGB', (img_size[0] * M, img_size[1] * N), 'white')

    for x in range(0, new_img.size[0], img_size[0]):
        for y in range(0, new_img.size[1], img_size[1]):
            new_img.paste(img, (x, y))

    return new_img
            
