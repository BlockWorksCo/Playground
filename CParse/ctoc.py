import sys

from pycparser import parse_file, c_parser, c_generator, c_ast




class FuncCallVisitor(c_ast.NodeVisitor):
    def __init__(self, funcname):
        self.funcname = funcname

    def visit_FuncCall(self, node):
        if node.name.name == self.funcname:
            print('%s called at %s' % (
                    self.funcname, node.name.coord))





def translate_to_c(filename):
    """ Simply use the c_generator module to emit a parsed AST.
    """
    ast = parse_file(filename, use_cpp=True)


    FuncCallVisitor('BlaaOne').visit(ast)

    generator = c_generator.CGenerator()
    print(generator.visit(ast))


def _zz_test_translate():
    # internal use
    parser = c_parser.CParser()
    ast = parser.parse(src)
    ast.show()
    generator = c_generator.CGenerator()

    print(generator.visit(ast))

    # tracing the generator for debugging
    #~ import trace
    #~ tr = trace.Trace(countcallers=1)
    #~ tr.runfunc(generator.visit, ast)
    #~ tr.results().write_results()


#------------------------------------------------------------------------------
if __name__ == "__main__":
    #_zz_test_translate()
    if len(sys.argv) > 1:
        translate_to_c(sys.argv[1])
    else:
        print("Please provide a filename as argument")






