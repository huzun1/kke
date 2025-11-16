import os
import subprocess


def is_cpp_file(filename):
    return filename.endswith((".cpp", ".hpp", ".h", ".cxx", ".cc"))


def format_all_cpp_files(root_dirs):
    for root_dir in root_dirs:
        for dirpath, dirnames, filenames in os.walk(root_dir):
            for filename in filenames:
                if is_cpp_file(filename):
                    filepath = os.path.join(dirpath, filename)
                    print(f"Formatting: {filepath}")
                    subprocess.run(
                        ["clang-format", "-i", "--style=file", filepath], check=True
                    )


if __name__ == "__main__":
    format_all_cpp_files(["src", "includes"])
