from PIL import Image
import os

current_path = os.path.abspath(__file__)

# img to bmp
def img2bmp(jpg_path, bmp_path):
    img = Image.open(jpg_path)
    img.save(bmp_path)
    
if __name__ == '__main__':
    img_path = input('Please input the path of img: ')
    img_path = os.path.join(os.path.dirname(current_path), img_path)
    bmp_path = input('Please input the path of bmp: ')
    bmp_path = os.path.join(os.path.dirname(current_path), bmp_path)
    img2bmp(img_path, bmp_path)
    print('Convert jpg to bmp successfully!')