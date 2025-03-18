import re
from enum import Enum
import LexAnalyzer
import copy as cp


class VN(Enum):
    emptypro = -1000
    A = -999
    M = -998
    N = -997
    P = -996
    add_expression = -995
    argument_list = -994
    assign_sentence = -993
    declare = -992
    declare_list = -991
    expression = -990
    factor = -989
    function_declare = -988
    if_sentence = -987
    inner_declare = -986
    inner_var_declare = -985
    item = -984
    param = -983
    parameter = -982
    parameter_list = -981
    return_sentence = -980
    sentence = -979
    sentence_block = -978
    sentence_list = -977
    var_declare = -976
    while_sentence = -975

VT_names = [
    "Null",
    "Integer",
    "String",
    "Boolean",
    "Floating",
    "Character",
    "Comment",
    "Macro",
    "WhiteSpace",
    "EndLine",
    # 运算符
    "{",
    "}",
    "[",
    "]",
    "(",
    ")",
    "+",
    "-",
    "*",
    "/",
    "%",
    "++",
    "--",
    "==",
    "!=",
    ">",
    "<",
    ">=",
    "<=",
    "&&",
    "||",
    "!",
    "&",
    "|",
    "~",
    "^",
    "<<",
    ">>",
    "=",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
    "<<=",
    ">>=",
    "&=",
    "|=",
    "^=",
    ",",
    ";",
    # 关键字
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
    "Identifier",
    "Word"
]

VN_names = ["emptypro",
            "A",
            "P",
            "M",
            "N",
            "add_expression",
            "argument_list",
            "assign_sentence",
            "declare",
            "declare_list",
            "expression",
            "factor",
            "function_declare",
            "if_sentence",
            "inner_declare",
            "inner_var_declare",
            "item",
            "param",
            "parameter",
            "parameter_list",
            "return_sentence",
            "sentence",
            "sentence_block",
            "sentence_list",
            "var_declare",
            "while_sentence"]

VN_MIN = VN.emptypro.value
VN_MAX = VN.while_sentence.value

PATTERNS = ["P,N,declare_list",
            "declare_list,declare,declare_list",
            "declare_list,declare",
            "declare,'int','Identifier',M,A,function_declare",
            "declare,'int','Identifier',var_declare",
            # "declare,'int','Identifier',array_declare",
            "declare,'void','Identifier',M,A,function_declare",
            "A,emptypro",
            "var_declare,';'",
            "function_declare,'(',parameter,')',sentence_block",
            # "array_declare,'[',Integer,",
            "parameter,parameter_list",
            "parameter,'void'",
            "parameter_list,param",
            "parameter_list,param,',',parameter_list",
            "param,'int','Identifier'",
            "sentence_block,'{',inner_declare,sentence_list,'}'",
            "inner_declare,emptypro",
            "inner_declare,inner_var_declare,';',inner_declare",
            "inner_var_declare,'int','Identifier'",
            "sentence_list,sentence,M,sentence_list",
            "sentence_list,sentence",
            "sentence,if_sentence",
            "sentence,while_sentence",
            "sentence,return_sentence",
            "sentence,assign_sentence",
            "assign_sentence,'Identifier','=',expression,';'",
            "return_sentence,'return',';'",
            "return_sentence,'return',expression,';'",
            "while_sentence,'while',M,'(',expression,')',A,sentence_block",
            "if_sentence,'if','(',expression,')',A,sentence_block",
            "if_sentence,'if','(',expression,')',A,sentence_block,N,'else',M,A,sentence_block",
            "N,emptypro",
            "M,emptypro",
            "expression,add_expression",
            "expression,add_expression,'>',add_expression",
            "expression,add_expression,'>=',add_expression",
            "expression,add_expression,'<',add_expression",
            "expression,add_expression,'<=',add_expression",
            "expression,add_expression,'==',add_expression",
            "expression,add_expression,'!=',add_expression",
            "add_expression,item",
            "add_expression,item,'+',add_expression",
            "add_expression,item,'-',add_expression",
            "item,factor",
            "item,factor,'*',item",
            "item,factor,'/',item",
            "factor,'Integer'",
            "factor,'Floating'",
            "factor,'(',expression,')'",
            "factor,'Identifier','(',argument_list,')'",
            "factor,'Identifier'",
            "argument_list,emptypro",
            "argument_list,expression",
            "argument_list,expression,',',argument_list",
            "NULL"]


