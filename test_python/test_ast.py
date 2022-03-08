#/usr/bin/env python3

import ast


class CodeVisitor(ast.NodeVisitor):

    def generic_visit(self, node):
        print('Generic visit ' + type(node).__name__)
        ast.NodeVisitor.generic_visit(self, node)

    def visit_FunctionDef(self, node):
        print('Fuction ' + type(node).__name__)
        ast.NodeVisitor.generic_visit(self, node)

    def visit_Assign(self, node):
        print('Assign ' + type(node).__name__)
        ast.NodeVisitor.generic_visit(self, node)

    def visit_Call(self, node):
        print('Call ' + type(node).__name__)
        ast.NodeVisitor.generic_visit(self, node)


#root_node = ast.parse('a + b')
root_node = ast.parse('Prev(1, 2)')

for field in ast.iter_fields(root_node):
    print(field)

print()

for field in ast.iter_child_nodes(root_node):
    print(field)

print()

visitor = CodeVisitor()
visitor.visit(root_node)
