#!/usr/bin/python3

import re
import sys

def remove_preprocessor_directives(file_path, output_file):
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()

        # Remove nested preprocessor directives (supporting #if, #ifndef, #ifdef)
        content = ''.join(lines)
        content = re.sub(r'#(if|ifndef|ifdef).*?#endif', '', content, flags=re.DOTALL)

        # Second pass to remove any remaining occurrences of #endif
        content = re.sub(r'#endif', '', content)

        # Reduce multiple consecutive empty lines to a single empty line
        content = re.sub(r'\n{3,}', '\n\n', content)

        # Write the modified content to the output file
        with open(output_file, 'w') as file:
            file.write(content)
        
        print(f"Preprocessor directives removed from {file_path}. Cleaned content saved to {output_file}")
    except FileNotFoundError:
        print(f"File not found: {file_path}")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <input_file_path> <output_file_path>")
    else:
        input_file = sys.argv[1]
        output_file = sys.argv[2]
        remove_preprocessor_directives(input_file, output_file)