def VN_to_string(i):
    if i < 0 or i > VN_MAX - VN_MIN:
        return ""
    else:
        return VN_names[i]


def VT_to_string(i):
    if i < 0 or (i > LexAnalyzer.VT_MAX - LexAnalyzer.VT_MIN):
        return ""
    else:
        return VT_names[i]


def VNStr_to_int(p):
    for i in range(0, VN_MAX - VN_MIN + 1):
        if p == VN_names[i]:
            return i + VN_MIN
    print(p)
    return 0


def thr(message):
    print(message)
    raise NameError(message)


def check_adjacent_quotation(elements):
    temp_ele = cp.deepcopy(elements)
    for i in range(0, len(elements) - 1):
        if elements[i][0] == elements[i + 1] == '\'':
            temp_ele.pop(i + 1)
            temp_ele[i] = "','"
    return temp_ele


class SyntaxAnalyzer:
    def __init__(self):
        self.VTs = []
        self.VNs = []
        self.PATs = []

        self.InitVTs()
        self.InitVNs()
        self.InitPATs()

    def InitVTs(self):
        for i in range(0, LexAnalyzer.VT_MAX - LexAnalyzer.VT_MIN + 1):
            tmp = [LexAnalyzer.VT_MIN + i, LexAnalyzer.VT_to_string(i)]
            self.VTs.append(tmp)

    def InitVNs(self):
        for i in range(0, VN_MAX - VN_MIN + 1):
            tmp = [VN_MIN + i, VN_to_string(i)]
            self.VNs.append(tmp)

    def InitPATs(self):
        for i in range(len(PATTERNS)):
            if PATTERNS[i] != "NULL":
                tmp = self.InitPATs_SingleLine(PATTERNS[i])
                self.PATs.append(tmp)
            else:
                break

    def InitPATs_SingleLine(self, p):
        tmp = []
        p = p.replace(' ', '')
        elements = p.split(',')
        elements = check_adjacent_quotation(elements)

        for c in elements:
            if c[0] == c[-1] == '\'':
                tmp.append(LexAnalyzer.VTStr_to_int(c[1:-1]))
            else:
                tmp.append(VNStr_to_int(c))
        return tmp

    def PrintVectorVTs(self):
        print("Here are contents of VTs\n")
        for item in self.VTs:
            print(f"<{item.key, item.value}>")

    def PrintVectorVNs(self):
        print("Here are contents of VNs\n")
        for item in self.VNs:
            print(f"<{item.key, item.value}>")

    def PrintVectorPATS(self):
        print("Here are contents of PATs\n")
        for item in self.PATs:
            for i, it2 in enumerate(item):
                if i == 0:
                    print(it2, "--> [ ", end="")
                elif i == len(item) - 1:
                    print(it2, " ", end="")
                else:
                    print(it2, ", ", end="")
            print("]\n", end="")

    def LR1_Analyze_file(self, out, out_display):
        out.write(f"{LexAnalyzer.VT_MAX - LexAnalyzer.VT_MIN + 1} {VN_MAX - VN_MIN} {len(self.PATs)}\n")

        for it in self.VTs:
            out.write(f"{it[0]} ")
        out.write("\n")

        for it in self.VNs:
            if it[0] != VN.emptypro.value:
                out.write(f"{it[0]} ")
        out.write("\n")

        for it in self.PATs:
            out.write(f"{len(it)} ")
            for ele in it:
                out.write(f"{ele} ")
            out.write("\n")

        PATTERNS_display = [pattern.replace(',', '---->', 1) for pattern in PATTERNS]
        for it in PATTERNS_display:
            out_display.write(f"{it}\n")
