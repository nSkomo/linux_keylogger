import os
import sys
import time

def touch_recursive(directory):
    """
    Recursively touches (updates modification time) all files in the given directory.
    """
    for root, _, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            try:
                os.utime(file_path, times=None)
            except Exception as e:
                print(f"Error touching {file_path}: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python touch_recursive.py <directory>")
        sys.exit(1)

    directory = sys.argv[1]
    if not os.path.isdir(directory):
        print(f"Error: {directory} is not a valid directory.")
        sys.exit(1)

    touch_recursive(directory)
