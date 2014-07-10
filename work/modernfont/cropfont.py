import PIL
from PIL import Image

im = Image.open('font.bmp')
im_width, im_height = im.size

char_width  = 16
char_height = 16

k = 0

for i in range(0, im_height, char_height):
    for j in range(0, im_width, char_width):
        char_box = (j, i, j + char_width, i + char_height)
        char = im.crop(char_box).save('images/' + str(k) + '.png')
        k +=1
