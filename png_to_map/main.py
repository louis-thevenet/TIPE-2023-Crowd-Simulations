from PIL import Image

def convert_image_to_text(image_path, output_path):
    image = Image.open(image_path)
    width, height = image.size

    with open(output_path, 'w') as file:
        for y in range(height):
            for x in range(width):
                pixel = image.getpixel((x, y))[3]
                if pixel > 128:
                    file.write('1 ')
                else:
                    file.write('0 ')

            # Write a new line after each row
            file.write('\n')

# Example usage
image_path = 'image.png'
output_path = '../maps/output.txt'
convert_image_to_text(image_path, output_path)

