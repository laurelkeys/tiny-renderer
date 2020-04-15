import os
import sys

if __name__ == "__main__":
    root = os.path.dirname(os.path.abspath(__file__))
    obj_name = sys.argv[1]
    for filename in os.listdir(root):
        if obj_name in filename:
            if "_diffuse" in filename:
                print(f"'{filename}' swapped for 'test_grid_diffuse.tga'")
                os.remove(os.path.join(root, filename))
            else:
                os.rename(
                    os.path.join(root, filename),
                    os.path.join(root, filename.replace(obj_name, "test_grid"))
                )