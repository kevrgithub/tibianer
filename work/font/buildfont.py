import PIL
from PIL import Image

char_width  = 18
char_height = 19

ascii_columns = 16
ascii_rows    = 6

ascii_num_chars = 96

im_width  = char_width  * ascii_columns
im_height = char_height * ascii_rows

im = Image.new('RGB', (im_width, im_height), (255, 0, 255))

x = 0
y = 0

for i in range(0, ascii_num_chars):

    im_char = Image.open('ascii/' + str(i) + '.png')

    im.paste(im_char, (x, y))

    x += char_width

    if x == im_width:
        x = 0
        y += char_height

im.save('ascii.bmp')
