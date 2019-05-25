#!/usr/bin/env python3

import os
import argparse
import re

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

parser = argparse.ArgumentParser('create-class.py')
parser.add_argument('type', choices=['state', 'component'])
parser.add_argument('name', help='Class name')
args = parser.parse_args()

class_type = args.type.capitalize()
class_name = args.name.capitalize()

if class_type == 'State':
    class_name = class_name + 'State'

hpp_template_path = os.path.join(BASE_DIR, '..', 'templates', f'{class_type}.hpp.template')
cpp_template_path = os.path.join(BASE_DIR, '..', 'templates', f'{class_type}.cpp.template')

hpp_path = os.path.join(BASE_DIR, '..', 'include', f'{class_name}.hpp')
cpp_path = os.path.join(BASE_DIR, '..', 'src', f'{class_name}.cpp')

with open(hpp_template_path, 'r') as file:
    data = file.read()
    data = data.replace('<CAMELCASENAME>', class_name)
    data = data.replace('<UPPERCASENAME>', class_name.upper())
    with open(hpp_path, 'w') as header_file:
        header_file.write(data)
        
with open(cpp_template_path, 'r') as file:
    data = file.read()
    data = data.replace('<CAMELCASENAME>', class_name)
    data = data.replace('<UPPERCASENAME>', class_name.upper())
    with open(cpp_path, 'w') as header_file:
        header_file.write(data)
