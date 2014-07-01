import os

import PIL
from PIL import Image, ImageDraw, ImageFont

sprite_size = 32

width  = 2048
height = 2048

spritesheet = Image.new('RGBA', (width, height), (0, 0, 0, 0))

index = 0

x = 0
y = 0

for f in os.listdir('sprites/png/numbered/'):

    if '.png' in f:

        sprite = Image.open('sprites/png/numbered/' + f)

        draw = ImageDraw.Draw(sprite)

        #font = ImageFont.load_default()
        font = ImageFont.truetype("Minecraftia.ttf", 8)

        #remove .png extension
        f = f[:-4]

        #remove leading zeros
        while f[0] == "0":
            f = f[1:]

        draw_x = 3
        draw_y = 0

        draw.text((draw_x+1, draw_y+1), f, font=font, fill='#000000')
        draw.text((draw_x-1, draw_y-1), f, font=font, fill='#000000')
        draw.text((draw_x+1, draw_y-1), f, font=font, fill='#000000')
        draw.text((draw_x-1, draw_y+1), f, font=font, fill='#000000')

        draw.text((draw_x+1, draw_y), f, font=font, fill='#000000')
        draw.text((draw_x-1, draw_y), f, font=font, fill='#000000')
        draw.text((draw_x, draw_y+1), f, font=font, fill='#000000')
        draw.text((draw_x, draw_y-1), f, font=font, fill='#000000')

        draw.text((draw_x, draw_y), f, font=font, fill='#FF00FF')

        del draw

        spritesheet.paste(sprite, (x, y))

        x += sprite_size

        if x == width:
            x = 0
            y += sprite_size

        index = index + 1
 
spritesheet.save('tiles_numbered.png')
