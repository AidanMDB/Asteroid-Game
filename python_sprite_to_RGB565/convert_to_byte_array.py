from PIL import Image


def create_16bit_array(filepath):


    img = Image.open(filepath).convert('RGB')
    width, height = img.size
    pixels = img.load()
    print(width)
    print(height)

    with open('array.txt', 'w') as f:
        f.write("{ \\\n")
        for y in range(height):
            for x in range(width):
                r,g,b = pixels[x,y]
                r5 = (r >> 3) & 0x1F
                g6 = (g >> 2) & 0x3F
                b5 = (b >> 3) & 0x1F
                high_byte = (r5 << 3) | (g6 >> 3)
                low_byte = ((g6 & 0x07) << 5) | b5
                f.write(f"0x{high_byte:02X},0x{low_byte:02X}, ")
            f.write(" \\\n")
        f.write("}")



def main():
    create_16bit_array("laser.bmp")



if __name__ == "__main__":
    main()